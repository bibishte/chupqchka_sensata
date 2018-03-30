/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   state_machine.h
 * Author: biba
 *
 * Created on 25 Ноември 2017, 09:12
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "config.h"

#define TIMEOUT_BOTH 50
#define DELAY_BUTTON_MS 10

typedef enum {
    SIMPLE_BUTTON_NOT_PRESSED,
    SIMPLE_BUTTON_PRESSED,
    SIMPLE_RIGHT_BUTTON,
    SIMPLE_LEFT_BUTTON,
    SIMPLE_HOMED,
    SIMPLE_NOT_HOMED
} simple_action;

typedef enum {
    DIR_CLOCK,
    DIR_AGAINST
} dir_type;

typedef enum {
    STATE_MOTOR_READY,
    STATE_ENABLE,
    STATE_DISABLE,
    STATE_ENABLE_ANSWER,
    STATE_DISABLE_ANSWER,
    STATE_GOTO,
    STATE_GOTO_AGAINST,
    STATE_GOTO_ANSWER,
    STATE_CON_SPD,
    STATE_CONSPD_ANSWER,
    STATE_STOP,
    STATE_STOP_ANSWER
} motor_states;

typedef enum {
    MACHINE_DONE,
    MACHINE_MOTOR_ENABLE,
    MACHINE_HOME,
    MACHINE_HOMED,
    MACHINE_GOTO,
    MACHINE_GOTO_1,
    MACHINE_DISABLE
} machine_state;


simple_action Check_button(void);
simple_action Check_home(void);
void Set_steps(uint32_t a);
uint32_t Get_steps(void);
void Set_acceleration(uint16_t a);
uint16_t Get_acceleration(void);
void Set_max_speed(uint16_t a);
uint16_t Get_max_speed(void);
void Set_delay(uint16_t a);
uint16_t Get_delay(void);
void Eeprom_store(void);
void Eeprom_restore(void);
void Auto_cycle(void);
void Get_state(void);
void Auto_state(void);
void Enable_motor1(motor_states a);
void Home_against_test(void);
void Home_clock(void);
void Init(void);



#endif /* STATE_MACHINE_H */

