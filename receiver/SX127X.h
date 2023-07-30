/*
 * SX127X.c
 * Author: Júlio Calandrin
 * Made with care, hate and sweat by someone that doesn't really knows what it's doing.
 *
 */

#ifndef SX127X_H
#define SX127X_H

/*
 * ERROR HANDLING
 */
#define RETURN_ON_ERROR(status);\
		if(status != HAL_OK){\
			return status;\
		}

/*
 * INCLUDES
 */
#include "ESP32_SPI.h"	
#include <stdlib.h>

/*
 * Mutual register addresses
 */
#define SX127X_REG_FIFO             		(uint8_t)0x00
#define SX127X_REG_OP_MODE          		(uint8_t)0x01
#define SX127X_REG_FRF_MSB						(uint8_t)0x06	// RF carrier Frequency MSB
#define SX127X_REG_FRF_MID			 			(uint8_t)0x07	// RF carrier Frequency MID
#define SX127X_REG_FRF_LSB						(uint8_t)0x08	// RF carrier Frequency LSB
#define SX127X_REG_PA_CONFIG					(uint8_t)0x09	// Power Amplifier Configuration
#define SX127X_REG_PA_RAMP						(uint8_t)0x0A	// Power Amplifier ramp up/down time
#define SX127X_REG_OCP		   				(uint8_t)0x0B	// Over Current Protection Configuration
#define SX127X_REG_LNA	 						(uint8_t)0x0C	// Low-Noise Amplifier Configuration
#define SX127X_REG_DIO_MAPPING_1				(uint8_t)0x40
#define SX127X_REG_DIO_MAPPING_2				(uint8_t)0x41
#define SX127X_REG_VERSION						(uint8_t)0x42	// Semtech ID relating the silicon revision
#define SX127X_REG_TCXO							(uint8_t)0x4B	// TCXO or XTAL input setting
#define SX127X_REG_PA_DAC           		(uint8_t)0x4D 	// High power settings of the Power Amplifier
#define SX127X_REG_FORMER_TEMP				(uint8_t)0x5B	// Stored temperature during the former IQ Calibration
#define SX127X_REG_AGC_REF						(uint8_t)0x61	// Adjustment of the AGC thresholds
#define SX127X_REG_AGC_THRESH1				(uint8_t)0x62	// Adjustment of the AGC thresholds
#define SX127X_REG_AGC_THRESH2				(uint8_t)0x63	// Adjustment of the AGC thresholds
#define SX127X_REG_AGC_THRESH3				(uint8_t)0x64	// Adjustment of the AGC thresholds
#define SX127X_REG_PLL							(uint8_t)0x70	// Control of the PLL bandwidth


/*
 * LoRa register addresses
 */

#define LORA_REG_FIFO_ADDR_PTR			0x0D	// SPI Interface FIFO Pointer
#define LORA_REG_FIFO_TX_BASE_ADDR		0x0E	// Base address for TX writing on FIFO
#define LORA_REG_FIFO_RX_BASE_ADDR		0x0F	// Base address for RX reading on FIFO
#define LORA_REG_FIFO_RX_CURRENT_ADDR	0x10	// Address of last packet received
#define LORA_REG_IRQ_FLAGS_MASK			0x11	// Interrupt Request Flags Mask
#define LORA_REG_IRQ_FLAGS					0x12	// Interrupt Request Flags
#define LORA_REG_RX_NB_BYTES				0x13	// Number of received bytes
#define LORA_REG_RX_HEADER_CNT_MSB		0x14	// Number of valid headers received since last transition into Rx mode MSB
#define LORA_REG_RX_HEADER_CNT_LSB		0x15	// Number of valid headers received since last transition into Rx mode LSB
#define LORA_REG_RX_PACKET_CNT_MSB		0x16	// Number of valid packets received since last transition into Rx mode MSB
#define LORA_REG_RX_PACKET_CNT_LSB		0x17	// Number of valid packets received since last transition into Rx mode LSB
#define LORA_REG_MODEM_STAT				0x18	// Modem status
#define LORA_REG_PKT_SNR_VALUE			0x19	// Last Packet Signal-to-Noise Ratio
#define LORA_REG_PKT_RSSI_VALUE			0x1A	// Last Packet Received Signal Strength Indicator
#define LORA_REG_RSSI_VALUE				0x1B	// Current Received Signal Strength Indicator
#define LORA_REG_HOP_CHANNEL				0x1C	// FHSS start channel
#define LORA_REG_MODEM_CONFIG_1			0x1D	// Modem Configuration 1
#define LORA_REG_MODEM_CONFIG_2			0x1E	// Modem Configuration 2
#define LORA_REG_SYMB_TIMEOUT_LSB		0x1F	// Receiver timeout LSB. MSBs are in Modem Config
#define LORA_REG_PREAMBLE_MSB				0x20	// Preamble size MSB
#define LORA_REG_PREAMBLE_LSB				0x21	// Preamble size MSB
#define LORA_REG_PAYLOAD_LENGTH			0x22	// Payload Lenght
#define LORA_REG_MAX_PAYLOAD_LENGTH		0x23	// Maximum Payload Lenght
#define LORA_REG_HOP_PERIOD				0x24	// FHSS HOP Period
#define LORA_REG_FIFO_RX_BYTE_ADDR		0x25	// Address of last byte written in FIFO
#define LORA_REG_MODEM_CONFIG_3			0x26	// Modem Configuration 3
#define LORA_REG_FREQ_ERROR_MSB			0x28	// Estimated frequency error MSB
#define LORA_REG_FREQ_ERROR_MID			0x29	// Estimated frequency error MID
#define LORA_REG_FREQ_ERROR_LSB			0x2A	// Estimated frequency error LSB
#define LORA_REG_RSSI_WIDEBAND			0x2C	// Wideband RSSI measurement
#define LORA_REG_LF_FREQ_1					0x2F	// Optimize receiver
#define LORA_REG_LF_FREQ_2					0x30	// Optimize receiver
#define LORA_REG_DETECTION_OPTIMIZE		0x31	// Detection Optimize for Spreading Factor 6
#define LORA_REG_INVERTIQ					0x33	// Invert LoRa I and Q signals
#define LORA_REG_HIGH_BW_OPTIMIZE1		0x36	// Sensitivity optimization for 500 kHz bandwidth
#define LORA_REG_DETECTION_THRESHOLD	0x37	// Detection Threshold for Spreading Factor 6
#define LORA_REG_SYNC_WORD					0x39	// Sync Word
#define LORA_REG_HIGH_BW_OPTIMIZE2		0x3A	// Sensitivity optimization for 500 kHz bandwidth
#define LORA_REG_INVERTIQ2					0x3B	// Optimize for inverted IQ

