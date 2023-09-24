/*
 * rc522.c
 *
 *  Created on: 01 feb. 2020
 *      Author: juliancho
 */


#include "rc522.h"

uint32_t demoraSpi;

const uint8_t tarjeta1[]={0x6b, 0x3c, 0xf, 0x1c, 0x44};
const uint8_t llaverito[]={0x7a, 0x86, 0x2f, 0x16, 0xc5};
uint8_t buzzer;
uint8_t fbuzzer;

uint32_t backlog;
uint8_t codigo[MAX_LEN];


/* Description: Shows TAG's serial number **************************************
 * Input parameter: null
 * Return: null					 */
void showSerialNumber(void)
{
	uint8_t serNum[11];	//ojo que necesito el doble para guardar los caracteres en Hexa
	uint8_t i;
	uint8_t status;
	uint8_t str[MAX_LEN];

	//4 bytes Serial number of card, the 5th bytes is verfiy bytes (xor bytes 1 -4)

	setup();
	//Search card, return card types
	status = MFRC522_Request(PICC_REQIDL, str);

	//Prevent conflict, return the 4 bytes Serial number of the card
	status = MFRC522_Anticoll(str);
	for(i=0; i<5; i++)
		serNum[i]=str[i];

	serNum[i] = '\0';
	if (status == MI_OK)
	{
		GPIO_Set( PIN_ROJO, LED_ON );
		sprintf((char *)serNum," %x %x %x %x %x ",str[0],str[1],str[2],str[3], str[4]);
		if( !strncmp( (char *)tarjeta1, (char *)str, 5) || !strncmp( (char *)llaverito, (char *)str, 5) )
		{
			beepOk();
		}
		else
		{
			beepNok();
		}
		Display_LCD( serNum , 1 , 0 ) ;
		GPIO_Set( PIN_ROJO, LED_OFF );
	}
	MFRC522_Halt();
}





/* Description: initilize RS232, SPI, pin **************************************
 * Input parameter: null
 * Return: null					 */
void setup(void)
{
	GPIO_Set( nss, OFF );
	//nss = 0;						//Activate the RFID reader
	GPIO_Set( nrst, ON );
	//nrst=1;							//digitalWrite(NRSTPD,HIGH);
	MFRC522_Init();
}

/* Description: initialize RC522 ************************************************
 * Input parameter: null
 * Return: null					 */
void MFRC522_Init(void) {
	GPIO_Set( nrst, ON );
	//nrst = 1;							//digitalWrite(NRSTPD,HIGH);
	MFRC522_Reset();
	//Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    Write_MFRC522(TModeReg, 	0x8D);			//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    Write_MFRC522(TPrescalerReg,0x3E);			//TModeReg[3..0] + TPrescalerReg
    Write_MFRC522(TReloadRegL, 	30	);
    Write_MFRC522(TReloadRegH, 	0	);
	Write_MFRC522(TxAutoReg, 	0x40);			//100%ASK
	Write_MFRC522(ModeReg, 		0x3D);			//CRC initilizate value 0x6363
	//ClearBitMask(Status2Reg, 	0x08);			//MFCrypto1On=0
	//Write_MFRC522(RxSelReg, 	0x86);			//RxWait = RxSelReg[5..0]
	//Write_MFRC522(RFCfgReg, 	0x7F);   		//RxGain = 48dB
	AntennaOn();								//turn on antenna
}


/* Description: reset RC522 ****************************************************
 * Input parameter:null
 * Return:null					*/
void MFRC522_Reset(void)
{
	Write_MFRC522(CommandReg, PCD_RESETPHASE);
}

/* Description: Turn on antenna, every time turn on or shut down antenna need at least 1ms delay
 * Input parameter: null
 * Return: null						*/
void AntennaOn(void)
{
	uint8_t temp;
	temp = Read_MFRC522(TxControlReg);
	if (!(temp & 0x03))
	{
		SetBitMask(TxControlReg, 0x03);
	}
}

/* Description: set RC522 register bit *****************************************
 * Input parameter:reg--register address; mask--value
 * Return: null						*/
void SetBitMask(uint8_t reg, uint8_t mask)
{
	uint8_t tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask);  	// set bit mask
}

/* Description: clear RC522 register bit ***************************************
 * Input parameter: reg--register address; mask--value
 * Return: null 						*/
void ClearBitMask(uint8_t reg, uint8_t mask)
{
	uint8_t tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask));	 // clear bit mask
}

