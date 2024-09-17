#include <Bus.h>

static void UpRstStopIndex(struct Bus* busPtr)
{
	busPtr->IsRunning = 0;
	busPtr->CurrentDirection = BUS_DIR_UP;
	busPtr->NextDirection = BUS_DIR_UP;
	busPtr->CurrentStopIndex = 0;
	busPtr->NextStopIndex = 1;
}
static void DownRstStopIndex(struct Bus* busPtr)
{
	busPtr->IsRunning = 0;
	uint8_t routeIndex = busPtr->RouteIndex;
	busPtr->CurrentDirection = BUS_DIR_DOWN;
	busPtr->NextDirection = BUS_DIR_DOWN;
	busPtr->CurrentStopIndex = busPtr->Routes[routeIndex].StopNum - 1;
	busPtr->NextStopIndex = busPtr->Routes[routeIndex].StopNum - 2;
}
static void UpdateRouteIndex(struct Bus* busPtr)
{
	if (busPtr->RouteIndex == busPtr->RouteNum - 1)
		busPtr->RouteIndex = 0;
	else
		busPtr->RouteIndex = busPtr->RouteIndex + 1;
}
static void UpdateStopIndex(struct Bus* busPtr)
{
	uint8_t routeIndex = busPtr->RouteIndex;
	//update current stop count
	if (busPtr->CurrentDirection == BUS_DIR_DOWN)
	{
		if (busPtr->CurrentStopIndex == 0)
		{
			busPtr->CurrentDirection = BUS_DIR_UP;
			busPtr->CurrentStopIndex++;
		}
		else
		{
			busPtr->CurrentStopIndex--;
		}
	}
	else if (busPtr->CurrentDirection == BUS_DIR_UP)
	{
		if (busPtr->CurrentStopIndex == busPtr->Routes[routeIndex].StopNum - 1)
		{
			busPtr->CurrentDirection = BUS_DIR_DOWN;
			busPtr->CurrentStopIndex--;
		}
		else
		{
			busPtr->CurrentStopIndex++;
		}
	}
	//update next stop count
	if (busPtr->NextDirection == BUS_DIR_DOWN)
	{
		if (busPtr->NextStopIndex == 0)
		{
			busPtr->NextDirection = BUS_DIR_UP;
			busPtr->NextStopIndex++;
		}
		else
		{
			busPtr->NextStopIndex--;
		}
	}
	else if (busPtr->NextDirection == BUS_DIR_UP)
	{
		if (busPtr->NextStopIndex == busPtr->Routes[routeIndex].StopNum - 1)
		{
			busPtr->NextDirection = BUS_DIR_DOWN;
			busPtr->NextStopIndex--;
		}
		else
		{
			busPtr->NextStopIndex++;
		}
	}
}

static void ReportCurrentStop(struct Bus* busPtr)
{
	uint8_t routeIndex = busPtr->RouteIndex;
	uint8_t currentStopCnt = busPtr->CurrentStopIndex;
	char* stopAudioName = busPtr->Routes[routeIndex].StopAudioNames[currentStopCnt];
	char* inAudioName = busPtr->InAudioName;
	char* audioNames = (char[]){ stopAudioName[0],stopAudioName[1],inAudioName[0],inAudioName[1] };
	busPtr->audioPtr->PlayCombine(busPtr->audioPtr, audioNames);
}
static void ReportNextStop(struct Bus* busPtr)
{
	uint8_t routeIndex = busPtr->RouteIndex;
	uint8_t nextStopIndex = busPtr->NextStopIndex;
	char* stopAudioIndex = busPtr->Routes[routeIndex].StopAudioNames[nextStopIndex];
	char* outAudioName = busPtr->OutAudioName;
	char* audioNames = (char[]){ outAudioName[0],outAudioName[1],stopAudioIndex[0],stopAudioIndex[1] };
	busPtr->audioPtr->PlayCombine(busPtr->audioPtr, audioNames);
}
static void ReportTips(struct Bus* busPtr)
{
	busPtr->audioPtr->PlayCombine(busPtr->audioPtr, busPtr->TipsAudioName);
}

