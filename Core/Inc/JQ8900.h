#ifndef INC_AUDIO_H_
#define INC_AUDIO_H_

#include <stdint.h>
#include <stddef.h>
#include <stm32f4xx_ll_usart.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_gpio.h>



struct JQ8900{
	union{
		struct{
			GPIO_TypeDef *TX_Port;
			uint16_t TX_Pin;
			GPIO_TypeDef *RX_Port;
			uint16_t RX_Pin;
			USART_TypeDef *USART;
		};
		struct{
			GPIO_TypeDef *SW_Port;
			uint16_t SW_Pin;
		};
	};

	void (*WriteCmdData)(struct JQ8900* audioPtr, uint8_t cmd, uint8_t *data, uint8_t dataLen);
	void (*PlayStart)(struct JQ8900* audioPtr);
	void (*PlayPause)(struct JQ8900* audioPtr);
	void (*PlayStop)(struct JQ8900* audioPtr);
	void (*PlayPrevious)(struct JQ8900* audioPtr);
	void (*PlayNext)(struct JQ8900* audioPtr);
	void (*PlaySelect)(struct JQ8900* audioPtr, uint8_t *soundName);
	void (*PlayCombine)(struct JQ8900* audioPtr, uint8_t *soundNames);
	void (*SetVolume)(struct JQ8900* audioPtr,uint8_t volume);
	void (*IncreaseVolume)(struct JQ8900* audioPtr);
	void (*ReduceVolume)(struct JQ8900* audioPtr);
};

struct JQ8900 CreateJQ8900_UART(USART_TypeDef *USART, GPIO_TypeDef *TX_Port, uint16_t TX_Pin, GPIO_TypeDef *RX_Port, uint16_t RX_Pin);

#endif /* INC_AUDIO_H_ */
