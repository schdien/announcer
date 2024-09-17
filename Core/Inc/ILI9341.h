#ifndef INC_ILI9341_H_
#define INC_ILI9341_H_

#include <stdint.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_spi.h>
#include <stm32f4xx_ll_gpio.h>
#include <font_lib.h>

#define DISP_CENTER		0xFFFF
#define DISP_FILL		0xFFFF

struct Color{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};


struct ILI9341{
	/*************************
	* Interface Pins Select
	************************/
	/* shared member variable */
	GPIO_TypeDef* CS_Port;
	uint32_t CS_Pin;
	GPIO_TypeDef* RST_Port;
	uint32_t RST_Pin;
	GPIO_TypeDef* RS_Port;
	uint32_t RS_Pin;
	union{
		struct{
			/* Parallel member variable */
			GPIO_TypeDef* WR_Port;
			uint32_t WR_Pin;
			GPIO_TypeDef* RD_Port;
			uint32_t RD_Pin;
			GPIO_TypeDef* DB_Port;
			uint32_t DB_Pin;
		};
		struct{
			/* Serial member variable */
			union{
				GPIO_TypeDef* SDI_Port;
				uint32_t SDI_Pin;
				GPIO_TypeDef* SDA_Port;
				uint32_t SDA_Pin;
			};
			GPIO_TypeDef* SDO_Port;
			uint32_t SDO_Pin;
			GPIO_TypeDef* SCLK_Port;
			uint32_t SCLK_Pin;
			SPI_TypeDef SPIx;
		};
	};

	 /*************
	 * Property
	 * **********/
	 uint16_t Width;
	 uint16_t Height;
	 uint16_t ScrollCnt;
	 uint16_t ScrollAreaLen;

	/***************
	* Function Table
	****************/
	uint8_t (*ReadData)(struct ILI9341 *dispPtr);//Undone               				  <-------------------------------------
	void (*WriteCmd) (struct ILI9341 *dispPtr, uint8_t cmd);
	void (*WriteData) (struct ILI9341 *dispPtr, uint8_t data);
	void (*WriteMem) (struct ILI9341 *dispPtr, struct Color *color);

	//level 2: abstract layer function: settings and display
	void (*FastFill)(struct ILI9341 *dispPtr, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, struct Color *color);
	void (*SetWindow)(struct ILI9341 *dispPtr, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void (*SetCursor)(struct ILI9341 *dispPtr, uint16_t x, uint16_t y);
	void (*SetScrollArea)(struct ILI9341 *dispPtr, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void (*ScrollOnce)(struct ILI9341 *dispPtr);
	void (*DispOn)(struct ILI9341 *dispPtr);
	void (*DispOff)(struct ILI9341 *dispPtr);
	void (*DispRectangle)(struct ILI9341 *dispPtr, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t edge, struct Color *color);
	uint8_t (*DispChar)(struct ILI9341 *dispPtr, struct FontLib *fontLibPtr, char *index,uint16_t x, uint16_t y, struct Color *color);
	void (*DispStr)(struct ILI9341 *dispPtr, struct FontLib *fontLibPtr, char *indices, uint16_t x, uint16_t y, struct Color *color);
};

//Constructor:
struct ILI9341 CreateILI9341_8080_8Bit(GPIO_TypeDef* CS_Port, uint16_t CS_Pin,
									   GPIO_TypeDef* RST_Port, uint16_t RST_Pin,
									   GPIO_TypeDef* RS_Port, uint16_t RS_Pin,
									   GPIO_TypeDef* WR_Port, uint16_t WR_Pin,
									   GPIO_TypeDef* RD_Port, uint16_t RD_Pin,
									   GPIO_TypeDef* DB_Port);

#endif

