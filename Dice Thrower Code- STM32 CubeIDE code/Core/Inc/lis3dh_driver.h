#include "stm32f4xx_hal.h"

#ifndef INC_LIS3DH_H_
#define INC_LIS3DH_H_

#define LIS3DH_ADDR         (0x18 << 1)

// Registers
#define LIS3DH_REG_WHO_AM_I     0x0F
#define LIS3DH_REG_CTRL1        0x20
#define LIS3DH_REG_CTRL4        0x23
#define LIS3DH_REG_OUT_X_L      0x28

// Expected ID
#define LIS3DH_WHO_AM_I_ID      0x33

// Function prototypes
uint8_t LIS3DH_Init(I2C_HandleTypeDef *hi2c);
void LIS3DH_ReadRawAcc(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z);

#endif
