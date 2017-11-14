#ifndef L3GD20_H
#define L3GD20_H

#include "stm32f429i_discovery_l3gd20.h"

#define L3G_Sensitivity_250dps     (float)114.285f        /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_500dps     (float)57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_2000dps    (float)14.285f         /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */

void gyro_init(void);
void GyroReadAngRate (float* pfData);
void gyro_calib(float* GyroData);

#endif
