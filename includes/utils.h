/*
 * utils.h
 *
 *  Created on: Jan 29, 2017
 *      Author: eronur
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_

#include <math.h>
#include <complex.h>
#include <libbladeRF.h>
#include <liquid/liquid.h>


float complex * convert_sc16q11_to_comlexfloat 	( int16_t * in, int16_t inlen );
int16_t * convert_comlexfloat_to_sc16q11 	( float complex *in, unsigned int  inlen  );



#endif /* INCLUDE_UTILS_H_ */
