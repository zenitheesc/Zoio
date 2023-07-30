/*
   SX127X.c
   Author: Júlio Calandrin
   Made with care, hate and sweat by someone that doesn't really knows what it's doing.

*/


/*
   INCLUDES
*/

#include "SX127X.h"

/*
   Function defines
*/

#define READ_REG(reg_addr, pvalue) \
  SPI_read_register(SX127X->spi_bus, SX127X->ss_pin, reg_addr, pvalue)

#define BURST_READ(start_addr, pvalue, size) \
  SPI_burst_read(SX127X->spi_bus, SX127X->ss_pin, start_addr, pvalue, size)

#define WRITE_REG(reg_addr, value) \
  SPI_write_register(SX127X->spi_bus, SX127X->ss_pin, reg_addr, value)

#define BURST_WRITE(start_addr, pvalues, size) \
  SPI_burst_write(SX127X->spi_bus, SX127X->ss_pin, start_addr, pvalues, size)


HAL_StatusTypeDef FSK_init(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;

  // didnt configure anything, all DEFAULT.
  status = SX127X_Reset(SX127X);
  RETURN_ON_ERROR(status);

  return HAL_OK;
}

// NEW CODE

HAL_StatusTypeDef SX127X_Config(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;

  // Universal Configs:

  status = SX127X_Reset(SX127X);
  RETURN_ON_ERROR(status);

  status = SX127X_set_op_mode(SX127X, SLEEP);
  RETURN_ON_ERROR(status);

  status = SX127X_set_modulation(SX127X, SX127X->Current_Modulation);
  RETURN_ON_ERROR(status);

  status = SX127X_set_frequency(SX127X, SX127X->Frequency);
  RETURN_ON_ERROR(status);

  status = FSK_set_pa_ramp_time(SX127X, SX127X->PaRampTime);
  RETURN_ON_ERROR(status);

  status = SX127X_set_lna_gain(SX127X, SX127X->LnaGain);
  RETURN_ON_ERROR(status);

  status = SX127X_set_lna_boost(SX127X, SX127X->LnaBoost);
  RETURN_ON_ERROR(status);

  status = SX127X_set_pa_output(SX127X, SX127X->PaSelect);
  RETURN_ON_ERROR(status);

  status = SX127X_set_tx_power(SX127X, SX127X->TxPower);
  RETURN_ON_ERROR(status);


  if (SX127X->Current_Modulation == FSK_Modulation) {

    status = FSK_set_freq_deviation(SX127X, SX127X->FSK_Config.FrequencyDeviation, SX127X->FSK_Config.Bitrate);  // Bitrate.BitrateValue
    RETURN_ON_ERROR(status);

    status = FSK_set_bitrate(SX127X, SX127X->FSK_Config.Bitrate);  // Bitrate.RegisterValue
    RETURN_ON_ERROR(status);

    status = FSK_set_preamb_len(SX127X, SX127X->FSK_Config.PreambleLength, SX127X->FSK_Config.PreamblePolarity);
    RETURN_ON_ERROR(status);

    if (SX127X->FSK_Config.SyncWordGeneration == true) {
      status = FSK_set_sync_word(SX127X, SX127X->FSK_Config.SyncWordSize, SX127X->FSK_Config.SyncWord);
      RETURN_ON_ERROR(status);
    } else {
      status = FSK_disable_sync_word(SX127X);
      RETURN_ON_ERROR(status);
    }

    status = FSK_set_packet_format(SX127X, SX127X->FSK_Config.PacketFormat);
    RETURN_ON_ERROR(status);

    status = FSK_set_data_processing_mode(SX127X, SX127X->FSK_Config.DataProcessingMode);
    RETURN_ON_ERROR(status);

    status = FSK_set_payload_length(SX127X, SX127X->FSK_Config.PayloadLength);
    RETURN_ON_ERROR(status);

    if (SX127X->FSK_Config.CrcCalculation == true) {
      status = FSK_enable_crc(SX127X);
      RETURN_ON_ERROR(status);
      status = FSK_set_crc_whitenning_type(SX127X, SX127X->FSK_Config.CrcWhitenningType);
      RETURN_ON_ERROR(status);
      status = FSK_set_crc_autoclear(SX127X, SX127X->FSK_Config.CrcAutoclear);
      RETURN_ON_ERROR(status);

    } else {
      status = FSK_disable_crc(SX127X);
      RETURN_ON_ERROR(status);
    }


    status = FSK_set_encoding(SX127X, SX127X->FSK_Config.Encoding);
    RETURN_ON_ERROR(status);

    status = FSK_set_data_shaping(SX127X, SX127X->FSK_Config.DataShaping);
    RETURN_ON_ERROR(status);

    status = FSK_set_auto_restart_RX(SX127X, SX127X->FSK_Config.AutoRestartRX);
    RETURN_ON_ERROR(status);


    // RX config:

    status = FSK_set_rx_bandwidth(SX127X, SX127X->FSK_Config.RxBandwidth);
    RETURN_ON_ERROR(status);

    status = FSK_set_afc_bandwidth(SX127X, SX127X->FSK_Config.AfcBandwidth);
    RETURN_ON_ERROR(status);


    if (SX127X->FSK_Config.PreambleDetectorOn == true) {
      status = FSK_enable_preamble_detector(SX127X, SX127X->FSK_Config.PreambleDetectorSize, SX127X->FSK_Config.PreambleDetectorTolerance);
      RETURN_ON_ERROR(status);
    } else {
      status = FSK_disable_preamble_detector(SX127X);
      RETURN_ON_ERROR(status);
    }


    /*
      status = FSK_set_auto_restart_rx(SX127X, SX127X->FSK_Config.AutoRestartRX);
      RETURN_ON_ERROR(status);

      status = FSK_set_restart_rx_on_collision(SX127X, SX127X->FSK_Config.RestartRxOnCollision);
      RETURN_ON_ERROR(status);
    */

    /*
      if(SX127X->FSK_Config.AddressFiltering != NoFiltering){
    	status = FSK_set_address_filter(SX127X, SX127X->FSK_Config.NodeAdress, SX127X->FSK_Config.BroadcastAddress);
    	RETURN_ON_ERROR(status);
      }else{
    	status = FSK_disable_address_filter(SX127X);
    	RETURN_ON_ERROR(status);
      }
    */

    status = FSK_set_rssi_offset(SX127X, SX127X->FSK_Config.RssiOffset);
    RETURN_ON_ERROR(status);

    status = FSK_set_rssi_smoothing(SX127X, SX127X->FSK_Config.RssiSmoothing);
    RETURN_ON_ERROR(status);

    // DIO config: (perhaps joining them in a single function that gets which DIO pin we are talking about may be useful)
    // For that we would need to make all dio functions fit in a  single enum for a single data type

    // For now, I'm reusing the function from the last version:
    status = SX127X_set_DIO_mapping(SX127X, SX127X->DioMapping1, SX127X->DioMapping2);
    RETURN_ON_ERROR(status);

    /*
      status = FSK_set_dio0_function(SX127X, SX127X->FSK_Config.DIO0Function);
      RETURN_ON_ERROR(status);

      status = FSK_set_dio1_function(SX127X, SX127X->FSK_Config.DIO1Function);
      RETURN_ON_ERROR(status);

      status = FSK_set_dio2_function(SX127X, SX127X->FSK_Config.DIO2Function);
      RETURN_ON_ERROR(status);

      status = FSK_set_dio3_function(SX127X, SX127X->FSK_Config.DIO3Function);
      RETURN_ON_ERROR(status);

      status = FSK_set_dio4_function(SX127X, SX127X->FSK_Config.DIO4Function);
      RETURN_ON_ERROR(status);

      status = FSK_set_dio5_function(SX127X, SX127X->FSK_Config.DIO5Function);
      RETURN_ON_ERROR(status);
    */
  }

  status = SX127X_set_op_mode(SX127X, STANDBY);
  RETURN_ON_ERROR(status);

  return status;
}