/* LoRa Operation Modes (section 4.1.3) */
#define LoRa_LOW_FREQ_MODE				0x08
#define LoRa_get_MODE(reg_val)			(reg_val&0b10000111)
#define LoRa_MODE_SLEEP					0x80
#define LoRa_MODE_STANDBY				0x81
#define LoRa_MODE_FSTX					0x82
#define LoRa_MODE_TX						0x83
#define LoRa_MODE_FSRX					0x84
#define LoRa_MODE_RX_CONT				0x85
#define LoRa_MODE_RX_SINGLE				0x86
#define LoRa_MODE_CAD					0x87

/* LoRa Cyclic Redundancy Check (CRC) */
#define LoRa_CRC_SET(reg_val)			(reg_val|0x04)
#define LoRa_CRC_RESET(reg_val)			(reg_val&0xFB)

/* LoRa Low Data Rate Optimize */
#define LoRa_LDO_SET(reg_val)			(reg_val|0b00001000)
#define LoRa_LDO_RESET(reg_val)			(reg_val&0b11110111)

/* LoRa Spreading Factor (section 4.1.1.2) */
#define LoRa_set_SF(reg_val, sf)			((reg_val&0x0F)|((sf<<4)&0xF0))

/* LoRa Power Amplifier (section 5.4.1 and 5.4.2)*/
#define LoRa_PA_RFO_PIN					0x70
#define LoRa_PA_BOOST_PIN				0x80

/* LoRa Over Current Protection (section 5.4.4) */
#define LoRa_OCP_SET(ocp_trim)			(0x20|(0x1F & ocp_trim))
#define LoRa_OCP_RESET					0x00

/* LoRa Coding Rate (section 4.1.1.3) */
#define LoRa_set_CR(reg_val, cr)			((reg_val&0xF1)|cr)
#define LoRa_CR_4_5						0x02
#define LoRa_CR_4_6						0x04
#define LoRa_CR_4_7						0x06
#define LoRa_CR_4_8						0x08
	
/* LoRa Header Modes (section 4.1.1.6) */
#define LoRa_EXPLICIT(reg_val)			(reg_val & 0b11111110)
#define LoRa_IMPLICIT(reg_val)			(reg_val | 0b00000001)

/* LoRa DIO LoRa Mapping (section 4.1.6.1) */
#define LoRa_RXDONE						0x00
#define LoRa_TXDONE						0x40

/* LoRa Idle config (section 4.2.8.2) */
#define LoRa_REG_SEQ_CONFIG_1			0x36

/* Macros */
#define LoRa_IGNORE_RESET_PIN			0xFFFF	// Use this in init to ignore reset pin
#define LoRa_TIMEOUT						100



/*
 * FSK register addresses
 */
#define FSK_REG_BITRATE_MSB		   		(uint8_t)0x02
#define FSK_REG_BITRATE_LSB     	  			(uint8_t)0x03
#define FSK_REG_FDEV_MSB						(uint8_t)0x04
#define FSK_REG_FDEV_LSB						(uint8_t)0x05

#define FSK_REG_RX_CONFIG	 					(uint8_t)0x0D	// RX configuration
#define FSK_REG_RSSI_CONFIG	 				(uint8_t)0x0E	// RSSI configuration
#define FSK_REG_RSSI_VALUE						(uint8_t)0x11
#define FSK_REG_RX_BW							(uint8_t)0x12	// Rx Bandwidth Configuration - Check page 88
#define FSK_REG_AFC_BW							(uint8_t)0x13 	// Automatic Frequency Correction Bandwidth Configuration
#define FSK_REG_PREAMBLE_DETECT				(uint8_t)0x1F	// Preamble Detector Settings
#define FSK_REG_PREAMBLE_LEN_MSB				(uint8_t)0x25	// Preamble Length Configuration MSB
#define FSK_REG_PREAMBLE_LEN_LSB				(uint8_t)0x26 	// Preamble Length Configuration LSB

#define FSK_REG_SYNC_CONFIG            	(uint8_t)0x27
#define FSK_REG_SYNC_VALUE_1           	(uint8_t)0x28
#define FSK_REG_SYNC_VALUE_2           	(uint8_t)0x29
#define FSK_REG_SYNC_VALUE_3           	(uint8_t)0x2A
#define FSK_REG_SYNC_VALUE_4           	(uint8_t)0x2B
#define FSK_REG_SYNC_VALUE_5           	(uint8_t)0x2C
#define FSK_REG_SYNC_VALUE_6           	(uint8_t)0x2D
#define FSK_REG_SYNC_VALUE_7           	(uint8_t)0x2E
#define FSK_REG_SYNC_VALUE_8           	(uint8_t)0x2F

#define FSK_REG_PACKET_CONFIG1         	(uint8_t)0x30
#define FSK_REG_PACKET_CONFIG2         	(uint8_t)0x31
#define FSK_REG_PAYLOAD_LENGHT         	(uint8_t)0x32
#define FSK_REG_FIFO_THRESH            	(uint8_t)0x35
#define FSK_REG_SEQ_CONFIG_1           	(uint8_t)0x36
#define FSK_REG_SEQ_CONFIG_2           	(uint8_t)0x37
#define FSK_REG_TIMER_RESOL            	(uint8_t)0x38
#define FSK_REG_TIMER_1_COEF           	(uint8_t)0x39
#define FSK_REG_TIMER_2_COEF           	(uint8_t)0x3A

