#ifndef __TGS2600_H__
#define __TGS2600_H__
#include "stm32f10x.h"                  // Device header
#define TGS2600_PIN GPIO_Pin_0
void TGS2600_init(void);
float TGS2600_read(void);

#endif