HAL_StatusTypeDef SX127X_Reset(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;

  //HAL_GPIO_WritePin(SX127X->reset_gpio_port, SX127X->reset_pin, GPIO_PIN_RESET);
  digitalWrite(SX127X->reset_pin, LOW);
  delay(30);

  //HAL_GPIO_WritePin(SX127X->reset_gpio_port, SX127X->reset_pin, GPIO_PIN_SET);
  digitalWrite(SX127X->reset_pin, HIGH);
  delay(30);
  return status;
}

HAL_StatusTypeDef SX127X_set_op_mode(SX127X_t *SX127X, OperationMode_t mode) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Used to preserve existing bits that should not be changed.
  status = READ_REG(SX127X_REG_OP_MODE, &read_value);
  RETURN_ON_ERROR(status);

  // If the radio is in LoRa and you want to put it in RX mode (cont or single) it needs to be in standby or sleep first.
  if ((SX127X->Current_Modulation == LORA_Modulation) && (mode == RX || mode == RXSINGLE)) {
    status = WRITE_REG(SX127X_REG_OP_MODE, ((read_value & SX127X_OP_MODE_MASK) | STANDBY));
    RETURN_ON_ERROR(status);
  }

  delay(10);

  status = WRITE_REG(SX127X_REG_OP_MODE, ((read_value & SX127X_OP_MODE_MASK) | mode));
  RETURN_ON_ERROR(status);

  // Now we update the OpMode in the main radio object.
  SX127X->Current_Op_Mode = mode;

  return status;
}

HAL_StatusTypeDef SX127X_set_modulation(SX127X_t *SX127X, Modulation_t modulation) {
  HAL_StatusTypeDef status = HAL_OK;
  OperationMode_t previous_op_mode = (OperationMode_t)0xFF;  // The initial value is used to indicate that a change has been made further down.
  uint8_t write_value = 0x00;
  uint8_t read_value = 0x00;

  // To change between FSK/OOK and LoRa the module must be in sleep mode:
  if (SX127X->Current_Op_Mode != SLEEP) {
    previous_op_mode = SX127X->Current_Op_Mode;
    status = SX127X_set_op_mode(SX127X, SLEEP);
    RETURN_ON_ERROR(status);
  }

  // Used to preserve existing bits that should not be changed.
  status = READ_REG(SX127X_REG_OP_MODE, &read_value);
  RETURN_ON_ERROR(status);

  if (modulation == FSK_Modulation) {  // Replace by if(modulation == FSK_Modulation || modulation == OOK_Modulation) when adding OOK
    // Change the three bits responsible for FSK/OOK configuration
    write_value = ((read_value & SX127X_FSK_OOK_MODE_MASK) | modulation);
  } else if (modulation == LORA_Modulation) {  // Remove this other if when adding OOK, bc there will be no other options
    write_value = ((read_value & SX127X_LORA_MODE_MASK) | modulation);
  }

  status = WRITE_REG(SX127X_REG_OP_MODE, write_value);
  RETURN_ON_ERROR(status);

  // We return to the original operation mode if it was modified:
  if (previous_op_mode != 0xFF) {
    status = SX127X_set_op_mode(SX127X, previous_op_mode);
  }

  return (status);
}


HAL_StatusTypeDef SX127X_set_frequency(SX127X_t *SX127X, long frequency) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0x00;
  uint8_t write_value = 0x00;
  OperationMode_t previous_op_mode = (OperationMode_t)0xFF;
  uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

  // To change the frequency the module must be in either sleep or standby mode:
  if (SX127X->Current_Op_Mode != SLEEP && SX127X->Current_Op_Mode != STANDBY) {
    previous_op_mode = SX127X->Current_Op_Mode;
    status = SX127X_set_op_mode(SX127X, STANDBY);
    RETURN_ON_ERROR(status);
  }

  status = WRITE_REG(SX127X_REG_FRF_MSB, (uint8_t)(frf >> 16));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(SX127X_REG_FRF_MID, (uint8_t)(frf >> 8));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(SX127X_REG_FRF_LSB, (uint8_t)(frf));
  RETURN_ON_ERROR(status);

  // Save the value in the radio's struct:
  if (frequency != SX127X->Frequency) SX127X->Frequency = frequency;

  // Now we set the LowFrequencyModeOn bit on the Op Mode register according to the frequency.

  // Used to preserve existing bits that should not be changed.
  status = READ_REG(SX127X_REG_OP_MODE, &read_value);
  RETURN_ON_ERROR(status);

  if (frequency > 525E6) {
    write_value = (read_value & SX127X_LOW_FREQUENCY_MODE_ON_MASK);  // Reset the 4th bit
  } else {
    write_value = (read_value | ((uint8_t)~SX127X_LOW_FREQUENCY_MODE_ON_MASK));  // Set the 4th bit
  }

  status = WRITE_REG(SX127X_REG_OP_MODE, write_value);

  // We return to the original operation mode if it was modified:
  if (previous_op_mode != 0xFF) {
    status = SX127X_set_op_mode(SX127X, previous_op_mode);
  }

  return (status);
}

HAL_StatusTypeDef FSK_WriteToFIFO(SX127X_t *SX127X, uint8_t *Data, uint8_t NumberOfBytes) {
  HAL_StatusTypeDef status;

  if (NumberOfBytes > 1) {
    status = BURST_WRITE(SX127X_REG_FIFO, Data, NumberOfBytes);

  } else {
    status = WRITE_REG(SX127X_REG_FIFO, *Data);
  }

  return status;
}

HAL_StatusTypeDef FSK_ReadFromFIFO(SX127X_t *SX127X, uint8_t *Packet, uint8_t Length) {
  HAL_StatusTypeDef status;

  status = BURST_READ(SX127X_REG_FIFO, Packet, Length);
  RETURN_ON_ERROR(status);

  return status;
}

// WARNING: This function only transmits up to 63 bytes (one extra for the payload length).
HAL_StatusTypeDef FSK_Transmit(SX127X_t *SX127X, uint8_t *Data, uint8_t PayloadLength) {
  HAL_StatusTypeDef status = HAL_OK;

  // Put to standby mode
  status = SX127X_set_op_mode(SX127X, STANDBY);

  // Configure the FIFO threshold to a value smaller than the payload length so that the top
  // level sequencer doesn't scream at us:
  if (PayloadLength <= 1) {
    status = FSK_set_FIFO_threshold(SX127X, (uint8_t)1)
      RETURN_ON_ERROR(status);
  } else {
    status = FSK_set_FIFO_threshold(SX127X, (uint8_t)(PayloadLength - 1));
    RETURN_ON_ERROR(status);
  }

  // If we are in Variable Packet Length Mode we must put the PayloadLength at the beginning of the FIFO first.
  if (SX127X->FSK_Config.PacketFormat == VariableLength) FSK_WriteToFIFO(SX127X, &PayloadLength, 1);

  FSK_WriteToFIFO(SX127X, Data, PayloadLength);

  // Configuring the Top Level Sequencer for automatic shutdown after packet transmission (Read page 101)
  status = WRITE_REG(FSK_REG_SEQ_CONFIG_1, 0b00010000);  //Start -> Transmit -> LowPowerSelection(off)
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SEQ_CONFIG_2, 0b00010000);  //Just in case, if RXTimeout happens, it goes to LowPowerSelection
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_TIMER_RESOL, 0b00000000);  //Disables Top Level Sequencer's timers.
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SEQ_CONFIG_1, 0b10010000);  //Turns TopLevelSequencer on: Start -> Transmit -> LowPowerSelection(off)
  RETURN_ON_ERROR(status);

  return status;
  //REMEMBER TO CHECK THE FLAG PACKETSENT before changing the mode
}

