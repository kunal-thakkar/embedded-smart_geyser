/*
 *
 * DS18B20 one wire temp --- PA3 stm8
 *
 */

#include "ds18b20.h"

void _delay_us(unsigned int i)
{
	i *= 3; 
	while(--i);
}

void DS18B20_Init(void)
{
	DS18B20_DQ_OUT;   
	DS18B20_DQ_PUSH_PULL;    
	DS18B20_DQ_HIGH;   
	_delay_us(10);
	DS18B20_DQ_LOW;   
	_delay_us(600);     //????

	DS18B20_DQ_IN;   
	DS18B20_DQ_PULL_UP;    
	_delay_us(100);     
	while(DS18B20_DQ_VALUE == 1);
	_delay_us(400);
}

void DS18B20_WriteByte(unsigned char _data)
{
	unsigned char i = 0;

	DS18B20_DQ_OUT;
	for (i = 0; i < 8; i++)
	{
		DS18B20_DQ_LOW;
		_delay_us(2);
		if (_data & 0x01)
		{
			DS18B20_DQ_HIGH;
		}
		_data >>= 1;
		_delay_us(60);
		DS18B20_DQ_HIGH;
	}
}

unsigned char DS18B20_ReadByte(void)
{
	unsigned char i = 0, _data = 0;

	for (i = 0; i < 8; i++)
	{
		DS18B20_DQ_OUT;
		DS18B20_DQ_LOW;
		_delay_us(5);
		_data >>= 1;
		DS18B20_DQ_HIGH;
		DS18B20_DQ_IN;
		if (DS18B20_DQ_VALUE)
		{
			_data |= 0x80;
		}
		DS18B20_DQ_OUT; 
		DS18B20_DQ_HIGH;
		_delay_us(60);
	}

	return _data;
}

float DS18B20_ReadTemperature(void)
{
	unsigned char temp = 0;
	float t = 0;
/*
 *
 *  
 *  0x44 is the command to calculate the temperature and save it in the scratchpad.  This takes up to 750 milliseconds.
 *
 *  0xBE is the command to send the contents of of the scratchpad.
 *
 *  What you are sending is "Everybody: Send your Scratchpad".  This only works if you only have one device.  
 *
 */
	DS18B20_Init();
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x44);

	DS18B20_Init();
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xbe);

	temp = DS18B20_ReadByte();
	t = (((temp & 0xf0) >> 4) + (temp & 0x07) * 0.125); 
	temp = DS18B20_ReadByte();
	t += ((temp & 0x0f) << 4);
	return t;
}
