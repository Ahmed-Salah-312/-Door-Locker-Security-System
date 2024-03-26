#include "Buzzer.h"

void Buzzer_init(void){
	/*Set the Buzzer Pin as an Output pin*/
GPIO_setupPinDirection(Buzzer_port, Buzzer_pin, PIN_OUTPUT);
	/*Initalize the Buzzer Pin with zero (turned off)*/
GPIO_writePin(Buzzer_port, Buzzer_pin, LOGIC_LOW);

}
 void Buzzer_on(void){
	 /*Trun on the buzzer*/
	 GPIO_writePin(Buzzer_port, Buzzer_pin, LOGIC_HIGH);

 }
 void Buzzer_off(void){
	 /*Trun off the buzzer*/
	 GPIO_writePin(Buzzer_port, Buzzer_pin, LOGIC_LOW);

 }
