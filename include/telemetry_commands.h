#ifndef __TELEMETRYPARSERS_H__
#define __TELEMETRYPARSERS_H__

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* KFly includes */
#include "configKFly.h"
#include "fmath.h"
#include "eemul.h"

/* Includes */
#include "iap.h"
#include "lpc_types.h"
#include "uart.h"
#include "input.h"
#include "pid.h"
#include "fmath.h"
#include "telemetry.h"

void rxNothing(void);
void rxPing(void);
void rxSaveToFlash(void);
void rxGetRegulatorData(void);
void rxSetRegulatorData(void);
void rxGetChannelMix(void);
void rxSetChannelMix(void);
void rxGetRCCalibration(void);
void rxSetRCCalibration(void);
void rxGetRCValues(void);

#endif
