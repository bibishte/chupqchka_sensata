/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   state_machine.c
 * Author: biba
 *
 * Created on 25 Ноември 2017, 09:12
 */

#include <config.h>
#include <interpreter.h>
#include <utils.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>

#include "state_machine.h"



#define STEPPER_ADDR 0xf1

/*
 * 
 */

typedef struct Parameters
{
    uint32_t Steps;
    uint16_t MaxSpeed;
    uint16_t Acceleration;
    uint8_t Direction;
    uint16_t delay_before_cycles;
} param;

static volatile uint16_t Timeout_button;
static struct Parameters parameters;
static volatile machine_state State_machine;
static volatile motor_states motorStateMachine;
static volatile simple_action Error_mess;
extern FILE usart_str;
extern FILE usart1_str;

simple_action Check_button(void)
{
    if ((!(CHECKPIN(IBUTTON1, BUTTON1))) &&
            (!(CHECKPIN(IBUTTON2, BUTTON2))) &&
            (Timeout_button < TIMEOUT_BOTH))
    {
        Timeout_button = 0;
        return SIMPLE_BUTTON_PRESSED;
    }


    if ((CHECKPIN(IBUTTON1, BUTTON1)) &&
            (CHECKPIN(IBUTTON2, BUTTON2)))
    {
        Timeout_button = 0;
        return SIMPLE_BUTTON_NOT_PRESSED;
    }

    if ((!(CHECKPIN(IBUTTON1, BUTTON1))) &&
            (CHECKPIN(IBUTTON2, BUTTON2)))
    {
        Timeout_button++;
        if (Timeout_button > TIMEOUT_BOTH)
            Timeout_button = TIMEOUT_BOTH + 1;
        _delay_ms(DELAY_BUTTON_MS);
        return SIMPLE_LEFT_BUTTON;
    }

    if ((CHECKPIN(IBUTTON1, BUTTON1)) &&
            (!(CHECKPIN(IBUTTON2, BUTTON2))))
    {
        Timeout_button++;
        if (Timeout_button > TIMEOUT_BOTH)
            Timeout_button = TIMEOUT_BOTH + 1;
        _delay_ms(DELAY_BUTTON_MS);
        return SIMPLE_RIGHT_BUTTON;
    }



    return SIMPLE_BUTTON_NOT_PRESSED;
}

simple_action Check_home(void)
{
    if (CHECKPIN(IHOME, HOME))
    {
        return SIMPLE_HOMED;
    } else
    {
        return SIMPLE_NOT_HOMED;
    }





}

void Set_steps(uint32_t a)
{
    parameters.Steps = a;
}

uint32_t Get_steps(void)
{
    return parameters.Steps;
}

void Set_acceleration(uint16_t a)
{
    parameters.Acceleration = a;
    fprintf_P(&usart1_str, PSTR("\r%02xracc=%u\r"), STEPPER_ADDR, parameters.Acceleration);



}

uint16_t Get_acceleration(void)
{
    return parameters.Acceleration;
}

void Set_max_speed(uint16_t a)
{
    parameters.MaxSpeed = a;
    fprintf_P(&usart1_str, PSTR("\r%02xrmaxspd=%u\r"), STEPPER_ADDR, parameters.MaxSpeed);
}

uint16_t Get_max_speed(void)
{
    return parameters.MaxSpeed;
}

void Set_delay(uint16_t a)
{
    parameters.delay_before_cycles = a;
}

uint16_t Get_delay(void)
{
    return parameters.delay_before_cycles;
}

void Eeprom_store(void)
{
    eeprom_write_dword((uint32_t *) (0), parameters.Steps);
    eeprom_write_word((uint16_t *) (4), parameters.Acceleration);
    eeprom_write_word((uint16_t *) (6), parameters.MaxSpeed);
    eeprom_write_word((uint16_t *) (8), parameters.delay_before_cycles);
    //eeprom_write_word( (uint16_t *) (8), parameters.HV1_up );
    //eeprom_write_word( (uint16_t *) (10), parameters.HV1_low );
    //eeprom_write_byte( (uint8_t *) (12), parameters.First_turns );
    //eeprom_write_byte( (uint8_t *) (13), parameters.Second_turns );
    //eeprom_write_byte( (uint8_t *) (14), parameters.First_speed );
    //eeprom_write_byte( (uint8_t *) (15), parameters.Second_speed );

}

