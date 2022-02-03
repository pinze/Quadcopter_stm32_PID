/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "gpio.h"
#include "mpu6050.h"
#include "inv_mpu.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint16_t 	Channel1HighTime, Channel2HighTime, Channel3HighTime, Channel4HighTime;
uint16_t 	Channel1Period, Channel2Period, Channel3Period, Channel4Period;
uint8_t  	Channel1Edge = 0, Channel2Edge = 0, Channel3Edge = 0, Channel4Edge = 0;
uint16_t 	Channel1Percent, Channel2Percent, Channel3Percent, Channel4Percent;
uint16_t 	Channel1RisingTimeLast=0, Channel1RisingTimeNow, Channel1FallingTime;
uint16_t 	Channel2RisingTimeLast=0, Channel2RisingTimeNow, Channel2FallingTime;
uint16_t 	Channel3RisingTimeLast=0, Channel3RisingTimeNow, Channel3FallingTime;
uint16_t 	Channel4RisingTimeLast=0, Channel4RisingTimeNow, Channel4FallingTime;
int states,states2,states3,states4;
uint16_t duty,reciver1,reciver2,reciver3,reciver4;
float pitch,roll,yaw;
float ipitch,iroll,iyaw;
int throttle;
int start = 0;

float pid_p_roll=10;
float pid_i_roll=5;
float pid_d_roll=15;
int pid_max_roll=400;
////3  3  3
float pid_p_pitch=10;
float pid_i_pitch=5;
float pid_d_pitch=15;
int pid_max_pitch=400;


float pid_p_yaw=10;
float pid_i_yaw=5;
float pid_d_yaw=15;
int pid_max_yaw = 400;

