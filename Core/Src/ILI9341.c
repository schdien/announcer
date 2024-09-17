#include<ili9341.h>

static void ILI9341_Init(struct ILI9341 *dispPtr)
{
	LL_GPIO_SetOutputPin(dispPtr->RD_Port,dispPtr->RD_Pin);
	LL_GPIO_SetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
	LL_GPIO_SetOutputPin(dispPtr->RS_Port,dispPtr->RS_Pin);
	LL_GPIO_SetOutputPin(dispPtr->CS_Port,dispPtr->CS_Pin);
	LL_GPIO_SetOutputPin(dispPtr->RST_Port,dispPtr->RST_Pin);

	LL_GPIO_SetOutputPin(dispPtr->RST_Port,dispPtr->RST_Pin);
	LL_mDelay(5);
	LL_GPIO_ResetOutputPin(dispPtr->RST_Port,dispPtr->RST_Pin);
	LL_mDelay(15);
	LL_GPIO_SetOutputPin(dispPtr->RST_Port,dispPtr->RST_Pin);
	LL_mDelay(15);

	LL_GPIO_SetOutputPin(dispPtr->CS_Port,dispPtr->CS_Pin);
	LL_GPIO_SetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
	LL_GPIO_ResetOutputPin(dispPtr->CS_Port,dispPtr->CS_Pin);

	dispPtr->WriteCmd(dispPtr, 0xCB);
	dispPtr->WriteData(dispPtr, 0x39);
	dispPtr->WriteData(dispPtr, 0x2C);
	dispPtr->WriteData(dispPtr, 0x00);
	dispPtr->WriteData(dispPtr, 0x34);
	dispPtr->WriteData(dispPtr, 0x02);

	dispPtr->WriteCmd(dispPtr, 0xCF);
	dispPtr->WriteData(dispPtr, 0x00);
	dispPtr->WriteData(dispPtr, 0XC1);
	dispPtr->WriteData(dispPtr, 0X30);

	dispPtr->WriteCmd(dispPtr, 0xE8);
	dispPtr->WriteData(dispPtr, 0x85);
	dispPtr->WriteData(dispPtr, 0x00);
	dispPtr->WriteData(dispPtr, 0x78);

	dispPtr->WriteCmd(dispPtr, 0xEA);
	dispPtr->WriteData(dispPtr, 0x00);
	dispPtr->WriteData(dispPtr, 0x00);

	dispPtr->WriteCmd(dispPtr, 0xED);
	dispPtr->WriteData(dispPtr, 0x64);
	dispPtr->WriteData(dispPtr, 0x03);
	dispPtr->WriteData(dispPtr, 0X12);
	dispPtr->WriteData(dispPtr, 0X81);

	dispPtr->WriteCmd(dispPtr, 0xF7);
	dispPtr->WriteData(dispPtr, 0x20);

	dispPtr->WriteCmd(dispPtr, 0xC0);
	dispPtr->WriteData(dispPtr, 0x23);

	dispPtr->WriteCmd(dispPtr, 0xC1);
	dispPtr->WriteData(dispPtr, 0x10);

	dispPtr->WriteCmd(dispPtr, 0xC5);
	dispPtr->WriteData(dispPtr, 0x3e);
	dispPtr->WriteData(dispPtr, 0x28);

	dispPtr->WriteCmd(dispPtr, 0xC7);
	dispPtr->WriteData(dispPtr, 0x86);

	dispPtr->WriteCmd(dispPtr, 0x36);
	dispPtr->WriteData(dispPtr, 0xc8);

	dispPtr->WriteCmd(dispPtr ,0x3A);
	dispPtr->WriteData(dispPtr, 0x65);

	dispPtr->WriteCmd(dispPtr, 0xB1);
	dispPtr->WriteData(dispPtr, 0x00);
	dispPtr->WriteData(dispPtr, 0x18);

	dispPtr->WriteCmd(dispPtr, 0xB6);
	dispPtr->WriteData(dispPtr, 0x08);
	dispPtr->WriteData(dispPtr, 0x82);
	dispPtr->WriteData(dispPtr, 0x27);

	dispPtr->WriteCmd(dispPtr, 0x11);
	LL_mDelay(120);

	dispPtr->WriteCmd(dispPtr, 0x29);

}
static void inline WriteCmd_8080(struct ILI9341 *dispPtr, uint8_t cmd)
{
	LL_GPIO_ResetOutputPin(dispPtr->RS_Port,dispPtr->RS_Pin);
	dispPtr->DB_Port->BSRR = (uint32_t)(uint8_t)~cmd << 16 | (uint32_t)(uint8_t)cmd;
	LL_GPIO_ResetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
	LL_GPIO_SetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
}
static void inline WriteData_8080(struct ILI9341 *dispPtr, uint8_t data)
{
	LL_GPIO_SetOutputPin(dispPtr->RS_Port,dispPtr->RS_Pin);
	dispPtr->DB_Port->BSRR = (uint32_t)(uint8_t)~data << 16 | (uint32_t)data;
	LL_GPIO_ResetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
	LL_GPIO_SetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
}

