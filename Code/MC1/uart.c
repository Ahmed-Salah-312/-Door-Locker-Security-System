 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr)
{
    // Calculate the UBRR value based on the given baud rate and configuration
    uint16 ubrr_value;
    if (Config_Ptr->baud_rate == UART_BAUD_9600) {
        ubrr_value = 51;
    } else if (Config_Ptr->baud_rate == UART_BAUD_115200) {
        ubrr_value = 3;
    } else {
        // Handle other baud rate options as needed
    }

    // Set the UBRRH and UBRRL registers
    UBRRH = (uint8)(ubrr_value >> 8);
    UBRRL = (uint8)ubrr_value;

    // Set the data bit format
    if (Config_Ptr->bit_data == UART_8_BIT) {
        SET_BIT(UCSRC, URSEL); // Choose UCSRC
        SET_BIT(UCSRB, UCSZ2); // UCSRB register for 8-bit data mode
        CLEAR_BIT(UCSRC, UCSZ1);
        CLEAR_BIT(UCSRC, UCSZ0);
    } else {
        // Handle other data bit options if needed
    }

    // Set the parity
    if (Config_Ptr->parity == UART_PARITY_EVEN) {
        SET_BIT(UCSRC, URSEL);
        SET_BIT(UCSRC, UPM1);
        CLEAR_BIT(UCSRC, UPM0);
    } else if (Config_Ptr->parity == UART_PARITY_ODD) {
        SET_BIT(UCSRC, URSEL);
        SET_BIT(UCSRC, UPM1);
        SET_BIT(UCSRC, UPM0);
    }

    // Set the number of stop bits
    if (Config_Ptr->stop_bit == UART_2_STOP_BITS) {
        SET_BIT(UCSRC, URSEL);
        SET_BIT(UCSRC, USBS);
    }

    // Enable receiver and transmitter
    SET_BIT(UCSRB, RXEN);
    SET_BIT(UCSRB, TXEN);
}
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
    // Wait for empty transmit buffer
    while (!(UCSRA & (1 << UDRE)));

    // Put the data into the buffer and send it
    UDR = data;
}


/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}		
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
