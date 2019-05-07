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
#include <string.h>
#include "tuntap.h"

unsigned int samples_len;
int16_t *tx_samples;
struct bladerf *devtx;

#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[])
{
	bool noise_active = TRUE;	
	
    int status;
    struct module_config config_tx;
    struct bladerf_devinfo dev_info;
   
    float complex y[BUFFER_SIZE];
    unsigned int i;
    int frame_complete = 0;
    int lastpos = 0;
    int cnt=0;
    int nread;


    bladerf_init_devinfo(&dev_info);

    if (argc >= 2) {
        strncpy(dev_info.serial, argv[1], sizeof(dev_info.serial) - 1);
    }
    status = bladerf_open_with_devinfo(&devtx, &dev_info);

    if (status != 0) {
        fprintf(stderr, "Unable to open device: %s\n", bladerf_strerror(status));
        return 1;
    }
    fprintf(stdout, "Device Serial: %s\tbladerf_open_with_devinfo: %s\n", dev_info.serial, bladerf_strerror(status));

    // load image to bladerf fpga.
    bladerf_load_fpga(devtx, "hostedx115-latest.rbf");

    /* Set up TX module parameters */
    config_tx.module     = BLADERF_MODULE_TX;
    config_tx.frequency  = FREQUENCY_USED;
    config_tx.bandwidth  = BANDWIDTH_USED;
    config_tx.samplerate = SAMPLING_RATE_USED;
    config_tx.rx_lna     = BLADERF_LNA_GAIN_MID;
    config_tx.vga1       = 10;
    config_tx.vga2       = 0;


    status = configure_module(devtx, &config_tx);
    if (status != 0) {
        fprintf(stderr, "Failed to configure RX module. Exiting.\n");
        return status;
    }
    fprintf(stdout, "configure_module: %s\n", bladerf_strerror(status));


    status = init_sync_tx(devtx);
    if (status != 0) {
    	fprintf(stderr, "init_sync_tx. Exiting.\n");
    	return status;
    }
    fprintf(stdout, "init_sync_tx: %s\n", bladerf_strerror(status));


    status = calibrate(devtx);
    if (status != 0) {
        fprintf(stderr, "Failed to calibrate RX module. Exiting.\n");
        return status;
    }
    fprintf(stdout, "calibrate: %s\n", bladerf_strerror(status));


     // options
    unsigned int M           = 64;          // number of subcarriers
    unsigned int cp_len      = 32;          // cyclic prefix length
    unsigned int taper_len   = 4;           // taper length
    unsigned int payload_len = 128;         // length of payload (bytes)
    unsigned int frame_sample_len =0;

    unsigned int counters =0;

    // allocate memory for header, payload, sample buffer
    unsigned int symbol_len = M + cp_len + payload_len;   // samples per OFDM symbol
    float complex buffer[symbol_len];       // time-domain buffer
    unsigned char header[8];                // header
    unsigned char payload[payload_len];     // payload
    unsigned char p[M];         // subcarrier allocation (null/pilot/data)
	//Noise params...
    float SNRdB = 20.0f;                        // signal-to-noise ratio [dB]


 // channel parameters
    float nstd = 0.2;//powf(10.0f, -SNRdB/20.0f); // noise standard deviation
    
    
    
    modulation_scheme ms = LIQUID_MODEM_QAM4;   // payload modulation scheme
    fec_scheme fec0  = LIQUID_FEC_NONE;         // inner FEC scheme
    fec_scheme fec1  = LIQUID_FEC_SECDED2216;   // outer FEC scheme
    crc_scheme check = LIQUID_CRC_16;           // data validity check

    //For overriding default values of OFDM generator
    ofdmflexframegenprops_s fgprops;
    //Get current OFDM generator properties....
    //ofdmflexframegen_getprops(fg, &fgprops);
    ofdmflexframegenprops_init_default(&fgprops);
    fgprops.check           = check;
    fgprops.fec0            = fec0;
    fgprops.fec1            = fec1;
    fgprops.mod_scheme      = ms;

     ofdmframe_init_default_sctype(M, p);

    // create frame generator object with default properties
    ofdmflexframegen fg = ofdmflexframegen_create(M, cp_len, taper_len, p, &fgprops);
    ofdmflexframegen_print(fg);

	printf("\n\nFlex frame create...");


    // INIT HEADER
    // initialize header/payload and assemble frame
    for (i=0; i<8; i++)           header[i]  = i      & 0xff;
    // enable for genearting random numbers in payload.
    //for (i=0; i<payload_len; i++) payload[i] = rand() & 0xff;


    while(status == 0 && counters < 101 )
    {
    		cnt ++;


            memset(payload, 0x00, PAYLOAD_LENGTH);
            sprintf((char*)payload,"Packet (%d)",cnt);
            memset(&payload[13], 0x00, PAYLOAD_LENGTH-13);
			ofdmflexframegen_assemble(fg, header, payload, payload_len);
			frame_sample_len = ofdmflexframegen_getframelen(fg);
			frame_complete = 0;
    		lastpos = 0;
    		while (!frame_complete) {
            	frame_complete = ofdmflexframegen_write(fg, buffer, symbol_len);
            
            
            
            if(noise_active == TRUE)
            {
             for (int i=0; i<symbol_len; i++) {
            
            cawgn(&buffer[i], nstd);            // add noise
			}
			}
            
            
            	//Each data retrieved data is written to a complex array for bladeRF to send....
            	memcpy(&y[lastpos], buffer, symbol_len*sizeof(float complex));
            	lastpos = lastpos + symbol_len;
        	}
        	// A convertion is done for data to conform the  for bladeRF..
        	samples_len=frame_sample_len;
        	tx_samples = convert_comlexfloat_to_sc16q11( y, BUFFER_SIZE );
        	if (tx_samples == NULL) {
        		fprintf(stdout, "malloc error: %s\n", bladerf_strerror(status));
        		return BLADERF_ERR_MEM;
        	}
        	//Data is send to bladeRF ....        	
        	status =  sync_tx(devtx, tx_samples, SAMPLE_SET_SIZE);
			if (status != 0) {
				fprintf(stderr, "Failed to sync_tx(). Exiting. %s\n", bladerf_strerror(status));
				goto out;
			}
			fprintf(stdout, "Packet %d transmitted\n", cnt);
			usleep(100000);
            counters++;
    }


out:
    bladerf_close(devtx);
    ofdmflexframegen_destroy(fg);
    fprintf(stderr, "TX STATUS: %u,  %s\n", status, bladerf_strerror(status));
    return status;
}
