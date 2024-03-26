#include "std_types.h"
#include "uart.h"
#include "Timer.h"
#include "twi.h"
#include "Buzzer.h"
#include "external_eeprom.h"
#include "DC_MOTOR.h"
#include "util/delay.h"
#include "avr/io.h"

#define rightpass_send 'R'
#define open_door_signal 'O'
#define alarm_mode 'A'
#define send_from_eerom 'M'
#define recieve_new_pass  'P'
#define PASSWORD_SET_FLAG_LOCATION		0xFF

void receive_password(uint8 *PasswordArr);
void send_password(uint8 *password_arr);

void timer1_tick(void);

uint8 Timer1Ticks = 0;
Timer1_ConfigType Timer1Config = {0 , 31250 ,TIMER1_CTC, TIMER1_F_CPU_256} ;

/* The configuration structure of the UART protocol */
UART_ConfigType UART_MyConfig = {UART_8_BIT , UART_PARITY_DISABLED , UART_1_STOP_BIT , UART_BAUD_9600};
/* The configuration structure of the I2c protocol */
TWI_ConfigType	I2C_MyConfig = {0x01 , TWI_BAUD_400K};
int main (void){


	uint8 Pass1[5] ;		/* Password buffer arrays */


	SREG |= 1<<7;		/* Enabling Global Interrupts Service */

	UART_init(&UART_MyConfig);
	TWI_init(&I2C_MyConfig);
	DcMotor_Init();
	Buzzer_init();
	Timer1_setCallBack(timer1_tick);

	while(1){

		switch(UART_recieveByte()){

		case rightpass_send:
			receive_password(Pass1);
			EEPROM_writeByte(Pass1, 0x0F); /* Write Pass1 in the external EEPROM */

			break;

		case send_from_eerom :		/* Getting ready to receive the first password */
			send_password(EEPROM_readByte(Pass1, 0x0F));
			break;

		case recieve_new_pass :		/* Getting ready to receive the second password */
			receive_password(Pass1);
			EEPROM_writeByte(Pass1, 0x0F); /* Write Pass1 in the external EEPROM */
			break;

		case open_door_signal	:							/* Unlocking the door */

			Timer1_init(&Timer1Config);

			DcMotor_Rotate(CW,100);
			while(Timer1Ticks != 15);		/* waiting 15 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();
			Timer1_init(&Timer1Config);

			DcMotor_Rotate(STOP,0);
			while(Timer1Ticks != 3);		/* waiting 3 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();
			Timer1_init(&Timer1Config);

			DcMotor_Rotate(ACW,100);
			while(Timer1Ticks != 15);		/* waiting 15 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();

			DcMotor_Rotate(STOP,0);

			break;

		case alarm_mode :				/* Alarm mode */

			Timer1_init(&Timer1Config);
			Buzzer_on();
			while(Timer1Ticks != 60);	/* waiting 60 seconds*/
			Timer1Ticks = 0;
			Timer1_deInit();

			Buzzer_off();

			break;
		}
	}
	return 0;
}

void receive_password(uint8 *PasswordArr){
	for(uint8 i = 0 ; i < 5 ; i++){
		PasswordArr[i] = UART_recieveByte();
		_delay_ms(15);
	}
}
void send_password(uint8 *password_arr) {
    for (int i = 0; i < 5; i++) {
        UART_sendByte(password_arr[i]);
        _delay_ms(15);
    }
}

void timer1_tick(void){
	Timer1Ticks++;
}