float pid_error;
float pid_set_roll , pid_i_mem_roll , pid_roll_d_error ,roll_input;
float pid_set_pitch , pid_i_mem_pitch , pid_pitch_d_error ,pitch_input;
float pid_set_yaw , pid_i_mem_yaw , pid_yaw_d_error ,yaw_input; 
int pid_roll,pid_pitch,pid_yaw;
int esc_1,esc_2,esc_3,esc_4;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
		pid_p_pitch=pid_p_roll;
		pid_i_pitch=pid_i_roll;
		pid_d_pitch=pid_d_roll;
		pid_max_pitch=pid_max_roll;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USB_DEVICE_Init();
	USB_Status_Init();
  /* USER CODE BEGIN 2 */
	/////////tim start
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
  /* USER CODE END 2 */
	usb_printf("usb START\t\n");
	HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD_GPIO_Port,LD_Pin,GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD_GPIO_Port,LD_Pin,GPIO_PIN_RESET);
	//////////esc initialization start
	usb_printf("wait 5 mins\n");
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1940);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1940);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1940);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1940);
	HAL_Delay(5000);
	usb_printf("push the throttle to the maximum\n");
	HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_SET);
	while(reciver3 < 1940)
	{
		if(states3==2)
			{
				usb_printf("Channel3 = %d	\n", Channel3HighTime);
				states3=0;
				reciver3 = Channel3HighTime;
			}
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, reciver3);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, reciver3);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, reciver3);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, reciver3);
	}
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1100);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1100);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1100);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1100);
		usb_printf("esc initialization OK!!!! \r\n");
	//////////mpu initialization
	HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_RESET);
	
	
	while(MPU_Init())
	{
		HAL_GPIO_TogglePin(LDD_GPIO_Port,LDD_Pin);
		HAL_GPIO_TogglePin(LD_GPIO_Port,LD_Pin);
		usb_printf("mpu initialization error!! \r\n");
		HAL_Delay(200);
	}
	usb_printf("mpu initialization OK----------------------- \r\n");
	HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_SET);
	/////////mpu6050_dmp initialization
	while(mpu_dmp_init())
	{
		HAL_GPIO_TogglePin(LDD_GPIO_Port,LDD_Pin);
		HAL_GPIO_TogglePin(LD_GPIO_Port,LD_Pin);
		usb_printf("load DMP error!! \r\n");
		HAL_Delay(200);
	}
	usb_printf("mpu DMP load OK----------------------- \r\n");
	init_dmp_data();                                                                  //////while
	
	HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_SET);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		while(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0);      ///when catch fall,catch again
		//usb_printf("pitch = %.2f\t| roll =%.2f\t|yaw = %.2f\t\n",pitch,roll,yaw);
		roll_input = roll-iroll;
		pitch_input = pitch-ipitch;
		yaw_input = iyaw-yaw;
		usb_printf("pitch = %.2f\t| roll =%.2f\t|yaw = %.2f\t\n",pitch_input,roll_input,yaw_input);		
    /* USER CODE END WHILE */
		///////reciver search
		channelsearch();
		/////channel 1 roll
		/////channel 2 elevator pitch
		/////channel 3 throttle
		/////channel 4 yaw
		if(reciver3<1200 && reciver2 > 1900)start = 1;
		if(start == 1)
		{
			start = 2;
			pid_i_mem_roll = 0;
			pid_roll_d_error = 0;
			pid_i_mem_pitch = 0;
			pid_pitch_d_error = 0;
			pid_i_mem_yaw = 0;
			pid_yaw_d_error = 0;
		}
		if(start == 2 && reciver3 < 1200 && reciver2 < 1200)start = 0;
		//////channel1 set point   roll
		pid_set_roll = 0;
		if(reciver1 > 1525)pid_set_roll = (reciver1 - 1525)/10;
		else if(reciver1 < 1515)pid_set_roll = (reciver1 - 1515)/10;
		//////channel2 set point   pitch
		pid_set_pitch = 0;
		if(reciver2 > 1525)pid_set_pitch= (reciver2 - 1525)/10;
		else if(reciver2 < 1515)pid_set_pitch = (reciver2 - 1515)/10;
		//////channel4 set point    yaw
		pid_set_yaw = 0;
		if(reciver4 > 1525)pid_set_yaw = (reciver4 - 1525)/10;
		else if(reciver4 < 1515)pid_set_yaw = (reciver4 - 1515)/10;
		/////set end
		
		
		if(start == 2)
		{
			pid_caculate();
			throttle = reciver3 + 100;
			esc_1 = throttle + pid_pitch + pid_roll + pid_yaw; //Calculate the pulse for esc 1 (front-right - CCW)
			esc_2 = throttle - pid_pitch + pid_roll - pid_yaw; //Calculate the pulse for esc 2 (rear-right - CW)
			esc_3 = throttle - pid_pitch - pid_roll + pid_yaw; //Calculate the pulse for esc 3 (rear-left - CCW)
			esc_4 = throttle + pid_pitch - pid_roll - pid_yaw; //Calculate the pulse for esc 4 (front-left - CW)
			if(esc_1>1940)esc_1=1940;
			else if(esc_1<1200)esc_1 = 1200;
			
			if(esc_2>1940)esc_2=1940;
			else if(esc_2<1200)esc_2 = 1200;
			
			if(esc_3>1940)esc_3=1940;
			else if(esc_3<1200)esc_3 = 1200;
			
			if(esc_4>1940)esc_4=1940;
			else if(esc_4<1200)esc_4 = 1200;
			iyaw = iyaw + pid_set_yaw;
			usb_printf("Esc1 = %d| Esc2 = %d|esc3 = %d|Esc4 = %d\n",esc_1,esc_2,esc_3,esc_4);
			HAL_GPIO_TogglePin(LDD_GPIO_Port,LDD_Pin);
			HAL_GPIO_TogglePin(LD_GPIO_Port,LD_Pin);
		}
		else if(start == 0)
		{
			throttle = 1100;
			esc_1 = 1100;
			esc_2 = 1100;
			esc_3 = 1100;
			esc_4 = 1100;
			HAL_GPIO_WritePin(LDD_GPIO_Port,LDD_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LD_GPIO_Port,LD_Pin,GPIO_PIN_SET);
		}
		
			
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, esc_1);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, esc_2);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, esc_3);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, esc_4);
		

			
    /* USER CODE BEGIN 3 */
		mpu_reset_fifo();
  }
  /* USER CODE END 3 */
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);
	/////////////////channel 1
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if(Channel1Edge == 0)
		{
			Channel1RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);//get rissing time
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);//change catch states
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
			Channel1Edge = 1;
			states=1;
		}
		else if(Channel1Edge == 1)  //when the state is high
		{
			Channel1FallingTime = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);	//get falling time
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);//change catch states
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
			states=2;
			if(Channel1FallingTime < Channel1RisingTimeNow)      //caculate high statet time
			{
				Channel1HighTime = Channel1FallingTime + 0xffff - Channel1RisingTimeNow + 1;
			}
			else
			{				
				Channel1HighTime = Channel1FallingTime - Channel1RisingTimeNow;
			}
			Channel1Edge = 0;
		}
	}
	///////////////channel2
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if(Channel2Edge == 0)
		{
			Channel2RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
			Channel2Edge = 1;
			states2 = 1;
		}
		else if(Channel2Edge == 1)
		{
			Channel2FallingTime = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);	
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
			states2 = 2;
			if(Channel2FallingTime < Channel2RisingTimeNow)
			{				
				Channel2HighTime = Channel2FallingTime + 0xffff - Channel2RisingTimeNow + 1;
			}
			else
			{
				Channel2HighTime = Channel2FallingTime - Channel2RisingTimeNow;
			}
			Channel2Edge = 0;
		}
	}
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if(Channel3Edge == 0)
		{
			Channel3RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_3);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
			Channel3Edge = 1;
			states3=1;
		}
		else if(Channel3Edge == 1)
		{
			Channel3FallingTime = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_3);	
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
			states3=2;
			if(Channel3FallingTime < Channel3RisingTimeNow)
			{
				Channel3HighTime = Channel3FallingTime + 0xffff - Channel3RisingTimeNow + 1;
			}
			else
			{
				Channel3HighTime = Channel3FallingTime - Channel3RisingTimeNow;
			}
			Channel3Edge = 0;
		}
	}
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if(Channel4Edge == 0)
		{
			Channel4RisingTimeNow = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
			Channel4Edge = 1;
			states4=1;
		}
		else if(Channel4Edge == 1)
		{
			Channel4FallingTime = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);	
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
			states4=2;
			if(Channel4FallingTime < Channel4RisingTimeNow)
			{
				Channel4HighTime = Channel4FallingTime + 0xffff - Channel4RisingTimeNow + 1;
			}
			else
			{
				Channel4HighTime = Channel4FallingTime - Channel4RisingTimeNow;
			}
			Channel4Edge = 0;
		}
	}
}

