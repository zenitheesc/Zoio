// ----------------- User Parameters ----------------- //

#define SERIAL_BAUD_RATE 115200
#define UTC_OFFSET 3
#define MAX_PACKETS 40

// ----------------- Includes ----------------- //

#include <SPI.h>
#include "SX127X.h"
#include <stdint.h>
#include <stdbool.h>



// ----------------- Pinout ----------------- //

#define BUZZER 23
#define RED_LED 13
#define GREEN_LED 12
#define BLUE_LED 25
#define BUTTON 0

#define RADIO_DIO0 26
#define RADIO_DIO1 35
#define RADIO_NRST 14
#define RADIO_SS 18
#define RADIO_MISO 19
#define RADIO_MOSI 27
#define RADIO_SCLK 5

#define OLED_SCL 15
#define OLED_SDA 4
#define OLED_RST 16


// ----------------- Button Debounce ----------------- //

#define DebounceInterval 500
unsigned long LastButtonPress = 0;


// ----------------- SPI Objects ----------------- //

// Uninitalised pointers to SPI objects
SPIClass *radio_spi = NULL;


// ----------------- Radio Packets Structs ----------------- //

SX127X_t SX127X;
bool DIO0_FLAG = false;
bool DIO1_FLAG = false;

typedef struct {
  // Position
  char zenith[7];  // Also contains \0 character at the end - used in strcmp later
  uint16_t numIncomingPackets;
} __attribute__((packed)) header_packet_values_t;

// Union of Packet data as struct and byte array
typedef union {
  header_packet_values_t values;
  uint8_t raw[sizeof(header_packet_values_t)];
} header_packet_t;


// ----------------- Program Control Variables ----------------- //

header_packet_t IncomingPacket;
uint8_t RxData[255];
int packetsReceived = 0;
int bytesReceived = 0;
uint8_t ReceivedDataSize;
bool CRCStatus;
volatile bool StartCollecting = false;
unsigned long previousPacketTimeStamp = 0;
unsigned long currentPacketTimeStamp = 0;
uint8_t *pPhoto;

// ----------------- Beeps ----------------- //

void hee() {  // Indicator for good packet reception
  tone(BUZZER, 3000);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(80);
  noTone(BUZZER);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}


void heehee() {  // Indicator for bad packet reception
  tone(BUZZER, 3000);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(80);
  noTone(BUZZER);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(50);
  tone(BUZZER, 3000);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(80);
  noTone(BUZZER);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}


void ImAliveBeeps() {
  tone(BUZZER, 2000);
  delay(100);
  tone(BUZZER, 2500);
  delay(100);
  tone(BUZZER, 3000);
  delay(200);
  noTone(BUZZER);
}


void PrintPhoto(uint8_t *pPhoto, int bytesReceived) {
  Serial.write(0xDE);
  Serial.write(0xAD);
  Serial.write(0xBE);
  Serial.write(0xEF);

  Serial.write(pPhoto, bytesReceived);
  Serial.flush();

  Serial.write(0xDE);
  Serial.write(0xAD);
  Serial.write(0xBE);
  Serial.write(0xEF);
}


// --------------------------------------------------------------------------- //
// ---------------------------------- Setup ---------------------------------- //
// --------------------------------------------------------------------------- //

void setup() {

  // ----- Serial port and Bluetooth Serial ----- //
  Serial.begin(SERIAL_BAUD_RATE);

  // ----- SPI initialization ----- //
  radio_spi = new SPIClass(VSPI);

  radio_spi->begin(RADIO_SCLK, RADIO_MISO, RADIO_MOSI, RADIO_SS);  //SCLK, MISO, MOSI, SS

  // Set up slave select pins as outputs as the Arduino API
  // Doesn't handle automatically pulling SS low, so each library (radio/flash) handles it idenpendetly
  pinMode(radio_spi->pinSS(), OUTPUT);  //VSPI SS

  // ----- Outputs ---- //
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RADIO_NRST, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

  // (This was done instead of dynamic allocation to avoid memory fragmenattion).
  pPhoto = (uint8_t *)malloc(MAX_PACKETS * 255);  // Saves enough space for MAX_PACKETS packets.
                                                  // This memory space is allocated once and reused for every packet.
                                                  // And no, you wont find a free anywhere
                                                  // And yes, I could have done a uint8_t Photo[MAX_PACKETS*255] and pointed at it, but
                                                  // I'm an electrical engineer, I have no idea what I'm doing.

  // ----- Inputs and Interrupts ----- //
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(RADIO_DIO0, INPUT);
  pinMode(RADIO_DIO1, INPUT);

  attachInterrupt(digitalPinToInterrupt(RADIO_DIO0), dealWithDIO0, RISING);
  attachInterrupt(digitalPinToInterrupt(RADIO_DIO1), dealWithDIO1, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON), dealWithButton, FALLING);

  FSKConfig(&SX127X);
  FSK_PutToRXMODE(&SX127X);
  FSK_set_FIFO_threshold(&SX127X, 9);  // Used for the FIFO Level Interrupt on DIO1 (when using PacketReceive).

  Serial.println("\nWaiting for packets...");
}