#define FSK_REG_IRQ_FLAGS1             	(uint8_t)0x3E
#define FSK_REG_IRQ_FLAGS2             	(uint8_t)0x3F

#define FSK_REG_BITRATE_FRAC           	(uint8_t)0x5D

#define FSK_REG_PA_DAC							(uint8_t)0x4D

/*
 * IRQ FLAGS MASKS
 */
#define FSK_IRQ1_MODE_READY                (uint8_t)0b10000000
#define FSK_IRQ1_RX_READY                  (uint8_t)0b01000000
#define FSK_IRQ1_TX_READY                  (uint8_t)0b00100000
#define FSK_IRQ1_PLL_LOCK                  (uint8_t)0b00010000
#define FSK_IRQ1_RSSI                      (uint8_t)0b00001000
#define FSK_IRQ1_TIME_OUT                  (uint8_t)0b00000100
#define FSK_IRQ1_PREAMBLE_DETECTD          (uint8_t)0b00000010
#define FSK_IRQ1_SYNC_ADDR_MATCH           (uint8_t)0b00000001
#define FSK_IRQ2_FIFO_FULL                 (uint8_t)0b10000000    //66 BYTES MAXIMUM // Wasn't it 64 ?????
#define FSK_IRQ2_FIFO_EMPTY                (uint8_t)0b01000000
#define FSK_IRQ2_FIFO_LEVEL                (uint8_t)0b00100000
#define FSK_IRQ2_FIFO_OVERRUN              (uint8_t)0b00010000
#define FSK_IRQ2_PACK_SENT                 (uint8_t)0b00001000
#define FSK_IRQ2_PAYLOAD_READY             (uint8_t)0b00000100
#define FSK_IRQ2_CRC_OK                    (uint8_t)0b00000010
#define FSK_IRQ2_LOW_BAT                   (uint8_t)0b00000001


/*
 * Over Current Protection (section 5.4.4)
 */
#define FSK_OCP_SET(ocp_trim)	 		(0x20|(0x1F & ocp_trim))
#define FSK_LORA_OCP_RESET			 	0x00


/* 
 * Power Amplifier (section 5.4.1 and 5.4.2)
 */
#define FSK_PA_RFO_PIN					0x70
#define FSK_PA_BOOST_PIN				0x80


/*
 * Bitrate settings
 */
typedef struct {
	uint32_t BitrateValue;
	uint16_t RegisterValue;
} FSK_Bitrate_t; 

#define FSK_BITRATE_1_2_KBPS 			(FSK_Bitrate_t){  1200, 0x682B} 
#define FSK_BITRATE_2_4_KBPS			(FSK_Bitrate_t){  2400, 0x3415} 
#define FSK_BITRATE_4_8_KBPS			(FSK_Bitrate_t){  4799, 0x1A0B} 
#define FSK_BITRATE_9_6_KBPS			(FSK_Bitrate_t){  9600, 0x0D05} 
#define FSK_BITRATE_12_5_KBPS			(FSK_Bitrate_t){ 12500, 0x0A00} 
#define FSK_BITRATE_19_2_KBPS			(FSK_Bitrate_t){ 19196, 0x0683} 
#define FSK_BITRATE_25_KBPS 			(FSK_Bitrate_t){ 25000, 0x0500} 
#define FSK_BITRATE_38_4_KBPS 		(FSK_Bitrate_t){ 38415, 0x0341}
#define FSK_BITRATE_50_KBPS 			(FSK_Bitrate_t){ 50000, 0x8000} 
#define FSK_BITRATE_57_6_KBPS 		(FSK_Bitrate_t){ 57554, 0x020C}
#define FSK_BITRATE_76_8_KBPS  		(FSK_Bitrate_t){ 76738, 0x01A1}
#define FSK_BITRATE_100_KBPS 			(FSK_Bitrate_t){100000, 0x0140} 
#define FSK_BITRATE_150_KBPS 			(FSK_Bitrate_t){150234, 0x00D5} 
#define FSK_BITRATE_153_6_KBPS 		(FSK_Bitrate_t){153846, 0x00D0}
#define FSK_BITRATE_200_KBPS 			(FSK_Bitrate_t){200000, 0x00A0} 
#define FSK_BITRATE_250_KBPS 			(FSK_Bitrate_t){250000, 0x0080} 
#define FSK_BITRATE_300_KBPS 			(FSK_Bitrate_t){299065, 0x006B} 

/*
 * FSK Rx Bandwidths
 */
#define FSK_BW_2_6_KHZ						(uint8_t)0b00010111
#define FSK_BW_3_1_KHZ						(uint8_t)0b00001111
#define FSK_BW_3_9_KHZ						(uint8_t)0b00000111
#define FSK_BW_5_2_KHZ						(uint8_t)0b00010110
#define FSK_BW_6_3_KHZ						(uint8_t)0b00001110
#define FSK_BW_7_8_KHZ						(uint8_t)0b00000110
#define FSK_BW_10_4_KHZ						(uint8_t)0b00010101
#define FSK_BW_12_5_KHZ						(uint8_t)0b00001101
#define FSK_BW_15_6_KHZ						(uint8_t)0b00000101
#define FSK_BW_20_8_KHZ						(uint8_t)0b00010100
#define FSK_BW_25_0_KHZ						(uint8_t)0b00001100
#define FSK_BW_31_3_KHZ						(uint8_t)0b00000100
#define FSK_BW_41_7_KHZ						(uint8_t)0b00010011
#define FSK_BW_50_0_KHZ						(uint8_t)0b00001011
#define FSK_BW_62_5_KHZ						(uint8_t)0b00000011
#define FSK_BW_83_3_KHZ						(uint8_t)0b00010010
#define FSK_BW_100_0_KHZ					(uint8_t)0b00001010
#define FSK_BW_125_0_KHZ					(uint8_t)0b00000010
#define FSK_BW_166_7_KHZ					(uint8_t)0b00010001
#define FSK_BW_200_0_KHZ					(uint8_t)0b00001001
#define FSK_BW_150_0_KHZ					(uint8_t)0b00000001