/* Description: write a byte data into one register of MFRC522 *****************
 * Input parameter: addr--register address; val--the value that need to write in
 * Return: Null						*/
void Write_MFRC522(uint8_t addr, uint8_t val)
{
	SPI_HW_Enable( );						//ClearCSSWSPI();	//digitalWrite(chipSelectPin, LOW);
	SPI_HW_Write( (addr<<1)&0x7E );			//WriteSWSPI((addr<<1)&0x7E);		//address format: 0XXXXXX0
	SPI_HW_Write( val );					//WriteSWSPI(val);
	SPI_HW_Disable( );						//SetCSSWSPI();		//digitalWrite(chipSelectPin, HIGH);

}

/* Description: read a byte data into one register of MFRC522 ******************
 * Input parameter: addr--register address
 * Return: return the read value		*/
uint8_t Read_MFRC522(uint8_t addr)
{
	uint8_t val;
	SPI_HW_Enable();		//ClearCSSWSPI();	//digitalWrite(chipSelectPin, LOW);
	SPI_HW_Write(((addr<<1)&0x7E) | 0x80);	//address format: 1XXXXXX0
	val = SPI_HW_Read( );
	SPI_HW_Disable();		//SetCSSWSPI();		//digitalWrite(chipSelectPin, HIGH);

	return val;


}


/* Description: 1 s delay  *****************************************************
 * Input parameter: null
 * Return: null					 */
void delay1s(void)			//ARREGLAR ESTO
{
	demoraSpi = 400;
	while( demoraSpi );
}

/* Description: Prevent conflict, read the card serial number ******************
 * Input parameter: serNum--return the 4 bytes card serial number, the 5th byte is recheck byte
 * return: return MI_OK if successed			*/
uint8_t MFRC522_Anticoll(uint8_t *serNum)
{
	uint8_t status;
	uint8_t i;
	uint8_t serNumCheck=0;
    uint32_t unLen;
    //ClearBitMask(Status2Reg, 0x08);		//TempSensclear
    //ClearBitMask(CollReg,0x80);			//ValuesAfterColl
	Write_MFRC522(BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
    if (status == MI_OK)
    {
		//Verify card serial number
		for (i=0; i<4; i++)
		{
			serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i])
		{
			status = MI_ERR;
		}
    }
    //SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1
    return status;
}


