/*
 * transceive.c
 *
 *  Created on: Jan 28, 2017
 *      Author: eronur
 */


#include "transceive.h"

int sync_tx(struct bladerf *dev,int16_t *tx_samples, unsigned int samples_len)
{
	int status = 0;

	bladerf_enable_module(dev, BLADERF_MODULE_TX, true);
    struct bladerf_metadata meta;
    memset(&meta, 0, sizeof(meta));

 //   meta.flags = BLADERF_META_FLAG_TX_NOW;
    meta.flags = BLADERF_META_FLAG_TX_BURST_START;


	status = bladerf_sync_tx(dev, tx_samples, samples_len, &meta, 5000);
	if (meta.status != 0) {
		fprintf(stderr, "Failed to TX samples: %s\n",bladerf_strerror(meta.status));
	}
	if (meta.actual_count > 0 )
		fprintf(stdout, "Meta Flag Actual Count = %u\n", meta.actual_count );


	return status;
}


int sync_rx(struct bladerf *dev, int (*process_samples)(int16_t *, unsigned int))
{
    int status=0, ret;
    bool done = false;
    /* "User" samples buffers and their associated sizes, in units of samples.
     * Recall that one sample = two int16_t values. */
    int16_t *rx_samples = NULL;
    unsigned int samples_len = SAMPLE_SET_SIZE; /* May be any (reasonable) size */
    /* Allocate a buffer to store received samples in */
    rx_samples = malloc(samples_len * 2 * sizeof(int16_t));
    if (rx_samples == NULL) {
        fprintf(stdout, "malloc error: %s\n", bladerf_strerror(status));
        return BLADERF_ERR_MEM;
    }

    bladerf_enable_module(dev, BLADERF_MODULE_RX, true);
    struct bladerf_metadata meta;
    memset(&meta, 0, sizeof(meta));
    /* Retrieve the current timestamp */
    if ((status=bladerf_get_timestamp(dev, BLADERF_MODULE_RX, &meta.timestamp)) != 0) {
        fprintf(stderr,"Failed to get current RX timestamp: %s\n",bladerf_strerror(status));
    }
    else
    {
        printf("Current RX timestamp: 0x%016"PRIx64"\n", meta.timestamp);
    }

      meta.flags = BLADERF_META_FLAG_RX_NOW;

    while (status == 0) {
        /* Receive samples */
        status = bladerf_sync_rx(dev, rx_samples, samples_len, &meta, 5000);
        //fprintf(stdout, "Meta Flag Actual Count = %u\n", meta.actual_count );
        if (status == 0) {
            /* TODO Process these samples, and potentially produce a response to transmit */
        	done = process_samples(rx_samples, meta.actual_count);
        } else {
            fprintf(stderr, "Failed to RX samples: %s\n", bladerf_strerror(status));
        }
    }
    if (status == 0) {
        /* Wait a few seconds for any remaining TX samples to finish
         * reaching the RF front-end */
        usleep(2000000);
    }
    out:
        ret = status;
        /* Free up our resources */
        free(rx_samples);
        return ret;

}


/*! \brief Receive samples from hardware.
 * Read \ref nsamps samples from each channel to buffers. buff[0] is the array for
 * the first channel. *ptimestamp is the time at which the first sample
 * was received.
 * \param device the hardware to use
 * \param[out] ptimestamp the time at which the first sample was received.
 * \param[out] buff An array of pointers to buffers for received samples. The buffers must be large enough to hold the number of samples \ref nsamps.
 * \param nsamps Number of samples. One sample is 2 byte I + 2 byte Q => 4 byte.
 * \param cc  Index of component carrier
 * \returns number of samples read
*/
 int trx_brf_read(struct bladerf *dev,int64_t *ptimestamp, void **buff, int nsamps, int cc)
{
  int status=0;
  brf_state_t brf;
  brf.rx_timeout_ms=5000;
  // BRF has only one rx/tx chain
  int16_t *samples = (int16_t*)buff[0];

  brf.meta_rx.flags = BLADERF_META_FLAG_RX_NOW;
  status = bladerf_sync_rx(dev, samples, (unsigned int) nsamps, &brf.meta_rx, 2*brf.rx_timeout_ms);


  if (status != 0) {
    fprintf(stderr, "RX failed: %s\n", bladerf_strerror(status));
  }
  return status;
}


/*! \brief Called to send samples to the BladeRF RF target
      \param device pointer to the device structure specific to the RF hardware target
      \param timestamp The timestamp at whicch the first sample MUST be sent
      \param buff Buffer which holds the samples
      \param nsamps number of samples to be sent
      \param cc index of the component carrier
      \param flags Ignored for the moment
      \returns 0 on success
*/
 int trx_brf_write(struct bladerf *dev,int64_t ptimestamp, void **buff, int nsamps, int cc, int flags)
{

  int status;
  brf_state_t brf ;
  /* BRF has only 1 rx/tx chaine : is it correct? */
  int16_t *samples = (int16_t*)buff[0];

  if (brf.meta_tx.flags == 0 )
    brf.meta_tx.flags = (BLADERF_META_FLAG_TX_BURST_START);// | BLADERF_META_FLAG_TX_BURST_END);// |  BLADERF_META_FLAG_TX_NOW);


  brf.meta_tx.timestamp= (uint64_t) (ptimestamp);
  status = bladerf_sync_tx(dev, samples, (unsigned int) nsamps, &brf.meta_tx, 2*brf.tx_timeout_ms);

  if (status != 0) {
    fprintf(stderr,"Failed to TX sample: %s\n", bladerf_strerror(status));

  }
  return(status);
}