#define SX127X_TIMEOUT						(uint8_t)100


/*
 * LoRa Bandwidths (section 4.1.1.4)
 */ 
#define LoRa_BW_CLR(reg_val)			(reg_val&0x0F)
#define LoRa_BW_MASK(reg_val)			(reg_val&0xF0)
#define LoRa_BW_7_8						(uint8_t)0x00	// 7.8 kHz
#define LoRa_BW_10_4					(uint8_t)0x10	// 10.4 kHz
#define LoRa_BW_15_6					(uint8_t)0x20	// 15.6 kHz
#define LoRa_BW_20_8					(uint8_t)0x30	// 20.8 kHz
#define LoRa_BW_31_25					(uint8_t)0x40	// 31.25 kHz
#define LoRa_BW_41_7					(uint8_t)0x50	// 41.7 kHz
#define LoRa_BW_62_5					(uint8_t)0x60	// 62.5 kHz
#define LoRa_BW_125						(uint8_t)0x70	// 125 kHz
#define LoRa_BW_250						(uint8_t)0x80	// 250 kHz
#define LoRa_BW_500						(uint8_t)0x90	// 500 kHz


/*
 * New Masks
 */

// Universal:
#define SX127X_OP_MODE_MASK             	(uint8_t)0b11111000
#define SX127X_FSK_OOK_MODE_MASK  			(uint8_t)0b00011111
#define SX127X_LORA_MODE_MASK		     	(uint8_t)0b01111111
#define SX127X_LOW_FREQUENCY_MODE_ON_MASK   (uint8_t)0b11110111
#define SX127X_LNA_BOOST_MASK				(uint8_t)0b11111100
#define SX127X_LNA_GAIN_MASK				(uint8_t)0b00011111
#define SX127X_PA_SELECT_MASK				(uint8_t)0b01111111

// LoRa:


// FSK:
#define FSK_AUTO_RESTART_RX_MASK			(uint8_t)0b00111111
#define FSK_PREAMBLE_POLARITY_MASK 			(uint8_t)0b11011111
#define FSK_PREAMBLE_DETECTOR_MASK 			(uint8_t)0b01111111
#define FSK_PREAMBLE_DETECTOR_TOL_MASK		(uint8_t)0b11100000
#define FSK_PREAMBLE_DETECTOR_SIZE_MASK		(uint8_t)0b10011111
#define FSK_SYNC_WORD_ON_MASK	 			(uint8_t)0b11101111
#define FSK_SYNC_WORD_SIZE_MASK	 			(uint8_t)0b11111000
#define FSK_PACKET_FORMAT_MASK     			(uint8_t)0b01111111
#define FSK_DATA_MODE_MASK     				(uint8_t)0b10111111
#define FSK_PAYLOAD_LENGTH_MASK     		(uint8_t)0b11111000
#define FSK_CRC_ON_MASK     				(uint8_t)0b11101111
#define FSK_CRC_AUTOCLEAR_OFF_MASK    		(uint8_t)0b11110111
#define FSK_CRC_WHITENING_TYPE_MASK			(uint8_t)0b11111110
#define FSK_RSSI_OFFSET_MASK				(uint8_t)0b00000111
#define FSK_RSSI_SMOOTHING_MASK				(uint8_t)0b11111000
#define FSK_DC_FREE_ENCODING_MASK			(uint8_t)0b10011111
#define FSK_MODULATION_SHAPING_MASK			(uint8_t)0b10011111
#define FSK_PA_RAMP_MASK					(uint8_t)0b11110000
#define FSK_FIFO_THRESHOLD_MASK				(uint8_t)0b11000000

/*
 * Other config masks
 */
#define FSK_SYNC_ON                        (uint8_t)0b00010000
#define FSK_MODE_READY                     (uint8_t)0b10000000
#define FSK_FSTEP						   			61            // Defined by: FSTEP = FXOSC/(2^19)  See section 4.1.4 


/*
 * New Defines
 */

#define FSK_SYNC_WORD_ENABLE	 			(uint8_t)0b00010000
#define FSK_SYNC_WORD_DISABLE	 			(uint8_t)0b00000000

#define FSK_CRC_ENABLE	 					(uint8_t)0b00010000
#define FSK_CRC_DISABLE	 					(uint8_t)0b00000000

#define FSK_CRC_AUTOCLEAR_ENABLE	 		(uint8_t)0b00001000
#define FSK_CRC_AUTOCLEAR_DISABLE		(uint8_t)0b00000000

//////////////////////////// NEW ////////////////////////////

/*
 * 
 * The Idea here is to add the configuration for the radio
 * to the "SX12x_t" data type.
 *
 * It will contain all the information (almost, read the
 * note at the end) and parameters required for the radio
 * to operate, like the SPI handler, the Chip Select and
 * Reset pins, and all current RF parameters for both FSK
 * and LoRa (maybe OOK?).
 *
 * Note: The only info not contained on the SX127X_t data
 * type are the DIO pin addresses, since they are usually
 * intended to work with interrupts and than need to be
 * configured accordingly on the used microcontroller.
 * 
 * The info is going to be contained on the config data
 * types "FSK_Config_t" and "LoRa_Config_t". Each one
 * will contain all necessary configuration info and will
 * be updated after any operation that configures the radio.
 * They may also contain different data types from the
 * standard ones, like "FSK_Bitrate_t" on the FSK configs.
 * 
 */
/*
enum
{
	true  = 1,
	false = 0
};
typedef uint8_t bool;
*/

