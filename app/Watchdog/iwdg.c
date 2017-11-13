#include <iwdg.h>

void iwdg_init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void iwdg_reload(void)
{
	IWDG_ReloadCounter();
}
