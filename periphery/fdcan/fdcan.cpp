#include "fdcan.h"
#include "stm32g4xx_hal_fdcan.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_rcc.h"

#include <cstdio>

FDCAN_HandleTypeDef fdcan1;

static void(*rx_complete_receive_callback)() = nullptr;

void FDCAN1_IT0_IRQHandler(void)
{
    HAL_FDCAN_IRQHandler(&fdcan1);
}

/**
  * @brief This function handles FDCAN1 interrupt 1.
  */
void FDCAN1_IT1_IRQHandler(void)
{
    HAL_FDCAN_IRQHandler(&fdcan1);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {

    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0){
        printf("receive\n");
        if (rx_complete_receive_callback != nullptr) {
            rx_complete_receive_callback();
        }
    }
}

int32_t FDCAN::get_message(uint32_t &message_id, uint8_t *rx_data, uint32_t &rx_data_size) {
    uint32_t rx_fifo0_size = HAL_FDCAN_GetRxFifoFillLevel(&fdcan_handle, FDCAN_RX_FIFO0);
    if (rx_fifo0_size > 0) {
        return 1;
    }
    FDCAN_RxHeaderTypeDef rx_header;

    if (HAL_FDCAN_GetRxMessage(&fdcan_handle, FDCAN_RX_FIFO0, &rx_header, rx_data) != HAL_OK) {
        return 2;
    }

    message_id = rx_header.Identifier;
    rx_data_size = rx_header.DataLength;

    return 0;
}

FDCAN :: FDCAN() {
    LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PCLK1);
    LL_AHB1_GRP1_EnableClock(RCC_APB1ENR1_FDCANEN); // SET_BIT(RCC->APB1ENR1, RCC_APB1ENR1_FDCANEN);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB); //  SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);


    LL_GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    fdcan_handle.Instance = FDCAN1;
    fdcan_handle.Init.ClockDivider = FDCAN_CLOCK_DIV1;
    fdcan_handle.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
    fdcan_handle.Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK; // FDCAN_MODE_NORMAL FDCAN_MODE_INTERNAL_LOOPBACK
    fdcan_handle.Init.AutoRetransmission = DISABLE;
    fdcan_handle.Init.TransmitPause = ENABLE;
    fdcan_handle.Init.ProtocolException = DISABLE;
    fdcan_handle.Init.NominalPrescaler = 4;
    fdcan_handle.Init.NominalSyncJumpWidth = 1;
    fdcan_handle.Init.NominalTimeSeg1 = 26;
    fdcan_handle.Init.NominalTimeSeg2 = 9;
    fdcan_handle.Init.DataPrescaler = 1;
    fdcan_handle.Init.DataSyncJumpWidth = 1;
    fdcan_handle.Init.DataTimeSeg1 = 26;
    fdcan_handle.Init.DataTimeSeg2 = 9;
    fdcan_handle.Init.StdFiltersNbr = 0;
    fdcan_handle.Init.ExtFiltersNbr = 0;
    fdcan_handle.Init.TxFifoQueueMode = FDCAN_TX_QUEUE_OPERATION;
    if (HAL_FDCAN_Init(&fdcan_handle) != HAL_OK) {

    }
    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x00;
    sFilterConfig.FilterID2 = 0x11;
    if (HAL_FDCAN_ConfigFilter(&fdcan_handle, &sFilterConfig) != HAL_OK)
    {

    }

    NVIC_SetPriority(FDCAN1_IT0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    NVIC_SetPriority(FDCAN1_IT1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(FDCAN1_IT1_IRQn);

    uint32_t status = 0;
    status = HAL_FDCAN_Start(&fdcan_handle);
    status |= HAL_FDCAN_ActivateNotification(&fdcan_handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    printf("FDCAN Start %d", status);
}

void FDCAN :: transmit(uint32_t message_id, uint8_t *p_data, uint32_t size){
    FDCAN_TxHeaderTypeDef tx_header;
    tx_header.Identifier = message_id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_4;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_FD_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;

    uint32_t status = 0;
    uint32_t len1 = HAL_FDCAN_GetTxFifoFreeLevel(&fdcan_handle);
    status |= HAL_FDCAN_AddMessageToTxFifoQ(&fdcan_handle, &tx_header, p_data);
    uint32_t len2 = HAL_FDCAN_GetTxFifoFreeLevel(&fdcan_handle);
    printf("%u %u %u\n", status, len1, len2);

}

void FDCAN::set_rx_complete_callback(void (*callback)()) {
    rx_complete_receive_callback = callback;
}