/*
   WARNING: This function is SLOW* and blocking!
   Unlike the normal transmit function in which we set everything up, put in TX and forget it, here we have to manually
   detect if we have room in the FIFO to fill in another byte until ALL bytes have been sent.

 * * Dynamic transmit and receive functions only work until the SPI can keep up with the incoming/transmitting data.
   The highest bitrate I was able to achieve was 76,8 kbps (when receiving packets smaller than 64 bytes with the ReadPacket
   function). After that the SPI (not overclocked) is not fast enough too feed data into the FIFO, resulting in duplicate
   bytes in the receiver (the FIFO got emptied and transmitted the last known byte one more time).

 * * When using the dynamic receive (ReceivePacket) I was only able to receive up to 12,5 kbps. After that the SPI is not
   fast enough to remove bytes from the FIFO, resulting in overwritten bytes.

   In the future I will try to optimize this functions by setting a larger FIFO Threshold and reading the FIFO a smaller
   amount of times. Also the PayloadReady flag can be used to control the receiving process.

*/

HAL_StatusTypeDef FSK_BigTransmit(SX127X_t *SX127X, uint8_t *Data, uint8_t PayloadLength) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t currentByte = 0;
  uint8_t temp = 0;

  // Put to standby mode
  status = SX127X_set_op_mode(SX127X, STANDBY);

  // Set the FIFO threshold to a single byte
  status = FSK_set_FIFO_threshold(SX127X, (uint8_t)(1));
  RETURN_ON_ERROR(status);

  // If we are in Variable Packet Length Mode we must put the PayloadLength at the beginning of the FIFO first.
  if (SX127X->FSK_Config.PacketFormat == VariableLength) FSK_WriteToFIFO(SX127X, &PayloadLength, 1);
  else {
    FSK_WriteToFIFO(SX127X, Data, 1);  // Only write one byte for now
    currentByte = 1;
  }

  // Configuring the Top Level Sequencer for automatic shutdown after packet transmission (Read page 101)
  status = WRITE_REG(FSK_REG_SEQ_CONFIG_1, 0b00010000);  //Start -> Transmit -> LowPowerSelection(off)
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SEQ_CONFIG_2, 0b00010000);  //Just in case, if RXTimeout happens, it goes to LowPowerSelection
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_TIMER_RESOL, 0b00000000);  //Disables Top Level Sequencer's timers.
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SEQ_CONFIG_1, 0b10010000);  //Turns TopLevelSequencer on: Start -> Transmit -> LowPowerSelection(off)
  RETURN_ON_ERROR(status);

  // Transmits the rest of the package
  while (currentByte <= (PayloadLength - 1)) {
    status = READ_REG(FSK_REG_IRQ_FLAGS2, &temp);
    if ((temp & FSK_IRQ2_FIFO_LEVEL) == 0) {  // We have room to put another byte
      status = WRITE_REG(SX127X_REG_FIFO, Data[currentByte]);
      currentByte++;
    }
  }

  return status;
  //REMEMBER TO CHECK THE FLAG PACKETSENT before changing the mode
}

HAL_StatusTypeDef FSK_PutToRXMODE(SX127X_t *SX127X) {
  uint8_t read_value = 0;

  //Put to standby mode
  SX127X_set_op_mode(SX127X, STANDBY);

  //put in the RX mode (pay attention to this)
  SX127X_set_op_mode(SX127X, FSRX);

  while (!read_value) {
    READ_REG(FSK_REG_IRQ_FLAGS1, &read_value);
    read_value &= FSK_MODE_READY;
  }

  SX127X_set_op_mode(SX127X, RX);

  return HAL_OK;

  //Remember to check the flag PAYLOADREADY before calling the function read package
}

/*
 * This function should only be called after an PacketReady event, usually indicated by a DIO interrupt.
 * It only reads the data already available in the FIFO, and does not wait for incoming bytes - FSK_ReceivePacket does that.
 *
 * MaximumLength: The max size of the buffer array pointed by *Packet.
 * Used to avoid writing to forbidden memory positions, that go beyond the size of the buffer array.
 *
 * ReceivedLength: Returns the size of the received packet.
 * This value is indicated by the first byte of the FIFO in Variable Packet Length mode.
 */

HAL_StatusTypeDef FSK_ReadPacket(SX127X_t *SX127X, uint8_t *Packet, uint8_t MaximumLength, uint8_t *PacketLength, bool *CrcError) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;
  uint8_t *FIFOData;

  // This register has to be read BEFORE putting in standby mode!
  READ_REG(FSK_REG_IRQ_FLAGS2, &read_value);

  // Put to STDBY MODE - Insures the FIFO won't be changed by incoming packets.
  status = SX127X_set_op_mode(SX127X, STANDBY);

  // Verify if the payload is really ready. If not, returns an error.
  if ((read_value & FSK_IRQ2_PAYLOAD_READY) == 0) return HAL_ERROR;

  // Get the CRC Status for the received packet (done before because the flag is cleared after the FIFO is emptied).
  if ((read_value & FSK_IRQ2_CRC_OK) == 0) *CrcError = true;  // Bad :(
  else *CrcError = false;                                     // Good :)

  // Get the number of bytes we are working with - Two options:
  if (SX127X->FSK_Config.PacketFormat == FixedLength) *PacketLength = SX127X->FSK_Config.PayloadLength;  // Configured by the user. Hopefully.
  else FSK_ReadFromFIFO(SX127X, PacketLength, 1);                                                        // The FIFO's first byte contains the received packet length.

  // Using the previously obtained PacketLength we get the remaining bytes:
  FIFOData = (uint8_t *)malloc(*PacketLength);
  status = FSK_ReadFromFIFO(SX127X, FIFOData, *PacketLength);

  for (int i = 0; ((i < *PacketLength) && (i < MaximumLength)); i++) {
    *(Packet + i) = FIFOData[i];
  }

  free(FIFOData);

  return status;
}


// DONE - NEED TO TEST.
/*
   This function is capable of collecting bytes from the FIFO in real time while the packet is being received.
   It's purpose is to receive large packets, larger than the 64 bytes of the FIFO, requiring real time collection
   of the bytes.

   It should be called after a FIFO Level interrupt, triggered by bytes being received, usually
   indicated by a DIO interrupt.

   WARNING: This function is BLOCKING, since it has to watch over every incoming byte. It's operation time is
   dependent on the data rate of the transmission and the packet length.

   You can optimize operation by setting the FIFO Threshold closer to 64. That way the first 60 ish bytes will be
   quickly collected from the FIFO and the function will only have to wait for the remainder bytes to arrive in
   real time.

   MaximumLength: The max size of the buffer array pointed by *Packet.
   Used to avoid writing to forbidden memory positions, that go beyond the size of the buffer array.

   ReceivedLength: Returns the size of the received packet.
   This value is indicated by the first byte of the FIFO in Variable Packet Length mode.

   CRCStatus: returns the CRC Status.
   If CRC calculation is off this will always return false.


   Later thoughts:
 * * Dynamic transmit and receive functions only work until the SPI can keep up with the incoming/transmitting data.
   The highest bitrate I was able to achieve was 76,8 kbps (when receiving packets smaller than 64 bytes with the ReadPacket
   function). After that the SPI (not overclocked) is not fast enough too feed data into the FIFO, resulting in duplicate
   bytes in the receiver (the FIFO got emptied and transmitted the last known byte one more time).

 * * When using the dynamic receive (ReceivePacket) I was only able to receive up to 12,5 kbps. After that the SPI is not
   fast enough to remove bytes from the FIFO, resulting in overwritten bytes.

   In the future I will try to optimize this functions by setting a larger FIFO Threshold and reading the FIFO a smaller
   amount of times. Also the PayloadReady flag can be used to control the receiving process.
   (The payload ready flag will be set after the number of bytes indicated by the first byte are received)


*/