typedef enum
{
	FSK_Modulation   = 0x00U,
	LORA_Modulation  = 0x80U
	//OOK_Modulation    0x20U  // If I have time to spare I'll do this one, but it's quite meh :p
}Modulation_t;

#define TX TXLEGAL
#define RX RXLEGAL

typedef enum
{
	SLEEP       = 0x00U,
	STANDBY     = 0x01U,
	FSTX        = 0X02U,
	TX          = 0X03U,
	FSRX        = 0X04U,
	RX          = 0X05U, // RX on FSK, RX continuous in LoRa.
	RXSINGLE    = 0X06U, // Only available on LoRa mode (reserved on FSK).
	CAD         = 0X07U  // Only available on LoRa mode (reserved on FSK).
}OperationMode_t;

typedef enum
{
	Polarity_AA = 0b00000000,
	Polarity_55 = 0b00100000
}PremblePolarity_t;

typedef enum
{
	FixedLength    = 0b00000000,
	VariableLength = 0b10000000
}PacketFormat_t;

typedef enum
{
	ContinuousMode = 0b00000000,
	PacketMode     = 0b01000000
}DataProcessingMode_t;

typedef enum
{
	None     = 0x00U,
	Manchester  = 0b00100000U,
	Whitenning  = 0b01000000U
}Encoding_t;

typedef enum
{
	NoFiltering                = 0x00U, // No filtering :p
	NodeFiltering              = 0b00000010U, // Address field must match NodeAddress
	NodeOrBroadcastFiltering   = 0b00000100U  // Address field must match NodeAddress or BroadcastAddress
}AddressFiltering_t;

typedef enum
{
	FSK_NoShaping  = 0x00U,
	Gaussian_1_0   = 0b00100000U,
	Gaussian_0_5   = 0b01000000U,
	Gaussian_0_3   = 0b01100000U
}FSK_DataShaping_t;

typedef enum
{
	OOK_NoShaping     = 0x00U,
	CutAtBitRate      = 0b00100000U,
	CutAtBitDoubleRate   = 0b01000000U
}OOK_DataShaping_t; // Not gonna use yet, but it's done just in case

typedef enum
{
	CCITT_CRC   = 0x00U, // CCITT CRC implementation with standard whitening
	IBM_CRC  = 0x01U  // IBM CRC implementation with alternate whitening
}CRC_WhiteningType_t;

typedef enum
{
	PLL_BW_75kHz   = 0x00U,
	PLL_BW_150kHz  = 0b01000000U,
	PLL_BW_225kHz  = 0b10000000U,
	PLL_BW_300kHz  = 0b11000000U
}PLL_Bandwidth_t;

typedef enum
{
	PaRampTime_3_4_ms = 0b00000000,
	PaRampTime_2_ms   = 0b00000001,
	PaRampTime_1_ms   = 0b00000010,
	PaRampTime_500_us = 0b00000011,
	PaRampTime_250_us = 0b00000100,
	PaRampTime_125_us = 0b00000101,
	PaRampTime_100_us = 0b00000110,
	PaRampTime_62_us  = 0b00000111,
	PaRampTime_50_us  = 0b00001000,
	PaRampTime_40_us  = 0b00001001, // Default
	PaRampTime_31_us  = 0b00001010,
	PaRampTime_25_us  = 0b00001011,
	PaRampTime_20_us  = 0b00001100,
	PaRampTime_15_us  = 0b00001101,
	PaRampTime_12_us  = 0b00001110,
	PaRampTime_10_us  = 0b00001111
} PaRampTime_t;

typedef enum
{
	LnaGainG1   = 0b00100000,  // highest gain // Default
	LnaGainG2   = 0b01000000,  // highest gain – 6 dB
	LnaGainG3   = 0b01100000,  // highest gain – 12 dB
	LnaGainG4   = 0b10000000,  // highest gain – 24 dB
	LnaGainG5   = 0b10100000,  // highest gain – 36 dB
	LnaGainG6   = 0b11000000   // highest gain – 48 dB
} LnaGain_t;

typedef enum
{
	AutorestartRX_Off       		= 0b00000000,
	AutorestartRX_On_NoPLL_Lock   	= 0b01000000, // On, without waiting for the PLL to re-lock.
	AutorestartRX_On_PLL_Lock  		= 0b10000000  // Default: On, wait for the PLL to lock (frequency changed).
} AutoRestartRX_t;

typedef enum
{
	RFO_Pin        = 0, // Maximum power of +14 dBm
	PA_BOOST_Pin   = 1  // Maximum power of +20 dBm
}PaSelect_t;

typedef enum
{
	RssiSmoothing_2_Samples    = 0b00000000,
	RssiSmoothing_4_Samples    = 0b00000001,
	RssiSmoothing_8_Samples    = 0b00000010, // Default
	RssiSmoothing_16_Samples   = 0b00000011,
	RssiSmoothing_32_Samples   = 0b00000100,
	RssiSmoothing_64_Samples   = 0b00000101,
	RssiSmoothing_128_Samples  = 0b00000110,
	RssiSmoothing_256_Samples  = 0b00000111,
}RssiSmoothing_t;

typedef enum
{
	NoTrigger         = 0b00000000,
	RssiInterrupt     = 0b00000001,
	PreambleDetect    = 0b00000110, // Default
	RssiAndPreamble   = 0b00000111
}RxTrigger_t;


// Check tables 29 and 30 on page 69 (nice) of the datasheet for more details
typedef enum{
	// Exclusive to Continuous Mode
	DIO0_SyncAddress_TxReady     = 0x00,
	DIO0_RSSI_PreambleDetect     = 0x01,
	DIO0_RxReady_TxReady         = 0x02,

	// Exclusive to Packet Mode
	DIO0_PayloadReady_PacketSent = 0x00,
	DIO0_CrcOk                   = 0x01,
	DIO0_TempChange_LowBat       = 0x03

} DIO0_Functions_t;

