/*
 * utils.c
 *
 *  Created on: Jan 29, 2017
 *      Author: eronur
 */

#include "utils.h"



float complex * convert_sc16q11_to_comlexfloat 	( int16_t * in, int16_t inlen )
{

	int i=0;
	float complex * out = NULL;
    out = (float complex *)malloc(inlen * sizeof(float complex));
    if (out != NULL) {
		for(i = 0; i < inlen ; i++){
				out[i]= in[2*i]/2048.0 + in[2*i+1]/2048.0 * I;
		}
    }
	return (float complex *)out;
}


int16_t * 		convert_comlexfloat_to_sc16q11 	( float complex *in, unsigned int  inlen  )
{
	int i=0;
	int16_t * out = NULL;
    out = (int16_t *)malloc(inlen * 2 * sizeof(int16_t));
    if (out != NULL) {
		for(i = 0; i < inlen ; i++){
				out[2*i]= round( crealf(in[i]) * 2048); // Since bladeRF uses Q4.11 complex 2048=2^11
				out[2*i+1]= round( cimagf(in[i]) * 2048);
				if ( out[2*i] > 2047  ) out[2*i]=2047;
				if ( out[2*i] < -2048  ) out[2*i]=-2048;
				if ( out[2*i+1] > 2047  ) out[2*i+1]=2047;
				if ( out[2*i+1] < -2048  ) out[2*i+1]=-2048;
		}
    }
	return (int16_t *)out;
}




