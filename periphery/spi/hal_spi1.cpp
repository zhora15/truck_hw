#include "hal_spi1.h"

void HAL_SPI1::configure() {
    LL_SPI_InitTypeDef SPI_InitStruct = {0};

    SPI_InitStruct.TransferDirection = transferDirection;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV16;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    SPI_InitStruct.CRCPoly = 10;
    LL_SPI_Disable(SPI1);
    LL_SPI_Init(SPI1, &SPI_InitStruct);
    LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_EnableNSSPulseMgt(SPI1);
    LL_SPI_Enable(SPI1);
}

int32_t HAL_SPI1::exchange_bytes_half_duplex(
    uint8_t *ptr_tx_bytes,
    size_t tx_size,
    uint8_t *ptr_rx_bytes,
    size_t rx_size
) {
    int32_t status = 0;
    configure();

    uint32_t temp_tick_counter = HAL_GetTick(); // TODO Remove in FreeRTOS
    while ((LL_SPI_IsActiveFlag_TXE(SPI1) == 0) || (LL_SPI_IsActiveFlag_BSY(SPI1) == 1)) {
        if (((HAL_GetTick() - temp_tick_counter)
            > time_out)) {
            status = 1;
            break;
        }
    }

    for (size_t i = 0; i < tx_size; ++i) {
        if (tx_size > 0) {
            LL_SPI_TransmitData8(SPI1, ptr_tx_bytes[i]);
        }
        while ((LL_SPI_IsActiveFlag_TXE(SPI1) == 0) || (LL_SPI_IsActiveFlag_BSY(SPI1) == 1)) {
            if (((HAL_GetTick() - temp_tick_counter) > time_out)) {
                status = 1;
                break;
            }
        }
        if (status != 0) {
            break;
        }
    }
    __DSB();
    __ISB();
    __DMB();
    while ((LL_SPI_IsActiveFlag_TXE(SPI1) == 0) || (LL_SPI_IsActiveFlag_BSY(SPI1) == 1)) {
        if (((HAL_GetTick() - temp_tick_counter)
            > time_out)) {
            status = 1;
            break;
        }
    }
    if (status != 0) {
        return status;
    }
    __DSB();
    __ISB();
    __DMB();
    LL_SPI_Disable(SPI1);
    LL_SPI_SetTransferDirection(SPI1, LL_SPI_HALF_DUPLEX_RX);
    LL_SPI_Enable(SPI1);
    int i = 0;
    while (i < rx_size) {
        if (LL_SPI_IsActiveFlag_RXNE(SPI1) == 1) {
            ptr_rx_bytes[i] = LL_SPI_ReceiveData8(SPI1);
            ++i;
        }
    }
    LL_SPI_Disable(SPI1);
    LL_SPI_SetTransferDirection(SPI1, LL_SPI_HALF_DUPLEX_TX);

    return status;
}

void HAL_SPI1::init() {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_7; // TODO add third pin
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
