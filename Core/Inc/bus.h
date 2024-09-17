#ifndef INC_BUS_H_
#define INC_BUS_H_

#include <stdint.h>
#include <ili9341.h>
#include <jq8900.h>

#define BUS_DIR_UP 0U
#define BUS_DIR_DOWN 1U

struct Route
{
	uint8_t StopNum;
	char *RouteName;
	char **StopNames;
	char *RouteAudioName;
	char **StopAudioNames;
};

struct Bus{
	void (*In)(struct Bus *busPtr);
	void (*Out)(struct Bus *busPtr);
	void (*ChangeRoute)(struct Bus *busPtr);
	void (*UpRst)(struct Bus *busPtr);
	void (*DownRst)(struct Bus *busPtr);
	void (*Tips)(struct Bus *busPtr);

	struct ILI9341 *DispPtr;
	struct JQ8900 *audioPtr;

	struct Color ForeColor;
	struct Color BackColor;

	uint8_t IsRunning;
	uint8_t CurrentDirection;
	uint8_t NextDirection;
	uint8_t RouteNum;
	uint8_t RouteIndex;
	uint8_t CurrentStopIndex;
	uint8_t NextStopIndex;
	char *TipsAudioName;
	char *InAudioName;
	char *OutAudioName;
	struct Route *Routes;
};

struct Bus CreateBus(struct JQ8900 *audioPtr, struct ILI9341 *dispPtr, struct Route *routes, char *tipsAudioName, char *inAudioName, char *outAudioName);
#endif /* INC_BUS_H_ */
