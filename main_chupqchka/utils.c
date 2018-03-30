/*
 * utils.c
 *
 *  Created on: 17.03.2011
 *      Author: Biba
 */

#include "avr/io.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
/**
 * @brief Calculates the checksum based on data buffer content.
 * @param data
 * @param csconst check sum constant.
 * @return
 */
uint8_t checksum_calc(const char *data, uint16_t csconst, uint8_t len)
{
	uint16_t csum = csconst;
	const char *c = data;

	//Add with carry for all data Bytes
	while (len--)
	{
		csum += *c++; // Sum
		csum += ((csum & 0x100) >> 8);// Add Carry
		csum &= 0x00ff; // Clear Carry Bit
	}

	return (uint8_t) csum;
}

/**
 *
 * @param name
 * @param len
 * @return true or false
 */
uint8_t isxdigit_string(const char *nptr, uint8_t len)
{
	char c;

	while (len--)
	{
		c = *nptr++;

		if (c < '0' || c > '9')
			return 0;
		else if (c < 'A' || c > 'F')
			return 0;
		else if (c >= 'a' || c <= 'f')
			return 0;
		else
			break;

	}

	return 1;
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

/**
 * Convert int8 to char using radix 16. dst string must have enough space
 * for the result. 2 * len + 1 for '\0' terminator.
 * @param dst string to store converted value
 */
void int8tostr(char *dst, const uint8_t *nptr, uint8_t len)
{
	register char *d = dst;
	register const uint8_t *s = nptr;

	while (len--)
	{
		//MSB
		if (((*s >> 4) & 0x0f) > 9)
		{
			*d = ((*s >> 4) & 0x0f) - 10 + 'a';
		}
		else
		{
			*d = ((*s >> 4) & 0x0f) + '0';
		}

		d++;

		//LSB
		if ((*s & 0x0f) > 9)
		{
			*d = (*s & 0x0f) - 10 + 'a';
		}
		else
		{
			*d = (*s & 0x0f) + '0';
		}

		d++;
		s++;
	}

	*d = '\0';
}

/**
 *
 * @param x
 * @return
 */
uint8_t num_of_bits_set(uint16_t x)
{
	//shift right until finding a zero
	uint16_t result = 1;
	uint8_t num = 0;

	for (uint8_t i = 0; i < sizeof x; i++)
	{
		if (x & result)
			num++;
		result <<= 1;
	}

	return num;
}
