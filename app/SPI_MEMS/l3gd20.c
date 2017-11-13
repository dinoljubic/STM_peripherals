#include <l3gd20.h>

//inicijalizacija
void gyro_init()
{
	L3GD20_InitTypeDef					L3GD20_InitStruct;
	L3GD20_FilterConfigTypeDef	L3GD20_FilterStruct;
	
	//	init struct
	L3GD20_InitStruct.Power_Mode				= L3GD20_MODE_ACTIVE;
	L3GD20_InitStruct.Output_DataRate		= L3GD20_OUTPUT_DATARATE_1;
	L3GD20_InitStruct.Axes_Enable				= L3GD20_AXES_ENABLE;
	L3GD20_InitStruct.Band_Width				= L3GD20_BANDWIDTH_4;
	L3GD20_InitStruct.BlockData_Update	= L3GD20_BlockDataUpdate_Continous;
	L3GD20_InitStruct.Endianness				= L3GD20_BLE_LSB;
	L3GD20_InitStruct.Full_Scale				= L3GD20_FULLSCALE_500;
	L3GD20_Init(&L3GD20_InitStruct);
	
	L3GD20_FilterStruct.HighPassFilter_Mode_Selection 		= L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStruct.HighPassFilter_CutOff_Frequency 	= L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStruct);
  
  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
	
}

// Kopirana funkcija za citanje podataka
void GyroReadAngRate (float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  int i =0;
  
  L3GD20_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  L3GD20_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);
  
  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & 0x40))
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  }
  else
  {
    for(i=0; i<3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & 0x30)
  {
  case 0x00:
    sensitivity=L3G_Sensitivity_250dps;
    break;
    
  case 0x10:
    sensitivity=L3G_Sensitivity_500dps;
    break;
    
  case 0x20:
    sensitivity=L3G_Sensitivity_2000dps;
    break;
  }
  /* divide by sensitivity */
  for(i=0; i<3; i++)
  {
  pfData[i]=(float)RawData[i]/sensitivity;
  }
}
// kalibracija
void gyro_calib(float* GyroData)
{
	float X_BiasError, Y_BiasError, Z_BiasError = 0.0;
  uint32_t BiasErrorSplNbr = 500;
  int i = 0;
  
  for (i = 0; i < BiasErrorSplNbr; i++)
  {
    GyroReadAngRate(GyroData);
    X_BiasError += GyroData[0];
    Y_BiasError += GyroData[1];
    Z_BiasError += GyroData[2];
  }
  /* Set bias errors */
  X_BiasError /= BiasErrorSplNbr;
  Y_BiasError /= BiasErrorSplNbr;
  Z_BiasError /= BiasErrorSplNbr;
  
  /* Get offset value on X, Y and Z */
  GyroData[0] = X_BiasError;
  GyroData[1] = Y_BiasError;
  GyroData[2] = Z_BiasError;
}
//
