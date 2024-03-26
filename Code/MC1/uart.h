 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
typedef enum {
    UART_5_BIT,
    UART_6_BIT,
    UART_7_BIT,
    UART_8_BIT,
    // Add more data bit options as needed
} UART_BitData;

typedef enum {
    UART_PARITY_DISABLED,
    UART_PARITY_EVEN,
    UART_PARITY_ODD,
    // Add more parity options as needed
} UART_Parity;

typedef enum {
    UART_1_STOP_BIT,
    UART_2_STOP_BITS,
    // Add more stop bit options as needed
} UART_StopBit;

typedef enum {
    UART_BAUD_9600,
    UART_BAUD_115200,
    // Add more baud rate options as needed
} UART_BaudRate;

typedef struct {
    UART_BitData bit_data;
    UART_Parity parity;
    UART_StopBit stop_bit;
    UART_BaudRate baud_rate;
} UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
