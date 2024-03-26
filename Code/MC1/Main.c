/*
 * Main.c
 *
 *  Created on: ٣١‏/١٠‏/٢٠٢٣
 *      Author: Ahmed Salah
 */
#include"lcd.h"
#include"keypad.h"
#include"uart.h"
#include"Timer.h"
#include"avr/io.h"
#include"util/delay.h"

void enter_pass(void);
void re_enter_pass(void);
void step2(void);
void open_door(void);
void change_pass(void);
void take_password_from_user(uint8 *pass);
void Alarm_mode(void) ;
void timer1_tick(void);
#define rightpass_send 'R'
#define open_door_signal 'O'
#define alarm_mode 'A'
#define send_from_eerom 'M'
#define send_new_pass  'P'
#define enter_key 14
// Set up the UART configuration structure
UART_ConfigType uartConfig = {UART_8_BIT,UART_PARITY_DISABLED,UART_1_STOP_BIT,UART_BAUD_9600};
// Set up the Timer1 configuration structure
Timer1_ConfigType Timer1Config ={0,500,TIMER1_F_CPU_1,TIMER1_CTC};

uint8 pass1[5];
uint8 pass2[5];
uint8 Timer1Ticks = 0;
uint8 tries = 0 ;
int main(void){
	// Enable global interrupts
	SREG |= 1<<7;
	// Initialize UART with the configuration
	UART_init(&uartConfig);
	LCD_init();
	Timer1_setCallBack(timer1_tick);

	while (1){
		/*step 1 (entering the password for the first time )*/
		enter_pass();
		re_enter_pass();
		for (uint8 i = 0 ; i < 5 ; i++){
			do{
			enter_pass();
			re_enter_pass();
			}while(pass1[i] != pass2[i]);
		}
				/*send to eeprom using uart*/
				UART_sendByte(rightpass_send);
				for (uint8 i = 0 ; i < 5 ; i++)
				{
				UART_sendByte(pass2[i]);
				}
		/*step 2 (asking for opening the door or changing the password)*/
		step2();
		if(KEYPAD_getPressedKey()){
			if (KEYPAD_getPressedKey()==16) // +
			{
				open_door();
			}
			else if (KEYPAD_getPressedKey()==12){ //-
				change_pass();
			}
			}
		}


	return 0 ;
}

void enter_pass(void)
{

	LCD_moveCursor(0,0);
	LCD_displayString("Plz Enter pass:");
	LCD_moveCursor(1,0);
	take_password_from_user(pass1);
}
void re_enter_pass(void){
	LCD_clearScreen(); /*clear the screen*/
	LCD_moveCursor(0,0);
	LCD_displayString("Plz Re-Enter the");
	LCD_moveCursor(1,0);
	LCD_displayString("Same Pass:");
	LCD_moveCursor(1,11);
	take_password_from_user(pass2);
	}

void step2(void){
		LCD_clearScreen(); /*clear the screen*/
		LCD_moveCursor(0,0);
		LCD_displayString("+ : Open Door");
		LCD_moveCursor(1,0);
		LCD_displayString("- : Change pass");
		}

void open_door(void){
	uint8 equal = 1;
	uint8 recieved_pass [5];
	enter_pass();
	UART_sendByte(send_from_eerom);
	for(uint8 i = 0 ;i < 5 ; i++){
		recieved_pass[i] =UART_recieveByte();
        _delay_ms(15);
	}

	    for (int i = 0; i < 5; i++) {
	        if (recieved_pass[i] != pass1[i]) {
	            equal = 0;  // Arrays are not equal
	            break;
	        }
	    }
	    if (equal ==1){
	    	/*open the door*/
	    	UART_sendByte(open_door_signal);
	    }
	    else {
	    	// check for 2 times more and give the alarm in the next
	    	do{
	    		for (int i = 0; i < 5; i++) {
	    			        if (recieved_pass[i] != pass1[i]) {
	    			            equal = 0;  // Arrays are not equal
	    			            break;
	    			        }
	    			    }
	    		tries ++ ;
	    	}while(tries<2 && equal !=1);
	    	if(tries=2 && equal !=1){ //faild must turn on the alarm
	    		UART_sendByte(alarm_mode);
	    		Alarm_mode();
	    	}

	    }



}
void change_pass(void){

	enter_pass();
	UART_sendByte(send_new_pass);
	send_password(pass1);

}


void send_password(uint8 *password_arr) {
    for (int i = 0; i < 5; i++) {
        UART_sendByte(password_arr[i]);
        _delay_ms(15);
    }
}


void timer1_tick(void) {
    Timer1Ticks++;
}

void Alarm_mode(void) {
    LCD_clearScreen();
    Timer1_init(&Timer1Config);
    while (Timer1Ticks != 60) {
        LCD_displayString("    WARNING!    ");
        _delay_ms(1000);
        LCD_clearScreen();
        _delay_ms(500);
    }
    Timer1Ticks = 0;
    Timer1_deInit();
}
void take_password_from_user(uint8 *pass) {
    for (uint8 i = 0; i < 5; i++) {
    	pass1[i] = KEYPAD_getPressedKey();
        _delay_ms(500); // pressing time
        if (pass1[i] >= '0' && pass1[i] <= '9') {
            LCD_displayCharacter(pass1[i]);
            _delay_ms(500);
            LCD_moveCursor(1, 5 + i);
            LCD_displayCharacter('*');
        } else {
            i--;
        }
        _delay_ms(15);
    }

    while (KEYPAD_getPressedKey() != enter_key);
}

