/************************************************************************
 Title:    Converting strings
 Author:   Bilyana Borisova <bibishte@gmail.com>
 File:     $Id: messages.c.c 2015/04/19
 Software: AVR-GCC 4.9.2, AVR Libc 1.8.0
 Hardware: DRV8825 & ATmega8
 License:  GNU General Public License v2
 Usage:    see Doxygen manual

 DESCRIPTION:
 This modul contains functions that are converting strings

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License v2 as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License v2 for more details.

 ************************************************************************/



#include "utils.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>



/**
 * Temporarry buffer for converting uint64_t to string
 */
static char buf[20];


/**
       * Function that converts ASCI to uint64_t
       * @param in the first argument.
		 * @return uint64_t 
       */
uint64_t ConvertASCItouint64( char *in )
{
	char *b;
	uint64_t c=0;
	uint64_t multi=1;
	
	b = in;

	while ( ( (*b > 47) && (*b < 58 )) || ( *b == 32 ) || ( *b == 46 ) || ( *b ==44 ) )
	{
		b++;
	}
	
	while(b>=in)
	{
		if(( (*b > 47) && (*b < 58 )))
		{
			c=c+multi*(*b-'0');
			multi=multi*10;
		}
		
		b--;
	}
	return c;
}

/**
       * Function that converts intiger to string.
       * @param t the first argument.
		 * @return char 
       */
char *int_to_string(uint64_t i)
{
	unsigned char temp;
	unsigned char s = 0,t = 0;
	if(i==0)
	{
		buf[0]='0';
		buf[1]=0;
		return buf;
	}
	while(i) {
		buf[s++] = i % 10 + '0';
		i /= (uint64_t)(10);
	}
	buf[s] = 0;
	s-=1;
	for(;t<s;t++,s--) {
		temp = buf[s];
		buf[s]=buf[t];
		buf[t] = temp;
	}
	return buf;
}

/**
       * Function that reaplaces the printf for uint64_t.
       * @param putc the first argument.
		 * @return void 
       */
void putstring(const char *putc)
{
	const char *p;
	p=putc;
	while (*p!=0)
	{
		usart_putchar(*p, stdout);
		p++;
	}
	
	return;
}


/**
       * Function that reaplaces the printf from Programme memmory space.
       * @param putc the first argument.
		 * @return void 
       */
void putstring_P(const char *putc)
{
	while (pgm_read_byte ( putc )!=0)
	{
		usart_putchar(pgm_read_byte ( putc ), stdout);
		putc++;
	}
	
	return;
}

/**
 * Convert string pointed to nptr using base 16.
 * @param dst
 * @param nptr
 * @param len
 * @return Return true or false if char is not xdigit
 */
int8_t strtoint8(uint8_t *dst, const char *nptr, uint8_t len)
{
	register char c;
	register uint8_t *d = dst;
	register uint8_t cnt = 0;

#define BASE 16

	*d = 0;

	while (len--)
	{
		c = *nptr++;

		if (cnt > 1)
		{
			d++;
			*d = 0; //set destination to 0;
			cnt = 0;
		}

		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'A' && c <= 'Z')
			c -= 'A' - 10;
		else if (c >= 'a' && c <= 'z')
			c -= 'a' - 10;
		else
			return 0;

		if (c >= BASE)
			return 0;

		*d = *d * BASE + c;

		cnt++;
	}

	return 1;
}