void loop() {

  while (!DIO1_FLAG) {  // Waits for DIO0(LoRa)/DIO1(FSK) Notification

    if ((millis() - previousPacketTimeStamp > 1000) && StartCollecting) {  // If there was a delay in between packets - Timeout
      if (packetsReceived >= (IncomingPacket.values.numIncomingPackets/2)) {  // If at least half the image has arrived - send it out anyway

        Serial.print("\nNew photo partially received");
        Serial.printf("| %d Packets received | %d Packets missing | %d Bytes received.", packetsReceived, IncomingPacket.values.numIncomingPackets - packetsReceived, bytesReceived);

        // Add the jpg ending bytes (even if the whole jpg hasn't arrived parts of the photo are readable).
        *(pPhoto + ++bytesReceived) = 0xFF;
        *(pPhoto + ++bytesReceived) = 0xD9;

        PrintPhoto(pPhoto, bytesReceived);

        Serial.println("\nDONE (With a half baked picture) :)");

        heehee();  // bad warning
      }

      StartCollecting = false;
      packetsReceived = 0;
      bytesReceived = 0;

      DIO1_FLAG = false;
      FSK_PutToRXMODE(&SX127X);
    }
  }

  FSK_ReceivePacket(&SX127X, RxData, sizeof(RxData), &ReceivedDataSize, &CRCStatus);
  previousPacketTimeStamp = millis();

  //Serial.printf("\nReceivedDataSize: %d\n", ReceivedDataSize);  // Uncommenting this affects the performance - uart is slow you know.

  if (!StartCollecting) {  // Checks the header packet:
    memcpy(IncomingPacket.raw, RxData, sizeof(IncomingPacket));
    // The text matched and the system can receive this amount of packets
    if (!strcmp(IncomingPacket.values.zenith, "ZENITH") && IncomingPacket.values.numIncomingPackets < MAX_PACKETS) {
      StartCollecting = true;

      //Serial.printf("\nString: %s    Comprimento: %d\n", IncomingPacket.values.zenith, IncomingPacket.values.numIncomingPackets); // Uncommenting this affects the performance - uart is slow you know.

    }

  } else {  // Collects the photo packets

    if (packetsReceived < MAX_PACKETS) {  // Protecting aginst illegal memory access just in case
      packetsReceived++;
      memcpy(pPhoto + bytesReceived, RxData, ReceivedDataSize);
      bytesReceived += ReceivedDataSize;

      // Uncommenting this affects the performance - printfs are hard you know.
      //Serial.printf("\npacketsReceived: %d",packetsReceived);

      if (packetsReceived == IncomingPacket.values.numIncomingPackets) {  // last packet

        // uint16_t finalTwoBytes = (uint16_t)(pPhoto[bytesReceived - 2] << 8 | pPhoto[bytesReceived - 1]);
        // Serial.printf("\nFinal two bytes: 0x%.4X \n", finalTwoBytes);

        Serial.printf("\nNew photo received | %d Packets | %d Bytes. Dumping Data\n", packetsReceived, bytesReceived);

        PrintPhoto(pPhoto, bytesReceived);

        Serial.println("\nDONE :)");

        //for (int i = 0; i < bytesReceived; i++) Serial.printf("%.2X ", pPhoto[i]);

        StartCollecting = false;
        packetsReceived = 0;
        bytesReceived = 0;
        hee();

        Serial.println("\nWaiting for new packets.\n");
      }

    } else {  // The packet limit has been violated - We restart packet reception
      Serial.println("\nToo many packets, It's too much power (>_<)\n");
      Serial.println("\nRestarting the reception of packtes (・3・)\n");

      StartCollecting = false;
      packetsReceived = 0;
      bytesReceived = 0;
    }
  }

  DIO1_FLAG = false;
  FSK_PutToRXMODE(&SX127X);
}


void dealWithDIO0() {
  DIO0_FLAG = true;
}

void dealWithDIO1() {
  DIO1_FLAG = true;
}

void dealWithButton() {
}
