#ifndef INC_EXTI_H_
#define INC_EXTI_H_
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_system.h"
#include <main.h>

void EXTIConfig();
void EXTI0_IRQHandler();
void EXTI1_IRQHandler();
void EXTI2_IRQHandler();
void EXTI3_IRQHandler();


#endif /* INC_KEYBOARD_H_ */
