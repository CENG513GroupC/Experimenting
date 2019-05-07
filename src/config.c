#include "config.h"


/* The RX and TX modules are configured independently for these parameters */

int configure_module(struct bladerf *dev, struct module_config *c)
{
    int status;
    status = bladerf_set_frequency(dev, c->module, c->frequency);
    if (status != 0) {
        fprintf(stderr, "Failed to set frequency = %u: %s\n",
                c->frequency, bladerf_strerror(status));
        return status;
    }
    status = bladerf_set_sample_rate(dev, c->module, c->samplerate, NULL);
    if (status != 0) {
        fprintf(stderr, "Failed to set samplerate = %u: %s\n",
                c->samplerate, bladerf_strerror(status));
        return status;
    }
    status = bladerf_set_bandwidth(dev, c->module, c->bandwidth, NULL);
    if (status != 0) {
        fprintf(stderr, "Failed to set bandwidth = %u: %s\n",
                c->bandwidth, bladerf_strerror(status));
        return status;
    }
    switch (c->module) {
        case BLADERF_MODULE_RX:
            /* Configure the gains of the RX LNA, RX VGA1, and RX VGA2  */
            status = bladerf_set_lna_gain(dev, c->rx_lna);
            if (status != 0) {
                fprintf(stderr, "Failed to set RX LNA gain: %s\n",
                        bladerf_strerror(status));
                return status;
            }
            status = bladerf_set_rxvga1(dev, c->vga1);
            if (status != 0) {
                fprintf(stderr, "Failed to set RX VGA1 gain: %s\n",
                        bladerf_strerror(status));
                return status;
            }
            status = bladerf_set_rxvga2(dev, c->vga2);
            if (status != 0) {
                fprintf(stderr, "Failed to set RX VGA2 gain: %s\n",
                        bladerf_strerror(status));
                return status;
            }
            break;
        case BLADERF_MODULE_TX:
            /* Configure the TX VGA1 and TX VGA2 gains */
            status = bladerf_set_txvga1(dev, c->vga1);
            if (status != 0) {
                fprintf(stderr, "Failed to set TX VGA1 gain: %s\n",
                        bladerf_strerror(status));
                return status;
            }
            status = bladerf_set_txvga2(dev, c->vga2);
            if (status != 0) {
                fprintf(stderr, "Failed to set TX VGA2 gain: %s\n",
                        bladerf_strerror(status));
                return status;
            }
            break;
        default:
            status = BLADERF_ERR_INVAL;
            fprintf(stderr, "%s: Invalid module specified (%d)\n",
                    __FUNCTION__, c->module);
    }
    return status;
}


int init_sync_rx(struct bladerf *dev)
{
    int status;
    /* These items configure the underlying asynch stream used by the sync
     * interface. The "buffer" here refers to those used internally by worker
     * threads, not the user's sample buffers.
     *
     * It is important to remember that TX buffers will not be submitted to
     * the hardware until `buffer_size` samples are provided via the
     * bladerf_sync_tx call.  Similarly, samples will not be available to
     * RX via bladerf_sync_rx() until a block of `buffer_size` samples has been
     * received.
     */
    const unsigned int num_buffers   = NUMBER_OF_BUFFERS;
    const unsigned int buffer_size   = BUFFER_SIZE;  /* Must be a multiple of 1024 */
    const unsigned int num_transfers = NUMBER_OF_TRANSFERS;
    const unsigned int timeout_ms    = TIMEOUT_IN_MS;
    /* Configure both the device's RX and TX modules for use with the synchronous
     * interface. SC16 Q11 samples *without* metadata are used. */
    status = bladerf_sync_config(dev,
                                 BLADERF_MODULE_RX,
                                 BLADERF_FORMAT_SC16_Q11,
                                 num_buffers,
                                 buffer_size,
                                 num_transfers,
                                 timeout_ms);
    if (status != 0) {
        fprintf(stderr, "Failed to configure RX sync interface: %s\n", bladerf_strerror(status));
        return status;
    }
    status = bladerf_enable_module(dev, BLADERF_MODULE_RX, true);
    if (status != 0) {
        fprintf(stderr, "Failed to enable RX module: %s\n", bladerf_strerror(status));
        return status;
    }
    return status;
}