typedef enum{
	// Exclusive to Continuous Mode
	DIO1_Dclk                    = 0x00,
	DIO1_RSSI_PreambleDetect     = 0x01,

	// Exclusive to Packet Mode
	DIO1_FifoLevel               = 0x00,
	DIO1_FifoEmpty               = 0x01,
	DIO1_FifoFull                = 0x02

} DIO1_Functions_t;

typedef enum{
	// Exclusive to Continuous Mode
	DIO2_Data                    = 0x00, // This pin is used to display the data serially on Contiuous Mode. All values set it to Data.

	// Exclusive to Packet Mode
	DIO2_FifoFull                = 0x00,
	DIO2_RxReady                 = 0x01,
	DIO2_FifoFull_TimeOut        = 0x02,
	DIO2_FifoFull_SyncAddress    = 0x03

} DIO2_Functions_t;

typedef enum{
	// Exclusive to Continuous Mode
	DIO3_Timeout                 = 0x00,
	DIO3_RSSI_PreambleDetect     = 0x01,
	DIO3_TempChange_LowBat       = 0x03,

	// Exclusive to Packet Mode
	DIO3_FifoEmpty               = 0x00, // All values set it to FifoEmpty.
	DIO3_TxReady                 = 0x01


} DIO3_Functions_t;

typedef enum{
	// Both Modes
	DIO4_TempChange_LowBat       = 0x00,
	DIO4_PllLock                 = 0x01,
	DIO4_Timeout                 = 0x02,

	// Exclusive to Continuous Mode
	DIO4_ModeReady               = 0x03,

	// Exclusive to Packet Mode
	DIO4_RSSI_PreambleDetect     = 0x03

} DIO4_Functions_t;

typedef enum{
	// Both Modes
	DIO5_ClkOut                  = 0x00,
	DIO5_PllLock                 = 0x01,
	DIO5_ModeReady               = 0x03,

	// Exclusive to Continuous Mode
	DIO5_RSSI_PreambleDetect     = 0x02,

	// Exclusive to Packet Mode
	DIO5_Data                    = 0x02

} DIO5_Functions_t;

// Meu deus isso foi uma péssima ideia, tem umas 48 variações pros pinos DIO
/*
typedef enum
{
   RxReady
   PayloadReady
   TxReady
   PacketSent

}DIOPinFunctions_t;
 */

typedef struct{

	/*
	 * General Configuration:
	 */

	// Frequencie deviation and bitrate
	uint32_t FrequencyDeviation;
	FSK_Bitrate_t Bitrate;

	// Preamble stuff:
	uint16_t PreambleLength; // 0 to f*cking 65536 BYTES
	PremblePolarity_t PreamblePolarity;

	// Sync Word:
	bool SyncWordGeneration; //(0 -> Off, 1 -> On)
	uint8_t SyncWordSize; // 0 to 8 bytes
	uint64_t SyncWord;

	// Packet formating:
	PacketFormat_t PacketFormat; // (0 -> Fixed, 1 -> Variable Length)
	DataProcessingMode_t DataProcessingMode; // (0 -> Continuous Mode, 1 -> Packet Mode)
	uint16_t PayloadLength; // This represents different things in different modes:
	// 1) If PacketFormat = 0 (fixed), it represents the payload length (used in automatic FIFO operations).
	// 2) If PacketFormat = 1 (variable), it represents the max length in Rx, not used in Tx.

	// CRC stuff:
	bool CrcCalculation; // (0 -> Off, 1 -> On)
	CRC_WhiteningType_t CrcWhitenningType; // (0 -> CCITT CRC, 1 -> IBM CRC)

	// Modifications:
	Encoding_t Encoding; // (0 -> None, 1 -> Manchester, 2 -> Whitenning)
	FSK_DataShaping_t DataShaping; // Gaussian filters values

	/*
	 * RX Specific Configuration:
	 */

	// Bandwidth:
	uint8_t RxBandwidth;
	uint8_t AfcBandwidth;

	// Preamble Detection:
	bool PreambleDetectorOn; // (0 -> Off, 1 -> On)
	uint8_t PreambleDetectorSize; // Number of Preamble bytes (B) to detect to trigger an interrupt: 00->1B, 01->2B, 10->3B, 11->Reserved
	uint8_t PreambleDetectorTolerance; // Number or chip errors tolerated over PreambleDetectorSize. 4 chips per bit.

	// CRC Autoclear:
	bool CrcAutoclear; // (The register is inverted (CrcAutoclearOff): 1 -> Shows packet even when CRC is bad, 0 -> Discards it)

	// Auto Restart RX Mode
	AutoRestartRX_t AutoRestartRX;
	bool RestartRxOnCollision;

	// Address Filtering
	AddressFiltering_t AddressFiltering; // (None, Node Address, Node Address or Broadcast Address)
	uint8_t NodeAdress;
	uint8_t BroadcastAddress;

	// RSSI Stuff
	int8_t RssiOffset; // Signed RSSI offset, to compensate for the possible losses/gains in the front-end (LNA, SAW filter...) 1dB / LSB, 2’s complement format
	RssiSmoothing_t RssiSmoothing;

	// FIFO Stuff
	uint8_t FifoThreshold;


	// TO DO:
	//bool BeaconMode; // (0 -> Off, 1 -> On) (Bacon, hummmm ~~~~)

	// PLL Bandwidth (have no idea how to use this:D)
	// PllBandwidth_t PllBandwidth;

	// Other things I don't give a shit about:
	// bool IoHomeOn; // (0 -> Off, 1 -> On) ????????

	// Auto corrections:
	// bool AfcAutoOn;
	// bool AgcAutoOn;
	// RxTrigger_t RxTrigger; // Triggers AFC or AGC on receiver startup. Check table 24 on page 60 for more info.

}FSKConfig_t;