/* Description: communicate between RC522 and ISO14443 *************************
 * Input parameter: command--MF522 command bits
 *			 sendData--send data to card via rc522
 *			 sendLen--send data length
 *			 backData--the return data from card
 *			 backLen--the length of return data
 * return: return MI_OK if successed				*/
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint32_t *backLen)
{
	uint8_t status = MI_ERR;
	uint8_t irqEn = 	0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits;
	uint8_t n;
    uint32_t i;
    switch (command)
    {
        case PCD_AUTHENT: 		//verify card password
			irqEn = 0x12;
			waitIRq = 0x10;
			break;

		case PCD_TRANSCEIVE:	//send data in the FIFO
			irqEn = 0x77;
			waitIRq = 0x30;
			break;

		default:
			break;
    }
    Write_MFRC522(CommIEnReg, irqEn|0x80);	//Allow interruption
    ClearBitMask(CommIrqReg, 0x80);			//Clear all the interrupt bits
    SetBitMask(FIFOLevelReg, 0x80);			//FlushBuffer=1, FIFO initilizate
	Write_MFRC522(CommandReg, PCD_IDLE);	//NO action;cancel current command
	//write data into FIFO
    for (i=0; i<sendLen; i++)
    {
    	Write_MFRC522(FIFODataReg, sendData[i]);
    }
	//procceed it
	Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {
    	SetBitMask(BitFramingReg, 0x80);	//StartSend=1,transmission of data starts
    }
	//waite receive data is finished
	i = 2000;	//i should adjust according the clock, the maxium the waiting time should be 25 ms
    do {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_MFRC522(CommIrqReg);
        i--;
    }while ((i!=0) && !(n&0x01) && !(n&waitIRq));
    ClearBitMask(BitFramingReg, 0x80);			//StartSend=0
    if (i != 0)
    {
        if(!(Read_MFRC522(ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {
            	status = MI_NOTAGERR;
            }
            if (command == PCD_TRANSCEIVE)
            {
               	n = Read_MFRC522(FIFOLevelReg);
              	lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                {
                	*backLen = (n-1)*8 + lastBits;
                }
                else
                {
                	*backLen = n*8;
                }
                if (n == 0)
                {
                	n = 1;
                }
                if (n > MAX_LEN)
                {
                	n = MAX_LEN;
                }
				//read the data from FIFO
                for (i=0; i<n; i++)
                {
                	backData[i] = Read_MFRC522(FIFODataReg);
                }
            }
        }
        else
        {
        	status = MI_ERR;
        }
    }
    //SetBitMask(ControlReg,0x80);           	//timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE);
    return status;
}


/* Description: Command the cards into sleep mode ******************************
 * Input parameters: null
 * return: null 						*/
void MFRC522_Halt(void)
{
	uint8_t status;
    uint32_t unLen;
    uint8_t buff[4];
    buff[0] = PICC_HALT;
    buff[1] = 0;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
    if(status == MI_OK )
    {

    }
}

/* Description: Use MF522 to caculate CRC **************************************
 * Input parameter: pIndata--the CRC data need to be read,len--data length,pOutData-- the caculated result of CRC
 * return: Null
 */
void CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData)
{
	uint8_t i, n;
    ClearBitMask(DivIrqReg, 0x04);			//CRCIrq = 0
    SetBitMask(FIFOLevelReg, 0x80);			//Clear FIFO pointer
    //Write_MFRC522(CommandReg, PCD_IDLE);
	//Write data into FIFO
    for (i=0; i<len; i++)
    {
    	Write_MFRC522(FIFODataReg, *(pIndata+i));
    }
    Write_MFRC522(CommandReg, PCD_CALCCRC);
	//waite CRC caculation to finish
    i = 0xFF;
    do {
        n = Read_MFRC522(DivIrqReg);
        i--;
    }while ((i!=0) && !(n&0x04));			//CRCIrq = 1
	//read CRC caculation result
    pOutData[0] = Read_MFRC522(CRCResultRegL);
    pOutData[1] = Read_MFRC522(CRCResultRegM);
}

/* Description: Searching card, read card type *********************************
 * Input parameter: reqMode -- search methods,
 *			 TagType--return card types
 *			 	0x4400 = Mifare_UltraLight
 *				0x0400 = Mifare_One(S50)
 *				0x0200 = Mifare_One(S70)
 *				0x0800 = Mifare_Pro(X)
 *				0x4403 = Mifare_DESFire
 * return:return MI_OK if successed		*/
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType)
{
	uint8_t status;
	uint32_t backBits;							//the data bits that received

	Write_MFRC522(BitFramingReg, 0x07);			//TxLastBists = BitFramingReg[2..0]
	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10))
	{
		status = MI_ERR;
	}
	return status;
}

void beepOkWrite( void )
{
	buzzer = BEEPSsOUND;
	fbuzzer = 2;
}

void beepOk( void )
{
	buzzer = BEEPSsOUND;
	fbuzzer = 1;
}
void beepNok( void )
{
	buzzer = BEEPSsOUND;
	fbuzzer = 0;
}

/* Description: write  TAG's memory bytes *****************************************
 * Input parameter: null
 * Return: null					 */
