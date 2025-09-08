/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SDRAM_BANK_ADDR         0xD0000000  // FMC , SDRAM Bank 2 starting address

#define SDRAM_SIZE_BYTES 		0x800000    		    // Number of 8-bit (bytes)
#define SDRAM_SIZE_WORDS        (SDRAM_SIZE_BYTES / 2)  // Number of 16-bit words
#define SDRAM_SIZE_DWORDS 	    (SDRAM_SIZE_BYTES / 4)  // Number of 32-bit dwords

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void SDRAM_Clear()
{
	for(uint32_t count = 0; count < SDRAM_SIZE_WORDS; count++)
	{
		*(__IO uint16_t*)(SDRAM_BANK_ADDR + count * 2) = (uint16_t)0x00;
	}
}
// ↑↑↑↑↑↑↑ same as above
// memset((void*)SDRAM_BANK_ADDR, 0x00, SDRAM_SIZE_BYTES);
// memset works for Only sets byte-by-byte (not 16-bit or 32-bit values)

void SDRAM_WriteIncrement_bytes()
{

	for (volatile uint32_t count = 0; count < SDRAM_SIZE_BYTES; count++)
	{
		*(__IO uint8_t*)(SDRAM_BANK_ADDR + count) = (uint8_t)count;
	}
}

void SDRAM_WriteIncrement_bytes_Hello_World()
{
    #define arrsize 12
    char arr[arrsize] = "Hello world\n";

    uint8_t* sdram_ptr = (uint8_t*)SDRAM_BANK_ADDR;

    for (uint32_t i = 0; i < arrsize; i++) {
        sdram_ptr[i] = arr[i];
    }
}

void SDRAM_WriteIncrement_words()
{
    for (uint32_t count = 0; count < SDRAM_SIZE_WORDS; count++)
    {
        *(__IO uint16_t*)(SDRAM_BANK_ADDR + count * 2) = (uint16_t)count;
    }

}

/*
 * 🔁 Alternative (Cleaner) Way
 * You can also use a pointer cast once, and just increment the pointer:
    void SDRAM_WriteIncrement_words()
	{
		__IO uint16_t* ptr = (__IO uint16_t*)SDRAM_BANK_ADDR;
		for (uint32_t i = 0; i < SDRAM_SIZE_WORDS; i++)
		{
			ptr[i] = (uint16_t)i;
		}
	}
*/


void SDRAM_WriteIncrement_dwords()
{
    for (uint32_t count = 0; count < SDRAM_SIZE_DWORDS; count++)
    {
        *(__IO uint32_t*)(SDRAM_BANK_ADDR + count * 4) = count; // 4 bytes in one count (Jump 4 C pointers)
    }
}
/*
  	void SDRAM_Fill32(uint32_t value)
	{
		__IO uint32_t* ptr = (__IO uint32_t*)SDRAM_BANK_ADDR;
		for (uint32_t i = 0; i < SDRAM_SIZE_DWORDS; i++)
		{
			ptr[i] = value;
		}
	}
 */

void FMC_Test(void)
{
	uint32_t fmcTestStart;
	uint32_t fmcTestStop;


	fmcTestStart = HAL_GetTick();
	SDRAM_Clear();
	fmcTestStop = HAL_GetTick() - fmcTestStart;
	HAL_Delay(50);

	fmcTestStart = HAL_GetTick();
	SDRAM_WriteIncrement_bytes();
	fmcTestStop = HAL_GetTick() - fmcTestStart;
	HAL_Delay(50);

	fmcTestStart = HAL_GetTick();
	SDRAM_WriteIncrement_words();
	fmcTestStop = HAL_GetTick() - fmcTestStart;
	HAL_Delay(50);

	fmcTestStart = HAL_GetTick();
	SDRAM_WriteIncrement_dwords();
	fmcTestStop = HAL_GetTick() - fmcTestStart;
	HAL_Delay(50);

	fmcTestStart = HAL_GetTick();
	SDRAM_WriteIncrement_bytes_Hello_World();
	fmcTestStop = HAL_GetTick() - fmcTestStart;
	HAL_Delay(50);

	HAL_Delay(50);
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
  MX_FMC_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET);

  FMC_Test();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
	FMC_SDRAM_CommandTypeDef Command;
	HAL_StatusTypeDef status;
	/* Step 1 and Step 2 already done in HAL_SDRAM_Init() */
	/* Step 3: Configure a clock configuration enable command */
	Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE; /* Set MODE bits to "001" */
	Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2; /* configure the Target Bank bits */
	Command.AutoRefreshNumber      = 1;
	Command.ModeRegisterDefinition = 0;
	status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
	HAL_Delay(1); /* Step 4: Insert 100 us minimum delay - Min HAL Delay is 1ms */
	/* Step 5: Configure a PALL (precharge all) command */
	Command.CommandMode            = FMC_SDRAM_CMD_PALL; /* Set MODE bits to "010" */
	status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
	/* Step 6: Configure an Auto Refresh command */
	Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE; /* Set MODE bits to "011" */
	Command.AutoRefreshNumber      = 2;
	status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
	/* Step 7: Program the external memory mode register */
	Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;/*set the MODE bits to "100" */
	Command.ModeRegisterDefinition =  (uint32_t)0 | 0<<3 | 3<<4 | 0<<7 | 1<<9;
	status = HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
	/* Step 8: Set the refresh rate counter - refer to section SDRAM refresh timer register in RM0455 */
	/* Set the device refresh rate
	* COUNT = [(SDRAM self refresh time / number of row) x  SDRAM CLK] – 20
			= [(64ms/4096) * 100MHz] - 20 = 1406.25 - 20 ~ 1386 = 0x56A */
	status = HAL_SDRAM_ProgramRefreshRate(&hsdram1, 0x56A);
  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

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
#ifdef USE_FULL_ASSERT
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
