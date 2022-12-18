#include "stm8s.h"
#include "ds18b20.h"

bool isWaterRunning = FALSE;

void GPIO_setup(void);
void EXTI_setup(void);
void clock_setup(void);
void TIM4_setup(void);

void delayTenMicro (void) {
	char a;
	for (a = 0; a < 50; ++a)
		_asm("nop");
}

void delay_ms(unsigned int i)
{
	while(i--)
	{
		_delay_us(1000);
	}
}

int main () {
	float objTemp;
	int currSensorVal;
	int prevSensorVal;

	GPIO_setup();
	EXTI_setup();
	clock_setup();
	TIM4_setup();
	GPIO_WriteLow(GPIOA, GPIO_PIN_3);//Turn LED OFF
	while (1) {
		objTemp = DS18B20_ReadTemperature();
		if (isWaterRunning && objTemp < 34.5)
		{
			GPIO_WriteHigh(GPIOA, GPIO_PIN_3);//Turn LED ON		
		} else {
			GPIO_WriteLow(GPIOA, GPIO_PIN_3);//Turn LED OFF
		}
		delayTenMicro();
	}
}

void GPIO_setup(void)
{
	GPIO_DeInit(GPIOA);    
	GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_IN_PU_IT);
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
}


void EXTI_setup(void)
{
	ITC_DeInit();
	ITC_SetSoftwarePriority(ITC_IRQ_PORTA, ITC_PRIORITYLEVEL_0);
	
	EXTI_DeInit();
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);
	EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
	
//	enableInterrupts();
}


void clock_setup(void)
{
	CLK_DeInit();
	
	CLK_HSECmd(DISABLE);
	CLK_LSICmd(DISABLE);
	CLK_HSICmd(ENABLE);
	while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
	
	CLK_ClockSwitchCmd(ENABLE);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, 
	DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
	
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
}

void TIM4_setup(void)
{	
	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 128);	
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM4_Cmd(ENABLE);
	enableInterrupts();
}