int8_t writeTagBlockMemory(void)
{
	//4 bytes Serial number of card, the 5th bytes is verify bytes
	uint8_t serNum[5];
	//buffer A password, 16 buffer, the password of every buffer is 6 bytes
	uint8_t sectorX_KeyA[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	uint8_t i;
	uint8_t status;
	uint8_t str[MAX_LEN];
    //Select operation buck address  0 - 63
	//char msg1[]={"Writing TAG's memory block"};
  //SECTOR 00     //0123456789ABCDEF
  //uchar data00[]="			    ";  //Do not write serial number or manufacturer data
  //uchar data01[]="William Martinez";
  //uchar data02[]="ID: 5602-8788-ME";
  //uchar data03[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 01
	uint8_t data04[]="Juliancho ";
	uint8_t data05[]="21102002";
	uint8_t data06[]="53722097";
  //uchar data07[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 02
//	uchar data08[]="Insurance data  ";
//	uchar data09[]="2014-BA         ";
//	uchar data10[]="Health service  ";
    //uchar data11[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 03
//	uchar data12[]="Hola a todos hoy";
//	uchar data13[]="hace un dia con ";
//	uchar data14[]="  buen clima!   ";
  //uchar data15[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 04
//	uchar data16[]="     _ /\ _     ";
//	uchar data17[]="     \ oo /     ";
//	uchar data18[]="     /    \     ";
  //uchar data19[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 05
//	uchar data20[]="                ";
//	uchar data21[]="                ";
//	uchar data22[]="                ";
  //uchar data23[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 06
//	uchar data24[]="                ";
//	uchar data25[]="                ";
//	uchar data26[]="                ";
  //uchar data27[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 07
//	uchar data28[]="                ";
//	uchar data29[]="                ";
//	uchar data30[]="                ";
  //uchar data31[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFFx07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 08
//	uchar data32[]="                ";
//	uchar data33[]="                ";
//	uchar data34[]="                ";
  //uchar data35[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 09
//	uchar data36[]="                ";
//	uchar data37[]="                ";
//	uchar data38[]="                ";
  //uchar data39[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 10
//	uchar data40[]="                ";
//	uchar data41[]="                ";
//	uchar data42[]="                ";
  //uchar data43[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 11
//	uchar data44[]="                ";
//	uchar data45[]="                ";
//	uchar data46[]="                ";
  //uchar data47[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 12
//	uchar data48[]="                ";
//	uchar data49[]="                ";
//	uchar data50[]="                ";
  //uchar data51[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 13
//	uchar data52[]="                ";
//	uchar data53[]="                ";
//	uchar data54[]="                ";
  //uchar data55[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 14
//	uchar data56[]="                ";
//	uchar data57[]="                ";
//	uchar data58[]="                ";
  //uchar data59[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
  //SECTOR 15
//	uchar data60[]="                ";
//	uchar data61[]="                ";
//	uchar data62[]="                ";
  //uchar data63[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,x07,0x80,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};	//Warning: only write sector trailer when you know what you're doing
	setup();

	//Search card, return card types
	status = MFRC522_Request(PICC_REQIDL, str);
	//if (status == MI_OK)
	//{

	//}
	//else
	//	{
		//beepNok();
	//	return (-1);
	//	}
	//Prevent conflict, return the 4 bytes Serial number of the card
	status = MFRC522_Anticoll(str);
	for(i=0; i<5; i++)
	{
		serNum[i]=str[i];
	}	//memcpy(serNum, str, 5);
	if (status == MI_OK)
	{

	}
	else
		{
		//beepNok();
		return (-1);
		}
		//putsUSART(msg1);putcUSART('\r');Delay10KTCYx(10);    }
	status = MFRC522_SelectTag(serNum);
  //status = MFRC522_Auth(0x60,1,sectorX_KeyA,serNum);
//		status = MFRC522_Auth(0x60,0,sectorX_KeyA,serNum);	//sector 0
//		if(status==MI_OK){
//			writeTagBlockData(1,data01);
//			writeTagBlockData(2,data02);	}
	status = MFRC522_Auth(0x60,4,sectorX_KeyA,serNum);	//sector 1
	if(status==MI_OK)
	{
		writeTagBlockData(4,data04);
		writeTagBlockData(5,data05);
		writeTagBlockData(6,data06);
	}
	else
		{
		//beepNok();
		return (-1);
		}
//		status = MFRC522_Auth(0x60,8,sectorX_KeyA,serNum);	//sector 2
//		if(status==MI_OK){
//			writeTagBlockData(8,data08);
//			writeTagBlockData(9,data09);
//			writeTagBlockData(10,data10);	}
//		status = MFRC522_Auth(0x60,12,sectorX_KeyA,serNum);	//sector 3
//		if(status==MI_OK){
//			writeTagBlockData(12,data12);
//			writeTagBlockData(13,data13);
//			writeTagBlockData(14,data14);	}
//		status = MFRC522_Auth(0x60,16,sectorX_KeyA,serNum);	//sector 4
//		if(status==MI_OK){
//			writeTagBlockData(16,data16);
//			writeTagBlockData(17,data17);
//			writeTagBlockData(18,data18);	}
//		status = MFRC522_Auth(0x60,20,sectorX_KeyA,serNum);	//sector 5
//		if(status==MI_OK){
//			writeTagBlockData(20,data20);
//			writeTagBlockData(21,data21);
//			writeTagBlockData(22,data22);	}
//		status = MFRC522_Auth(0x60,24,sectorX_KeyA,serNum);	//sector 6
//		if(status==MI_OK){
//			writeTagBlockData(24,data24);
//			writeTagBlockData(25,data25);
//			writeTagBlockData(26,data26);	}
//		status = MFRC522_Auth(0x60,28,sectorX_KeyA,serNum);	//sector 7
//		if(status==MI_OK){
//			writeTagBlockData(28,data28);
//			writeTagBlockData(29,data29);
//			writeTagBlockData(30,data30);	}
//		status = MFRC522_Auth(0x60,32,sectorX_KeyA,serNum);	//sector 8
//		if(status==MI_OK){
//			writeTagBlockData(32,data32);
//			writeTagBlockData(33,data33);
//			writeTagBlockData(34,data34);	}
//		status = MFRC522_Auth(0x60,36,sectorX_KeyA,serNum);	//sector 9
//		if(status==MI_OK){
//			writeTagBlockData(36,data36);
//			writeTagBlockData(37,data37);
//			writeTagBlockData(38,data38);	}
//		status = MFRC522_Auth(0x60,40,sectorX_KeyA,serNum);	//sector 10
//		if(status==MI_OK){
//			writeTagBlockData(41,data41);
//			writeTagBlockData(42,data42);
//			writeTagBlockData(43,data43);	}
//		status = MFRC522_Auth(0x60,44,sectorX_KeyA,serNum);	//sector 11
//		if(status==MI_OK){
//			writeTagBlockData(44,data44);
//			writeTagBlockData(45,data45);
//			writeTagBlockData(46,data46);	}
//		status = MFRC522_Auth(0x60,48,sectorX_KeyA,serNum);	//sector 12
//		if(status==MI_OK){
//			writeTagBlockData(48,data48);
//			writeTagBlockData(49,data49);
//			writeTagBlockData(50,data50);	}
//		status = MFRC522_Auth(0x60,52,sectorX_KeyA,serNum);	//sector 13
//		if(status==MI_OK){
//			writeTagBlockData(52,data52);
//			writeTagBlockData(53,data53);
//			writeTagBlockData(54,data54);	}
//		status = MFRC522_Auth(0x60,56,sectorX_KeyA,serNum);	//sector 14
//		if(status==MI_OK){
//			writeTagBlockData(56,data56);
//			writeTagBlockData(57,data57);
//			writeTagBlockData(58,data58);	}
//		status = MFRC522_Auth(0x60,60,sectorX_KeyA,serNum);	//sector 15
//		if(status==MI_OK){
//			writeTagBlockData(60,data60);
//			writeTagBlockData(61,data61);
//			writeTagBlockData(62,data62);	}
	MFRC522_Halt();
	beepOkWrite();
	return 0;

}

/* Description: Select card, read card storage volume *************************
 * Input parameter :serNum--Send card serial number
 * return: return the card storage volume			 */
uint8_t MFRC522_SelectTag(uint8_t *serNum)
{
	uint8_t i;
	uint8_t status;
	uint8_t size;
    uint32_t recvBits;
    uint8_t buffer[9];
	//ClearBitMask(Status2Reg, 0x08);			//MFCrypto1On=0
    buffer[0] = PICC_SElECTTAG;
    buffer[1] = 0x70;
    for (i=0; i<5; i++)
    {
    	buffer[i+2] = *(serNum+i);
    }
	CalulateCRC(buffer, 7, &buffer[7]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
    if ((status == MI_OK) && (recvBits == 0x18))
    {
    	size = buffer[0];
    }
    else
    {
    	size = 0;
    }
    return size;
}

/* Description: Write data to TAG's memory ************************************
 * Input parameter: block to be written, dataArray
 * Return: null					 */
void writeTagBlockData(int32_t block, uint8_t *data) {
	uint8_t status;
	int8_t string[31];
	sprintf( (char*)string, "Sector %02d successfully written", block);
	status = MFRC522_Write(block, data);
	if(status == MI_OK)
	{

	}
}

/* Description:verify card password ********************************************
 * Input parameters:authMode--password verify mode
                 0x60 = verify A passowrd key
                 0x61 = verify B passowrd key
             BlockAddr--Block address
             Sectorkey--Block password
             serNum--Card serial number ,4 bytes
 * return:return MI_OK if successed				*/
uint8_t MFRC522_Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum)
{
	uint8_t status;
    uint32_t recvBits;
    uint8_t i;
    uint8_t buff[12];
	//Verify command + block address + buffer password + card SN
    buff[0] = authMode;
    buff[1] = BlockAddr;
    for (i=0; i<6; i++)
    {
    	buff[i+2] = *(Sectorkey+i);
    }
    for (i=0; i<4; i++)
    {
    	buff[i+8] = *(serNum+i);
    }
    status = MFRC522_ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);

    if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08)))
    {
    	status = MI_ERR;
    }
    return status;
}

