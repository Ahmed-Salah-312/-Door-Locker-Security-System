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
	uint16 ubrr_value = 0;

/* Set the data bit format, parity, and stop bit as per the configuration structure */
uint8 UCSRC_config = (1 << URSEL); // URSEL must be one
if (Config_Ptr->bit_data == UART_8_BIT) {
    UCSRC_config |= (1 << UCSZ0) | (1 << UCSZ1);
} else {
    // Handle other data bit options if needed
}

if (Config_Ptr->parity == UART_PARITY_EVEN) {
    UCSRC_config |= (1 << UPM1);
} else if (Config_Ptr->parity == UART_PARITY_ODD) {
    UCSRC_config |= (1 << UPM0) | (1 << UPM1);
}

if (Config_Ptr->stop_bit == UART_2_STOP_BITS) {
    UCSRC_config |= (1 << USBS);
}

/* Set the baud rate based on the provided configuration */
// Calculate ubrr_value as per the baud rate
// Set the U2X bit for double transmission speed if needed

/* Set UCSRC with the configured values */
UCSRC = UCSRC_config;

/* Set UBRRH and UBRRL with ubrr_value */

/* Enable receiver and transmitter */
UCSRB = (1 << RXEN) | (1 << TXEN);
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