void init_dmp_data(void)
{
	int it = 0;
	while(it<40)
	{
		while(mpu_dmp_get_data(&ipitch,&iroll,&iyaw)!=0);      ///when catch fall,catch again
		usb_printf("ipitch = %.2f\t| iroll =%.2f\t|iyaw = %.2f\t\n",ipitch,iroll,iyaw);
		mpu_reset_fifo();
		it++;
	}
}

void channelsearch(void)
{
	if(states==2)
			{
				//usb_printf("Channel1 = %d	\n", Channel1HighTime);
				states=0;
				reciver1 = Channel1HighTime;
			}
			//////channel2 search
			if(states2==2)
			{
				//usb_printf("Channel2 = %d	\n", Channel2HighTime);
				states2=0;
				reciver2 = Channel2HighTime;
			}
			//////channel3 search
			if(states3==2)
			{
				//usb_printf("Channel3 = %d	\n", Channel3HighTime);
				states3=0;
				reciver3 = Channel3HighTime;
			}
			//////channel4 search
			if(states4==2)
			{
				//usb_printf("Channel4 = %d	\n", Channel4HighTime);
				states4=0;
				reciver4 = Channel4HighTime;
			}
}

void pid_caculate(void)
{
	//roll caculate
	pid_error = roll_input-pid_set_roll;		//error value
	pid_i_mem_roll += pid_i_roll*pid_error;		//integral
	if(pid_i_mem_roll>pid_max_roll)
		pid_i_mem_roll=pid_max_roll;
	else if(pid_i_mem_roll<pid_max_roll*-1)
		pid_i_mem_roll=pid_max_roll*-1;
	
	pid_roll = pid_p_roll*pid_error+pid_i_mem_roll+pid_d_roll*(pid_error-pid_roll_d_error);	//pid caculate
	if(pid_roll>pid_max_roll)
		pid_roll=pid_max_roll;
	else if(pid_roll<pid_max_roll*-1)
		pid_roll=pid_max_roll*-1;
	pid_roll_d_error = pid_error;
	
	//pitch caculate
	pid_error = pitch_input - pid_set_pitch;		//error value
	pid_i_mem_pitch += pid_i_pitch*pid_error;	//integral
	if(pid_i_mem_pitch>pid_max_pitch)
		pid_i_mem_pitch=pid_max_pitch;
	else if(pid_i_mem_pitch<pid_max_pitch*-1)
		pid_i_mem_pitch=pid_max_pitch*-1;
		
	pid_pitch = pid_p_pitch*pid_error + pid_i_mem_pitch + pid_d_pitch*(pid_error - pid_pitch_d_error);	//pid caculate
	if(pid_pitch>pid_max_pitch)
		pid_pitch=pid_max_pitch;
	else if(pid_pitch<pid_max_pitch*-1)
		pid_pitch=pid_max_pitch*-1;
	pid_pitch_d_error = pid_error;
	
	//yaw caculate
	pid_error = yaw_input - pid_set_yaw;		//error value
	pid_i_mem_yaw += pid_i_yaw*pid_error;		//integral
	if(pid_i_mem_yaw>pid_max_yaw)
		pid_i_mem_yaw=pid_max_yaw;
	else if(pid_i_mem_yaw<pid_max_yaw*-1)
		pid_i_mem_yaw=pid_max_yaw*-1;
		
	pid_yaw = pid_p_yaw*pid_error + pid_i_mem_yaw + pid_d_yaw*(pid_error - pid_yaw_d_error);	//pid caculate
	if(pid_yaw>pid_max_yaw)
		pid_yaw=pid_max_yaw;
	else if(pid_yaw<pid_max_yaw*-1)
		pid_yaw=pid_max_yaw*-1;
	pid_yaw_d_error = pid_error;
	
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
