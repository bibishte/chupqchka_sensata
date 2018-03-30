/************************************************************************
 Title:    This is the main fail with continuos loop 
 Author:   Bilyana Borisova <bibishte@gmail.com>
 File:     $Id: main.c 2015/04/19
 Software: AVR-GCC 4.9.2, AVR Libc 1.8.0
 Hardware: DRV8825 & ATmega8
 License:  GNU General Public License v2
 Usage:    see Doxygen manual

 DESCRIPTION:
 This turns on the motor and directly goes in the continuos loop for controlling it. 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License v2 as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License v2 for more details.

 ************************************************************************/



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "usart.h"
#include "messages.h"
#include "interpreter.h"
#include "config.h"
#include "drv_8825.h"
#include "utils.h"
#include "messages.h"
#include "main.h"

/**@brief define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 11059200UL
#endif

/**@brief 115200 baud */
#define USART_BAUD_RATE 115200
#define NODE_ADDRESS 0xf1

uint8_t address __attribute__((section(".eeprom"))) = NODE_ADDRESS;
uint16_t max_speed __attribute__((section(".eeprom"))) = 30;
uint16_t acceleration __attribute__((section(".eeprom"))) = 60;
uint8_t current_mode __attribute__((section(".eeprom"))) = MODE_32_MICROSTEP;
uint8_t current_decay __attribute__((section(".eeprom"))) = AUTO_DECAY;
/**
 * Set function for standart IO getchart and putchart from UART library
 */
FILE usart_str =
        FDEV_SETUP_STREAM(usart_putchar, usart_getchar, _FDEV_SETUP_RW);

