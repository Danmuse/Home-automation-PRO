/*
===============================================================================
 Name        : ServoSG90.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <misc.h>


int main(void)
{
	uint8_t estado = 0, i, data;
	uint32_t add = 0;
	Inicializar();

    while(1)
    {
    	TimerEvent( );
    	if( GetFlagEeprom() )
    	{
    		SetFlagEeprom(0);

    		switch( estado )
    		{
    		case 0:
        		IIC_Start();
    			IIC_Write(EEPROM_24LC256_address);
    			IIC_Write((add >> 8)&0x7F);
    			IIC_Write(add &0xFF);
    			IIC_Write(i);
    			IIC_Stop();

    			estado = 1;
    			break;

    		case 1:
        		IIC_Start();
    			IIC_Write(EEPROM_24LC256_address);
    			IIC_Write((add >> 8)&0x7F);
    			IIC_Write(add &0xFF);
    			IIC_Start();
    			IIC_Write(EEPROM_24LC256_address | 1);
    			data = IIC_Read(0);
    			IIC_Stop();

    			if( data == i )
    			{
    				i++;
    				add ++;
    			}
    			estado = 0;

    			break;

    		default:
    			estado = 0;
    			break;
    		}

    	}

    }
    return 0 ;
}
