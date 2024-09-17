#ifndef INC_FONT_LIB_H_
#define INC_FONT_LIB_H_

#include <stdint.h>


struct Font{
	char *Index;
	uint8_t *Bitmap;
};


struct FontLib{
	uint32_t FontNum;
	uint8_t Bpp;
	uint8_t Height;
	uint8_t *(*GetFont)(struct FontLib *FontLibPtr ,char* Index);
	struct Font Font[];
};

extern const struct FontLib SimSun16;
extern const struct FontLib SimHei32;
#endif