HAL_StatusTypeDef FSK_ReceivePacket(SX127X_t *SX127X, uint8_t *Packet, uint8_t MaximumLength, uint8_t *PacketLength, bool *CrcError) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t i = 0;
  uint8_t read_value;

  // Get the number of bytes we are working with - Two options:
  if (SX127X->FSK_Config.PacketFormat == FixedLength) *PacketLength = SX127X->FSK_Config.PayloadLength;  // Configured by the user. Hopefully.
  else FSK_ReadFromFIFO(SX127X, PacketLength, 1);                                                        // The FIFO's first byte contains the received packet length.

  // Using the previously obtained PacketLength we get the remaining bytes:
  while ((i < *PacketLength) && (i < MaximumLength)) {
    status = READ_REG(FSK_REG_IRQ_FLAGS2, &read_value);  // Read the IRQ registers

    if ((read_value & FSK_IRQ2_FIFO_EMPTY) == 0) {  // If there is something in the FIFO

      // If the entire packet has passed through the packet handler we can get the CRC.
      if (i == *PacketLength) {
        if ((read_value & FSK_IRQ2_CRC_OK) == 0) *CrcError = true;  // Bad :(
        else *CrcError = false;                                     // Good :)
      }

      status = FSK_ReadFromFIFO(SX127X, (Packet + i++), 1);  // Get one byte from the FIFO
    }
  }

  return status;
}

bool FSK_CheckPayloadReady(SX127X_t *SX127X) {
  uint8_t read_value;

  READ_REG(FSK_REG_IRQ_FLAGS2, &read_value);

  return ((read_value & FSK_IRQ2_PAYLOAD_READY) >> 2);
}

bool FSK_CheckFIFOEmpty(SX127X_t *SX127X) {
  uint8_t read_value;

  READ_REG(FSK_REG_IRQ_FLAGS2, &read_value);

  return ((read_value & FSK_IRQ2_FIFO_EMPTY) >> 6);
}

int FSK_OPModeStatus(SX127X_t *SX127X, uint8_t mode) {
  uint8_t read_value;

  READ_REG(SX127X_REG_OP_MODE, &read_value);

  //check the OP mode
  if (mode == read_value)
    return 1;
  //if it is not returns
  return 0;
}


HAL_StatusTypeDef SX127X_set_pa_output(SX127X_t *SX127X, PaSelect_t PaSelect) {
  HAL_StatusTypeDef status = HAL_OK;

  if (PaSelect == PA_BOOST_Pin) {
    status = SX127X_set_bits(SX127X, SX127X_REG_PA_CONFIG, ~SX127X_PA_SELECT_MASK);
  } else {
    status = SX127X_clear_bits(SX127X, SX127X_REG_PA_CONFIG, ~SX127X_PA_SELECT_MASK);
  }

  SX127X->PaSelect = PaSelect;

  return (status);
}


HAL_StatusTypeDef SX127X_set_tx_power(SX127X_t *SX127X, uint8_t TxPower) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;
  uint8_t OutputPower;

  // PaSelect = 1 (PA_BOOST pin, from +2 to +17 dBm)
  if (SX127X->PaSelect == PA_BOOST_Pin) {
    // Stupidity protection:
    if (TxPower < 2) TxPower = 2;
    if (TxPower > 17 && TxPower < 20) TxPower = 17;
    if (TxPower == 20) {
      OutputPower = 0b10001111;

      // Putting the PaDac register in +20 dBm operation mode.
      status = READ_REG(FSK_REG_PA_DAC, &read_value);
      RETURN_ON_ERROR(status);
      read_value = (read_value & 0b11111000) | 0b00000111;
      status = WRITE_REG(FSK_REG_PA_DAC, read_value);
      RETURN_ON_ERROR(status);

      // Setting the over current protection.
      status = SX127X_set_ocp(SX127X, 200);
      RETURN_ON_ERROR(status);

    } else {
      OutputPower = ((uint8_t)(TxPower - 2) & 0b00001111) | 0b10000000;

      // Putting the PaDac register in default operation mode.
      status = READ_REG(FSK_REG_PA_DAC, &read_value);
      RETURN_ON_ERROR(status);
      read_value = (read_value & 0b11111000) | 0b00000100;
      status = WRITE_REG(FSK_REG_PA_DAC, read_value);
      RETURN_ON_ERROR(status);

      // Setting the over current protection.
      status = SX127X_set_ocp(SX127X, 120);
      RETURN_ON_ERROR(status);
    }

    // PaSelect = 0 (RFO pin, from -4 to +14 dBm) I have no idea how to use the negative values.
    // No "if" in case anyone puts nonsense on that function field we will fall here.
  } else {
    // Stupidity protection:
    if (TxPower < 1) TxPower = 1;
    if (TxPower > 14) TxPower = 14;

    // MaxPower must be between 0 and 7 (three bits)
    // Pmax = 10,8 + (0,6 * MaxPower)
    // I'm gonna make my life easier and ignore that the Pmax parameter can be set to many different values
    // The whole point is to never limit the transmission power during a transmission, so I will fix it to
    // +15 dBm since the maximum value for RFO pin is +14 dBm.

    OutputPower = ((uint8_t)TxPower & 0b00001111) | 0b01110000;

    // Putting the PaDac register in default operation mode.
    status = READ_REG(FSK_REG_PA_DAC, &read_value);
    RETURN_ON_ERROR(status);
    read_value = (read_value & 0b11111000) | 0b00000100;
    status = WRITE_REG(FSK_REG_PA_DAC, read_value);
    RETURN_ON_ERROR(status);

    // Setting the over current protection.
    status = SX127X_set_ocp(SX127X, 60);
    RETURN_ON_ERROR(status);
  }

  SX127X->TxPower = TxPower;

  // Writes Final value on the PaConfig register.
  return WRITE_REG(SX127X_REG_PA_CONFIG, OutputPower);
}

HAL_StatusTypeDef SX127X_set_ocp(SX127X_t *SX127X, uint16_t current_mA) {
  uint8_t ocp_trim = 0;
  uint8_t ocp_val = 0;

  /* calculates register value corresponding to current (section 5.4.4) */
  if (current_mA >= 240) {
    ocp_trim = 28;
  } else if (current_mA > 120) {
    ocp_trim = (current_mA + 30) / 10;
  } else if (current_mA >= 45) {
    ocp_trim = (current_mA - 45) / 5;
  } else {
    return WRITE_REG(SX127X_REG_OCP, FSK_LORA_OCP_RESET);
  }

  ocp_val = FSK_OCP_SET(ocp_trim);

  // Saving it into the struct:
  SX127X->OcpCurrent = current_mA;

  return WRITE_REG(SX127X_REG_OCP, ocp_val);
}


HAL_StatusTypeDef FSK_set_freq_deviation(SX127X_t *SX127X, uint32_t fdev, FSK_Bitrate_t bitrate) {
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t brf = bitrate.BitrateValue;
  uint16_t frequency_dev;

  // Limiting values according to page 47 of the datasheet:

  if (fdev > (250000 - (brf / 2)) || fdev > 200000) {
    if (brf < 100000) {
      fdev = 200000;
    } else {
      fdev = (250000 - (brf / 2));
    }
  }

  // Calculating value that will be written in the register:
  //frequency_dev = (uint16_t) (FDEV / FSK_FSTEP); // No rounding
  frequency_dev = (uint16_t)((fdev + FSK_FSTEP - 1) / FSK_FSTEP);  // With weird rounding that I found on stack overflow :D

  // Writing it into the register:
  status = WRITE_REG(FSK_REG_FDEV_MSB, (uint8_t)(frequency_dev >> 8));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_FDEV_LSB, (uint8_t)(frequency_dev));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.FrequencyDeviation = fdev;

  return HAL_OK;
}


HAL_StatusTypeDef FSK_set_bitrate(SX127X_t *SX127X, FSK_Bitrate_t bitrate) {
  HAL_StatusTypeDef status = HAL_OK;

  // Writing it into the register:
  status = WRITE_REG(FSK_REG_BITRATE_MSB, (uint8_t)(bitrate.RegisterValue >> 8));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_BITRATE_LSB, (uint8_t)(bitrate.RegisterValue));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.Bitrate = bitrate;

  return status;
}

