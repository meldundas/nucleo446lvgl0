/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lvgl.h"
#include "ui.h"  //eezstudio
#include "vars.h" //eezstudio
//#include "screens.h"
#include <stdbool.h>
#include <stdlib.h> //rand
#include <stdio.h>	//sprintf
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TFT_HOR_RES 320
#define TFT_VER_RES 240

#define BYTES_PER_PIXEL LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t buf1[TFT_HOR_RES*TFT_VER_RES / 30 * BYTES_PER_PIXEL];	//5120
//uint8_t buf1[TFT_HOR_RES*TFT_VER_RES / 20 * BYTES_PER_PIXEL];	//7680
//uint8_t buf1[TFT_HOR_RES*TFT_VER_RES / 15 * BYTES_PER_PIXEL];	//10240
//uint8_t buf1[TFT_HOR_RES*TFT_VER_RES / 10 * BYTES_PER_PIXEL];	//15360
//uint8_t buf1[TFT_HOR_RES*TFT_VER_RES /  3 * BYTES_PER_PIXEL]; //51200 bytes

//uint8_t buf2[TFT_HOR_RES*TFT_VER_RES /  3 * BYTES_PER_PIXEL];  //51200 bytes

static lv_obj_t * label;

typedef struct {
	uint8_t isTouch;
	uint16_t Xpos;
	uint16_t Ypos;
}sTouchData;

sTouchData tData;

//extern volatile uint8_t Touch_PenDown;

bool B1State = false;

extern objects_t objects;

const char *get_var_therm_temp();  //get thermistor

lv_display_t * display1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CRC_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int myrandom(int min, int max){
   return (rand() % (max + 1 - min) + min);
}


const char *get_var_therm_temp()  //get thermistor
{
	static int count = 0;
	static char tempbuf[10];

	count++;
	if(count > 60 * 10 * 5)
	{
		count = 0;



	int val = myrandom(200, 225);

	float temp = val/10.0;

	sprintf(tempbuf, "%0.1f", temp);
	}
	return tempbuf;

}

static lv_obj_t * spinbox;

static void lv_spinbox_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(spinbox);
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(spinbox);
    }
}

void action_switch_changed(lv_event_t * e)
{

}


//static void ConvHL(uint8_t *s, int32_t l)
//{
//	uint8_t v;
//	while (l > 0) {
//		v = *(s+1);
//		*(s+1) = *s;
//		*s = v;
//		s += 2;
//		l -= 2;
//	}
//}




void tft_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{

    uint16_t height = area->y2 - area->y1 + 1;
    uint16_t width = area->x2 - area->x1 + 1;

	Displ_SetAddressWindow(area->x1, area->y1, area->x2, area->y2);
//    Displ_SetAddressWindow(area->x1, area->y1, width, height);

	   uint16_t * buf16 = (uint16_t *)px_map; /* Let's say it's a 16 bit (RGB565) display */

	   //NON DMA - 20 fps at 80-90% CPU

	   /* The most simple case (also the slowest) to send all rendered pixels to the screen one-by-one. */

//    int32_t x, y;
//   for(y = area->y1; y <= area->y2; y++) {
//        for(x = area->x1; x <= area->x2; x++) {
//        	Displ_Pixel(x, y, *buf16);
//            buf16++;
//        }
//   }


	   //DMA - 60 fps at 3-5% CPU



////    	  ConvHL((uint8_t *)buf16, (int32_t)width*height);


      Displ_Transmit(SPI_DATA, (uint8_t *)buf16, height*width*2, 1 );  	//1 swaps bytes for 8 bit spi
    																	//*2 sending bytes not uint16_t



    /* IMPORTANT!!!
     * Inform LVGL that flushing is complete so buffer can be modified again. */
    lv_display_flush_ready(display);



}


void my_input_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    if(tData.isTouch) {
        data->point.x = tData.Xpos;
        data->point.y = tData.Ypos;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);

    /*Refresh the text*/
    lv_label_set_text_fmt(label, "%"LV_PRId32, lv_slider_get_value(slider));
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  Displ_Init(Displ_Orientat_90);			// initialize display controller - set orientation parameter as per your needs
  Displ_CLS(GREEN);							// clear the screen - BLACK or any other color you prefer
  Displ_BackLight('I');  					// initialize backlight

  //setup lvgl
  lv_init();
  lv_tick_set_cb(HAL_GetTick);



  //mel
  display1 = lv_display_create(TFT_HOR_RES, TFT_VER_RES);

  lv_display_set_buffers(display1, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
//mel buf2 instead of NULL

  lv_display_set_flush_cb(display1, tft_flush_cb);


  /* Create and set up at least one display before you register any input devices. */
  lv_indev_t * indev = lv_indev_create();        /* Create input device connected to Default Display. */

  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);

  lv_indev_set_read_cb(indev, my_input_read);    /* Set driver function. */

  //mel for eezstudio
  ui_init();


//  create_screen_main();

  /*Change the active screen's background color*/
//  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFF0000), LV_PART_MAIN);


//  spinbox = lv_spinbox_create(lv_screen_active());
//  lv_spinbox_set_range(spinbox, -1000, 25000);
//  lv_spinbox_set_digit_format(spinbox, 5, 2);
//  lv_spinbox_step_prev(spinbox);
//  lv_obj_set_width(spinbox, 100);
//  lv_obj_center(spinbox);
//
//  int32_t h = lv_obj_get_height(spinbox);
//
//  lv_obj_t * btn = lv_button_create(lv_screen_active());
//  lv_obj_set_size(btn, h, h);
//  lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
//  lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_PLUS, 0);
//  lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL,  NULL);
//
//  btn = lv_button_create(lv_screen_active());
//  lv_obj_set_size(btn, h, h);
//  lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
//  lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_MINUS, 0);
//  lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, NULL);

  lv_obj_t * label1 = lv_label_create(lv_screen_active());
  lv_label_set_text_fmt(label1, "Value: %s", get_var_therm_temp() );

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
	  {
		  B1State = false;
//		  lv_obj_add_flag(objects.led_button, LV_OBJ_FLAG_HIDDEN);  //hide
//		  lv_obj_invalidate(objects.led_off);
//		  lv_imagebutton_set_state(objects.led_button, LV_IMAGEBUTTON_STATE_PRESSED);
//		  lv_obj_invalidate(objects.led_button);



	  }
	  else
	  {
		  B1State = true;
//		  lv_imagebutton_set_state(objects.led_button, LV_IMAGEBUTTON_STATE_RELEASED);
//		  lv_obj_clear_flag(objects.led_button, LV_OBJ_FLAG_HIDDEN);  //show
//		  lv_obj_invalidate(objects.led_button);
//		  lv_label_set_text_fmt(label1, "Value: %s", get_var_therm_temp() );
	  }

	  Touch_GetXYtouch(&tData.Xpos, &tData.Ypos, &tData.isTouch);


	  ui_tick();
//	  tick_screen_main();
	  lv_timer_handler();
//	  lv_task_handler(); //m

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 4500-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISPL_DC_GPIO_Port, DISPL_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISPL_CS_Pin|DISPL_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DISPL_DC_Pin */
  GPIO_InitStruct.Pin = DISPL_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DISPL_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCH_CS_Pin */
  GPIO_InitStruct.Pin = TOUCH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TOUCH_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCH_INT_Pin */
  GPIO_InitStruct.Pin = TOUCH_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TOUCH_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPL_CS_Pin DISPL_RST_Pin */
  GPIO_InitStruct.Pin = DISPL_CS_Pin|DISPL_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
