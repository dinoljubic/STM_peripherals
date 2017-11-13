#include <main.h>

#define ABS(x)                     (x < 0) ? (-x) : x
	
float Buffer[6];
float Gyro[3];
uint8_t Xval, Yval = 0x00;

int main()
{
	gyro_init();
	gyro_calib(Gyro);
	
	while(1)
		MEMS();
}


void MEMS(void)
{
	GyroReadAngRate(Buffer);

  Buffer[0] = (int8_t)Buffer[0] - (int8_t)Gyro[0];
  Buffer[1] = (int8_t)Buffer[1] - (int8_t)Gyro[1];
  
  /* Update autoreload and capture compare registers value*/
  Xval = ABS((int8_t)(Buffer[0]));
  Yval = ABS((int8_t)(Buffer[1])); 
	
	for (int i = 0; i < 0x000FFFFF; i++);
}
