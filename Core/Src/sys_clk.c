#include <sys_clk.h>
void SystemClockConfig(void)
{
	  LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
	  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
	  {
	  }
	  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	  LL_RCC_HSI_SetCalibTrimming(16);
	  LL_RCC_HSI_Enable();

	   /* Wait till HSI is ready */
	  while(LL_RCC_HSI_IsReady() != 1)
	  {

	  }
	  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_8, 100, LL_RCC_PLLP_DIV_2);
	  LL_RCC_PLL_Enable();

	   /* Wait till PLL is ready */
	  while(LL_RCC_PLL_IsReady() != 1)
	  {

	  }
	  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	   /* Wait till System clock is ready */
	  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	  {

	  }
	  LL_Init1msTick(100000000);
	  LL_SetSystemCoreClock(100000000);
	  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}
