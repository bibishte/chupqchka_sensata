#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "interpreter.h"
#include "utils.h"
#include "config.h"
#include "version.h"
#include "state_machine.h"
//#include "state_machine.h"
/*
 * Define CPU frequency in MHz here if not defined in Makefile
 */
#ifndef F_CPU
#define F_CPU 11059200UL
#endif
/* ****** BEGIN OF CONFIGURATION SECTION ****** */
//Set usarts baud rate here
#define USART_BAUD_RATE 115200
#define USART1_BAUD_RATE 115200
//time in n * 10ms
#define TIME_TO_MARK 50
#define SERIAL_ID 10
/* ****** END OF CONFIGURATION SECTION ****** */
//


FILE usart_str =
        FDEV_SETUP_STREAM(usart_putchar, usart_getchar, _FDEV_SETUP_RW);
FILE usart1_str = FDEV_SETUP_STREAM(usart1_putchar, usart1_getchar,
        _FDEV_SETUP_RW);






//+++ EEPROM datas +++
//store marker switch on time
//uint16_t mark __attribute__( (section( ".eeprom" )) ) = TIME_TO_MARK;
//uint8_t serial __attribute__( (section( ".eeprom" )) ) = SERIAL_ID;

//declaration of static functions
static void Init_Input_Output(void);
void InitTimer1(void);

/**
 *
 * @return
 */
