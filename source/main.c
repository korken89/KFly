#include "main.h"

void main( void )
{
	/* Configure the hardware for use. */
	prvSetupHardware();

	/* Load Tasks */
	/*xTaskCreate(vTask1,
				"Blink",
				20,
				NULL,
				1,
				NULL);
				
	xTaskCreate(vTaskControlLoop,
				"Control",
				200,
				NULL,
				2,
				NULL);*/
				
	xTaskCreate(vTaskArmDisarm,
				"Arm/Disarm",
				40,
				NULL,
				1,
				NULL);
	
	/* Start the scheduler. */
	vTaskStartScheduler();

	/* 	Will only get here if there was insufficient memory to create the idle
		task.  The idle task is created within vTaskStartScheduler(). */
	float temp[3];
	while(1)
	{

	}
}

void prvSetupHardware( void )
{
	__disable_irq();
	LED_Init();
	UART_Init();
	//GPS_UART_Init();
	PWM_Init();
	I2C0_Init();
	vInitInputs();
	Timer2_Init();
	SystemInit();
	__enable_irq();
	//BMA180_Init(BW_150HZ, RANGE_2G);
	//ITG3200_Init();
}

void vTask1(void *pvParameters)
{
	while (1)
	{
		setLED(1);
		clearLED(2);
		vTaskDelay(200 / portTICK_RATE_MS);
		
		setLED(2);
		clearLED(1);
		vTaskDelay(200 / portTICK_RATE_MS);
	}
}

void vTaskControlLoop(void *pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	
	float acc_tmp[2];
	float gyro_tmp[3];
	float pid_tmp[3];
	kalman_data data_xz;
	kalman_data data_yz;
	
	pid_data data_roll;
	pid_data data_pitch;
	
	InitKalman(&data_xz);
	InitKalman(&data_yz);
	InitPID(&data_roll);
	InitPID(&data_pitch);
	
	while (1)
	{
		//ReadAccAngle(acc_tmp);
		//ReadGyroRate(gyro_tmp);
		
		/*UpdKalman(&data_xz, acc_tmp[0], gyro_tmp[0]);
		UpdKalman(&data_yz, acc_tmp[1], gyro_tmp[1]);
		
		pid_tmp[0] = PIDUpdatePitch(&data_pitch, &data_xz);
		pid_tmp[1] = PIDUpdateRoll(&data_roll, &data_yz);*/
		
		
		/*ftoa(data_xz.x1);
		UART0_SendChar('\t');
		
		ftoa(data_yz.x1);
		
		UART0_SendChar('\n');*/
		
		vTaskDelayUntil(&xLastWakeTime, 10 / portTICK_RATE_MS);
	}
}

void vTaskArmDisarm(void *pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	static uint16_t arm_counter = 0;
	static uint16_t disarm_counter = 0;
	
	while (1)
	{
		if (arm_counter > 99)
		{
			arm_counter = 0;
			disarm_counter = 0;
			setLED(1);
			setLED(2);
		}
		else if ((GetInputLevel(THROTTLE_CHANNEL) < 0.05f) && (GetInputLevel(YAW_CHANNEL) < -0.95f))
			arm_counter++;
		else
			arm_counter = 0;
			
		if (disarm_counter > 99)
		{
			arm_counter = 0;
			disarm_counter = 0;
			clearLED(1);
			clearLED(2);
		}
		else if ((GetInputLevel(THROTTLE_CHANNEL) < 0.05f) && (GetInputLevel(YAW_CHANNEL) > 0.95f))
			disarm_counter++;
		else
			disarm_counter = 0;
			
		vTaskDelayUntil(&xLastWakeTime, 10 / portTICK_RATE_MS);
	}
}

void vConfigureTimerForRunTimeStats( void )
{
	Timer1_Init();
}
