/*
 * utils.h
 *
 *  Created on: 17.03.2011
 *      Author: Biba
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>


uint8_t checksum_calc(const char *data, uint16_t csconst, uint8_t len);
uint8_t isxdigit_string(const char *nptr, uint8_t len);
int8_t strtoint8(uint8_t *dst, const char *nptr, uint8_t len);
void int8tostr(char *dst, const uint8_t *nptr, uint8_t len);
uint8_t num_of_bits_set(uint16_t x);

#endif /* UTILS_H_ */