void Eeprom_restore(void)
{
    parameters.Steps = eeprom_read_dword((uint32_t *) (0));
    parameters.Acceleration = eeprom_read_word((uint16_t *) (4));
    parameters.MaxSpeed = eeprom_read_word((uint16_t *) (6));
    parameters.delay_before_cycles = eeprom_read_word((uint16_t *) (8));
}

void Auto_cycle()
{
    char mess[20];
    if(CHECKPIN(IEMERGECY_STOP, EMERGECY_STOP))
    {
        fprintf_P(&usart1_str, PSTR("\r%02xstopn\r"), STEPPER_ADDR);
        _delay_ms(50);
        fgets(mess, sizeof mess - 1, &usart1_str);
        fprintf_P(&usart1_str, PSTR("\r%02xdisbl\r"), STEPPER_ADDR);
        _delay_ms(50);
        fgets(mess, sizeof mess - 1, &usart1_str);
        motorStateMachine = STATE_MOTOR_READY;
        State_machine = MACHINE_DONE;
        return;
        
    }
    
    
    switch (State_machine)
    {
        case MACHINE_DONE:
            //if (Check_button() == SIMPLE_BUTTON_PRESSED)
            set_bit(PLED1, LED1);
            clr_bit(PLED2, LED2);
            clr_bit(PLED3, LED3);
            if (Check_button() == SIMPLE_BUTTON_PRESSED)
            {
                //set_bit(PLED3, LED3);
                //clr_bit(PLED1, LED1);
                //clr_bit(PLED2, LED2);
                motorStateMachine = STATE_ENABLE;
                State_machine = MACHINE_MOTOR_ENABLE;
            }

            break;

        case MACHINE_MOTOR_ENABLE:
            set_bit(PLED3, LED3);
            clr_bit(PLED1, LED1);
            clr_bit(PLED2, LED2);
            if (motorStateMachine == STATE_MOTOR_READY)
            {
                if (Check_home() != SIMPLE_HOMED)
                {
                    motorStateMachine = STATE_CON_SPD;
                    State_machine = MACHINE_HOME;
                } else
                {
                    State_machine = MACHINE_HOMED;
                }
            }

            break;

        case MACHINE_HOME:
            if (motorStateMachine == STATE_MOTOR_READY)
            {
                if ((Check_home() == SIMPLE_HOMED))
                {
                    motorStateMachine = STATE_STOP;
                    State_machine = MACHINE_HOMED;
                }
            }
            break;

        case MACHINE_HOMED:
            if (motorStateMachine == STATE_MOTOR_READY)
            {
                motorStateMachine = STATE_GOTO_AGAINST;
                State_machine = MACHINE_GOTO;
            }

            break;

        case MACHINE_GOTO:
            if (motorStateMachine == STATE_MOTOR_READY)
            {
                set_bit(PLED2, LED2);

                for (int i = 0; i <= parameters.delay_before_cycles; i++)
                {
                    _delay_ms(1);
                }
                clr_bit(PLED2, LED2);
                motorStateMachine = STATE_GOTO;
                State_machine = MACHINE_GOTO_1;
            }
            break;

        case MACHINE_GOTO_1:
            if (motorStateMachine == STATE_MOTOR_READY)
            {
                motorStateMachine = STATE_DISABLE;
                State_machine = MACHINE_DISABLE;
            }
            break;

        case MACHINE_DISABLE:
            if ((motorStateMachine == STATE_MOTOR_READY) && (Check_button() == SIMPLE_BUTTON_NOT_PRESSED))
            {
                motorStateMachine = STATE_DISABLE;
                State_machine = MACHINE_DONE;
            }
            break;

    }
}

