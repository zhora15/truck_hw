#include "pwm_servo.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_bus.h"

#define PWM_SERVO1_PORT          GPIOB
#define PWM_SERVO2_PORT          GPIOB
#define PWM_SERVO1_PIN           LL_GPIO_PIN_0
#define PWM_SERVO2_PIN           LL_GPIO_PIN_1
#define PWM_SERVO1_CH            LL_TIM_CHANNEL_CH3
#define PWM_SERVO2_CH            LL_TIM_CHANNEL_CH4

bool PWMServo::is_common_tim_initialized = false;

int32_t PWMServo::init() {
    int32_t status = 0;
    if (!is_initialized) {
        static LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
        if (!is_common_tim_initialized) {
            LL_TIM_InitTypeDef TIM_InitStruct = {0};

            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
            TIM_InitStruct.Prescaler = tim_preload;
            TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
            TIM_InitStruct.Autoreload = tim_autoreload;
            TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
            LL_TIM_Init(TIM3, &TIM_InitStruct);
            LL_TIM_DisableARRPreload(TIM3);
            LL_TIM_SetTriggerOutput(TIM3, LL_TIM_TRGO_RESET);
            LL_TIM_DisableMasterSlaveMode(TIM3);

            TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
            TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
            TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
            TIM_OC_InitStruct.CompareValue = 0;
            TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;

            is_common_tim_initialized = true;
        }

        if (type_ == PWM_SERVO_1) {
            LL_TIM_OC_EnablePreload(TIM3, PWM_SERVO1_CH);
            LL_TIM_OC_Init(TIM3, PWM_SERVO1_CH, &TIM_OC_InitStruct);
            LL_TIM_OC_DisableFast(TIM3, PWM_SERVO1_CH);

            LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
            LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
            GPIO_InitStruct.Pin = PWM_SERVO1_PIN;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
            GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
            GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
            LL_GPIO_Init(PWM_SERVO1_PORT, &GPIO_InitStruct);
            is_initialized = true;
        } else if (type_ == PWM_SERVO_2) {
            LL_TIM_OC_EnablePreload(TIM3, PWM_SERVO2_CH);
            LL_TIM_OC_Init(TIM3, PWM_SERVO2_CH, &TIM_OC_InitStruct);
            LL_TIM_OC_DisableFast(TIM3, PWM_SERVO2_CH);

            LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
            LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
            GPIO_InitStruct.Pin = PWM_SERVO2_PIN;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
            GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
            GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
            LL_GPIO_Init(PWM_SERVO2_PORT, &GPIO_InitStruct);
            is_initialized = true;
        }
    }
    return status;
}

static int32_t tim_set_compare_value(TIM_TypeDef *tim, uint32_t channel, uint32_t compare_value) {
    int32_t status = 0;
    switch (channel) {
        case LL_TIM_CHANNEL_CH1: LL_TIM_OC_SetCompareCH1(tim, compare_value); break;
        case LL_TIM_CHANNEL_CH2: LL_TIM_OC_SetCompareCH2(tim, compare_value); break;
        case LL_TIM_CHANNEL_CH3: LL_TIM_OC_SetCompareCH3(tim, compare_value); break;
        case LL_TIM_CHANNEL_CH4: LL_TIM_OC_SetCompareCH4(tim, compare_value); break;
        default: status = 1; break;
    }
    return status;
}

int32_t PWMServo::set_us_impulse(uint32_t us) {
    int32_t status = 0;
    if (is_initialized){
        if (us < tim_autoreload) {
            switch (type_) {
                case PWM_SERVO_1:
                    LL_TIM_CC_EnableChannel(TIM3, PWM_SERVO1_CH);
                    LL_TIM_EnableCounter(TIM3);
                    status = tim_set_compare_value(TIM3, PWM_SERVO1_CH, us);
                    break;
                case PWM_SERVO_2:
                    LL_TIM_CC_EnableChannel(TIM3, PWM_SERVO2_CH);
                    LL_TIM_EnableCounter(TIM3);
                    status = tim_set_compare_value(TIM3, PWM_SERVO2_CH, us);
                    break;
            }
        } else {
            status = 2;
        }
    } else {
        status = 1;
    }

    return status;
}

int32_t PWMServo::stop() {
    int32_t status = 0;
    if (is_initialized) {
        switch (type_) {
            case PWM_SERVO_1:
                LL_TIM_CC_DisableChannel(TIM3, PWM_SERVO1_CH);
                break;
            case PWM_SERVO_2:
                LL_TIM_CC_DisableChannel(TIM3, PWM_SERVO2_CH);
                break;
            default:
                status = 2;
                break;
        }
    } else {
        status = 1;
    }
    return status;
}