#include "lis3dh_driver.h"
#include "stm32f4xx_hal.h"

uint8_t LIS3DH_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t ctrl1 = 0x57;  // 100Hz, all axes enabled
    uint8_t ctrl4 = 0x00;  // ±2g, High-resolution mode

    HAL_I2C_Mem_Write(hi2c, LIS3DH_ADDR, LIS3DH_REG_CTRL1, 1, &ctrl1, 1, HAL_MAX_DELAY);
    HAL_I2C_Mem_Write(hi2c, LIS3DH_ADDR, LIS3DH_REG_CTRL4, 1, &ctrl4, 1, HAL_MAX_DELAY);

    uint8_t id = 0;
    HAL_I2C_Mem_Read(hi2c, LIS3DH_ADDR, LIS3DH_REG_WHO_AM_I, 1, &id, 1, HAL_MAX_DELAY);
    return (id == LIS3DH_WHO_AM_I_ID);
}

void LIS3DH_ReadRawAcc(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    HAL_I2C_Mem_Read(hi2c, LIS3DH_ADDR, LIS3DH_REG_OUT_X_L | 0x80, 1, data, 6, HAL_MAX_DELAY);

    *x = ((int16_t)(data[1] << 8 | data[0])) >> 4;
    *y = ((int16_t)(data[3] << 8 | data[2])) >> 4;
    *z = ((int16_t)(data[5] << 8 | data[4])) >> 4;
}