void Init(void)
{

    State_machine = MACHINE_DONE;
}

void Get_state(void)
{
    printf("machine_state= %d\n", State_machine);
    printf("motor_state= %d\n", motorStateMachine);
}

void Auto_state(void)
{
    char mess[20];
    char *ptr;
    switch (motorStateMachine)
    {
        case STATE_MOTOR_READY:

            break;

        case STATE_ENABLE:
            fprintf_P(&usart1_str, PSTR("\r%02xenbl\r"), STEPPER_ADDR);
            motorStateMachine = STATE_ENABLE_ANSWER;
            break;

        case STATE_ENABLE_ANSWER:
            if ((ptr = fgets(mess, sizeof mess - 1, &usart1_str)))
            {
                if (!strncmp(ptr, "Enable", 5))
                {
                    motorStateMachine = STATE_MOTOR_READY;
                }
            }
            break;

        case STATE_DISABLE:
            fprintf_P(&usart1_str, PSTR("\r%02xdisbl\r"), STEPPER_ADDR);
            motorStateMachine = STATE_DISABLE_ANSWER;
            break;

        case STATE_DISABLE_ANSWER:
            if ((ptr = fgets(mess, sizeof mess - 1, &usart1_str)))
            {
                if (!strncmp(ptr, "Disable", 5))
                {
                    motorStateMachine = STATE_MOTOR_READY;
                }
            }
            break;

        case STATE_GOTO_AGAINST:
            fprintf_P(&usart1_str, PSTR("\r%02xg=against %lu\r"), STEPPER_ADDR, parameters.Steps);
            motorStateMachine = STATE_GOTO_ANSWER;

            break;

        case STATE_GOTO:
            fprintf_P(&usart1_str, PSTR("\r%02xg=clock %lu\r"), STEPPER_ADDR, parameters.Steps);
            motorStateMachine = STATE_GOTO_ANSWER;
            break;

        case STATE_GOTO_ANSWER:
            if ((ptr = fgets(mess, sizeof mess - 1, &usart1_str)))
            {
                _delay_ms(20);
                printf("message is:%s\n", mess);
                if (!strncmp(ptr, "done", 4))
                {
                    motorStateMachine = STATE_MOTOR_READY;
                    //printf("2 %d\n",motorStateMachine);
                }
            }
            break;

        case STATE_CON_SPD:
            fprintf_P(&usart1_str, PSTR("\r%02xconspd=clock\r"), STEPPER_ADDR);
            motorStateMachine = STATE_CONSPD_ANSWER;
            break;


        case STATE_CONSPD_ANSWER:
            if ((ptr = fgets(mess, sizeof mess - 1, &usart1_str)))
            {
                _delay_ms(100);
                printf("message is:%s\n", mess);
                if (!strncmp(ptr, "DONE", 4))
                {
                    motorStateMachine = STATE_MOTOR_READY;
                    //printf("2 %d\n",motorStateMachine);
                }
            }

            break;


        case STATE_STOP:
            fprintf_P(&usart1_str, PSTR("\r%02xstopn\r"), STEPPER_ADDR);
            motorStateMachine = STATE_STOP_ANSWER;

            //motorStateMachine=STATE_MOTOR_READY;
            break;


        case STATE_STOP_ANSWER:
            if ((ptr = fgets(mess, sizeof mess - 1, &usart1_str)))
            {
                _delay_ms(20);
                printf("message is:%s\n", mess);
                if (!strncmp(ptr, " Stop", 4))
                {
                    motorStateMachine = STATE_MOTOR_READY;
                    //printf("2 %d\n",motorStateMachine);
                }
            }

            break;




    }
}

void Enable_motor1(motor_states a)
{
    motorStateMachine = a;
}

void Home_against_test(void)
{
    motorStateMachine = STATE_GOTO_AGAINST;
}

void Home_clock(void)
{
    motorStateMachine = STATE_GOTO;
}