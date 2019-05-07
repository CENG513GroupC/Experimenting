/*
 * tryframing.c
 *
 *  Created on: Jan 30, 2017
 *      Author: eronur
 */

#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen

#include<netinet/if_ether.h>  //For ETH_P_ALL
#include <netinet/ip.h>
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>

#include "utils.h"

void DisplayPacket(unsigned char*, int);
int tun_tx_fd, tun_rx_fd;

// static callback function
static int callback(unsigned char * _header, int _header_valid,
		unsigned char * _payload, unsigned int _payload_len, int _payload_valid,
		framesyncstats_s _stats, void * _userdata) {
	int nwrite = 0, nread = 0;
	char buffer[1500];
//	printf("*** callback invoked ***\n");
//
//	printf("  payload length (%d)\n", _payload_len);
	if (_payload_valid) {
		DisplayPacket((unsigned char *) _payload, _payload_len);
		nwrite = write(tun_rx_fd, (void *) _payload, _payload_len);
		if (nwrite < 0) {
			perror("Writing to interface");
			close(tun_rx_fd);
			exit(1);
		} else {
			nread = read(tun_rx_fd, buffer, sizeof(buffer));
			if (nread < 0) {
				perror("Writing to interface");
				close(tun_rx_fd);
				exit(1);
			} else {
				nwrite = write(tun_tx_fd, buffer, nread);
				if (nwrite < 0) {
					perror("Writing to interface");
					close(tun_rx_fd);
					exit(1);
				}
			}
		}
	} else
	{
		printf("  header (%s)\n", _header_valid ? "valid" : "INVALID");
		printf("  payload (%s)\n", _payload_valid ? "valid" : "INVALID");
	}
	return 0;
}

int main() {

	// CREATE frame generator
	flexframegenprops_s ffp;
	int nread;
	int err, i;
	char buffer[150000];
	// data header and payload
	unsigned int payload_len = 256;   // number of bytes in payload
	unsigned char header[14];           // user-defined header
	// generate the frame in blocks
	unsigned int buf_len = 1;
	float complex buf[buf_len];
	int frame_complete = 0;
	int lastpos;
	float complex y[150000];
	unsigned int symbol_len;

	for (int i = 0; i < 14; i++)
		header[i] = i;

	flexframegenprops_init_default(&ffp);
	//	    ffp.check = false;
	ffp.fec0 = LIQUID_FEC_CONV_V27P78;
	ffp.fec1 = LIQUID_FEC_CONV_V27P78;
	ffp.mod_scheme = LIQUID_MODEM_BPSK;



	// create frame generator with default properties
	flexframegen fg = flexframegen_create(&ffp);
	// create frame synchronizer using default properties
	flexframesync fs = flexframesync_create(callback, NULL);
	flexframegen_setprops(fg, &ffp);

	if ((tun_tx_fd = open("/dev/tun6", O_RDWR)) == -1) {
		perror("open /dev/tun6");
		exit(1);
	}
	if ((tun_rx_fd = open("/dev/tun7", O_RDWR)) == -1) {
		perror("open /dev/tun7");
		exit(1);
	}
	printf("%d\n", system("ifconfig tun6 10.10.10.1 10.10.10.255"));
	printf("%d\n", system("ifconfig tun6 up"));
	printf("%d\n", system("ifconfig tun6"));
	printf("%d\n", system("sudo route add 10.10.70/24 -interface tun6"));

	printf("%d\n", system("ifconfig tun7 10.10.70.1 10.10.70.255"));
	printf("%d\n", system("ifconfig tun7 up"));
	printf("%d\n", system("ifconfig tun7"));
	printf("%d\n", system("sudo route add 10.10.10/24 -interface tun7"));

	while (1) {
		/* Note that "buffer" should be at least the MTU size of the interface, eg 1500 bytes */
		//write(tap_fd, buffer,  sizeof(buffer));
		memset(buffer, 0, sizeof(buffer));
		nread = read(tun_tx_fd, buffer, sizeof(buffer));
		if (nread < 0) {
			perror("Reading from interface");
			close(tun_tx_fd);
			exit(1);
		}

		//unsigned char payload[payload_len]; // user-defined payload
		flexframegen_assemble(fg, header, (unsigned char *) buffer, nread);

		memset(y, 0, 10000);
		buf_len = 1;
		frame_complete = 0;
		lastpos = 0;
		while (!frame_complete) {
			// write samples to buffer
			frame_complete = flexframegen_write_samples(fg, buf, buf_len);
			memcpy(&y[lastpos], buf, buf_len*sizeof(float complex));
			lastpos = lastpos + buf_len;
		}
		symbol_len = lastpos;
		printf("framegen64_execute after %d\n", symbol_len);
//		    /************** CHANNEL IMPAIRMENTS ************/
//		    float phase_offset=0.01f;        // carrier phase offset
//		    float frequency_offset=0.002f;   // carrier frequency offset
//		    float SNRdB = 10.0f;            // signal-to-noise ratio [dB]
//		    float noise_floor = -40.0f;     // noise floor [dB]
//		    // add channel impairments (attenuation, carrier offset, noise)
//		    float nstd  = powf(10.0f, noise_floor/20.0f);        // noise std. dev.
//		    float gamma = powf(10.0f, (SNRdB+noise_floor)/20.0f);// channel gain
//		    for (i=0; i<1340; i++) {
//		        y[i] *= gamma;
//		        y[i] *= cexpf(_Complex_I*(phase_offset + i*frequency_offset));
//		    }
//		    /**************************/
//		        y[i] += nstd * (randnf() + _Complex_I*randnf())*M_SQRT1_2;

//		int16_t * tx_samples = convert_comlexfloat_to_sc16q11(y, symbol_len);
//		float complex * y2 = convert_sc16q11_to_comlexfloat(tx_samples,symbol_len);
//
//		for (int i=0; i<5; i++)
//			printf("%12.8f + j*%12.8f  ", crealf(y[i])-crealf(y2[i]), cimagf(y[i])-cimagf(y2[i]));

		//sleep(2);

		for (int i = 0; i < symbol_len; i = i + 32)
			flexframesync_execute(fs, &y[i], 32);

	}

	flexframegen_destroy(fg);
	flexframesync_destroy(fs);

}

void DisplayPacket(unsigned char* buffer, int size) {

	struct ip *iph = (struct ip*) (buffer);
	printf("Packet from (%s) ", inet_ntoa(iph->ip_src));
	printf("to (%s) ", inet_ntoa(iph->ip_dst));
	printf(" of (%hu) bytes\n", ntohs(iph->ip_len));
	//printf("Header Length datagram :%hu\n",  iph->ip_hl * 4);

}

