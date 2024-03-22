#include "main.h"
#include "adc.h"
#include "fdcan.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "ucpd.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "performance_counter.h"
#include <cstdio>
#include "MotorControl.h"
#include "SensorPolling.h"
#include "stm32g4xx_hal_def.h"

#define SIMPLEFOC_DISABLE_DEBUG

void SystemClock_Config(void);

void vApplicationTickHook() {
    LL_TIM_ClearFlag_UPDATE(TIM6);
}

extern void MX_LPUART1_UART_Init(void);
#include "lpuart1.c"

extern "C" void __aeabi_atexit() {};     // for virtual destructors
extern "C" void __cxa_pure_virtual();   // for abstract base class
extern "C" void __cxa_guard_acquire();  // for local static init
extern "C" void __cxa_guard_release();  // for local static init

#pragma weak __cxa_pure_virtual = __aeabi_atexit
#pragma weak __cxa_guard_acquire = __aeabi_atexit
#pragma weak __cxa_guard_release = __aeabi_atexit

#include "FreeRTOS.h"
#include "task.h"

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    MX_LPUART1_UART_Init();
    printf("Start\n");
    MX_TIM6_Init();
    static MotorControl MT;
    static SensorPolling SP;
//    MT.init();
//    MT.calibrate();
    SP.init();
    SP.start();
    printf("Scheduling\n");
    LL_TIM_ClearFlag_UPDATE(TIM6);
    LL_TIM_EnableIT_UPDATE(TIM6);
    LL_TIM_EnableCounter(TIM6);
    vTaskStartScheduler();

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}

void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4)
    {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSE_Enable();
    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1)
    {
    }

    LL_RCC_HSI_Enable();
    /* Wait till HSI is ready */
    while(LL_RCC_HSI_IsReady() != 1)
    {
    }

    LL_RCC_HSI_SetCalibTrimming(64);
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_2, 24, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_2, 24, LL_RCC_PLLQ_DIV_6);
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_EnableDomain_48M();
    LL_RCC_PLL_Enable();
    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1)
    {
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }

    /* Insure 1us transition state at intermediate medium speed clock*/
    for (__IO uint32_t i = (170 >> 1); i !=0; i--);

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_SetSystemCoreClock(144000000);

    /* Update the time base */
    HAL_InitTick (TICK_INT_PRIORITY);
}

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