static void FastFill_8080_8Bit_565(struct ILI9341 *dispPtr, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, struct Color *color)
{
	uint64_t Cnt = 0;
	uint64_t PxNum = (x2-x1+1)*(y2-y1+1);
	uint8_t MemData1 = (color->R & 0b11111000) | (color->G & 0b11100000)>>5;
	uint8_t MemData2 = (color->G & 0b00011100)<<3 | (color->B & 0b11111000)>>3;
	uint32_t DB_BSRR_Val1 = (uint32_t)(uint8_t)~MemData1 << 16 | (uint32_t)MemData1;
	uint32_t DB_BSRR_Val2 = (uint32_t)(uint8_t)~MemData2<< 16 | (uint32_t)MemData2;
	uint32_t WR_BSRR_Val1 = (uint32_t)dispPtr->WR_Pin << 16;
	uint32_t WR_BSRR_Val2 = dispPtr->WR_Pin;

	dispPtr->SetWindow(dispPtr,x1, y1, x2, y2);
	dispPtr->RS_Port->BSRR = (uint32_t)dispPtr->RS_Pin;
	for(; Cnt < PxNum; Cnt++)
	{
		dispPtr->DB_Port->BSRR = DB_BSRR_Val1;
		dispPtr->WR_Port->BSRR = WR_BSRR_Val1;
		dispPtr->WR_Port->BSRR = WR_BSRR_Val2;

		dispPtr->DB_Port->BSRR = DB_BSRR_Val2;
		dispPtr->WR_Port->BSRR = WR_BSRR_Val1;
		dispPtr->WR_Port->BSRR = WR_BSRR_Val2;
	}
}

static void inline WriteMem_8080_8Bit_565(struct ILI9341 *dispPtr, struct Color *color)
{	
	uint8_t MemData1 = (color->R & 0b11111000) | (color->G & 0b11100000)>>5;
	uint8_t MemData2 = (color->G & 0b00011100)<<3 | (color->B & 0b11111000)>>3;
	LL_GPIO_SetOutputPin(dispPtr->RS_Port,dispPtr->RS_Pin);

	//dispPtr->DB_Port->BSRR = (0xFFFFFF)&(~MemData1 << 16) | (uint32_t)MemData1; //uint8_t variable will be converted to int automatically when ~ calculate, equivalent to the statement below
	dispPtr->DB_Port->BSRR = (uint32_t)(uint8_t)~MemData1 << 16 | (uint32_t)MemData1; //the reason of (uint8_t) is uint8_t variable will be convert to int when ~ calculate
	LL_GPIO_ResetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
	LL_GPIO_SetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);

	dispPtr->DB_Port->BSRR = (uint32_t)(uint8_t)~MemData2 << 16 | (uint32_t)MemData2;
	LL_GPIO_ResetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
	LL_GPIO_SetOutputPin(dispPtr->WR_Port,dispPtr->WR_Pin);
}