HAL_StatusTypeDef FSK_set_rx_bandwidth(SX127X_t *SX127X, uint8_t bandwidth) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(FSK_REG_RX_BW, &read_value);
  RETURN_ON_ERROR(status);

  bandwidth = ((read_value & 0b11100000) | bandwidth);

  status = WRITE_REG(FSK_REG_RX_BW, bandwidth);
  RETURN_ON_ERROR(status);

  SX127X->FSK_Config.RxBandwidth = bandwidth;

  return HAL_OK;
}

HAL_StatusTypeDef FSK_set_afc_bandwidth(SX127X_t *SX127X, uint8_t bandwidth) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(FSK_REG_AFC_BW, &read_value);
  RETURN_ON_ERROR(status);

  bandwidth = ((read_value & 0b11100000) | bandwidth);

  status = WRITE_REG(FSK_REG_AFC_BW, bandwidth);
  RETURN_ON_ERROR(status);

  SX127X->FSK_Config.AfcBandwidth = bandwidth;

  return HAL_OK;
}

HAL_StatusTypeDef FSK_set_preamb_len(SX127X_t *SX127X, uint16_t PreambleLength, PremblePolarity_t PreamblePolarity) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Writing the preamble length:
  status = WRITE_REG(FSK_REG_PREAMBLE_LEN_MSB, (uint8_t)(PreambleLength >> 8));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PREAMBLE_LEN_LSB, (uint8_t)(PreambleLength));
  RETURN_ON_ERROR(status);

  // Setting the polarity:
  status = READ_REG(FSK_REG_SYNC_CONFIG, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_CONFIG, (uint8_t)((read_value & FSK_PREAMBLE_POLARITY_MASK) | PreamblePolarity));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.PreambleLength = PreambleLength;
  SX127X->FSK_Config.PreamblePolarity = PreamblePolarity;

  return status;
}


HAL_StatusTypeDef FSK_set_sync_word(SX127X_t *SX127X, uint8_t SyncWordSize, uint64_t SyncWord) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Enabling Sync Word generation and detection:
  status = READ_REG(FSK_REG_SYNC_CONFIG, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_CONFIG, (uint8_t)(read_value & FSK_SYNC_WORD_ON_MASK) | FSK_SYNC_WORD_ENABLE);
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.SyncWordGeneration = true;

  // Setting the sync word size:
  status = READ_REG(FSK_REG_SYNC_CONFIG, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_CONFIG, (uint8_t)((read_value & FSK_SYNC_WORD_SIZE_MASK) | (SyncWordSize - 1)));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.SyncWordSize = SyncWordSize;

  // Setting the sync word itself:
  status = WRITE_REG(FSK_REG_SYNC_VALUE_1, (uint8_t)(SyncWord));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_VALUE_2, (uint8_t)(SyncWord >> 8));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_VALUE_3, (uint8_t)(SyncWord >> 16));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_VALUE_4, (uint8_t)(SyncWord >> 24));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_VALUE_5, (uint8_t)(SyncWord >> 32));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_VALUE_6, (uint8_t)(SyncWord >> 40));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_VALUE_7, (uint8_t)(SyncWord >> 48));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_VALUE_8, (uint8_t)(SyncWord >> 56));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.SyncWord = SyncWord;

  return status;
}

HAL_StatusTypeDef FSK_disable_sync_word(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Disabling Sync Word generation and detection:
  status = READ_REG(FSK_REG_SYNC_CONFIG, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_SYNC_CONFIG, (uint8_t)(read_value & FSK_SYNC_WORD_ON_MASK) | FSK_SYNC_WORD_DISABLE);
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.SyncWordGeneration = false;

  return (status);
}

HAL_StatusTypeDef FSK_set_packet_format(SX127X_t *SX127X, PacketFormat_t PacketFormat) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Setting the packet format:
  status = READ_REG(FSK_REG_PACKET_CONFIG1, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PACKET_CONFIG1, (uint8_t)((read_value & FSK_PACKET_FORMAT_MASK) | PacketFormat));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.PacketFormat = PacketFormat;

  return (status);
}

HAL_StatusTypeDef FSK_set_data_processing_mode(SX127X_t *SX127X, DataProcessingMode_t DataProcessingMode) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Setting the data processing mode:
  status = READ_REG(FSK_REG_PACKET_CONFIG2, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PACKET_CONFIG2, (uint8_t)((read_value & FSK_DATA_MODE_MASK) | DataProcessingMode));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.DataProcessingMode = DataProcessingMode;

  return (status);
}

HAL_StatusTypeDef FSK_set_payload_length(SX127X_t *SX127X, uint16_t PayloadLength) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Setting the data processing mode:
  status = READ_REG(FSK_REG_PACKET_CONFIG2, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PACKET_CONFIG2, (uint8_t)((read_value & FSK_PAYLOAD_LENGTH_MASK) | (uint8_t)PayloadLength >> 8));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PAYLOAD_LENGHT, (uint8_t)PayloadLength);

  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.PayloadLength = PayloadLength;

  return (status);
}



// Maybe we should make enable and disable functions for more things, all using the set/clear bits functions

HAL_StatusTypeDef FSK_enable_crc(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;

  status = SX127X_set_bits(SX127X, FSK_REG_PACKET_CONFIG1, ~FSK_CRC_ON_MASK);

  // Saving it into the struct:
  SX127X->FSK_Config.CrcCalculation = true;

  return (status);
}

HAL_StatusTypeDef FSK_disable_crc(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;

  status = SX127X_clear_bits(SX127X, FSK_REG_PACKET_CONFIG1, ~FSK_CRC_ON_MASK);

  // Saving it into the struct:
  SX127X->FSK_Config.CrcCalculation = false;

  return (status);
}


HAL_StatusTypeDef FSK_enable_preamble_detector(SX127X_t *SX127X, uint8_t PreambleDetectorSize, uint8_t PreambleDetectorTolerance) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Stupidity protection:
  if (PreambleDetectorSize > 3) PreambleDetectorSize = 3;

  status = READ_REG(FSK_REG_PREAMBLE_DETECT, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PREAMBLE_DETECT, ((FSK_PREAMBLE_DETECTOR_SIZE_MASK & read_value) | (((uint8_t)PreambleDetectorSize - 1) << 5)));
  RETURN_ON_ERROR(status);

  status = READ_REG(FSK_REG_PREAMBLE_DETECT, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PREAMBLE_DETECT, ((FSK_PREAMBLE_DETECTOR_TOL_MASK & read_value) | ((uint8_t)PreambleDetectorTolerance) & ~FSK_PREAMBLE_DETECTOR_TOL_MASK));
  RETURN_ON_ERROR(status);

  status = SX127X_set_bits(SX127X, FSK_REG_PREAMBLE_DETECT, ~FSK_PREAMBLE_DETECTOR_MASK);
  RETURN_ON_ERROR(status);

  status = READ_REG(FSK_REG_PREAMBLE_DETECT, &read_value);

  // Saving it into the struct:
  SX127X->FSK_Config.PreambleDetectorOn = true;
  SX127X->FSK_Config.PreambleDetectorSize = PreambleDetectorSize;
  SX127X->FSK_Config.PreambleDetectorTolerance = PreambleDetectorTolerance;

  return (status);
}

HAL_StatusTypeDef FSK_disable_preamble_detector(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;

  status = SX127X_clear_bits(SX127X, FSK_REG_PREAMBLE_DETECT, ~FSK_PREAMBLE_DETECTOR_MASK);

  // Saving it into the struct:
  SX127X->FSK_Config.PreambleDetectorOn = false;

  return (status);
}


