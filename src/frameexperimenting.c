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

//Tuntap library...
#include "tuntap.h"


/* The RX and TX modules are configured independently for these parameters */
    unsigned int samples_len = SAMPLE_SET_SIZE;
    int16_t *tx_samples;
    struct bladerf *devtx;
    int tun_rx_fd;

//For tun0 device
struct device *dev;


/*
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */






int main(int argc, char *argv[])
{
    int status;
    struct module_config config_rx;
    struct module_config config_tx;
    struct bladerf_devinfo dev_info;
   
   /*
    
    #define  SAMPLE_SET_SIZE 8192 //must be multiple of 2
	#define  BUFFER_SIZE    SAMPLE_SET_SIZE*sizeof(int32_t)
	#define  NUMBER_OF_TRANSFERS 8
	#define  TIMEOUT_IN_MS 1000
	#define  FREQUENCY_USED 713000000
	#define  BANDWIDTH_USED 3000000
	#define  SAMPLING_RATE_USED 600000
	#define  PAYLOAD_LENGTH 1500
    
    */
   
    unsigned char header[8];        // data header
    unsigned char payload[PAYLOAD_LENGTH]={0};      // data payload
    float complex y[BUFFER_SIZE];          // frame samples
    unsigned int  buf_len = PAYLOAD_LENGTH;
    float complex buf[buf_len];
    unsigned int i;
    unsigned int symbol_len;
    int frame_complete = 0;
    int lastpos = 0;
    int cnt=0;
    flexframegenprops_s ffp;
    int nread;




    flexframegenprops_init_default(&ffp);
	//ffp.check = false;
    ffp.fec0 = LIQUID_FEC_NONE;
    ffp.fec1 = LIQUID_FEC_NONE;
    ffp.mod_scheme = LIQUID_MODEM_QAM4;
/*
On the transmitter side, the appropriate frame generator object is created, configured, and executed.
* While the specifics of the frame64 and flexframe structures are different, both frames consist of six basic parts:

ramp/up gracefully increases the output signal level to avoid "key clicking" and reduce spectral side-lobes in the transmitted signal..
preamble phasing is a BPSK pattern which flips phase for each transmitted symbol ( +1,-1,+1,-1,... ). 
p/n sequence is an mm -sequence (see [section-sequence] ) exhibiting good auto- and cross-correlation properties. 
header is a fixed-length data sequence which contains a small amount of information about the rest of the frame.
payload is the meat of the frame, containing the raw data to be transferred across the link. 
ramp/down gracefully decreases the output signal level as per ramp/up.
* 
*/



    flexframegen fg = flexframegen_create(&ffp);
    
    
    
    flexframegen_print(fg);



// INIT HEADER
    for (i=0; i<8; i++)
        header[i] = i;

   
			memset(payload, 0x00, PAYLOAD_LENGTH);
			//sprintf((char*)payload,"abcdef (%d)",cnt);
			//memset(&payload[13], 0x00, PAYLOAD_LENGTH-13);
            
        
			
			
		/*		flexframegen_assemble()
		 * 
		 *     // reset object
		 *     // set decoded payload length
		 *     // copy user-defined header to internal
		 *     // add FLEXFRAME_PROTOCOL
		 *     // add payload length
		 *     // add modulation scheme/depth (pack into single byte)
		 *     // add CRC, forward error-correction schemes
			   //  CRC     : most-significant  3 bits of [n+4]
			   //  fec0    : least-significant 5 bits of [n+4]
			   //  fec1    : least-significant 5 bits of [n+5]
			   * 
			   // encode/modulate header
			   // add pilots
			   // reconfigure payload
			   // encode/modulate payload
			   // set assembled flag


				Assembles header payload and others applies the required modulation, error correction etc. 
				Internally stores this processed data...
				*/



			flexframegen_assemble(fg, header, payload, PAYLOAD_LENGTH);
			flexframegen_print(fg);
			
			/*
			 * // get frame length (number of samples)

			 * 
			 *unsigned int flexframegen_getframelen(flexframegen _q)
{
    if (!_q->frame_assembled) {
        fprintf(stderr,"warning: flexframegen_getframelen(), frame not assembled!\n");
        return 0;
    }
    unsigned int num_frame_symbols =
            64 +                    // preamble p/n sequence length
            _q->header_sym_len +    // header symbols
            _q->payload_sym_len +   // number of modulation symbols
            2*_q->m;                // number of tail symbols

    return num_frame_symbols*_q->k; // k samples/symbol
} 
			 * 
			 * 
			 * */
			
			symbol_len = flexframegen_getframelen(fg);






			frame_complete = 0;
    		lastpos = 0;
    		while (!frame_complete) {
				
				/*
				 * / write samples of assembled frame, two samples at a time, returning
					// '1' when frame is complete, '0' otherwise. Zeros will be written
					// to the buffer if the frame is not assembled
					//  _q          :   frame generator object
					//  _buffer     :   output buffer [size: _buffer_len x 1]
					//  _buffer_len :   output buffer length
				 * 
				 * Writes internally stored data to passed _buffer pointer
				 * Internally keep a counter so in each iteration it iterates over internally stored data and writes specified _buffer_len bytes to
				 * _buffer object....
				 * 
				 * It is used to get internally assembled frame data...
				 * */			
            	frame_complete = flexframegen_write_samples(fg, buf, buf_len);
            	
            	
            	//Each data retrieved data is written to a complex array for bladeRF to send....
            	memcpy(&y[lastpos], buf, buf_len*sizeof(float complex));
            	lastpos = lastpos + buf_len;
        	}
        	
        	
        	
        	printf("number of samples %u %u\n", symbol_len, lastpos);
        	
        	// A convertion is done for data to conform the  for bladeRF..
        	samples_len=symbol_len;
        	

        	
        
		
			
			fprintf(stdout, "Packet %d transmitted\n", cnt);
			//usleep(10000);
    


out:
    flexframegen_destroy(fg);
    

    return status;
}
