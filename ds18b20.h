/*
 *
 * DS18B20 one wire temp --- PA3 stm8
 *
 */

#include "stm8s.h"

#define PA_ODR			*(unsigned char*)0x005000 // Port A data output latch register 
#define PA_IDR			*(unsigned char*)0x005001 // Port A input pin value register 
#define PA_DDR			*(unsigned char*)0x005002 // Port A data direction register  
#define PA_CR1			*(unsigned char*)0x005003 // Port A control register 1  
#define PA_CR2			*(unsigned char*)0x005004 // Port A control register 2  

//http://www.cnblogs.com/chenlidong/articles/1823542.html
#define DS18B20_DQ_OUT          PA_DDR |= (1<<2)   //??
#define DS18B20_DQ_IN           PA_DDR &= ~(1<<2)   //??
#define DS18B20_DQ_HIGH         PA_ODR |= (1<<2)   //??
#define DS18B20_DQ_LOW          PA_ODR &= ~(1<<2)    //??
#define DS18B20_DQ_PULL_UP      PA_CR1 |= (1<<2)   //??
#define DS18B20_DQ_FLOATING     PA_CR1 &=  ~(1<<2)  //??
#define DS18B20_DQ_PUSH_PULL    PA_CR1 |= (1<<2)   //??
#define DS18B20_DQ_OPEN_DRAIN   PA_CR1 &= ~(1<<2)   //??
#define DS18B20_DQ_VALUE        PA_IDR & (1<<2)      //DQ?

void _delay_us(unsigned int i);
void DS18B20_Init(void);
void DS18B20_WriteByte(unsigned char _data);
unsigned char DS18B20_ReadByte(void);
float DS18B20_ReadTemperature(void);