HAL_StatusTypeDef FSK_set_crc_whitenning_type(SX127X_t *SX127X, CRC_WhiteningType_t CrcWhitenningType) {
  HAL_StatusTypeDef status = HAL_OK;

  if (CrcWhitenningType == CCITT_CRC) {
    status = SX127X_clear_bits(SX127X, FSK_REG_PACKET_CONFIG1, ~FSK_CRC_WHITENING_TYPE_MASK);
  } else {
    status = SX127X_set_bits(SX127X, FSK_REG_PACKET_CONFIG1, ~FSK_CRC_WHITENING_TYPE_MASK);
  }

  // Saving it into the struct:
  SX127X->FSK_Config.CrcWhitenningType = CrcWhitenningType;

  return (status);
}

HAL_StatusTypeDef FSK_set_crc_autoclear(SX127X_t *SX127X, bool CrcAutoclear) {
  HAL_StatusTypeDef status = HAL_OK;

  if (CrcAutoclear) {
    status = SX127X_clear_bits(SX127X, FSK_REG_PACKET_CONFIG1, ~FSK_CRC_AUTOCLEAR_OFF_MASK);
  } else {
    status = SX127X_set_bits(SX127X, FSK_REG_PACKET_CONFIG1, ~FSK_CRC_AUTOCLEAR_OFF_MASK);
  }

  // Saving it into the struct:
  SX127X->FSK_Config.CrcAutoclear = CrcAutoclear;

  return (status);
}


HAL_StatusTypeDef SX127X_set_lna_gain(SX127X_t *SX127X, LnaGain_t LnaGain) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(SX127X_REG_LNA, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(SX127X_REG_LNA, ((SX127X_LNA_GAIN_MASK & read_value) | ((uint8_t)LnaGain << 5)));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->LnaGain = LnaGain;

  return (status);
}

HAL_StatusTypeDef SX127X_set_lna_boost(SX127X_t *SX127X, bool LnaBoost) {
  HAL_StatusTypeDef status = HAL_OK;

  if (LnaBoost) {
    status = SX127X_set_bits(SX127X, SX127X_REG_LNA, ~SX127X_LNA_BOOST_MASK);
  } else {
    status = SX127X_clear_bits(SX127X, SX127X_REG_LNA, ~SX127X_LNA_BOOST_MASK);
  }

  // Saving it into the struct:
  SX127X->LnaBoost = LnaBoost;

  return (status);
}


HAL_StatusTypeDef FSK_set_rssi_offset(SX127X_t *SX127X, int8_t RssiOffset) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(FSK_REG_RSSI_CONFIG, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_RSSI_CONFIG, ((FSK_RSSI_OFFSET_MASK & read_value) | (int8_t)RssiOffset));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.RssiOffset = RssiOffset;

  return (status);
}

HAL_StatusTypeDef FSK_set_rssi_smoothing(SX127X_t *SX127X, RssiSmoothing_t RssiSmoothing) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(FSK_REG_RSSI_CONFIG, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_RSSI_CONFIG, ((FSK_RSSI_SMOOTHING_MASK & read_value) | (uint8_t)RssiSmoothing));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.RssiSmoothing = RssiSmoothing;

  return (status);
}


// Encoding can only be used in Packet Mode!
HAL_StatusTypeDef FSK_set_encoding(SX127X_t *SX127X, Encoding_t Encoding) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(FSK_REG_PACKET_CONFIG1, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(FSK_REG_PACKET_CONFIG1, ((FSK_DC_FREE_ENCODING_MASK & read_value) | (uint8_t)Encoding));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.Encoding = Encoding;

  return (status);
}


HAL_StatusTypeDef FSK_set_data_shaping(SX127X_t *SX127X, FSK_DataShaping_t DataShaping) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(SX127X_REG_PA_RAMP, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(SX127X_REG_PA_RAMP, ((FSK_MODULATION_SHAPING_MASK & read_value) | (uint8_t)DataShaping));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.DataShaping = DataShaping;

  return (status);
}


HAL_StatusTypeDef FSK_set_pa_ramp_time(SX127X_t *SX127X, PaRampTime_t PaRampTime) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(SX127X_REG_PA_RAMP, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(SX127X_REG_PA_RAMP, ((FSK_PA_RAMP_MASK & read_value) | (uint8_t)PaRampTime));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->PaRampTime = PaRampTime;

  return (status);
}


HAL_StatusTypeDef SX127X_set_DIO_mapping(SX127X_t *SX127X, uint8_t DIOMapping1,
                                         uint8_t DIOMapping2) {
  HAL_StatusTypeDef status = HAL_OK;

  status = WRITE_REG(SX127X_REG_DIO_MAPPING_1, DIOMapping1);
  RETURN_ON_ERROR(status);

  status = WRITE_REG(SX127X_REG_DIO_MAPPING_2, DIOMapping2);
  RETURN_ON_ERROR(status);

  return (status);
}

HAL_StatusTypeDef FSK_set_auto_restart_RX(SX127X_t *SX127X, AutoRestartRX_t AutoRestartRX) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(FSK_REG_SYNC_CONFIG, &read_value);
  RETURN_ON_ERROR(status);

  status = WRITE_REG(FSK_REG_SYNC_CONFIG, ((FSK_AUTO_RESTART_RX_MASK & read_value) | (uint8_t)AutoRestartRX));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.AutoRestartRX = AutoRestartRX;

  return (status);
}


HAL_StatusTypeDef FSK_set_FIFO_threshold(SX127X_t *SX127X, uint8_t FifoThreshold) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(FSK_REG_SYNC_CONFIG, &read_value);
  RETURN_ON_ERROR(status);

  status = WRITE_REG(FSK_REG_FIFO_THRESH, ((FSK_FIFO_THRESHOLD_MASK & read_value) | (FifoThreshold & ~FSK_FIFO_THRESHOLD_MASK)));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->FSK_Config.FifoThreshold = FifoThreshold;

  return (status);
}

HAL_StatusTypeDef FSK_get_packet_rssi(SX127X_t *SX127X, uint8_t *rssi) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;
  uint8_t rssi_value;

  status = READ_REG(FSK_REG_RSSI_VALUE, &rssi_value);
  RETURN_ON_ERROR(status);

  status = READ_REG(FSK_REG_RSSI_CONFIG, &read_value);
  RETURN_ON_ERROR(status);

  *rssi = (rssi_value - ((read_value & 0b11111000) >> 3)) / 2;

  return (status);
}

// Receives the register and a mask, where 1 shows the bits to be set: 0b00001000 sets the fourth bit.
HAL_StatusTypeDef SX127X_set_bits(SX127X_t *SX127X, uint8_t Reg, uint8_t Mask) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Setting the required bit:
  status = READ_REG(Reg, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(Reg, (uint8_t)(read_value | Mask));
  RETURN_ON_ERROR(status);

  return (status);
}

// Receives the register and a mask, where 1 shows the bits to be cleared: 0b00001000 clears the fourth bit.
HAL_StatusTypeDef SX127X_clear_bits(SX127X_t *SX127X, uint8_t Reg, uint8_t Mask) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  // Setting the required bit:
  status = READ_REG(Reg, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(Reg, (uint8_t)(read_value & ~Mask));
  RETURN_ON_ERROR(status);

  return (status);
}


/*
   LoRa Functions
*/

HAL_StatusTypeDef LoRa_set_signal_bandwidth(SX127X_t *SX127X, uint8_t Bandwidth) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0;

  status = READ_REG(LORA_REG_MODEM_CONFIG_1, &read_value);
  RETURN_ON_ERROR(status);

  read_value = (LoRa_BW_CLR(read_value)) | Bandwidth;

  status = WRITE_REG(LORA_REG_MODEM_CONFIG_1, read_value);
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  SX127X->LoRa_Config.Bandwidth = Bandwidth;

  return LoRa_set_ldo_flag(SX127X);
}


