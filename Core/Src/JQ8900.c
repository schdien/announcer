#include <JQ8900.h>


static void UART_WriteCmdData(struct JQ8900* audioPtr, uint8_t cmd, uint8_t *data, uint8_t dataLen)
{
	uint8_t buffer[4 + dataLen];
	buffer[0] = 0xAA;
	buffer[1] = cmd;
	buffer[2] = dataLen;
	buffer[3 + dataLen] = buffer[0] + buffer[1] + buffer[2];
	uint8_t i;
	for (i = 0;i < dataLen;i++)
	{
		buffer[3 + i] = data[i];
		buffer[3 + dataLen] += data[i];
	}
	for(i = 0; i < 4+dataLen; i++)
	{
		LL_USART_TransmitData8(audioPtr->USART, *(buffer+i));
		while(!LL_USART_IsActiveFlag_TXE(audioPtr->USART)); //confirm if Tx buffer is active, if not, wait
	}
}

static void UART_PlayStart(struct JQ8900* audioPtr)
{
	audioPtr->WriteCmdData(audioPtr, 0x02, NULL, 0);
}
static void UART_PlayPause(struct JQ8900* audioPtr)
{
	audioPtr->WriteCmdData(audioPtr, 0x03, NULL, 0);
}
static void UART_PlayStop(struct JQ8900* audioPtr)
{
	audioPtr->WriteCmdData(audioPtr, 0x04, NULL, 0);
}
static void UART_PlayPrevious(struct JQ8900* audioPtr)
{
	audioPtr->WriteCmdData(audioPtr, 0x05, NULL, 0);
}
static void UART_PlayNext(struct JQ8900* audioPtr)
{
	audioPtr->WriteCmdData(audioPtr, 0x06, NULL, 0);
}
static void UART_PlaySelect(struct JQ8900 *audioPtr, uint8_t *soundName)
{
	audioPtr->WriteCmdData(audioPtr, 0x07, soundName, 2);
}
static void UART_PlayCombine(struct JQ8900 *audioPtr, uint8_t *soundNames)
{
	uint8_t dataLen=0;
	while(*(soundNames+dataLen)!= 0)
	{
		dataLen++;
	}
	audioPtr->WriteCmdData(audioPtr, 0x1b, soundNames, dataLen);
}
static void UART_IncreaseVolume(struct JQ8900 *audioPtr)
{
	//audioPtr->WriteCmdData(&audio, );
}
static void UART_ReduceVolume(struct JQ8900 *audioPtr)
{
	//audioPtr->WriteCmdData(&audio, );
}
static void UART_SetVolume(struct JQ8900 *audioPtr, uint8_t Volume)
{
	audioPtr->WriteCmdData(audioPtr,0x13,&Volume,1);
}


struct JQ8900 CreateJQ8900_UART(USART_TypeDef *USART, GPIO_TypeDef *TX_Port, uint16_t TX_Pin, GPIO_TypeDef *RX_Port, uint16_t RX_Pin)
{
	//function init:
	struct JQ8900 audio={
		.USART = USART,
		.TX_Port = TX_Port,
		.TX_Pin = TX_Pin,
		.RX_Port = RX_Port,
		.RX_Pin = RX_Pin,
		.WriteCmdData = UART_WriteCmdData,
		.PlayStart = UART_PlayStart,
		.PlayPause = UART_PlayPause,
		.PlayStop = UART_PlayStop,
		.PlayPrevious = UART_PlayPrevious,
		.PlayNext = UART_PlayNext,
		.PlaySelect = UART_PlaySelect,
		.PlayCombine = UART_PlayCombine,
		.SetVolume = UART_SetVolume,
		.IncreaseVolume = UART_IncreaseVolume,
		.ReduceVolume = UART_ReduceVolume,
	};

	//hardware init:
	//Clock enable
	if(audio.TX_Port == GPIOA || audio.RX_Port == GPIOA) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	if(audio.TX_Port == GPIOB || audio.RX_Port == GPIOB) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	if(audio.TX_Port == GPIOC || audio.RX_Port == GPIOC) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	if(audio.TX_Port == GPIOD || audio.RX_Port == GPIOD) LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
	//public initial configuration
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	//TX Initial
	LL_GPIO_ResetOutputPin(audio.TX_Port, audio.TX_Pin);
	GPIO_InitStruct.Pin = audio.TX_Pin;
	LL_GPIO_Init(audio.TX_Port, &GPIO_InitStruct);
	//RX Initial
	LL_GPIO_ResetOutputPin(audio.RX_Port, audio.RX_Pin);
	GPIO_InitStruct.Pin = audio.RX_Pin;
	LL_GPIO_Init(audio.RX_Port, &GPIO_InitStruct);

	/*UART initial*/
	//Clock enable
	if (audio.USART == USART1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	if (audio.USART == USART2) LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	if (audio.USART == USART6) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);
	//initial configuration
	LL_USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.BaudRate = 9600;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(audio.USART, &(USART_InitStruct));
	LL_USART_ConfigAsyncMode(audio.USART);//Asynchronous mode
	LL_USART_Enable(audio.USART);

	return audio;
}