static void DispCurrentStop(struct Bus* busPtr)
{
	struct ILI9341* dispPtr = busPtr->DispPtr;
	uint8_t routeIndex = busPtr->RouteIndex;
	uint8_t currentStopCnt = busPtr->CurrentStopIndex;
	char* stopName = busPtr->Routes[routeIndex].StopNames[currentStopCnt];
	dispPtr->FastFill(dispPtr, 16, 100, 304, 132, &(busPtr->BackColor));
	dispPtr->DispStr(dispPtr, &SimHei32, stopName, DISP_CENTER, 100, &(busPtr->ForeColor));//display bus stop now
}
static void DispNextStop(struct Bus* busPtr)
{
	struct ILI9341* dispPtr = busPtr->DispPtr;
	uint8_t routeIndex = busPtr->RouteIndex;
	uint8_t nextStopIndex = busPtr->NextStopIndex;
	char* stopName = busPtr->Routes[routeIndex].StopNames[nextStopIndex];
	dispPtr->FastFill(dispPtr, 88, 147, 232, 163, &(busPtr->BackColor));
	dispPtr->DispStr(dispPtr, &SimSun16, stopName, DISP_CENTER, 147, &(busPtr->ForeColor));//display next bus stop
}
static void DispRoute(struct Bus* busPtr)
{
	struct ILI9341* dispPtr = busPtr->DispPtr;
	uint8_t routeIndex = busPtr->RouteIndex;
	char* routeName = busPtr->Routes[routeIndex].RouteName;
	dispPtr->FastFill(dispPtr, 140, 68, 180, 84, &(busPtr->BackColor));
	dispPtr->DispStr(dispPtr, &SimSun16, routeName, DISP_CENTER, 68, &(busPtr->ForeColor)); //display bus route now
}
static void DispRunStatus(struct Bus* busPtr)
{
	if (busPtr->IsRunning == 1)
	{
		struct Color color = {130, 0, 0};
		struct ILI9341 *dispPtr = busPtr->DispPtr;
		for (uint8_t i = 0; i < 13; i++)
		{
			color.R -= 10;
			dispPtr->DispRectangle(dispPtr, DISP_CENTER, DISP_CENTER, dispPtr->Width - 1 - i, dispPtr->Height - 1 - i, 0, &color);
		}
	}
	else
	{
		struct Color color = {0, 78, 0};
		struct ILI9341 *dispPtr = busPtr->DispPtr;
		for (uint8_t i = 0; i < 10; i++)
		{
			color.G -= 6;
			dispPtr->DispRectangle(dispPtr, DISP_CENTER, DISP_CENTER, dispPtr->Width - 1 - i, dispPtr->Height - 1 - i, 0, &color);
		}
	}
}

static void InitGUI(struct Bus* busPtr)
{
	struct ILI9341* dispPtr = busPtr->DispPtr;
	dispPtr->FastFill(dispPtr, 0, 0, 319, 239, &(busPtr->BackColor));
	dispPtr->DispRectangle(dispPtr, DISP_CENTER, 65, 43, 22, 0, &(busPtr->ForeColor));
	DispRoute(busPtr);
	DispCurrentStop(busPtr);
	DispNextStop(busPtr);
	DispRunStatus(busPtr);
}

static void In(struct Bus* busPtr)
{
	if (busPtr->IsRunning == 1)
	{
		busPtr->IsRunning = 0;
		ReportCurrentStop(busPtr);

		DispRunStatus(busPtr);
	}
}

static void Out(struct Bus* busPtr)
{
	if (busPtr->IsRunning == 0)
	{
		busPtr->IsRunning = 1;
		ReportNextStop(busPtr);

		UpdateStopIndex(busPtr);
		DispCurrentStop(busPtr);
		DispNextStop(busPtr);

		DispRunStatus(busPtr);

	}
}
static void ChangeRoute(struct Bus* busPtr)
{
	UpdateRouteIndex(busPtr);
	UpRstStopIndex(busPtr);
	DispRoute(busPtr);
	DispCurrentStop(busPtr);
	DispNextStop(busPtr);
}
static void UpRst(struct Bus* busPtr)
{
	UpRstStopIndex(busPtr);
	DispCurrentStop(busPtr);
	DispNextStop(busPtr);
}
static void DownRst(struct Bus* busPtr)
{
	DownRstStopIndex(busPtr);
	DispCurrentStop(busPtr);
	DispNextStop(busPtr);
}

struct Bus CreateBus(struct JQ8900* audioPtr, struct ILI9341* dispPtr, struct Route* routes, char* tipsAudioName, char* inAudioName, char* outAudioName)
{
	struct Bus bus = {
		//functions
		.In = In,
		.Out = Out,
		.ChangeRoute = ChangeRoute,
		.UpRst = UpRst,
		.DownRst = DownRst,
		.Tips = ReportTips,

		//Peripheral
		.audioPtr = audioPtr,
		.DispPtr = dispPtr,

		//Color
		.ForeColor = {255,255,255},
		.BackColor = {0,0,0},

		//bus data
		.IsRunning = 0,
		.CurrentDirection = BUS_DIR_UP,
		.NextDirection = BUS_DIR_UP,
		.RouteNum = 2,
		.RouteIndex = 0,
		.CurrentStopIndex = 0,
		.NextStopIndex = 1,
		.TipsAudioName = tipsAudioName,
		.InAudioName = inAudioName,
		.OutAudioName = outAudioName,
		.Routes = routes,
	};
	InitGUI(&bus);
	return bus;
}