typedef struct{
	
	// Base addresses:
	uint8_t RxFifoBaseAddress;
	uint8_t TxFifoBaseAddress;

	// Bandwidth:
	uint8_t Bandwidth;




}LoRaConfig_t;

typedef struct{

	// ------- Basic Stuff  ------- //

	// SPI Handler:
	SPIClass * spi_bus;

	// Chip Select Pin:
	//GPIO_TypeDef* ss_gpio_port;
	uint16_t ss_pin;

	// Reset Pin:
	//GPIO_TypeDef* reset_gpio_port;
	uint16_t reset_pin;

	// ------- Radio Stuff  ------- //

	// Current Status:
	Modulation_t Current_Modulation;
	OperationMode_t Current_Op_Mode;
	//PacketInfo_t PacketInfo; // This one contains the packet array pointer, as well as its size (etc?).

	// ------- General Configurations  ------- // (Same registers on LoRa and FSK):
	
	// Frequency:
	long Frequency;

	// Over current protection:
	uint16_t OcpCurrent;

	// Receiver Gain Config:
	LnaGain_t LnaGain;
	bool LnaBoost; // (0 -> Default LNA current, 1 -> Boost on, 150% LNA current)

	// TX Power
	PaSelect_t PaSelect;
	uint8_t TxPower;
	PaRampTime_t PaRampTime; // Rise/Fall time of ramp up/down

	// DIO pins
	uint8_t DioMapping1;
	uint8_t DioMapping2;
	/*
   DIO0_Functions_t DIO0Function;
   DIO1_Functions_t DIO1Function;
   DIO2_Functions_t DIO2Function;
   DIO3_Functions_t DIO3Function;
   DIO4_Functions_t DIO4Function;
   DIO5_Functions_t DIO5Function;
	 */

	// ------- Specific Configurations  ------- //
	FSKConfig_t FSK_Config;
	LoRaConfig_t LoRa_Config;

}SX127X_t;


////////////////////////// END NEW //////////////////////////

/*
 * Universal Functions:
 */
HAL_StatusTypeDef SX127X_Reset(SX127X_t *SX127X);
HAL_StatusTypeDef SX127X_set_op_mode(SX127X_t *SX127X, OperationMode_t mode);
HAL_StatusTypeDef SX127X_set_modulation(SX127X_t *SX127X, Modulation_t modulation);
HAL_StatusTypeDef SX127X_set_frequency(SX127X_t *SX127X, long frequency);
HAL_StatusTypeDef SX127X_set_pa_output(SX127X_t *SX127X, PaSelect_t PaSelect);
HAL_StatusTypeDef SX127X_Config(SX127X_t *SX127X);
HAL_StatusTypeDef SX127X_set_tx_power(SX127X_t *SX127X, uint8_t TxPower);
HAL_StatusTypeDef SX127X_set_ocp(SX127X_t *SX127X, uint16_t current_mA);
HAL_StatusTypeDef SX127X_set_lna_gain(SX127X_t *SX127X, LnaGain_t LnaGain);
HAL_StatusTypeDef SX127X_set_lna_boost(SX127X_t *SX127X, bool LnaBoost);
HAL_StatusTypeDef SX127X_set_bits(SX127X_t *SX127X, uint8_t Reg, uint8_t Mask);
HAL_StatusTypeDef SX127X_clear_bits(SX127X_t *SX127X, uint8_t Reg, uint8_t Mask);
HAL_StatusTypeDef SX127X_set_DIO_mapping(SX127X_t *SX127X, uint8_t DIOMapping1, uint8_t DIOMapping2);


/*
 * FSK init functions
 */
HAL_StatusTypeDef FSK_init(SX127X_t *SX127X);

/*
 * FSK register write and read functions
 */
HAL_StatusTypeDef FSK_WriteToFIFO(SX127X_t *SX127X, uint8_t *Data, uint8_t PayloadLenght);
HAL_StatusTypeDef FSK_ReadFromFIFO(SX127X_t *SX127X, uint8_t *Packet, uint8_t Length);

/*
 * FSK status functions
 */
int FSK_OPModeStatus(SX127X_t *SX127X, uint8_t mode);
int FSK_IRQFlagStatus(SX127X_t *SX127X, uint8_t Register1or2, uint8_t flag);
bool FSK_CheckPayloadReady(SX127X_t *SX127X);

/*
 * FSK Transmit and Receive function
 */
HAL_StatusTypeDef FSK_Transmit(SX127X_t *SX127X, uint8_t *Data, uint8_t PayloadLength);
HAL_StatusTypeDef FSK_BigTransmit(SX127X_t *SX127X, uint8_t *Data, uint8_t PayloadLength);
HAL_StatusTypeDef FSK_ReadPacket(SX127X_t *SX127X, uint8_t *Packet, uint8_t MaximumLength ,uint8_t *PacketLength, bool *CrcError);
HAL_StatusTypeDef FSK_ReceivePacket(SX127X_t *SX127X, uint8_t *Packet, uint8_t MaximumLength, uint8_t *PacketLength, bool *CrcError);

/*
 * FSK config functions
 */
