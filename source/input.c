#include "input.h"

volatile input_data *sInputData;
volatile input_calibration sInputCalibration;

void vInitInputs(void)
{
	// Fattigmanskalibrering!
	sInputCalibration.ch_bottom[0] = 1056;
	sInputCalibration.ch_bottom[1] = 1084;
	sInputCalibration.ch_bottom[2] = 1121;
	sInputCalibration.ch_bottom[3] = 1020;
	
	sInputCalibration.ch_center[0] = 1501;
	sInputCalibration.ch_center[1] = 1084;
	sInputCalibration.ch_center[2] = 1496;
	sInputCalibration.ch_center[3] = 1497;
	
	sInputCalibration.ch_top[0] = 1938;
	sInputCalibration.ch_top[1] = 1764;
	sInputCalibration.ch_top[2] = 1934;
	sInputCalibration.ch_top[3] = 1884;
	
	
	EINT_Init();
	sInputData = EINT_GetPointerToValues();
}

/**
 * Return the status of the inputs.
 * 
 * Bit 0 = Channel 1
 * Bit 1 = Cahnnel 2
 * 		...
 * Bit 5 = Channel 6
 * 
 * Bit Set = Normal Operation
 * Bit Cleared = No Connection
 **/
uint8_t GetInputStatus(void)
{
	uint8_t status;
	for (uint8_t i = 0; i < 6; i++)
	{
		if (GetInputLevel(i) > 0)
			status |= (1<<i);
	}
	
	return status;
}

/**
 * Resets old calibrated values in order for new
 * values to be written.
 **/
void ResetCalibration(void)
{
	for (uint8_t i = 0; i < 6; i++)
	{
		sInputCalibration.ch_bottom[i] = 1500;
		sInputCalibration.ch_center[i] = 1500;
		sInputCalibration.ch_top[i] = 1500;
	}
}

/**
 * Calibrates the top and bottom level of the inputs.
 * Loop it while calibrating
 **/
void CalibrateInputLevels(void)
{
	for (uint8_t i = 0; i < 6; i++)
	{
		if (GetRawInputLevel(i) > sInputCalibration.ch_top[i])
			sInputCalibration.ch_top[i] = GetRawInputLevel(i);
			
		if (GetRawInputLevel(i) < sInputCalibration.ch_bottom[i])
			sInputCalibration.ch_top[i] = GetRawInputLevel(i);
	}
}

/**
 * Calibrates the center level of the inputs.
 * 
 * If the vaules are withing 12.5% of top or bottom then the
 * center value will equal top or bottom when calibrated in order 
 * to get -100 to 100%, 0 to 100% and -100 to 0%.
 **/
void CalibrateCenterLevels(void)
{
	for (uint8_t i = 0; i < 6; i++)
	{
		if (GetRawInputLevel(i) > (sInputCalibration.ch_top[i] - sInputCalibration.ch_top[i]>>3))
			sInputCalibration.ch_center[i] = sInputCalibration.ch_top[i];
		
		else if (GetRawInputLevel(i) < (sInputCalibration.ch_bottom[i] + sInputCalibration.ch_bottom[i]>>3))
			sInputCalibration.ch_center[i] = sInputCalibration.ch_bottom[i];
		
		else
			sInputCalibration.ch_center[i] = GetRawInputLevel(i);	
	}
}

void SaveCalibratedDataToFlashBuffer(void)
{
	/* Save all calibrated data to the flash buffer function goes here... */
}

/**
 * Returns the current RC stick position with bias compensation
 * in the format (-1.0 ... 0.0 ... 1.0) as (MIN ... MID ... MAX).
 * If MID = MAX or MIN then it will become:
 * (0.0 ... 1.0) as (MID ... MAX) and (-1.0 ... 0.0) as (MIN ... MID)
 **/
float GetInputLevel(uint8_t channel)
{
	if (GetRawInputLevel(channel) == 0)
		return 0.0f;
	
	int16_t level = (int16_t)GetRawInputLevel(channel) - (int16_t)sInputCalibration.ch_center[channel];
	float temp;
	
	if (level > 0)
	{
		if (sInputCalibration.ch_center[channel] == sInputCalibration.ch_top[channel])
			return 0.0f;
		
		temp = (float)level/(float)(sInputCalibration.ch_top[channel] - sInputCalibration.ch_center[channel]);
		
		if (temp > 1.0f)	// Just in case something has happened
			return 1.0f;
		else 
			return temp;
	}
		
	else if (level < 0)
	{
		if (sInputCalibration.ch_center[channel] == sInputCalibration.ch_bottom[channel])
			return 0.0f;
		
		temp = (float)level/(float)(sInputCalibration.ch_center[channel] - sInputCalibration.ch_bottom[channel]);
		
		if (temp < -1.0f)	// Just in case something has happened
			return -1.0f;
		else 
			return temp;
	}
	else
		return 0.0f;	
}

/**
 * Returns the current RC stick position without bias compensation.
 **/
uint16_t GetRawInputLevel(uint8_t channel)
{
	return sInputData->ch[channel];
}