static void DispOn(struct ILI9341 *dispPtr)
{
	dispPtr->WriteCmd(dispPtr, 0x29);
}
static void DispOff(struct ILI9341 *dispPtr)
{
	dispPtr->WriteCmd(dispPtr, 0x28);
}

static void SetWindow(struct ILI9341 *dispPtr, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2)
{
	dispPtr->WriteCmd(dispPtr, 0x2a);
	dispPtr->WriteData(dispPtr, Y1 >> 8);
	dispPtr->WriteData(dispPtr, Y1);
	dispPtr->WriteData(dispPtr, Y2 >> 8);
	dispPtr->WriteData(dispPtr, Y2);
	dispPtr->WriteCmd(dispPtr, 0x2b);
	dispPtr->WriteData(dispPtr, X1 >> 8);
	dispPtr->WriteData(dispPtr, X1);
	dispPtr->WriteData(dispPtr, X2 >> 8);
	dispPtr->WriteData(dispPtr, X2);

	dispPtr->WriteCmd(dispPtr, 0x2c);
}

static void SetCursor(struct ILI9341 *dispPtr, uint16_t x, uint16_t y)
{
	dispPtr->WriteCmd(dispPtr, 0x2A);
	dispPtr->WriteData(dispPtr, y >> 8);
	dispPtr->WriteData(dispPtr, y);
	dispPtr->WriteCmd(dispPtr, 0x2B);
	dispPtr->WriteData(dispPtr, x >> 8);
	dispPtr->WriteData(dispPtr, x);

	dispPtr->WriteCmd(dispPtr, 0x2c);
}


void SetScrollArea(struct ILI9341 *dispPtr, uint16_t TopFixedAreaLen, uint16_t ScrollAreaLen, uint16_t BottomFixedAreaLen)
{
	dispPtr->WriteCmd(dispPtr, 0x33);
	dispPtr->WriteData(dispPtr, TopFixedAreaLen>>8);
	dispPtr->WriteData(dispPtr, TopFixedAreaLen);
	dispPtr->WriteData(dispPtr, ScrollAreaLen>>8);
	dispPtr->WriteData(dispPtr, ScrollAreaLen);
	dispPtr->WriteData(dispPtr, BottomFixedAreaLen>>8);
	dispPtr->WriteData(dispPtr, BottomFixedAreaLen);

	dispPtr->ScrollAreaLen = ScrollAreaLen;
}

void ScrollOnce(struct ILI9341 *dispPtr, uint8_t ScrollDir,uint8_t Period, uint8_t Step)
{
	dispPtr->WriteCmd(dispPtr, 0x37);
	dispPtr->WriteData(dispPtr, dispPtr->ScrollCnt>>8);
	dispPtr->WriteData(dispPtr, dispPtr->ScrollCnt);
	LL_mDelay(Period);
	if(ScrollDir)
	{
		if(dispPtr->ScrollCnt == dispPtr->ScrollAreaLen) dispPtr->ScrollCnt = 0;
		else dispPtr->ScrollCnt+=Step;
	}
	else
	{
		if(dispPtr->ScrollCnt == 0) dispPtr->ScrollCnt = dispPtr->ScrollAreaLen;
		else dispPtr->ScrollCnt-=Step;
	}

}