/**@brief main loop*/
int16_t main(void) //Main Loop
{
    char message[32]; // 1 byte + for string termination

    int8_t cmd;
    char *cptr;
    char * pch;
    uint64_t ch;
    uint8_t i;
    uint8_t received_address;
    //const uint8_t node_address = NODE_ADDRESS;
    


    SFIOR &= 0xfb; //11111011 -> PUD=0
    /*
     * Set ports data direction
     */
    Init_Input_Output();

    usart_init(USART_BAUD_SELECT(USART_BAUD_RATE, F_CPU));



    /*
     * now enable interrupt, since USART library is interrupt controlled
     */
    sei();
    stdout = stdin = &usart_str;
    
    const uint8_t node_address = eeprom_read_byte(&address);
    //const uint8_t node_address = 0xf1;
    _delay_ms(100);


    InitStepper();

    wdt_enable(WDTO_60MS);
    for (;;) {
        wdt_reset();
        
        
        //Message(M8);
        //_delay_ms(100);
        
        
        
        if ((cptr = fgets(message, sizeof message - 1, stdin))) {
            
            //Message(M8);
            //_delay_ms(100);
            
            //putstring(cptr);
            //store converted to int8 value of address
            if (!strtoint8(&received_address, cptr, 2))
                goto message_exit;

            //cut address
            cptr = cptr + 2;

            
            
            
            //check if node is addressed
            if (received_address != NODE_ADDRESS)
                goto message_exit;

            
            cmd = get_cmd_id(cptr);

            switch (cmd) {
                case VERSION:
                    putstring_P(PSTR("1.0\r"));
                    break;


                case HELP:

                    i = 0;

                    do {
                        cptr = get_cmd_name(i++);
                        if (cptr) {
                            putstring(cptr);
                            wdt_reset();
                            _delay_ms(10);
                            putstring_P(PSTR("\r"));
                        }
                    } while (cptr);

                    break;
                    
                case CONFIGURATION:

//                    //TODO revision of this functionality
//                    puts_P(PSTR("NAD: "));
//
//                    while (!(cptr = fgets(message, sizeof message - 1,
//                            stdin))) {
//                        wdt_reset();
//                    }
//
//                    /*
//                     * Validate string length
//                     * "AAaa\n" 4 data bytes + 1 line terminator
//                     * AA - current node address, aa - new Node address
//                     */
//                    if (strlen(cptr) == 5)
//                        if (strtoint8(&received_address, cptr + 2, 2)) {
//                            eeprom_write_byte(&address, received_address);
//                            puts(cptr);
//
//                            while (1)
//                                ; //reset device
//                        } else
//                            puts(cptr);
//                    else
//                        puts_P(PSTR("Elen")); //error in string length

                    
                    pch = strstr(message, "=");
                    if (pch != NULL) {
                       // pch = strstr(pch, "=");
                        ch = strtoint8(&received_address, pch+1 , 2);
                        if (ch > 0) {
                            eeprom_write_byte(&address, received_address);
                            puts(pch+1);
                            while(1);
                            break;
                        } else {
                            Message(ERROR);
                            break;
                        }
                    }
                    break;

                case SET_DEC_SLOW:
                    Decay(SLOW_DECAY);
                    Message(SDEC);
                    break;

                case SET_DEC_FAST:
                    Decay(FAST_DECAY);
                    Message(FDEC);
                    break;

                case SET_DEC_AUTO:
                    Decay(AUTO_DECAY);
                    Message(ADEC);
                    break;

                case SET_MODE_FULL:
                    Mode(MODE_FULL_STEP);
                    Message(FSTEP);
                    break;

                case SET_MODE_HALF:
                    Mode(MODE_HALF_STEP);
                    Message(HSTEP);
                    break;

                case SET_MODE_QUATER:
                    Mode(MODE_QUATER_STEP);
                    Message(QSTEP);
                    break;

                case SET_MODE_8:
                    Mode(MODE_8_MICROSTEP);
                    Message(M8);
                    break;

                case SET_MODE_16:
                    Mode(MODE_16_MICROSTEP);
                    Message(M16);
                    break;

                case SET_MODE_32:
                    Mode(MODE_32_MICROSTEP);
                    Message(M32);
                    break;

                

                case SET_ACC:
                    pch = strstr(message, "=");
                    if (pch != NULL) {
                        pch = strstr(pch, "=");
                        ch = ConvertASCItouint64(pch + 1);
                        if (ch > 0) {
                            SetAcceleration(ch);
                            Message_xcall(ACC, GetAcceleration);
                            break;
                        } else {
                            Message(ERROR);
                            break;
                        }
                    }
                    break;


                case SET_MAX_SPEED:
                    pch = strstr(message, "=");
                    if (pch != NULL) {
                        pch = strstr(pch, "=");
                        ch = ConvertASCItouint64(pch + 1);
                        if (ch > 0) {
                            SetMaxSpeed(ch);
                            Message_xcall(MAXSPD, GetMaxSpeed);
                            break;
                        } else {
                            Message(ERROR);
                            break;
                        }
                    }
                    break;


                

                case SET_R_MAX_SPD:

                    pch = strstr(message, "=");
                    if (pch != NULL) {
                        pch = strstr(pch, "=");
                        ch = ConvertASCItouint64(pch + 1);
                        if (ch > 0) {


                            SetRealMaxSpeed(ch);
                            Message_xcall(RMAXSPD, GetRealMaxSpeed);
                            break;
                        } else {
                            Message(ERROR);
                            break;
                        }
                    }

                    break;


                case SET_R_ACC:

                    pch = strstr(message, "=");
                    if (pch != NULL) {
                        pch = strstr(pch, "=");
                        ch = ConvertASCItouint64(pch + 1);
                        if (ch > 0) {
                            SetRealAcc(ch);
                            Message_xcall(RACC, GetRealAcc);
                            break;
                        } else {
                            Message(ERROR);
                            break;
                        }
                    }

                    break;


                case GET_ACC:
                    Message_xcall(ACC, GetAcceleration);
                    break;

                case GET_MAX_SPEED:
                    Message_xcall(MAXSPD, GetMaxSpeed);
                    break;



                case GET_R_MAX_SPD:
                    Message_xcall(RMAXSPD, GetRealMaxSpeed);
                    break;

                case GET_R_ACC:
                    Message_xcall(RACC, GetRealAcc);
                    break;



                case STOPF:
                    Stop_Motion_fast();
                    Message(STOP);
                    break;

                case STOPN:
                    Stop_Motion_normal();
                    Message(STOP);
                    break;

                case CONSTSPD:
                    pch = strstr(message, "clock");
                    if (pch != NULL) {
                        Way_Speed(STEP_CLOCKWISE);
                        putstring_P(PSTR("DONE\r "));
                        break;

                    }

                    pch = strstr(message, "against");
                    if (pch != NULL) {

                        Way_Speed(STEP_COUNTER_CLOCKWISE);
                        putstring_P(PSTR("DONE\r"));
                        break;
                    }

                    Message(ERROR);
                    break;


                case ENABLE:
                    Enable_Stepper();
                    Message(ENBL);
                    break;

                case DISABLE:
                    Disabled_Stepper();
                    Message(DISBL);
                    break;





                case GOTO:
                    pch = strstr(message, "clock");
                    if (pch != NULL) {
                        pch = strstr(pch, " ");
                        ch = ConvertASCItouint64(pch);

                        if (ch > 0) {
                            Count_Step(STEP_CLOCKWISE, ch);
                            //Message(DONE);
                            //putstring(int_to_string(ch));
                            //Message(STEPMSG);
                            break;
                        } else {
                            Message(ERROR);
                            break;
                        }
                    }

                    pch = strstr(message, "against");
                    if (pch != NULL) {
                        pch = strstr(pch, " ");
                        ch = ConvertASCItouint64(pch);
                        if (ch > 0) {
                            Count_Step(STEP_COUNTER_CLOCKWISE, ch);
                            Message(DONE);
                            putstring(int_to_string(ch));
                            Message(STEPMSG);
                            break;
                        } else {
                            Message(ERROR);
                            break;
                        }
                    }
                    break;

                case STATUS:
                    switch (Get_Decay()) {
                        case SLOW_DECAY:
                            Message(SDEC);
                            break;

                        case FAST_DECAY:
                            Message(FDEC);
                            break;

                        case AUTO_DECAY:
                            Message(ADEC);
                            break;

                        default:
                        {
                            Message(NSD);
                            break;
                        }
                    }

                    switch (Get_Mode()) {
                        case MODE_FULL_STEP:
                            Message(FSTEP);
                            break;

                        case MODE_HALF_STEP:
                            Message(HSTEP);
                            break;

                        case MODE_QUATER_STEP:
                            Message(QSTEP);
                            break;

                        case MODE_8_MICROSTEP:
                            Message(M8);
                            break;

                        case MODE_16_MICROSTEP:
                            Message(M16);
                            break;

                        case MODE_32_MICROSTEP:
                            Message(M32);
                            break;

                        default:
                        {
                            Message(NSM);
                            break;
                        }

                    }








                default:

                    if ((cmd < 0) && (!((message[0] == 0x0d) || (message[0] == 0x0a))))
                        putstring_P(PSTR("Comand not implement\r"));
                    break;

            } //switch (cmd)
        } //if (USART_RxEOL > 0)

message_exit:
        ;


    } //for (;;)

    return (0);
} //End int main(void)

