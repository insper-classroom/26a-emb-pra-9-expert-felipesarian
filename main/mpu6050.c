#include "mpu6050.h"
#include "pins.h"

#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/i2c.h"

#define MPU_I2C_PORT i2c0
#define MPU_ADDRESS  0x68
#define I2C_TIMEOUT_US 5000

void mpu6050_init(void) {
    i2c_init(MPU_I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA_GPIO, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_GPIO, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_GPIO);
    gpio_pull_up(I2C_SCL_GPIO);

    uint8_t buf[] = {0x6B, 0x00};
    int n = i2c_write_timeout_us(MPU_I2C_PORT, MPU_ADDRESS, buf, 2, false,
                                 I2C_TIMEOUT_US);
    if (n < 0) {
        printf("[mpu] ERRO: I2C nao responde (cheque fiacao SDA=%d SCL=%d)\n",
               I2C_SDA_GPIO, I2C_SCL_GPIO);
    } else {
        printf("[mpu] PWR_MGMT_1 escrito ok\n");
    }
}

void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    uint8_t buffer[14] = {0};

    uint8_t reg = 0x3B;
    int n = i2c_write_timeout_us(MPU_I2C_PORT, MPU_ADDRESS, &reg, 1, true,
                                 I2C_TIMEOUT_US);
    if (n < 0) {
        accel[0] = accel[1] = accel[2] = 0;
        gyro[0] = gyro[1] = gyro[2] = 0;
        *temp = 0;
        return;
    }
    n = i2c_read_timeout_us(MPU_I2C_PORT, MPU_ADDRESS, buffer, 14, false,
                            I2C_TIMEOUT_US);
    if (n < 0) {
        accel[0] = accel[1] = accel[2] = 0;
        gyro[0] = gyro[1] = gyro[2] = 0;
        *temp = 0;
        return;
    }

    for (int i = 0; i < 3; i++) {
        accel[i] = (int16_t)((buffer[i * 2] << 8) | buffer[i * 2 + 1]);
    }
    *temp = (int16_t)((buffer[6] << 8) | buffer[7]);
    for (int i = 0; i < 3; i++) {
        gyro[i] = (int16_t)((buffer[8 + i * 2] << 8) | buffer[8 + i * 2 + 1]);
    }
}
