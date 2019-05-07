#include "config.h"
#include "utils.h"
#include <libbladeRF.h>
#include "transceive.h"
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include <netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h> 
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/ioctl.h>
#include<sys/types.h>
#include <math.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <stdio.h>
#include "tuntap.h"

ofdmflexframesync fs;

unsigned char print_array[13];
unsigned int payload_length = 256;
unsigned int n = 6;
int packet_count = 0;
int numofvalids = 0;
static int mycallback(unsigned char *  _header,
                      int              _header_valid,
                      unsigned char *  _payload,
                      unsigned int     _payload_len,
                      int              _payload_valid,
                      framesyncstats_s _stats,
                      void *           _userdata){
  int nwrite = 0;
  //printf("***** callback invoked!\n");
  //unsigned int* counter = (unsigned int*) _userdata;
  //(*counter) ++;
  if (_header_valid){
    
    memcpy(print_array, _payload, 13* sizeof(unsigned char));
    printf("Print Array: %s\n", print_array);
    //printf("Inside header is valid check.");
    packet_count = packet_count + 1;
    printf("Inside payload is valid check. %i, Payload valid?: %i\n", packet_count, _payload_valid);
    if(_payload_valid){
        numofvalids ++;
    }
    printf("Valid Check Pass: %i / %i\n", numofvalids, 1000);
    //unsigned char payload[payload_length];
    //memset(payload, 0, payload_length);
    //snprintf((char*) payload, 7, "Packet");
    //unsigned int num_bit_errors = count_bit_errors_array(payload, _payload, n);
    //printf("[%u]: (%s): %3u / %3u\t RSSI= (%5.5f)\n", *counter, _payload, num_bit_errors, n*8, _stats.rssi);
  }
  else{
    printf("  header (%s)\n", _header_valid ? "valid" : "INVALID");
    printf("  payload (%s)\n", _payload_valid ? "valid" : "INVALID");
  }
  return 0;
}


int process_samples(int16_t * samples, unsigned int sample_length){
	int status=0;
	//After assembling data we have transformed it for bladeRF, here we are backing up the original assembled form...	
	float complex * y = convert_sc16q11_to_comlexfloat(samples, sample_length);
	if ( y != NULL ){
		//flexframesync_execute copies data internall buffer when it receives full frame it calls the call back function of sync
		for (int i=0; i<=sample_length; i=i+32)
			ofdmflexframesync_execute(fs, &y[i], 32);
		free(y);
	}
	else{
		status = BLADERF_ERR_MEM;
	}
    return status;
}


int main(int argc, char *argv[]){
    
    int status;
    struct module_config config_rx;
    struct bladerf *devrx = NULL;
    struct bladerf_devinfo dev_info;
    unsigned int frame_counter = 0;

    /*---------------------------- BladeRF Configuration ---------------------------------------*/
    // get bladerf info.
    bladerf_init_devinfo(&dev_info);
    // get bladerf status.
    status = bladerf_open_with_devinfo(&devrx, &dev_info);
    if (status != 0) {
        fprintf(stderr, "Unable to open device: %s\n", bladerf_strerror(status));
        return 1;
    }
    // write bladerf status.
    fprintf(stdout, "bladerf_open_with_devinfo: %s\n", bladerf_strerror(status));

    // write config to bladerf fpga.
    status = bladerf_load_fpga(devrx,  "hostedx115-latest.rbf");
    if (status != 0) {
        fprintf(stderr, "Unable to bladerf_load_fpga  device: %s\n", bladerf_strerror(status));
        return status;
    }
    // check fpga status.
    fprintf(stdout, "bladerf_load_fpga: %s\n", bladerf_strerror(status));
    
    // set up RX module parameters
    config_rx.module     = BLADERF_MODULE_RX;
    config_rx.frequency  = FREQUENCY_USED;
    config_rx.bandwidth  = BANDWIDTH_USED;
    config_rx.samplerate = SAMPLING_RATE_USED;
    config_rx.rx_lna     = BLADERF_LNA_GAIN_MAX;
    config_rx.vga1       = 30;
    config_rx.vga2       = 3;
    // check rx module status.
    status = configure_module(devrx, &config_rx);
       if (status != 0) {
           fprintf(stderr, "Failed to configure RX module. Exiting.\n");
           return status;
       }
    fprintf(stdout, "configure_module: %s\n", bladerf_strerror(status));
    // initialize synch interface on RX module.
    status = init_sync_rx(devrx);
    if (status != 0) {
        return status;
    }
    // calibrate RX module.
    status = calibrate(devrx);
    if (status != 0) {
        fprintf(stderr, "Failed to calibrate RX module. Exiting.\n");
        return status;
    }
    fprintf(stdout, "calibrate: %s\n", bladerf_strerror(status));
    /*---------------------------- BladeRF Configuration ---------------------------------------*/

    printf("BladeRF Configuration Done.\n");

    /*---------------------------- OFDM Flex Frame Sync Configuration ---------------------------------------*/
    // options
    unsigned int M = 64;        // number of subcarriers
    unsigned int cp_len = 16;   // cyclic prefix length
    unsigned int taper_len = 4; // taper length
    unsigned char p[M];         // subcarrier allocation (null/pilot/data)
    void * userdata;            // user-defined data

    // initialize subcarrier allocation to default
    ofdmframe_init_default_sctype(M, p);
    // create frame synchronizer object
    fs = ofdmflexframesync_create(M, cp_len, taper_len, NULL, mycallback, NULL);
    
    if (fs==NULL){
        fprintf(stderr, "Failed to ofdmflexframesync_create. Exiting.\n");
        return 0;
    }
    // write sync object.
    ofdmflexframesync_print(fs);
    /*---------------------------- OFDM Flex Frame Sync Configuration ---------------------------------------*/

    printf("OFDM Flex Frame Sync Configuration Done.\n");

    //It loop until a data arrives bladeRF, then calls process_samples function...
    status =  sync_rx(devrx, &process_samples);
    if (status != 0) {
            fprintf(stderr, "Failed to sync_rx(). Exiting. %s\n", bladerf_strerror(status));
            goto out;
    }

    out:
        bladerf_close(devrx);
        fprintf(stderr, "RX STATUS: %u,  %s\n", status, bladerf_strerror(status));
        ofdmflexframesync_destroy(fs);
        return status;
}