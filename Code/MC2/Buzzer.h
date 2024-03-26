
#ifndef BUZZER_H_
#define BUZZER_H_
#include"gpio.h"
#include "std_types.h"
#define Buzzer_pin PORTC_ID
#define Buzzer_port PIN5_ID
 void Buzzer_init(void);
 void Buzzer_on(void);
 void Buzzer_off(void);



#endif /* BUZZER_H_ */