int init_sync_tx(struct bladerf *dev)
{
    int status;
    /* These items configure the underlying asynch stream used by the sync
     * interface. The "buffer" here refers to those used internally by worker
     * threads, not the user's sample buffers.
     *
     * It is important to remember that TX buffers will not be submitted to
     * the hardware until `buffer_size` samples are provided via the
     * bladerf_sync_tx call.  Similarly, samples will not be available to
     * RX via bladerf_sync_rx() until a block of `buffer_size` samples has been
     * received.
     */
    const unsigned int num_buffers   = NUMBER_OF_BUFFERS;
    const unsigned int buffer_size   = BUFFER_SIZE;  /* Must be a multiple of 1024 */
    const unsigned int num_transfers = NUMBER_OF_TRANSFERS;
    const unsigned int timeout_ms    = TIMEOUT_IN_MS;
    /* Configure both the device's RX and TX modules for use with the synchronous
     * interface. SC16 Q11 samples *without* metadata are used. */

    status = bladerf_sync_config(dev,
                                 BLADERF_MODULE_TX,
                                 BLADERF_FORMAT_SC16_Q11,
                                 num_buffers,
                                 buffer_size,
                                 num_transfers,
                                 timeout_ms);
    if (status != 0) {
        fprintf(stderr, "Failed to configure TX sync interface: %s\n",
                bladerf_strerror(status));
    }
    status = bladerf_enable_module(dev, BLADERF_MODULE_TX, true);
    if (status != 0) {
        fprintf(stderr, "Failed to enable RX module: %s\n",
                bladerf_strerror(status));
        return status;
    }

    return status;
}


int graceful_destruct_sync_rx(struct bladerf *dev)
{
    int status;
    /* Disable RX module, shutting down our underlying RX stream */
    status = bladerf_enable_module(dev, BLADERF_MODULE_RX, false);
    if (status != 0) {
        fprintf(stderr, "Failed to disable RX module: %s\n",
                bladerf_strerror(status));
    }

    return status;

}

int graceful_destruct_sync_tx(struct bladerf *dev)
{
    int status;
    /* Disable RX module, shutting down our underlying RX stream */

    /* Disable TX module, shutting down our underlying TX stream */
    status = bladerf_enable_module(dev, BLADERF_MODULE_TX, false);
    if (status != 0) {
        fprintf(stderr, "Failed to disable TX module: %s\n",
                bladerf_strerror(status));
    }
    return status;

}

int calibrate(struct bladerf *dev)
{
	int status = 0 ;
	status = bladerf_calibrate_dc(dev, BLADERF_DC_CAL_LPF_TUNING);
	status = bladerf_calibrate_dc(dev, BLADERF_DC_CAL_TX_LPF);
	status = bladerf_calibrate_dc(dev, BLADERF_DC_CAL_RX_LPF);
	status = bladerf_calibrate_dc(dev, BLADERF_DC_CAL_RXVGA2);


	return status;
}






/*! \brief Calibrate LMSSDR RF
 * \param device the hardware to use
 */
