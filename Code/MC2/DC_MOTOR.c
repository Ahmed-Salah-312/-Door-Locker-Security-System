#include "DC_MOTOR.h"
#include "PWM.h"


void DcMotor_Init(void){
	/*Set the pins EN & IN1 & IN2 as output pins*/
	GPIO_setupPinDirection(Motor_port,Motor_IN1_pin,PIN_OUTPUT);
	GPIO_setupPinDirection(Motor_port,Motor_IN2_pin,PIN_OUTPUT);
	GPIO_setupPinDirection(Motor_port,Motor_EN_pin,PIN_OUTPUT);

	GPIO_writePin(Motor_port,Motor_EN_pin,LOGIC_LOW);	//enable is zero
}

void DcMotor_Rotate(uint8 state,uint8 speed){
	GPIO_writePin(Motor_port,Motor_EN_pin,LOGIC_HIGH);	//enable is high
	PWM_Timer0_Start(speed);
	switch(state){
	case  ACW :
		GPIO_writePin(Motor_port,Motor_IN1_pin,LOGIC_LOW);
		GPIO_writePin(Motor_port,Motor_IN2_pin,LOGIC_HIGH);
		break ;
	case CW :
		GPIO_writePin(Motor_port,Motor_IN1_pin,LOGIC_HIGH);
		GPIO_writePin(Motor_port,Motor_IN2_pin,LOGIC_LOW);
		break ;
	case STOP:
		GPIO_writePin(Motor_port,Motor_IN1_pin,LOGIC_LOW);
		GPIO_writePin(Motor_port,Motor_IN2_pin,LOGIC_LOW);
	    break;
	default :
		GPIO_writePin(Motor_port,Motor_IN1_pin,LOGIC_LOW);
		GPIO_writePin(Motor_port,Motor_IN2_pin,LOGIC_LOW);
	    break;
	}

}