int main(void) //Main Loop
{
    char message[34]; //1 byte + for string termination
    signed char cmd;
    const char *cptr;
    char *nptr;
    uint16_t tmp16;
    uint32_t tmp32;
    uint8_t i = 0;

    //    uint16_t tmp16;

    /*
     * Set ports data direction
     */
    Init_Input_Output();

    usart_init(USART_BAUD_SELECT(USART_BAUD_RATE, F_CPU));
    usart1_init(USART_BAUD_SELECT(USART1_BAUD_RATE, F_CPU));

    /*
     * now enable interrupt, since USART library is interrupt controlled
     */

    sei();




    //Read and set node_address variable from the EEPROM
    //mark_time = eeprom_read_word( &mark );
    //serial_id = eeprom_read_byte( &serial );

    stdout = stdin = &usart_str;


    Eeprom_restore();

    Init();

    for (;;)
    {
        Auto_cycle();
        Auto_state();

        //printf("helloooo");
        //_delay_ms(100);
        //printf("Butt=%d\n",Check_button());
        //_delay_ms(100);
        //        if(Check_button()==SIMPLE_BUTTON_PRESSED)
        //        {
        //            
        //                if(Enable_motor()==SIMPLE_TURNING)
        //                {
        //                   /* _delay_ms(10000);
        //                    if(Disable_motor()==SIMPLE_TURNING)
        //                    {
        //                        printf("success");
        //                    }
        //                    else
        //                    {
        //                        printf("error1");
        //                    }*/
        //                    
        //                    
        //                    if(Goto(DIR_CLOCK)==SIMPLE_DONE)
        //                    {
        //                        if(Disable_motor()==SIMPLE_TURNING)
        //                        {
        //                            printf("success");
        //                        }
        //                        else
        //                        {
        //                            printf("error1");
        //                        }
        //                    }
        //                    else
        //                    {
        //                        printf("error4");
        //                    }
        //                }
        //                else
        //                {
        //                    printf("error2");
        //                }
        //                
        //                printf("error3");
        //            }


        //       Auto_cycle();

         /*if ( (cptr = fgets(message, sizeof message - 1, &usart1_str)) )
         {
             fputs(cptr, stdout);

         }*/

        if ((cptr = fgets(message, sizeof message - 1, stdin)))
        {


            //fprintf_P( &usart1_str, PSTR( "\rf1hlp?\r" ) );
            if (!strncmp(cptr, "steps=", 6))
            {
                cptr = cptr + 6;


                tmp32= strtol(cptr, &nptr, 10);
                //if ( tmp16 < 0 || tmp16 > 100 )
                if (tmp32 < 0)
                {
                    puts_P(PSTR("Wrong value\r"));
                    goto message_exit;
                }
                Set_steps(tmp32);
                Eeprom_store();
                printf("OK\r");

            }


            if (!strncmp(cptr, "acc=", 4))
            {
                cptr = cptr + 4;


                tmp16 = strtol(cptr, &nptr, 10);
                //if ( tmp16 < 0 || tmp16 > 100 )
                if (tmp16 < 0)
                {
                    puts_P(PSTR("Wrong value\r"));
                    goto message_exit;
                }
                Set_acceleration(tmp16);
                Eeprom_store();
                printf("OK\r");

            }


            if (!strncmp(cptr, "maxspd=", 7))
            {
                cptr = cptr + 7;


                tmp16 = strtol(cptr, &nptr, 10);
                //if ( tmp16 < 0 || tmp16 > 100 )
                if (tmp16 < 0)
                {
                    puts_P(PSTR("Wrong value\r"));
                    goto message_exit;
                }
                Set_max_speed(tmp16);
                Eeprom_store();
                printf("OK\r");

            }
            
            
            if (!strncmp(cptr, "delay=", 6))
            {
                cptr = cptr + 6;


                tmp16 = strtol(cptr, &nptr, 10);
                //if ( tmp16 < 0 || tmp16 > 100 )
                if (tmp16 < 0)
                {
                    puts_P(PSTR("Wrong value\r"));
                    goto message_exit;
                }
                Set_delay(tmp16);
                Eeprom_store();
                printf("OK\r");

            }

            /* if (!strncmp(cptr, "dir=", 7))
             {
                 cptr = cptr + 4;


                 tmp16 = strtol(cptr, &nptr, 10);
                 //if ( tmp16 < 0 || tmp16 > 100 )
                 if (tmp16 < 0)
                 {
                     puts_P(PSTR("Wrong value\r"));
                     goto message_exit;
                 }
                 //Set_max_speed(tmp16);
                 Eeprom_store();
                 printf("OK\r");

             }*/

            if (*cptr == 'B') //RS485 Remote command
            {
                cptr++;
                if (*cptr == '\n')
                    goto message_exit;

                fputs(cptr, &usart1_str);
                fputs(cptr, &usart_str);


            }




            //           else //Local commands
            //            {
            cmd = get_cmd_id(cptr);

            switch (cmd)
            {

                case VERSION:
                    puts_P(PSTR(VERSION_STRING));
                    break;


                case HELP:
                    do
                    {
                        cptr = get_cmd_name(i++);
                        if (cptr != NULL)
                        {
                            fputs(cptr, stdout);
                        }
                    } while (cptr);

                    puts_P(PSTR("Baa to bridge on RS485"));
                    puts_P(PSTR("steps="));
                    puts_P(PSTR("acc="));
                    puts_P(PSTR("maxspd="));
                    break;



                case CHECK_STEPS:
                    printf("%lu\n", Get_steps());
                    break;

                case CHECK_ACCELERATION:
                    printf("%d\n", Get_acceleration());
                    break;

                case CHECK_MAX_SPEED:
                    printf("%d\n", Get_max_speed());
                    break;
                    
                case CHECK_DELAY:
                    printf("%d\n", Get_delay());
                    break;

                case GET_STATE:
                    Get_state();
                    break;

                case LED_ON:
                    set_bit(PLED1, LED1);
                    break;


                case LED_OFF:
                    clr_bit(PLED1, LED1);

                    break;

                
                    
                case AUTO:
                    Auto_state();
                    //Enable_motor(STATE_ENABLE);
                    break;
                    
                case AUTO1:
                    Enable_motor1(STATE_ENABLE);
                    break;
                    
                    
                case HOME_AGAINST:
                    Home_against_test();
                    printf("against");
                    break;
                    
                case HOME_CLOCK:
                    Home_clock();;
                    printf("clock");
                    break;
            }
            //           }

        } //if (fgets(message, sizeof message - 1, stdin))

message_exit:
        wdt_reset();

        /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ *
         *						CYLINDER MOVEMENT						  *
         * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

        //--------------- BEGIN START BUTTONS SECTION ---------------//



    } //for (;;)

    return 0;
} //End int main(void)

/**
 *
 */
static void Init_Input_Output(void)
{

    SFIOR &= 0xfb; //11111011 -> PUD=0 enable pull-up

    //Inputs
    //set_as_input(DSTART1, START1);
    //set_bit(PSTART1, START1);


    set_as_input(DBUTTON1, BUTTON1);
    set_bit(PBUTTON1, BUTTON1);
    
    set_as_input(DBUTTON2, BUTTON2);
    set_bit(PBUTTON2, BUTTON2);
    
    set_as_input(DHOME, HOME);
    set_bit(PHOME, HOME);
    
    set_as_input(DEMERGECY_STOP, EMERGECY_STOP);
    set_bit(PEMERGECY_STOP, EMERGECY_STOP);


    //output
    set_as_output(DLED1, LED1);
    clr_bit(PLED1, LED1);
    //set_bit(PLED1,LED1);


    set_as_output(DLED2, LED2);
    clr_bit(PLED2, LED2);

    
    set_as_output(DLED3, LED3);
    clr_bit(PLED3, LED3);


}

