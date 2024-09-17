#include "main.h"

#define MODE_NORMAL 	0
#define MODE_SETTING 	1


uint8_t B1Flag = 0;
uint8_t B2Flag = 0;
uint8_t B3Flag = 0;
uint8_t B4Flag = 0;

int main(void)
{
	/* MCU Configuration--------------------------------------------------------*/
	// Reset of all peripherals, Initializes the Flash interface and the Systick.
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
	// Configure the system clock
	SystemClockConfig();
	// System interrupt init
	EXTIConfig();


	//init user peripherals
	struct ILI9341 disp1 = CreateILI9341_8080_8Bit(
		GPIOB,LL_GPIO_PIN_8, 	//CS
		GPIOB,LL_GPIO_PIN_9, 	//RST
		GPIOB,LL_GPIO_PIN_10,	//RS
		GPIOB,LL_GPIO_PIN_12,	//WR
		GPIOB,LL_GPIO_PIN_13,	//RD
		GPIOB);			   		//DB
	struct JQ8900 audio1 = CreateJQ8900_UART(
		USART6, 				//USART
		GPIOC, LL_GPIO_PIN_6,  	//TX
		GPIOC, LL_GPIO_PIN_7);	//RX
	audio1.SetVolume(&audio1,15);


	//init route
	struct Route routes[2] = {
		{
			.StopNum = 7,
			.RouteName = "302·",
			.RouteAudioName = "00",
			.StopNames = (char*[]){"�������·վ","���ӿƴ�ҽԺվ","���ӿƴ���У��վ","����·ˮɼ·��վ",
						"��Դ�������·��վ","���ӿƴ���У��վ","���ӿƴ��϶���վ"},
			.StopAudioNames = (char*[]){"01","02","03","04","05","06","07"},//ָ��ָ�������ָ��
		},
		{
			.StopNum = 16,
			.RouteName = "205·",
			.RouteAudioName = "10",
			.StopNames = (char*[]){"�з�·վ","�����н�·վ","��������л�·��վ","�����������·��վ",
						"ģ�߹�ҵ԰վ","�������·վ","����·վ","���ӿƴ�ҽԺվ","���ӿƴ���У��վ",
						"����·����·��վ","�콾·��վ","�콾·��վ","��Դ�������·��վ",
						"���ӿƴ���У��վ","���ӿƴ��϶���վ","˳���վ"},
			.StopAudioNames = (char*[]){"11","12","13","14","15","16","17","18","19","1A","1B","1C","1D","1F","1G","1H"},
		}
	};


	// create busSystem announcer system
	struct Bus busSystem = CreateBus(&audio1, &disp1, routes, "TI", "IN", "OU");

	// run
	uint8_t Mode = MODE_NORMAL;
	while (1)
	{
		switch(Mode)
		{
			case MODE_NORMAL:
				if(B1Flag)
				{
					B1Flag = 0;
					busSystem.Out(&busSystem);
				}
				if(B2Flag)
				{
					B2Flag = 0;
					busSystem.In(&busSystem);
				}
				if(B3Flag)
				{
					B3Flag = 0;
					busSystem.Tips(&busSystem);
				}
				if(B4Flag)
				{
					B4Flag = 0;
					Mode = MODE_SETTING;
				}
				break;
			case MODE_SETTING:
				if(B1Flag)
				{
					B1Flag = 0;
					busSystem.ChangeRoute(&busSystem);
				}
				if(B2Flag)
				{
					B2Flag = 0;
					busSystem.UpRst(&busSystem);
				}
				if(B3Flag)
				{
					B3Flag = 0;
					busSystem.DownRst(&busSystem);
				}
				if(B4Flag)
				{
					B4Flag = 0;
					Mode = MODE_NORMAL;
				}
				break;
		}

	}
}


void EXTI_Callback(uint32_t EXTI_Line)
{
	switch(EXTI_Line)
	{
		case LL_EXTI_LINE_0:
			LL_mDelay(10);
			if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_0))
				B1Flag = 1;
			break;
		case LL_EXTI_LINE_1:
			LL_mDelay(10);
			if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_1))
				B2Flag = 1;
			break;
		case LL_EXTI_LINE_2:
			LL_mDelay(10);
			if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_2))
				B3Flag = 1;
			break;
		case LL_EXTI_LINE_3:
			LL_mDelay(10);
			if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_3))
				B4Flag = 1;
			break;
	}
}

void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