static void DispRectangle(struct ILI9341 *dispPtr, uint16_t x, uint16_t y, uint16_t width, uint16_t Height, uint16_t Edge, struct Color *color)
{
	uint16_t X1 = x;
	uint16_t X2 = x + width;
	uint16_t Y1 = y;
	uint16_t Y2 = y + Height;
	if(x == DISP_CENTER)
	{
		X1 = (dispPtr->Width-width)/2;
		X2 = X1 + width;
	}
	if(y == DISP_CENTER)
	{
		Y1 = (dispPtr->Height-Height)/2;
		Y2 = Y1 + Height;
	}

	if(Edge >= width || Edge >= Height )
	{
		dispPtr->FastFill(dispPtr, X1, Y1, X2, Y2, color);
	}
	else
	{
		dispPtr->FastFill(dispPtr, X1, Y1, X2, Y1+Edge, color);
		dispPtr->FastFill(dispPtr, X1, Y2, X2, Y2+Edge, color);
		dispPtr->FastFill(dispPtr, X1, Y1, X1+Edge, Y2, color);
		dispPtr->FastFill(dispPtr, X2, Y1, X2+Edge, Y2, color);

	}
}
static uint8_t DispChar(struct ILI9341* dispPtr, struct FontLib *fontLibPtr, char *index, uint16_t x, uint16_t y, struct Color *color)
{
	uint16_t i;
	uint8_t j;
	uint16_t tempX = x;

	uint8_t height = fontLibPtr->Height;
	uint8_t width;
	uint8_t bpp = fontLibPtr->Bpp;
	uint8_t* bitmap = fontLibPtr->GetFont(fontLibPtr, index);
	if (index[0] < 0x80) width = height / 2;
	else width = height;
	uint16_t bitmapNum = width*height/8*bpp;
	uint8_t Msk = (int8_t)0b10000000>>(bpp-1);  //use signed int

	for(i=0; i<bitmapNum; i++)
	{
		for(j=0; j<8; j+=bpp)
		{
			uint8_t maxGray = Msk>>j;
			uint8_t gray = bitmap[i] & maxGray;
			if(gray == maxGray)
			{
				dispPtr->SetCursor(dispPtr, tempX, y);
				dispPtr->WriteMem(dispPtr, color);
			}
			else if(gray == 0);
			else
			{
				struct Color tempColor = {
					.R = (uint32_t)(color->R * gray / maxGray),
					.G = (uint32_t)(color->G * gray / maxGray),
					.B = (uint32_t)(color->B * gray / maxGray),
				};//uint32 prevent from overflow
				dispPtr->SetCursor(dispPtr, tempX, y);
				dispPtr->WriteMem(dispPtr, &tempColor);
			}

			if(tempX == x + width - 1)
			{
				tempX=x;
				y++;
			}
			else tempX++;
		}
	}
	return width;
}
static void DispStr(struct ILI9341* dispPtr, struct FontLib *fontLibPtr, char *indices, uint16_t x, uint16_t y, struct Color *color)
{
	uint16_t tempX=x;
	uint16_t tempY=y;
	uint8_t width;
	volatile char index[2];    //error in optimization level 3 without volatile
	if(x == DISP_CENTER)
	{
		uint16_t widthSum=0;
		for(uint8_t Cnt = 0;indices[Cnt] != '\0';)
		{
			if(indices[Cnt]<=0x80)
			{
				index[0] = indices[Cnt];
				index[1] = '\0';
				Cnt++;
				widthSum += fontLibPtr->Height / 2;
			}
			else
			{
				index[0] = indices[Cnt];
				index[1] = indices[Cnt+1];
				Cnt+=2;
				widthSum += fontLibPtr->Height;
			}
		}
		tempX=(dispPtr->Width-widthSum)/2;
	}
	if(y == DISP_CENTER)
	{
		uint16_t height = fontLibPtr->Height;
		tempY = (dispPtr->Height-height)/2;
	}


	for(uint8_t Cnt = 0;indices[Cnt] != '\0';)
	{
		if(indices[Cnt]<=0x80)
		{
			//index=&(char[]){indices[Cnt],'\0'};
			index[0] = indices[Cnt];
			index[1] = '\0';
			Cnt++;
		}
		else
		{
			//index=&(char[]){indices[Cnt],indices[Cnt+1],'\0'};
			index[0] = indices[Cnt];
			index[1] = indices[Cnt+1];
			Cnt+=2;
		}
		width = DispChar(dispPtr, fontLibPtr, index, tempX, tempY, color);
		tempX += width;
	}
}
struct ILI9341 CreateILI9341_8080_8Bit(
		GPIO_TypeDef* CS_Port, uint16_t CS_Pin,
		GPIO_TypeDef* RST_Port, uint16_t RST_Pin,
		GPIO_TypeDef* RS_Port, uint16_t RS_Pin,
		GPIO_TypeDef* WR_Port, uint16_t WR_Pin,
		GPIO_TypeDef* RD_Port, uint16_t RD_Pin,
		GPIO_TypeDef* DB_Port)
{

	struct ILI9341 Disp = {
		.CS_Port = CS_Port,
		.CS_Pin = CS_Pin,
		.RST_Port = RST_Port,
		.RST_Pin = RST_Pin,
		.RS_Port = RS_Port,
		.RS_Pin = RS_Pin,
		.WR_Port = WR_Port,
		.WR_Pin = WR_Pin,
		.RD_Port = RD_Port,
		.RD_Pin = RD_Pin,
		.DB_Port = DB_Port,
		.DB_Pin = 0xFF,

		.Width = 320,
		.Height = 240,
		.ScrollCnt = 0,
		.ScrollAreaLen = 0,

		.WriteCmd = WriteCmd_8080,
		.WriteData = WriteData_8080,
		.WriteMem = WriteMem_8080_8Bit_565,
		.FastFill = FastFill_8080_8Bit_565,

		.SetWindow = SetWindow,
		.SetCursor = SetCursor,
		.DispOn = DispOn,
		.DispOff = DispOff,
		.DispRectangle = DispRectangle,
		.DispChar = DispChar,
		.DispStr = DispStr,
	};


	//initial peripheral in stm32
	if(Disp.CS_Port == GPIOA || Disp.RST_Port == GPIOA || Disp.RD_Port == GPIOA ||
	   Disp.WR_Port == GPIOA || Disp.RS_Port == GPIOA || Disp.DB_Port == GPIOA)
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	if(Disp.CS_Port == GPIOB || Disp.RST_Port == GPIOB || Disp.RD_Port == GPIOB ||
	   Disp.WR_Port == GPIOB || Disp.RS_Port == GPIOB || Disp.DB_Port == GPIOB)
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	if(Disp.CS_Port == GPIOC || Disp.RST_Port == GPIOC || Disp.RD_Port == GPIOC ||
	   Disp.WR_Port == GPIOC || Disp.RS_Port == GPIOC || Disp.DB_Port == GPIOC)
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	if(Disp.CS_Port == GPIOD || Disp.RST_Port == GPIOD || Disp.RD_Port == GPIOD ||
	   Disp.WR_Port == GPIOD || Disp.RS_Port == GPIOD || Disp.DB_Port == GPIOD)
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

	LL_GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	LL_GPIO_ResetOutputPin(Disp.CS_Port, Disp.CS_Pin);
	GPIO_InitStruct.Pin = Disp.CS_Pin;
	LL_GPIO_Init(Disp.CS_Port, &GPIO_InitStruct);

	LL_GPIO_ResetOutputPin(Disp.RST_Port, Disp.RST_Pin);
	GPIO_InitStruct.Pin = Disp.RST_Pin;
	LL_GPIO_Init(Disp.RST_Port, &GPIO_InitStruct);

	LL_GPIO_ResetOutputPin(Disp.WR_Port, Disp.WR_Pin);
	GPIO_InitStruct.Pin = Disp.WR_Pin;
	LL_GPIO_Init(Disp.WR_Port, &GPIO_InitStruct);

	LL_GPIO_ResetOutputPin(Disp.RS_Port, Disp.RS_Pin);
	GPIO_InitStruct.Pin = Disp.RS_Pin;
	LL_GPIO_Init(Disp.RS_Port, &GPIO_InitStruct);

	LL_GPIO_ResetOutputPin(Disp.RD_Port, Disp.RD_Pin);
	GPIO_InitStruct.Pin = Disp.RD_Pin;
	LL_GPIO_Init(Disp.RD_Port, &GPIO_InitStruct);

	LL_GPIO_ResetOutputPin(Disp.DB_Port, Disp.DB_Pin);
	GPIO_InitStruct.Pin = Disp.DB_Pin;
	LL_GPIO_Init(Disp.DB_Port, &GPIO_InitStruct);

	ILI9341_Init(&Disp);

	return Disp;
}