// PARTIAL - Need to implement Special Case SF6
HAL_StatusTypeDef LoRa_set_spreading_factor(SX127X_t *SX127X, uint8_t sf) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t reg_val = 0;

  /* Protects against invalid spreading factor values */
  if (sf < 6) {
    sf = 6;
  } else if (sf > 12) {
    sf = 12;
  }

  /* Special Case: Spreading Factor = 6 (section 4.1.1.2) */
  if (sf == 6) {
    /* Set header to Implicit Mode */
    /* Set bits 2-0 of register detection optimize (address 0x31) to value "0b101" */
    /* Set register detection threshold (address 0x37) to value 0x0C */
  }

  /* Set sf in Register ModemConfig2 */
  status = READ_REG(LORA_REG_MODEM_CONFIG_2, &reg_val);
  RETURN_ON_ERROR(status);
  reg_val = LoRa_set_SF(reg_val, sf);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_2, reg_val);
  RETURN_ON_ERROR(status);

  return LoRa_set_ldo_flag(SX127X);
}

HAL_StatusTypeDef LoRa_set_preamble_lenght(SX127X_t *SX127X, uint16_t lenght) {
  HAL_StatusTypeDef status = HAL_OK;

  status = WRITE_REG(LORA_REG_PREAMBLE_LSB, (uint8_t)(lenght));
  RETURN_ON_ERROR(status);
  status = WRITE_REG(LORA_REG_PREAMBLE_MSB, (uint8_t)(lenght >> 8));
  return status;
}

HAL_StatusTypeDef LoRa_set_sync_word(SX127X_t *SX127X, uint8_t sync_word) {
  return WRITE_REG(LORA_REG_SYNC_WORD, sync_word);
}

// Need to test & protect macros against wrong values
HAL_StatusTypeDef LoRa_set_coding_rate(SX127X_t *SX127X, uint8_t coding_rate) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t reg_value = 0;
  status = READ_REG(LORA_REG_MODEM_CONFIG_1, &reg_value);
  RETURN_ON_ERROR(status);
  reg_value = LoRa_set_CR(reg_value, coding_rate);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_1, reg_value);
  RETURN_ON_ERROR(status);

  return LoRa_set_ldo_flag(SX127X);
}


HAL_StatusTypeDef LoRa_enable_crc(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0;

  status = READ_REG(LORA_REG_MODEM_CONFIG_2, &read_value);
  RETURN_ON_ERROR(status);
  read_value = LoRa_CRC_SET(read_value);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_2, read_value);
  return status;
}

HAL_StatusTypeDef LoRa_disable_crc(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0;

  status = READ_REG(LORA_REG_MODEM_CONFIG_2, &read_value);
  RETURN_ON_ERROR(status);
  read_value = LoRa_CRC_RESET(read_value);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_2, read_value);
  return status;
}


HAL_StatusTypeDef LoRa_explicit_header_mode(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t reg_value = 0;

  status = READ_REG(LORA_REG_MODEM_CONFIG_1, &reg_value);
  RETURN_ON_ERROR(status);
  reg_value = LoRa_EXPLICIT(reg_value);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_1, reg_value);
  return status;
}

HAL_StatusTypeDef LoRa_implicit_header_mode(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t reg_value = 0;

  status = READ_REG(LORA_REG_MODEM_CONFIG_1, &reg_value);
  RETURN_ON_ERROR(status);
  reg_value = LoRa_IMPLICIT(reg_value);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_1, reg_value);
  return status;
}

// DONE - Need to test
HAL_StatusTypeDef LoRa_set_ldo_flag(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  long sbw = 0;
  uint8_t sf = 0;
  long symbol_duration;
  uint8_t reg_config3_val = 0;

  /* Read arguments from registers*/
  status = LoRa_get_signal_bandwidth(SX127X, &sbw);
  RETURN_ON_ERROR(status);
  status = LoRa_get_spreading_factor(SX127X, &sf);
  RETURN_ON_ERROR(status);
  status = READ_REG(LORA_REG_MODEM_CONFIG_3, &reg_config3_val);
  RETURN_ON_ERROR(status);

  /* Calculates and sets ldo value */
  symbol_duration = 1000 / (sbw / (1L << sf));  // Section 4.1.1.5 and 4.1.1.6
  if (symbol_duration > 16) {
    reg_config3_val = LoRa_LDO_SET(reg_config3_val);
  } else {
    reg_config3_val = LoRa_LDO_RESET(reg_config3_val);
  }

  return WRITE_REG(LORA_REG_MODEM_CONFIG_3, reg_config3_val);
}


// DONE - Need to test and verify formula
HAL_StatusTypeDef LoRa_get_packet_rssi(SX127X_t *SX127X, uint8_t *rssi) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0;

  status = READ_REG(LORA_REG_PKT_RSSI_VALUE, &read_value);
  RETURN_ON_ERROR(status);
  *rssi = (-1) * (read_value - (SX127X->Frequency < 868E6 ? 164 : 157));
  return HAL_OK;
}

// DONE - Need to test and verify formula
HAL_StatusTypeDef LoRa_get_packet_snr(SX127X_t *SX127X, float *snr) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0;

  status = READ_REG(LORA_REG_PKT_SNR_VALUE, &read_value);
  RETURN_ON_ERROR(status);
  *snr = read_value * 0.25;
  return HAL_OK;
}

// DONE - Need to test
HAL_StatusTypeDef LoRa_get_spreading_factor(SX127X_t *SX127X, uint8_t *sf) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0;

  status = READ_REG(LORA_REG_MODEM_CONFIG_2, &read_value);
  RETURN_ON_ERROR(status);
  *sf = read_value >> 4;
  return HAL_OK;
}

// DONE - Need to test
HAL_StatusTypeDef LoRa_get_raw_signal_bandwidth(SX127X_t *SX127X, uint8_t *raw_sbw) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value = 0;

  status = READ_REG(LORA_REG_MODEM_CONFIG_1, &read_value);
  RETURN_ON_ERROR(status);
  *raw_sbw = LoRa_BW_MASK(read_value);
  return HAL_OK;
}

// DONE - Need to test
HAL_StatusTypeDef LoRa_get_signal_bandwidth(SX127X_t *SX127X, long *sbw) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t raw_sbw = 0;

  status = LoRa_get_raw_signal_bandwidth(SX127X, &raw_sbw);
  RETURN_ON_ERROR(status);
  status = LoRa_raw_sbw_to_long(raw_sbw, sbw);
  return status;
}

// DONE - Need to test
HAL_StatusTypeDef LoRa_raw_sbw_to_long(uint8_t raw_sbw, long *sbw) {
  switch (raw_sbw) {
    case LoRa_BW_7_8:
      *sbw = 7.8E3;
      return HAL_OK;
    case LoRa_BW_10_4:
      *sbw = 10.4E3;
      return HAL_OK;
    case LoRa_BW_15_6:
      *sbw = 15.6E3;
      return HAL_OK;
    case LoRa_BW_20_8:
      *sbw = 20.8E3;
      return HAL_OK;
    case LoRa_BW_31_25:
      *sbw = 31.25E3;
      return HAL_OK;
    case LoRa_BW_41_7:
      *sbw = 41.7E3;
      return HAL_OK;
    case LoRa_BW_62_5:
      *sbw = 62.5E3;
      return HAL_OK;
    case LoRa_BW_125:
      *sbw = 125E3;
      return HAL_OK;
    case LoRa_BW_250:
      *sbw = 250E3;
      return HAL_OK;
    case LoRa_BW_500:
      *sbw = 500E3;
      return HAL_OK;
  }
  return HAL_ERROR;
}

// DONE - Need to test
HAL_StatusTypeDef SX127X_random(SX127X_t *SX127X, uint8_t *random) {
  /* RSSI Wideband can be used to generate a random number */
  return READ_REG(LORA_REG_RSSI_WIDEBAND, random);
}


