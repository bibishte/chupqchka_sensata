/*
 * usartm162.h
 *
 *  Created on: 08.02.2011
 *      Author: Biba
 */

#ifndef USARTM162_H_
#define USARTM162_H_

/* This file should only be included from <avr/io.h>, never directly. */

#ifndef USART_H_
#  error "Include <com/usart.h> instead of this file."
#endif

#ifndef USARTXXX_H_
#  define USARTXXX_H_ "usartm162.h"
#else
#  error "Attempt to include more than one <com/usartXXX.h> file."
#endif

/* ATmega with two USART */
#define ATMEGA_USART1

#endif /* USARTM162_H_ */
