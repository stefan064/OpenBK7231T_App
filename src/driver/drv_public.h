#ifndef __DRV_PUBLIC_H__
#define __DRV_PUBLIC_H__

#include "../httpserver/new_http.h"

enum {
	OBK_VOLTAGE, // must match order in cmd_public.h
	OBK_CURRENT,
	OBK_POWER,
	OBK_POWER_APPARENT,
	OBK_POWER_REACTIVE,
	OBK_POWER_FACTOR,
	OBK_NUM_MEASUREMENTS,
};

enum {
	OBK_CONSUMPTION_TOTAL = OBK_NUM_MEASUREMENTS,
	OBK_CONSUMPTION_LAST_HOUR,
	OBK_CONSUMPTION_STATS,
	OBK_CONSUMPTION_YESTERDAY,
	OBK_CONSUMPTION_TODAY,
	OBK_CONSUMPTION_CLEAR_DATE,
	OBK_NUM_ENUMS_MAX
};

#define OBK_NUM_COUNTERS            (OBK_NUM_EMUNS_MAX-OBK_NUM_MEASUREMENTS)
#define OBK_NUM_SENSOR_COUNT         OBK_NUM_EMUNS_MAX

struct energy_sensor_info {	
	const char* hass_dev_class;
	const char* units;
	const char* name_friendly;
	const char* name_mqtt;
	byte rounding_precision;				
} extern energy_sensors[OBK_NUM_ENUMS_MAX];

extern int g_dhtsCount;

void DRV_Generic_Init();
void DRV_AppendInformationToHTTPIndexPage(http_request_t* request);
void DRV_OnEverySecond();
void DHT_OnEverySecond();
void DHT_OnPinsConfigChanged();
void DRV_RunQuickTick();
void DRV_StartDriver(const char* name);
void DRV_StopDriver(const char* name);
// right now only used by simulator
void DRV_ShutdownAllDrivers();
bool DRV_IsRunning(const char* name);
void DRV_OnChannelChanged(int channel, int iVal);
#if PLATFORM_BK7231N
void SM16703P_setMultiplePixel(uint32_t pixel, uint8_t *data, bool push);
#endif
void SM2135_Write(float* rgbcw);
void BP5758D_Write(float* rgbcw);
void BP1658CJ_Write(float* rgbcw);
void SM2235_Write(float* rgbcw);
void KP18058_Write(float *rgbcw);
void DRV_DGR_OnLedDimmerChange(int iVal);
void DRV_DGR_OnLedEnableAllChange(int iVal);
void DRV_DGR_OnLedFinalColorsChange(byte rgbcw[5]);

// OBK_POWER etc
float DRV_GetReading(int type);
bool DRV_IsMeasuringPower();
bool DRV_IsMeasuringBattery();
bool DRV_IsSensor();
void BL09XX_SaveEmeteringStatistics();

// TuyaMCU exports for LED
void TuyaMCU_OnRGBCWChange(const float *rgbcw, int bLightEnableAll, int iLightMode, float brightnessRange01, float temperatureRange01);
bool TuyaMCU_IsLEDRunning();


#endif /* __DRV_PUBLIC_H__ */

