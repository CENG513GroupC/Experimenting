/*
 * transceive.h
 *
 *  Created on: Jan 28, 2017
 *      Author: eronur
 */

#ifndef INCLUDE_TRANSCEIVE_H_
#define INCLUDE_TRANSCEIVE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <libbladeRF.h>
#include "config.h"

int sync_rx(struct bladerf *dev, int (*process_samples)(int16_t *, unsigned int));
int sync_tx(struct bladerf *dev,int16_t *tx_samples, unsigned int samples_len);
 int trx_brf_read(struct bladerf *dev,int64_t *ptimestamp, void **buff, int nsamps, int cc);
 int trx_brf_write(struct bladerf *dev,int64_t ptimestamp, void **buff, int nsamps, int cc, int flags);

#endif /* INCLUDE_TRANSCEIVE_H_ */