HAL_StatusTypeDef FSK_PutToRXMODE(SX127X_t *SX127X);
HAL_StatusTypeDef FSK_set_freq_deviation(SX127X_t *SX127X, uint32_t fdev, FSK_Bitrate_t bitrate);
HAL_StatusTypeDef FSK_set_bitrate(SX127X_t *SX127X, FSK_Bitrate_t bitrate);
HAL_StatusTypeDef FSK_set_rx_bandwidth(SX127X_t *SX127X, uint8_t bandwidth);
HAL_StatusTypeDef FSK_set_afc_bandwidth(SX127X_t *SX127X, uint8_t bandwidth);
HAL_StatusTypeDef FSK_set_preamb_len(SX127X_t *SX127X, uint16_t preambLength, PremblePolarity_t PreamblePolarity);
HAL_StatusTypeDef FSK_set_packet_config(SX127X_t *SX127X, uint8_t PacketConfig1, uint8_t packetConfig2);
HAL_StatusTypeDef FSK_set_sync_word(SX127X_t *SX127X, uint8_t SyncWordSize, uint64_t SyncWord);
HAL_StatusTypeDef FSK_disable_sync_word (SX127X_t *SX127X);
HAL_StatusTypeDef FSK_set_packet_format(SX127X_t *SX127X, PacketFormat_t PacketFormat);
HAL_StatusTypeDef FSK_set_data_processing_mode(SX127X_t *SX127X, DataProcessingMode_t DataProcessingMode);
HAL_StatusTypeDef FSK_set_payload_length(SX127X_t *SX127X, uint16_t PayloadLength);
HAL_StatusTypeDef FSK_enable_crc(SX127X_t *SX127X);
HAL_StatusTypeDef FSK_disable_crc(SX127X_t *SX127X);
HAL_StatusTypeDef FSK_enable_preamble_detector(SX127X_t *SX127X, uint8_t PreambleDetectorSize , uint8_t PreambleDetectorTolerance);
HAL_StatusTypeDef FSK_disable_preamble_detector(SX127X_t *SX127X);
HAL_StatusTypeDef FSK_set_crc_whitenning_type(SX127X_t *SX127X, CRC_WhiteningType_t CrcWhitenningType);
HAL_StatusTypeDef FSK_set_crc_autoclear(SX127X_t *SX127X, bool CrcAutoclear);
HAL_StatusTypeDef FSK_set_rssi_offset(SX127X_t *SX127X, int8_t RssiOffset);
HAL_StatusTypeDef FSK_set_rssi_smoothing(SX127X_t *SX127X, RssiSmoothing_t RssiSmoothing);
HAL_StatusTypeDef FSK_get_packet_rssi(SX127X_t *SX127X, uint8_t* rssi);
HAL_StatusTypeDef FSK_set_pa_ramp_time(SX127X_t *SX127X, PaRampTime_t PaRampTime);
HAL_StatusTypeDef FSK_set_data_shaping(SX127X_t *SX127X, FSK_DataShaping_t DataShaping);
HAL_StatusTypeDef FSK_set_encoding(SX127X_t *SX127X, Encoding_t Encoding);
HAL_StatusTypeDef FSK_set_auto_restart_RX(SX127X_t *SX127X, AutoRestartRX_t AutoRestartRX);
HAL_StatusTypeDef FSK_set_FIFO_threshold(SX127X_t *SX127X, uint8_t FifoThreshold);

/*
 * LoRa config functions
 */
HAL_StatusTypeDef LoRa_set_spreading_factor(SX127X_t* SX127x, uint8_t sf);
HAL_StatusTypeDef LoRa_set_signal_bandwidth(SX127X_t* SX127x, uint8_t bw);
HAL_StatusTypeDef LoRa_set_preamble_lenght(SX127X_t* SX127x, uint16_t lenght);
HAL_StatusTypeDef LoRa_set_sync_word(SX127X_t* SX127x, uint8_t sync_word);
HAL_StatusTypeDef LoRa_set_coding_rate(SX127X_t* SX127x, uint8_t coding_rate);
HAL_StatusTypeDef LoRa_set_ocp(SX127X_t* SX127x, uint16_t current_mA);
HAL_StatusTypeDef LoRa_disable_ocp(SX127X_t* SX127x);
HAL_StatusTypeDef LoRa_enable_crc(SX127X_t* SX127x);
HAL_StatusTypeDef LoRa_disable_crc(SX127X_t* SX127x);
HAL_StatusTypeDef LoRa_explicit_header_mode(SX127X_t* SX127x);
HAL_StatusTypeDef LoRa_implicit_header_mode(SX127X_t* SX127x);
HAL_StatusTypeDef LoRa_set_FIFO_base_address (SX127X_t* SX127X, uint8_t RxAddress, uint8_t TxAddress);

// enable/disable invert_iq
HAL_StatusTypeDef LoRa_set_ldo_flag(SX127X_t* SX127x);

/* SX127x Data */
HAL_StatusTypeDef LoRa_get_packet_rssi(SX127X_t* SX127x, uint8_t* rssi);
HAL_StatusTypeDef LoRa_get_packet_snr(SX127X_t* SX127x, float* snr);
HAL_StatusTypeDef LoRa_get_spreading_factor(SX127X_t* SX127x, uint8_t* sf);
HAL_StatusTypeDef LoRa_get_raw_signal_bandwidth(SX127X_t* SX127x, uint8_t* raw_sbw);
HAL_StatusTypeDef LoRa_get_signal_bandwidth(SX127X_t* SX127x, long* sbw);
HAL_StatusTypeDef LoRa_raw_sbw_to_long(uint8_t raw_sbw, long* sbw);
// packet_frequency_error
HAL_StatusTypeDef LoRa_random(SX127X_t* SX127x, uint8_t* random);
HAL_StatusTypeDef LoRa_enable_low_data_rate_optimization(SX127X_t *SX127X);
HAL_StatusTypeDef LoRa_disable_low_data_rate_optimization(SX127X_t *SX127X);

/* Packet Transmission */
HAL_StatusTypeDef LoRa_Transmit(SX127X_t* SX127x, void* packet, uint8_t length);
HAL_StatusTypeDef LoRa_ReadPacket(SX127X_t* SX127x, void* packet, uint8_t length, uint8_t *CRC_status);

/* Extras added by third party developers during prototyping :)*/
HAL_StatusTypeDef LoRa_Is_Rx_Done(SX127X_t* SX127x);
HAL_StatusTypeDef LoRa_Op_Mode_Check(SX127X_t* SX127x, uint8_t *read_value_location);
HAL_StatusTypeDef LoRa_DIO_Mapping_Config(SX127X_t* SX127x, uint8_t mode);
HAL_StatusTypeDef LoRa_Seq_Sandby_Idle(SX127X_t* SX127x);


#endif /* SX127X_H */
