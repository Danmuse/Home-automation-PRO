/*
 * rc522.h
 *
 *  Created on: 30 0ct. 2022
 *      Author: Mariano
 */

#ifndef RC522_H_
#define RC522_H_

#include "gpio.h"
#include "tipos.h"
#include "lcd.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>

/* Pinout LCP845-BRK

		P0.16	o		o	VDD 	3.3V
		P0.17	o		o	P0.00
		P0.18	o		o	P0.01
		P0.19	o		o	P0.02
		P0.20	o		o	P0.02
		P0.21	o		o	P0.04
		P0.22	o		o	P0.05
		P0.23	o		o	P0.06
		P0.05	o		o	P0.07
		P0.24	o		o	P1.0
		P0.25	o		o	P1.1
		P0.26	o		o	P1.2
		P0.27	o		o	P0.15
		P0.28	o		o	P0.14
SCK		P0.29	o		o	P0.13
MOSI	P0.30	o		o	P0.12
MISO	P0.31	o		o	P0.11
SDA/NSS	P0.09	o		o	P0.10
NRST	P0.08	o		o	P0.09
		GND		o		o	P0.08

 */

#define		nrst		0,8
#define		nss			0,9
#define		miso		0,31
#define		mosi		0,30
#define		sck			0,29

#define		ON			1
#define 	OFF			0

#define		MAX_LEN			16
#define 	BEEPSsOUND		100

//MF522 command bits
#define PCD_IDLE              0x00               //NO action; cancel current commands
#define PCD_AUTHENT           0x0E               //verify password key
#define PCD_RECEIVE           0x08               //receive data
#define PCD_TRANSMIT          0x04               //send data
#define PCD_TRANSCEIVE        0x0C               //send and receive data
#define PCD_RESETPHASE        0x0F               //reset
#define PCD_CALCCRC           0x03               //CRC check and caculation

//Mifare_One card command bits
#define PICC_REQIDL           0x26               //Search the cards that not into sleep mode in the antenna area
#define PICC_REQALL           0x52               //Search all the cards in the antenna area
#define PICC_ANTICOLL         0x93               //prevent conflict
#define PICC_SElECTTAG        0x93               //select card
#define PICC_AUTHENT1A        0x60               //verify A password key
#define PICC_AUTHENT1B        0x61               //verify B password key
#define PICC_READ             0x30               //read
#define PICC_WRITE            0xA0               //write
#define PICC_DECREMENT        0xC0               //deduct value
#define PICC_INCREMENT        0xC1               //charge up value
#define PICC_RESTORE          0xC2               //Restore data into buffer
#define PICC_TRANSFER         0xB0               //Save data into buffer
#define PICC_HALT             0x50               //sleep mode

//THe mistake code that return when communicate with MF522
#define MI_OK                 0
#define MI_NOTAGERR           1
#define MI_ERR                2

//------------------MFRC522 register ----------------------
//Page 0:Command and Status
#define     Reserved00            0x00
#define     CommandReg            0x01
#define     CommIEnReg            0x02
#define     DivlEnReg             0x03
#define     CommIrqReg            0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F
//Page 1:Command
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F
//Page 2:CFG
#define     Reserved20            0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg	          0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
//Page 3:TestRegister
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     Reserved31            0x3C
#define     Reserved32            0x3D
#define     Reserved33            0x3E
#define     Reserved34			  0x3F
//---------------------------------------------------------



//prototype functions
void delay1s(void);
void setup(void);
void MFRC522_Init(void);
void Write_MFRC522(uint8_t addr, uint8_t val);
uint8_t Read_MFRC522(uint8_t addr);
void SetBitMask(uint8_t reg, uint8_t mask);
void ClearBitMask(uint8_t reg, uint8_t mask);
void AntennaOn(void);
void AntennaOff(void);
void MFRC522_Reset(void);
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType);
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint32_t *backLen);
uint8_t MFRC522_Anticoll(uint8_t *serNum);
void CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);
uint8_t MFRC522_SelectTag(uint8_t *serNum);
uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum);
uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData);
uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData);
void MFRC522_Halt(void);
void showSerialNumber(void);
void sendToSerialASCII(int32_t sector, int32_t block, uint8_t status, uint8_t *str);
void sendToSerialHEX(int32_t block, uint8_t status, uint8_t *str);
void readDataASCII(void);
void readDataHEX(void);
void clearTagsMemory(void);
int8_t writeTagBlockMemory(void);
void witeDataToTagMemory(void);
void writeTagBlockData(int32_t block, uint8_t *data);
void readDataHEX(void);
void readDataASCII(void);
void sendToSerialASCII(int32_t , int32_t , uint8_t , uint8_t *);
void sendToSerialHEX(int32_t , uint8_t , uint8_t *);
int8_t readDataASCIIsectorBlock(uint8_t, uint8_t, uint8_t);

void beepOk( void );
void beepOkWrite( void );
void beepNok( void );
void InitRC522( void );

#endif /* RC522_H_ */