// DONE - Need to test. Depending on SPI burst read/write
HAL_StatusTypeDef LoRa_Transmit(SX127X_t *SX127X, void *packet, uint8_t length) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t reg_op_val = 0;
  uint8_t fifo_addr_ptr = 0;
  uint8_t read_value = 0;

  /* Verifies if is not transmiting */
  status = READ_REG(SX127X_REG_OP_MODE, &reg_op_val);
  RETURN_ON_ERROR(status);
  reg_op_val = LoRa_get_MODE(reg_op_val);
  if (reg_op_val != LoRa_MODE_SLEEP && reg_op_val != LoRa_MODE_STANDBY) {
    //HAL_UART_Transmit(&huart2, (uint8_t*)"O SX127X nao esta em Sleep ou Standby\n", sizeof("O SX127X nao esta em Sleep ou Standby\n"), HAL_MAX_DELAY);
    //return HAL_ERROR;
  }

  /* Set Standby Mode */
  status = SX127X_set_op_mode(SX127X, STANDBY);
  if (status != HAL_OK) {
    //HAL_UART_Transmit(&huart2, (uint8_t*)"Erro ao colocar o SX127X em Standby\n", sizeof("Erro ao colocar o SX127X em Standby\n"), HAL_MAX_DELAY);
  }
  RETURN_ON_ERROR(status);

  /* Tx Init */
  status = READ_REG(LORA_REG_FIFO_TX_BASE_ADDR, &fifo_addr_ptr);
  if (status != HAL_OK) {
    //HAL_UART_Transmit(&huart2, (uint8_t*)"Erro ao ler o registrador 'LORA_REG_FIFO_TX_BASE_ADDR'\n", sizeof("Erro ao ler o resgistrador 'LORA_REG_FIFO_TX_BASE_ADDR'\n"), HAL_MAX_DELAY);
  }
  RETURN_ON_ERROR(status);
  status = WRITE_REG(LORA_REG_FIFO_ADDR_PTR, fifo_addr_ptr);
  if (status != HAL_OK) {
    //HAL_UART_Transmit(&huart2, (uint8_t*)"Erro ao escrever no registrador 'LORA_REG_FIFO_ADDR_PTR'\n", sizeof("Erro ao escrever no registrador 'LORA_REG_FIFO_ADDR_PTR'\n"), HAL_MAX_DELAY);
  }
  RETURN_ON_ERROR(status);

  status = WRITE_REG(LORA_REG_PAYLOAD_LENGTH, length);
  if (status != HAL_OK) {
    //HAL_UART_Transmit(&huart2, (uint8_t*)"Erro ao escrever no registrador 'LORA_REG_PAYLOAD_LENGTH'\n", sizeof("Erro ao escrever no registrador 'LORA_REG_PAYLOAD_LENGTH'\n"), HAL_MAX_DELAY);
  }
  RETURN_ON_ERROR(status);
  status = WRITE_REG(SX127X_REG_FIFO, length);
  if (status != HAL_OK) {
    //HAL_UART_Transmit(&huart2, (uint8_t*)"Erro ao escrever no registrador 'LORA_REG_FIFO'\n", sizeof("Erro ao escrever no registrador 'LORA_REG_FIFO'\n"), HAL_MAX_DELAY);
  }
  RETURN_ON_ERROR(status);

  /* Write Data FIFO */
  for (int i = 0; i < length; i++) {
    status = WRITE_REG(SX127X_REG_FIFO, (((uint8_t *)packet)[i]));
    if (status != HAL_OK) {
      //HAL_UART_Transmit(&huart2, (uint8_t*)"Erro ao escrever no FIFO\n", sizeof("Erro ao escrever no FIFO\n"), HAL_MAX_DELAY);
    }
    RETURN_ON_ERROR(status);
  }

  /* Set Tx Mode */
  status = WRITE_REG(LORA_REG_IRQ_FLAGS, 0b00001000);
  RETURN_ON_ERROR(status);

  return SX127X_set_op_mode(SX127X, TX);
}


HAL_StatusTypeDef LoRa_ReadPacket(SX127X_t *SX127X, uint8_t *Packet, uint8_t MaximumLength, uint8_t *PacketLength, bool *CRCStatus) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t *FIFOData;
  uint8_t reg_op_val, iqr_val, fifo_addr_ptr;
  uint8_t i;

  /* Verifies if is not transmiting */
  status = READ_REG(SX127X_REG_OP_MODE, &reg_op_val);
  RETURN_ON_ERROR(status);
  reg_op_val = LoRa_get_MODE(reg_op_val);
  if (reg_op_val == LoRa_MODE_FSTX && reg_op_val == LoRa_MODE_TX) {
    //HAL_UART_Transmit(&huart2,(uint8_t*)"\nVerifies if not transmiting\n", 29, HAL_MAX_DELAY);
    return HAL_ERROR;
  }

  /* Set Standby Mode */
  //status = LoRa_set_op_mode(SX127X, LoRa_MODE_STANDBY);
  //RETURN_ON_ERROR(status);

  /* Set Header Mode */
  //status = LoRa_explicit_header_mode(SX127X);
  //RETURN_ON_ERROR(status);

  /* Verifies Received Packet and Consistency */
  status = READ_REG(LORA_REG_IRQ_FLAGS, &iqr_val);
  RETURN_ON_ERROR(status);
  if ((iqr_val & 0b01100000) != 0b01000000) {
    *CRCStatus = 1;
  } else {
    *CRCStatus = 0;
  }

  // Reset the flags
  status = WRITE_REG(LORA_REG_IRQ_FLAGS, iqr_val);
  RETURN_ON_ERROR(status);

  // Packet Length
  status = READ_REG(LORA_REG_RX_NB_BYTES, PacketLength);
  RETURN_ON_ERROR(status);

  // Setting the RX FIFO Address
  status = READ_REG(LORA_REG_FIFO_RX_CURRENT_ADDR, &fifo_addr_ptr);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(LORA_REG_FIFO_ADDR_PTR, fifo_addr_ptr);
  RETURN_ON_ERROR(status);

  // Using the previously obtained PacketLength we get the remaining bytes:
  FIFOData = (uint8_t *)malloc(*PacketLength);
  status = FSK_ReadFromFIFO(SX127X, FIFOData, *PacketLength);

  for (int i = 0; ((i < (*PacketLength - 1)) && (i < MaximumLength)); i++) {
    *(Packet + i) = FIFOData[i + 1];
  }

  free(FIFOData);

  (*PacketLength)--;  // It originally also counts the payload length byte.

  status = SX127X_set_op_mode(SX127X, RX);
  return status;
}


HAL_StatusTypeDef LoRa_Is_Rx_Done(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t iqr_val;

  /* Verifies Received Packet and Consistency */
  status = READ_REG(LORA_REG_IRQ_FLAGS, &iqr_val);
  RETURN_ON_ERROR(status);
  if ((iqr_val & 0b01100000) != 0b01000000) {
    return HAL_ERROR;
  }
}

HAL_StatusTypeDef LoRa_set_FIFO_base_address(SX127X_t *SX127X, uint8_t RxAddress, uint8_t TxAddress) {
  HAL_StatusTypeDef status = HAL_OK;

  /* Set Base address */
  status = WRITE_REG(LORA_REG_FIFO_RX_BASE_ADDR, RxAddress);
  RETURN_ON_ERROR(status);

  status = WRITE_REG(LORA_REG_FIFO_TX_BASE_ADDR, TxAddress);
  RETURN_ON_ERROR(status);
}

HAL_StatusTypeDef LoRa_enable_low_data_rate_optimization(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(LORA_REG_MODEM_CONFIG_3, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_3, ((read_value & 0b11110111) | 0b00001000));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  //SX127X->LoRa_Config.LowDataRateOptimize = true;

  return (status);
}

HAL_StatusTypeDef LoRa_disable_low_data_rate_optimization(SX127X_t *SX127X) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t read_value;

  status = READ_REG(LORA_REG_MODEM_CONFIG_3, &read_value);
  RETURN_ON_ERROR(status);
  status = WRITE_REG(LORA_REG_MODEM_CONFIG_3, (read_value & 0b11110111));
  RETURN_ON_ERROR(status);

  // Saving it into the struct:
  //SX127X->LoRa_Config.LowDataRateOptimize = false;

  return (status);
}