int calibrate1(struct bladerf *dev)
{
  int status = 0 ;
  brf_state_t brf;
  int64_t ptimestamp;
  int16_t *calib_buffp,*calib_tx_buffp;
  int16_t calib_buff[2*RXDCLENGTH];
  int16_t calib_tx_buff[2*RXDCLENGTH];
  int i,j,offI,offQ,offIold,offQold,offInew,offQnew,offphase,offphaseold,offphasenew,offgain,offgainold,offgainnew;
  int32_t meanI,meanQ,meanIold,meanQold;
  int cnt=0,loop;

  fprintf(stderr, "in calibrate() 1\n");

  // put TX on a far-away frequency to avoid interference in RX band
  bladerf_set_frequency(dev,BLADERF_MODULE_TX, (unsigned int) FREQUENCY_USED + 200e6);
  // Set gains to close to max
  bladerf_set_gain(dev, BLADERF_MODULE_RX, 60);
  bladerf_set_gain(dev, BLADERF_MODULE_TX, 60);


  fprintf(stderr, "in calibrate 2()\n");

  // fill TX buffer with fs/8 complex sinusoid
  j=0;
  for (i=0;i<RXDCLENGTH;i++) {
    calib_tx_buff[j++] = cos_fsover8[i&7];
    calib_tx_buff[j++] = cos_fsover8[(i+6)&7];  // sin
  }
  fprintf(stderr, "in calibrate 2.1()\n");

  calib_buffp = &calib_buff[0];
  calib_tx_buffp = &calib_tx_buff[0];
  // Calibrate RX DC offset
  offIold=offQold=2048;
  fprintf(stderr, "in calibrate 2.2()\n");
  bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_I,offIold);
  bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_Q,offQold);
  fprintf(stderr, "in calibrate 2.3()\n");


  for (i=0;i<10;i++)
    trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);

  fprintf(stderr, "in calibrate 3()\n");

  for (meanIold=meanQold=i=j=0;i<RXDCLENGTH;i++) {
    meanIold+=calib_buff[j++];
    meanQold+=calib_buff[j++];
  }
  meanIold/=RXDCLENGTH;
  meanQold/=RXDCLENGTH;
  printf("[BRF] RX DC: (%d,%d) => (%d,%d)\n",offIold,offQold,meanIold,meanQold);
  offI=offQ=-2048;
  bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_I,offI);
  bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_Q,offQ);


  fprintf(stderr, "in calibrate 4()\n");


  for (i=0;i<10;i++)
    trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);

  for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
    meanI+=calib_buff[j++];
    meanQ+=calib_buff[j++];
  }
  meanI/=RXDCLENGTH;
  meanQ/=RXDCLENGTH;
  //  printf("[BRF] RX DC: (%d,%d) => (%d,%d)\n",offI,offQ,meanI,meanQ);
  while (cnt++ < 12) {
    offInew=(offIold+offI)>>1;
    offQnew=(offQold+offQ)>>1;
    if (meanI*meanI < meanIold*meanIold) {
      meanIold = meanI;
      offIold = offI;
      printf("[BRF] *** RX DC: offI %d => %d\n",offIold,meanI);
    }
    if (meanQ*meanQ < meanQold*meanQold) {
      meanQold = meanQ;
      offQold = offQ;
      printf("[BRF] *** RX DC: offQ %d => %d\n",offQold,meanQ);
    }
    offI = offInew;
    offQ = offQnew;
    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_I,offI);
    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_Q,offQ);
    for (i=0;i<10;i++)
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);

    for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
      meanI+=calib_buff[j++];
      meanQ+=calib_buff[j++];
    }
    meanI/=RXDCLENGTH;
    meanQ/=RXDCLENGTH;
    printf("[BRF] RX DC: (%d,%d) => (%d,%d)\n",offI,offQ,meanI,meanQ);
  }
  printf("[BRF] RX DC: (%d,%d) => (%d,%d)\n",offIold,offQold,meanIold,meanQold);
  bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_I,offIold);
  bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_LMS_DCOFF_Q,offQold);
  // TX DC offset
  // PUT TX as f_RX + fs/4
  // loop back BLADERF_LB_RF_LNA1
  bladerf_set_frequency(dev,BLADERF_MODULE_TX, (unsigned int) FREQUENCY_USED + (unsigned int) SAMPLING_RATE_USED/4);
  bladerf_set_loopback (dev,BLADERF_LB_RF_LNA1);
  offIold=2048;
  bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_I,offIold);
  for (i=0;i<10;i++) {
    trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
    trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
  }
  for (meanIold=meanQold=i=j=0;i<RXDCLENGTH;i++) {
    switch (i&3) {
    case 0:
      meanIold+=calib_buff[j++];
      break;
    case 1:
      meanQold+=calib_buff[j++];
      break;
    case 2:
      meanIold-=calib_buff[j++];
      break;
    case 3:
      meanQold-=calib_buff[j++];
      break;
    }
  }
  //  meanIold/=RXDCLENGTH;
  //  meanQold/=RXDCLENGTH;
  printf("[BRF] TX DC (offI): %d => (%d,%d)\n",offIold,meanIold,meanQold);
  offI=-2048;
  bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_I,offI);
  for (i=0;i<10;i++) {
    trx_brf_read(dev, &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
    trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
  }
  for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
    switch (i&3) {
    case 0:
      meanI+=calib_buff[j++];
      break;
    case 1:
      meanQ+=calib_buff[j++];
      break;
    case 2:
      meanI-=calib_buff[j++];
      break;
    case 3:
      meanQ-=calib_buff[j++];
      break;
    }
  }
  //  meanI/=RXDCLENGTH;
  //  meanQ/=RXDCLENGTH;
  printf("[BRF] TX DC (offI): %d => (%d,%d)\n",offI,meanI,meanQ);
  cnt = 0;
  while (cnt++ < 12) {
    offInew=(offIold+offI)>>1;
    if (meanI*meanI+meanQ*meanQ < meanIold*meanIold +meanQold*meanQold) {
      printf("[BRF] TX DC (offI): ([%d,%d]) => %d : %d\n",offIold,offI,offInew,meanI*meanI+meanQ*meanQ);
      meanIold = meanI;
      meanQold = meanQ;
      offIold = offI;
    }
    offI = offInew;
    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_I,offI);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
      switch (i&3) {
      case 0:
	meanI+=calib_buff[j++];
	break;
      case 1:
	meanQ+=calib_buff[j++];
	break;
      case 2:
	meanI-=calib_buff[j++];
	break;
      case 3:
	meanQ-=calib_buff[j++];
	break;
      }
    }
    //    meanI/=RXDCLENGTH;
    //   meanQ/=RXDCLENGTH;
    //    printf("[BRF] TX DC (offI): %d => (%d,%d)\n",offI,meanI,meanQ);
  }
  bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_I,offIold);
  offQold=2048;
  bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_Q,offQold);
  for (i=0;i<10;i++) {
    trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
    trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
  }
  // project on fs/4
  for (meanIold=meanQold=i=j=0;i<RXDCLENGTH;i++) {
    switch (i&3) {
    case 0:
      meanIold+=calib_buff[j++];
      break;
    case 1:
      meanQold+=calib_buff[j++];
      break;
    case 2:
      meanIold-=calib_buff[j++];
      break;
    case 3:
      meanQold-=calib_buff[j++];
      break;
    }
  }
  //  meanIold/=RXDCLENGTH;
  //  meanQold/=RXDCLENGTH;
  printf("[BRF] TX DC (offQ): %d => (%d,%d)\n",offQold,meanIold,meanQold);
  offQ=-2048;
  bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_Q,offQ);
  for (i=0;i<10;i++) {
    trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
    trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
  }
  for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
    switch (i&3) {
    case 0:
      meanI+=calib_buff[j++];
      break;
    case 1:
      meanQ+=calib_buff[j++];
      break;
    case 2:
      meanI-=calib_buff[j++];
      break;
    case 3:
      meanQ-=calib_buff[j++];
      break;
    }
  }
  //  meanI/=RXDCLENGTH;
  //  meanQ/=RXDCLENGTH;
  printf("[BRF] TX DC (offQ): %d => (%d,%d)\n",offQ,meanI,meanQ);
  cnt=0;
  while (cnt++ < 12) {
    offQnew=(offQold+offQ)>>1;
    if (meanI*meanI+meanQ*meanQ < meanIold*meanIold +meanQold*meanQold) {
      printf("[BRF] TX DC (offQ): ([%d,%d]) => %d : %d\n",offQold,offQ,offQnew,meanI*meanI+meanQ*meanQ);
      meanIold = meanI;
      meanQold = meanQ;
      offQold = offQ;
    }
    offQ = offQnew;
    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_Q,offQ);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
      switch (i&3) {
      case 0:
	meanI+=calib_buff[j++];
	break;
      case 1:
	meanQ+=calib_buff[j++];
	break;
      case 2:
	meanI-=calib_buff[j++];
	break;
      case 3:
	meanQ-=calib_buff[j++];
	break;
      }
    }
    //    meanI/=RXDCLENGTH;
    //   meanQ/=RXDCLENGTH;
    //    printf("[BRF] TX DC (offQ): %d => (%d,%d)\n",offQ,meanI,meanQ);
  }
  printf("[BRF] TX DC: (%d,%d) => (%d,%d)\n",offIold,offQold,meanIold,meanQold);
  bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_LMS_DCOFF_Q,offQold);
  // TX IQ imbalance
  for (loop=0;loop<2;loop++) {
    offphaseold=4096;
    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_PHASE,offphaseold);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    // project on fs/8 (Image of TX signal in +ve frequencies)
    for (meanIold=meanQold=i=j=0;i<RXDCLENGTH;i++) {
      meanIold+= (calib_buff[j]*cos_fsover8[i&7] - calib_buff[j+1]*cos_fsover8[(i+2)&7])>>11;
      meanQold+= (calib_buff[j]*cos_fsover8[(i+2)&7] + calib_buff[j+1]*cos_fsover8[i&7])>>11;
      j+=2;
    }

    meanIold/=RXDCLENGTH;
    meanQold/=RXDCLENGTH;
    printf("[BRF] TX IQ (offphase): %d => (%d,%d)\n",offphaseold,meanIold,meanQold);

    offphase=-4096;
    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_PHASE,offphase);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    // project on fs/8 (Image of TX signal in +ve frequencies)
    for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
      meanI+= (calib_buff[j]*cos_fsover8[i&7] - calib_buff[j+1]*cos_fsover8[(i+2)&7])>>11;
      meanQ+= (calib_buff[j]*cos_fsover8[(i+2)&7] + calib_buff[j+1]*cos_fsover8[i&7])>>11;
      j+=2;
    }

    meanI/=RXDCLENGTH;
    meanQ/=RXDCLENGTH;
    printf("[BRF] TX IQ (offphase): %d => (%d,%d)\n",offphase,meanI,meanQ);

    cnt=0;
    while (cnt++ < 13) {

      offphasenew=(offphaseold+offphase)>>1;
      printf("[BRF] TX IQ (offphase): ([%d,%d]) => %d : %d\n",offphaseold,offphase,offphasenew,meanI*meanI+meanQ*meanQ);
      if (meanI*meanI+meanQ*meanQ < meanIold*meanIold +meanQold*meanQold) {

	meanIold = meanI;
	meanQold = meanQ;
	offphaseold = offphase;
      }
      offphase = offphasenew;
      bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_PHASE,offphase);

      for (i=0;i<10;i++) {
	trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
	trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
      }
      // project on fs/8 (Image of TX signal in +ve frequencies)
      for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
	meanI+= (calib_buff[j]*cos_fsover8[i&7] - calib_buff[j+1]*cos_fsover8[(i+2)&7])>>11;
	meanQ+= (calib_buff[j]*cos_fsover8[(i+2)&7] + calib_buff[j+1]*cos_fsover8[i&7])>>11;
	j+=2;
      }
      meanI/=RXDCLENGTH;
      meanQ/=RXDCLENGTH;

      //    printf("[BRF] TX DC (offQ): %d => (%d,%d)\n",offQ,meanI,meanQ);
    }

    printf("[BRF] TX IQ offphase: %d => (%d,%d)\n",offphaseold,meanIold,meanQold);

    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_PHASE,offphaseold);

    offgainold=4096;
    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_GAIN,offgainold);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    // project on fs/8 (Image of TX signal in +ve frequencies)
    for (meanIold=meanQold=i=j=0;i<RXDCLENGTH;i++) {
      meanIold+= (calib_buff[j]*cos_fsover8[i&7] - calib_buff[j+1]*cos_fsover8[(i+2)&7])>>11;
      meanQold+= (calib_buff[j]*cos_fsover8[(i+2)&7] + calib_buff[j+1]*cos_fsover8[i&7])>>11;
      j+=2;
    }

    meanIold/=RXDCLENGTH;
    meanQold/=RXDCLENGTH;
    printf("[BRF] TX IQ (offgain): %d => (%d,%d)\n",offgainold,meanIold,meanQold);

    offgain=-4096;
    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_GAIN,offgain);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    // project on fs/8 (Image of TX signal in +ve frequencies)
    for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
      meanI+= (calib_buff[j]*cos_fsover8[i&7] - calib_buff[j+1]*cos_fsover8[(i+2)&7])>>11;
      meanQ+= (calib_buff[j]*cos_fsover8[(i+2)&7] + calib_buff[j+1]*cos_fsover8[i&7])>>11;
      j+=2;
    }

    meanI/=RXDCLENGTH;
    meanQ/=RXDCLENGTH;
    printf("[BRF] TX IQ (offgain): %d => (%d,%d)\n",offgain,meanI,meanQ);

    cnt=0;
    while (cnt++ < 13) {

      offgainnew=(offgainold+offgain)>>1;
      if (meanI*meanI+meanQ*meanQ < meanIold*meanIold +meanQold*meanQold) {
	printf("[BRF] TX IQ (offgain): ([%d,%d]) => %d : %d\n",offgainold,offgain,offgainnew,meanI*meanI+meanQ*meanQ);

	meanIold = meanI;
	meanQold = meanQ;
	offgainold = offgain;
      }
      offgain = offgainnew;
      bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_GAIN,offgain);

      for (i=0;i<10;i++) {
	trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
	trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
      }
      // project on fs/8 (Image of TX signal in +ve frequencies)
      for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
	meanI+= (calib_buff[j]*cos_fsover8[i&7] - calib_buff[j+1]*cos_fsover8[(i+2)&7])>>11;
	meanQ+= (calib_buff[j]*cos_fsover8[(i+2)&7] + calib_buff[j+1]*cos_fsover8[i&7])>>11;
	j+=2;
      }
      meanI/=RXDCLENGTH;
      meanQ/=RXDCLENGTH;

      //    printf("[BRF] TX DC (offQ): %d => (%d,%d)\n",offQ,meanI,meanQ);
    }

    printf("[BRF] TX IQ offgain: %d => (%d,%d)\n",offgainold,meanIold,meanQold);

    bladerf_set_correction(dev,BLADERF_MODULE_TX,BLADERF_CORR_FPGA_GAIN,offgainold);
  }
  // RX IQ imbalance
  for (loop=0;loop<2;loop++) {
    offphaseold=4096;
    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_PHASE,offphaseold);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    // project on -3fs/8 (Image of TX signal in -ve frequencies)
    for (meanIold=meanQold=i=j=0;i<RXDCLENGTH;i++) {
      meanIold+= (calib_buff[j]*cos_3fsover8[i&7] - calib_buff[j+1]*cos_3fsover8[(i+2)&7])>>11;
      meanQold+= (calib_buff[j]*cos_3fsover8[(i+2)&7] + calib_buff[j+1]*cos_3fsover8[i&7])>>11;
      j+=2;
    }

    meanIold/=RXDCLENGTH;
    meanQold/=RXDCLENGTH;
    printf("[BRF] RX IQ (offphase): %d => (%d,%d)\n",offphaseold,meanIold,meanQold);

    offphase=-4096;
    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_PHASE,offphase);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    // project on -3fs/8 (Image of TX signal in -ve frequencies)
    for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
      meanI+= (calib_buff[j]*cos_3fsover8[i&7] - calib_buff[j+1]*cos_3fsover8[(i+2)&7])>>11;
      meanQ+= (calib_buff[j]*cos_3fsover8[(i+2)&7] + calib_buff[j+1]*cos_3fsover8[i&7])>>11;
      j+=2;
    }

    meanI/=RXDCLENGTH;
    meanQ/=RXDCLENGTH;
    printf("[BRF] RX IQ (offphase): %d => (%d,%d)\n",offphase,meanI,meanQ);

    cnt=0;
    while (cnt++ < 13) {

      offphasenew=(offphaseold+offphase)>>1;
      printf("[BRF] RX IQ (offphase): ([%d,%d]) => %d : %d\n",offphaseold,offphase,offphasenew,meanI*meanI+meanQ*meanQ);
      if (meanI*meanI+meanQ*meanQ < meanIold*meanIold +meanQold*meanQold) {

	meanIold = meanI;
	meanQold = meanQ;
	offphaseold = offphase;
      }
      offphase = offphasenew;
      bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_PHASE,offphase);

      for (i=0;i<10;i++) {
	trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
	trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
      }
      // project on -3fs/8 (Image of TX signal in -ve frequencies)
      for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
	meanI+= (calib_buff[j]*cos_3fsover8[i&7] - calib_buff[j+1]*cos_3fsover8[(i+2)&7])>>11;
	meanQ+= (calib_buff[j]*cos_3fsover8[(i+2)&7] + calib_buff[j+1]*cos_3fsover8[i&7])>>11;
	j+=2;
      }
      meanI/=RXDCLENGTH;
      meanQ/=RXDCLENGTH;

      //    printf("[BRF] TX DC (offQ): %d => (%d,%d)\n",offQ,meanI,meanQ);
    }

    printf("[BRF] RX IQ offphase: %d => (%d,%d)\n",offphaseold,meanIold,meanQold);

    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_PHASE,offphaseold);

    offgainold=4096;
    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_GAIN,offgainold);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0,0);
    }
    // project on -3fs/8 (Image of TX signal in +ve frequencies)
    for (meanIold=meanQold=i=j=0;i<RXDCLENGTH;i++) {
      meanIold+= (calib_buff[j]*cos_3fsover8[i&7] - calib_buff[j+1]*cos_3fsover8[(i+2)&7])>>11;
      meanQold+= (calib_buff[j]*cos_3fsover8[(i+2)&7] + calib_buff[j+1]*cos_3fsover8[i&7])>>11;
      j+=2;
    }

    meanIold/=RXDCLENGTH;
    meanQold/=RXDCLENGTH;
    printf("[BRF] RX IQ (offgain): %d => (%d,%d)\n",offgainold,meanIold,meanQold);

    offgain=-4096;
    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_GAIN,offgain);
    for (i=0;i<10;i++) {
      trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
      trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
    }
    // project on 3fs/8 (Image of TX signal in -ve frequencies)
    for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
      meanI+= (calib_buff[j]*cos_3fsover8[i&7] - calib_buff[j+1]*cos_3fsover8[(i+2)&7])>>11;
      meanQ+= (calib_buff[j]*cos_3fsover8[(i+2)&7] + calib_buff[j+1]*cos_3fsover8[i&7])>>11;
      j+=2;
    }

    meanI/=RXDCLENGTH;
    meanQ/=RXDCLENGTH;
    printf("[BRF] RX IQ (offgain): %d => (%d,%d)\n",offgain,meanI,meanQ);

    cnt=0;
    while (cnt++ < 13) {

      offgainnew=(offgainold+offgain)>>1;
      if (meanI*meanI+meanQ*meanQ < meanIold*meanIold +meanQold*meanQold) {
	printf("[BRF] RX IQ (offgain): ([%d,%d]) => %d : %d\n",offgainold,offgain,offgainnew,meanI*meanI+meanQ*meanQ);

	meanIold = meanI;
	meanQold = meanQ;
	offgainold = offgain;
      }
      offgain = offgainnew;
      bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_GAIN,offgain);

      for (i=0;i<10;i++) {
	trx_brf_read(dev,  &ptimestamp, (void **)&calib_buffp, RXDCLENGTH, 0);
	trx_brf_write(dev, ptimestamp+5*RXDCLENGTH, (void **)&calib_tx_buffp, RXDCLENGTH, 0, 0);
      }
      // project on -3fs/8 (Image of TX signal in -ve frequencies)
      for (meanI=meanQ=i=j=0;i<RXDCLENGTH;i++) {
	meanI+= (calib_buff[j]*cos_3fsover8[i&7] - calib_buff[j+1]*cos_3fsover8[(i+2)&7])>>11;
	meanQ+= (calib_buff[j]*cos_3fsover8[(i+2)&7] + calib_buff[j+1]*cos_3fsover8[i&7])>>11;
	j+=2;
      }
      meanI/=RXDCLENGTH;
      meanQ/=RXDCLENGTH;

      //    printf("[BRF] TX DC (offQ): %d => (%d,%d)\n",offQ,meanI,meanQ);
    }

    printf("[BRF] RX IQ offgain: %d => (%d,%d)\n",offgainold,meanIold,meanQold);

    bladerf_set_correction(dev,BLADERF_MODULE_RX,BLADERF_CORR_FPGA_GAIN,offgainold);
  }
  bladerf_set_frequency(dev,BLADERF_MODULE_TX, (unsigned int) FREQUENCY_USED);
  bladerf_set_loopback(dev,BLADERF_LB_NONE);
  bladerf_set_gain(dev, BLADERF_MODULE_RX, (unsigned int) BLADERF_RXVGA1_GAIN_MAX);
  bladerf_set_gain(dev, BLADERF_MODULE_TX, (unsigned int) BLADERF_TXVGA1_GAIN_MAX);
  //  write_output("blade_rf_test.m","rxs",calib_buff,RXDCLENGTH,1,1);
  return status;
}
