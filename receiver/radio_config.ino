
/* -------------------------- LoRa Config -------------------------- */

HAL_StatusTypeDef LoRaConfig(SX127X_t* SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t InitErrorCounter = 0;

  // Basic stuff first:
  SX127X->spi_bus = radio_spi;
  SX127X->ss_pin = RADIO_SS;
  SX127X->reset_pin = RADIO_NRST;

  status = SX127X_Reset(SX127X);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_op_mode(SX127X, SLEEP);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_modulation(SX127X, LORA_Modulation);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_frequency(SX127X, 915E6); // remember to put the LoWFrequencyModeOn bit to its correct position
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_lna_gain(SX127X, LnaGainG1);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_lna_boost(SX127X, true);
  if (status != HAL_OK) InitErrorCounter++;


  // TX Config:
  status = LoRa_set_FIFO_base_address(SX127X, 0x00, 0x00);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_pa_output(SX127X, PA_BOOST_Pin);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_tx_power(SX127X, 20);
  if (status != HAL_OK) InitErrorCounter++;


  // Specific Config:
  status = LoRa_set_signal_bandwidth(SX127X, LoRa_BW_125);
  if (status != HAL_OK) InitErrorCounter++;
  
  status = LoRa_set_spreading_factor(SX127X, 11);
  if (status != HAL_OK) InitErrorCounter++;

  status = LoRa_set_coding_rate(SX127X, LoRa_CR_4_5);
  if (status != HAL_OK) InitErrorCounter++;

  status = LoRa_set_preamble_lenght(SX127X, 8);
  if (status != HAL_OK) InitErrorCounter++;

  status = LoRa_set_sync_word(SX127X, 'A');
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_DIO_mapping(SX127X, 0b00000000, 0b00000000);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_op_mode(SX127X, STANDBY); 
  if (status != HAL_OK) InitErrorCounter++;

  if(InitErrorCounter) return HAL_ERROR;
  else  return HAL_OK;
}



/* -------------------------- FSK Config -------------------------- */

HAL_StatusTypeDef FSKConfig(SX127X_t* SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t InitErrorCounter = 0;

  /*
     Fdev and BitRate must respect the following formula: 0.5 <= (2*Fdev)/(BitRate) <= 10
     Also, Fdev + (BitRate/2) <= 250 kHz   and   600 Hz < Fdev < 200kHz
     BitRate must also respect: BitRate < 2*Bandwidth
  */

  // Basic stuff first:
  SX127X->spi_bus = radio_spi;
  SX127X->ss_pin = RADIO_SS;
  SX127X->reset_pin = RADIO_NRST;

  status = SX127X_Reset(SX127X);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_op_mode(SX127X, SLEEP);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_modulation(SX127X, FSK_Modulation);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_frequency(SX127X, 914E6); // remember to put the LoWFrequencyModeOn bit to its correct position
  if (status != HAL_OK) InitErrorCounter++;


  // FSK Specific Settings:
  status = FSK_set_freq_deviation(SX127X, 30000, FSK_BITRATE_38_4_KBPS); // Bitrate.BitrateValue
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_bitrate(SX127X, FSK_BITRATE_38_4_KBPS); // Bitrate.RegisterValue
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_preamb_len(SX127X, 20, Polarity_AA);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_sync_word(SX127X, 4, 0xF0F01234);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_packet_format(SX127X, VariableLength);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_data_processing_mode(SX127X, PacketMode);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_payload_length(SX127X, 255); // This represents different things in different modes. In this case: Max RX length. More on the .h file.
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_enable_crc(SX127X);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_crc_autoclear(SX127X, false);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_encoding(SX127X, Whitenning);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_data_shaping(SX127X, Gaussian_1_0);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_pa_ramp_time(SX127X, PaRampTime_40_us);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_auto_restart_RX(SX127X, AutorestartRX_Off);
  if (status != HAL_OK) InitErrorCounter++;


  // RX config:
  status = FSK_set_rx_bandwidth(SX127X, FSK_BW_83_3_KHZ);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_afc_bandwidth(SX127X, FSK_BW_83_3_KHZ);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_enable_preamble_detector(SX127X, 2, 10);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_lna_gain(SX127X, LnaGainG1);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_lna_boost(SX127X, true);
  if (status != HAL_OK) InitErrorCounter++;

  status = FSK_set_rssi_smoothing(SX127X, RssiSmoothing_8_Samples);
  if (status != HAL_OK) InitErrorCounter++;


  // TX Config:
  status = SX127X_set_pa_output(SX127X, PA_BOOST_Pin);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_tx_power(SX127X, 20);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_DIO_mapping(SX127X, 0b00001000, 0b11110001);
  if (status != HAL_OK) InitErrorCounter++;

  status = SX127X_set_op_mode(SX127X, STANDBY);
  if (status != HAL_OK) InitErrorCounter++;

  if(InitErrorCounter) return HAL_ERROR;
  else  return HAL_OK;
}
