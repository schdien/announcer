#include <exti.h>


void EXTIConfig(void)
{
	  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE0);
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE1);
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE2);
	  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE3);

	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	  LL_EXTI_Init(&EXTI_InitStruct);

	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_1;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	  LL_EXTI_Init(&EXTI_InitStruct);

	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	  LL_EXTI_Init(&EXTI_InitStruct);

	  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_3;
	  EXTI_InitStruct.LineCommand = ENABLE;
	  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	  LL_EXTI_Init(&EXTI_InitStruct);

	  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_0, LL_GPIO_PULL_UP);
	  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_1, LL_GPIO_PULL_UP);
	  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);
	  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
	  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
	  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
	  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_2, LL_GPIO_MODE_INPUT);
	  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);

	  NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(EXTI0_IRQn);
	  NVIC_SetPriority(EXTI1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(EXTI1_IRQn);
	  NVIC_SetPriority(EXTI2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(EXTI2_IRQn);
	  NVIC_SetPriority(EXTI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(EXTI3_IRQn);
}
void EXTI0_IRQHandler(void)
{

  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
    EXTI_Callback(LL_EXTI_LINE_0);
  }

}
void EXTI1_IRQHandler(void)
{

  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
    EXTI_Callback(LL_EXTI_LINE_1);
  }

}
void EXTI2_IRQHandler(void)
{

  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
    EXTI_Callback(LL_EXTI_LINE_2);
  }

}
void EXTI3_IRQHandler(void)
{

  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
    EXTI_Callback(LL_EXTI_LINE_3);
  }

}