/**
 * Initial initialisation of input and output of microcontroller
 * @return void 
 */
void Init_Input_Output(void) {
    //input init

    set_as_input(DRXD, RXD);
    set_as_input(DHOME, HOME);
    set_as_input(DSCL, SCL);
    set_as_input(DMISO, MISO);
    set_as_input(DFAULT, FAULT);

    set_bit(PRXD, RXD);
    set_bit(PSCL, SCL);
    set_bit(PMISO, MISO);
    set_bit(PHOME, HOME);

    //output init

    set_as_output(DENABLE_STEPPER, ENABLE_STEPPER);
    set_as_output(DTXD, TXD);
    set_as_output(DRESET_DRV, RESET_DRV);
    set_as_output(DMODE0, MODE0);
    set_as_output(DMODE1, MODE1);
    set_as_output(DMODE2, MODE2);
    set_as_input(DDECAY, DECAY);
    set_as_output(DSTEP, STEP);
    set_as_output(DMOSI, MOSI);
    set_as_output(DDIRECTION, DIRECTION);
    set_as_output(DDIR_485, DIR_485);

    set_bit(PENABLE_STEPPER, ENABLE_STEPPER);
    set_bit(PRESET_DRV, RESET_DRV);
    clr_bit(PMODE0, MODE0);
    clr_bit(PMODE1, MODE1);
    clr_bit(PMODE2, MODE2);
    clr_bit(PDECAY, DECAY);
    clr_bit(PDIRECTION, DIRECTION);
    set_bit(PDIR_485, DIR_485);

}

/**
 * Just for testing
 * @return void 
 */


