#ifndef __CONFIG_HEADER_INCLUDED__
#define __CONFIG_HEADER_INCLUDED__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libbladeRF.h>
#include <liquid/liquid.h>
#include "transceive.h"


#define  NUMBER_OF_BUFFERS 16
#define  SAMPLE_SET_SIZE 8192 //must be multiple of 2
#define  BUFFER_SIZE    SAMPLE_SET_SIZE*sizeof(int32_t)
#define  NUMBER_OF_TRANSFERS 8
#define  TIMEOUT_IN_MS 1000
#define  FREQUENCY_USED 713000000
#define  BANDWIDTH_USED 3000000
#define  SAMPLING_RATE_USED 600000
#define  PAYLOAD_LENGTH 1500

struct module_config {
    bladerf_module module;
    unsigned int frequency;
    unsigned int bandwidth;
    unsigned int samplerate;
    /* Gains */
    bladerf_lna_gain rx_lna;
    int vga1;
    int vga2;
};
#define RXDCLENGTH 16384

static int16_t cos_fsover8[8]  = {2047,   1447,      0,  -1448,  -2047,  -1448,     0,   1447};
static int16_t cos_3fsover8[8] = {2047,  -1448,      0,   1447,  -2047,   1447,     0,  -1448};


/*! \brief BladeRF specific data structure */
typedef struct {
  //! opaque BladeRF device struct. An empty ("") or NULL device identifier will result in the first encountered device being opened (using the first discovered backend)
  struct bladerf *dev;

  //! Number of buffers
  unsigned int num_buffers;
  //! Buffer size
  unsigned int buffer_size;
  //! Number of transfers
  unsigned int num_transfers;
  //! RX timeout
  unsigned int rx_timeout_ms;
  //! TX timeout
  unsigned int tx_timeout_ms;
  //! Metadata for RX
  struct bladerf_metadata meta_rx;
  //!Metadata for TX
  struct bladerf_metadata meta_tx;
  //! Sample rate
  unsigned int sample_rate;
  //! time offset between transmiter timestamp and receiver timestamp;
  double tdiff;
  //! TX number of forward samples use brf_time_offset to get this value
  int tx_forward_nsamps; //166 for 20Mhz
  // --------------------------------
  // Debug and output control
  // --------------------------------
  //! Number of underflows
  int num_underflows;
  //! Number of overflows
  int num_overflows;
  //! number of sequential errors
  int num_seq_errors;
  //! number of RX errors
  int num_rx_errors;
  //! Number of TX errors
  int num_tx_errors;
  //! timestamp of current TX
  uint64_t tx_current_ts;
  //! timestamp of current RX
  uint64_t rx_current_ts;
  //! number of actual samples transmitted
  uint64_t tx_actual_nsamps;
  //! number of actual samples received
  uint64_t rx_actual_nsamps;
  //! number of TX samples
  uint64_t tx_nsamps;
  //! number of RX samples
  uint64_t rx_nsamps;
  //! number of TX count
  uint64_t tx_count;
  //! number of RX count
  uint64_t rx_count;
  //! timestamp of RX packet
  int64_t rx_timestamp;
} brf_state_t;

int configure_module(struct bladerf *dev, struct module_config *c);
int init_sync_rx(struct bladerf *dev);
int init_sync_tx(struct bladerf *dev);
int graceful_destruct_sync_rx(struct bladerf *dev);
int graceful_destruct_sync_tx(struct bladerf *dev);
int calibrate(struct bladerf *dev);

#endif //__CONFIG_HEADER_INCLUDED__