/* Description: write block data ***********************************************
 * Input parameters: blockAddr--block address; writeData--Write 16 bytes data into block
 * return: return MI_OK if successed						*/

uint8_t MFRC522_Write(uint8_t blockAddr, uint8_t *writeData)
{
	uint8_t status;
    uint32_t recvBits;
    uint8_t i;
    uint8_t buff[18];
    buff[0] = PICC_WRITE;
    buff[1] = blockAddr;
    CalulateCRC(buff, 2, &buff[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {
    	status = MI_ERR;
    }
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {
        	buff[i] = *(writeData+i);	//Write 16 bytes data into FIFO
        }
        CalulateCRC(buff, 16, &buff[16]);
        status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
		if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
		{
			status = MI_ERR;
		}
    }
    return status;
}

/* Description: Read data ******************************************************
 * Input parameters: blockAddr--block address; recvData--the block data which are read
 * return: return MI_OK if successed						*/
uint8_t MFRC522_Read(uint8_t blockAddr, uint8_t *recvData)
{
	uint8_t status;
    uint32_t unLen;
    recvData[0] = PICC_READ;
    recvData[1] = blockAddr;
    CalulateCRC(recvData,2, &recvData[2]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
    if ((status != MI_OK) || (unLen != 0x90))
    {
    	status = MI_ERR;
    }
    return status;
}

/* Description: Send data read to serial monitor HEX format ********************
 * Input parameter: null
 * Return: null					 */
void readDataHEX(void)
{
	//4 bytes Serial number of card, the 5 bytes is verfiy bytes
	uint8_t serNum[5];
	//buffer A password, 16 buffer, the passowrd of every buffer is 6 byte
	uint8_t sectorX_KeyA[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	uint8_t i,j;
	uint8_t status;
	uint8_t str[MAX_LEN];
    //Select operation buck address  0 - 63
	setup();
	//Search card, return card types
	status = MFRC522_Request(PICC_REQIDL, str);	//ESTO NO DA OK
	if (status == MI_OK)
	{

	}

	//Prevent conflict, return the 4 bytes Serial number of the card
	status = MFRC522_Anticoll(str);
	for(i=0; i<5; i++)
		serNum[i]=str[i];

	if (status == MI_OK)
	{

	}
	status = MFRC522_SelectTag(serNum);
	status = MFRC522_Auth(0x60,0,sectorX_KeyA,serNum);	//sector 0
	for(j=0;j<4;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,4,sectorX_KeyA,serNum);	//sector 1
	for(j=4;j<8;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,8,sectorX_KeyA,serNum);	//sector 2
	for(j=8;j<12;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,12,sectorX_KeyA,serNum);	//sector 3
	for(j=12;j<16;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,16,sectorX_KeyA,serNum);	//sector 4
	for(j=16;j<20;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,20,sectorX_KeyA,serNum);	//sector 5
	for(j=20;j<24;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,24,sectorX_KeyA,serNum);	//sector 6
	for(j=24;j<28;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,28,sectorX_KeyA,serNum);	//sector 7
	for(j=28;j<32;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,32,sectorX_KeyA,serNum);	//sector 8
	for(j=32;j<36;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,36,sectorX_KeyA,serNum);	//sector 9
	for(j=36;j<40;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,40,sectorX_KeyA,serNum);	//sector 10
	for(j=40;j<44;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,44,sectorX_KeyA,serNum);	//sector 11
	for(j=44;j<48;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,48,sectorX_KeyA,serNum);	//sector 12
	for(j=48;j<52;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,52,sectorX_KeyA,serNum);	//sector 13
	for(j=52;j<56;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,56,sectorX_KeyA,serNum);	//sector 14
	for(j=56;j<60;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	status = MFRC522_Auth(0x60,60,sectorX_KeyA,serNum);	//sector 15
	for(j=60;j<64;j++)
	{
		status = MFRC522_Read(j, str);
		sendToSerialHEX(j,status, str);
	}
	MFRC522_Halt();

}

/* Description: Send data read to serial monitor ASCII format *******************
 * Input parameter: null
 * Return: null					 */
void readDataASCII(void)
{
	//4 bytes Serial number of card, the 5 bytes is verfiy bytes
	uint8_t serNum[5];
	//buffer A password, 16 buffer, the passowrd of every buffer is 6 byte
	uint8_t sectorX_KeyA[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	uint8_t i,j, sector = 0;
	uint8_t status;
	uint8_t str[MAX_LEN];
	int8_t msg2[]={"\n TAG's data in ASCII format:"};
	setup();
	//Search card, return card types
	status = MFRC522_Request(PICC_REQIDL, str);
	if (status == MI_OK)
	{

	}

	status = MFRC522_Anticoll(str);
	for(i=0; i<5; i++)
		serNum[i]=str[i];
	if (status == MI_OK)
	{
		UART0_PushTx('\r');
		UART0_PushTx('\n');
		UART0_Send( (char*) msg2, 0);
		UART0_PushTx('\r');
		UART0_PushTx('\n');

	}
	status = MFRC522_SelectTag(serNum);

	i = 0;
	status = MFRC522_Auth(0x60,sector,sectorX_KeyA,serNum);	//sector 0
	for(j=0;j<64;j++)
	{
		if( !(j%4) && j >= 4 )
		{
		sector += 4;
		status = MFRC522_Auth(0x60,sector,sectorX_KeyA,serNum);	//sector x+4
		i++;
		}

		status = MFRC522_Read(j, str);
		sendToSerialASCII(i,j,status, str);


	}
	MFRC522_Halt();
 }

int8_t readDataASCIIsectorBlock( uint8_t sector, uint8_t block, uint8_t line)
{
	//4 bytes Serial number of card, the 5 bytes is verfiy bytes
		uint8_t serNum[5];
		//buffer A password, 16 buffer, the passowrd of every buffer is 6 byte
		uint8_t sectorX_KeyA[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		uint8_t status, i;
		uint8_t str[MAX_LEN];
		int8_t msg2[]={"\n TAG's data in ASCII format:"};
		setup();
		//Search card, return card types
		status = MFRC522_Request(PICC_REQIDL, str);
		if (status == MI_OK)
		{

		}

		status = MFRC522_Anticoll(str);
		for(i=0; i<5; i++)
			serNum[i]=str[i];
		if (status == MI_OK)
		{
			UART0_PushTx('\r');
			UART0_PushTx('\n');
			UART0_Send( (char*) msg2, 0);
			UART0_PushTx('\r');
			UART0_PushTx('\n');

		}
		UART0_PushTx('\r');
		UART0_PushTx('\n');
		UART0_Send( (char*)serNum, 0);
		UART0_PushTx('\r');
		UART0_PushTx('\n');
		status = MFRC522_SelectTag(serNum);
		status = MFRC522_Auth(0x60,sector,sectorX_KeyA,serNum);
		status = MFRC522_Read(line, str);
		sendToSerialASCII(sector/4,line,status, str);
		strcpy( (char*)codigo, (char*)str );


	if( status == MI_OK )
		return 0;
	else
		return (-1);
}


/* Description: Send data read to serial monitor ASCII format ******************
 * Input parameter: sector, block, status and pointer to string read
 * Return: null					 */
void sendToSerialASCII(int32_t sector, int32_t block, uint8_t status, uint8_t *str)
{
	int32_t i;
	int8_t string[22];
	sprintf( (char*)string,"Sector %2d, block %2d: ", sector, block);
	if(status == MI_OK)
	{
		UART0_Send( (char*)string, 0 );
		for(i=0;i<16;i++)
		{
			UART0_PushTx(str[i]);

		}
		UART0_PushTx('\r');
		UART0_PushTx('\n');
	}
}

/* Description: Send data read to serial monitor HEX format ********************
 * Input parameter: sector, block, status and pointer to string read
 * Return: null					 */
void sendToSerialHEX(int32_t block, uint8_t status, uint8_t *str)
{
	int8_t string0[5];
	int8_t string1[49];
	sprintf( (char*)string0,"%2d: ",block);
	sprintf( (char*)string1,"%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x",
		str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7], str[8], str[9], str[10], str[11], str[12], str[13], str[14], str[15]);
	if(status == MI_OK)
	{
		UART0_Send( string0, 0 );
		UART0_Send( string1, 0 );
		UART0_PushTx('\r');
		UART0_PushTx('\n');
	}
}

void InitRC522( void )
{

	GPIO_Set(nrst, OFF);


}

