/*
 * interpreter.h
 *
 *  Created on: 28.07.2010
 *      Author: Biba
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

enum {
    VERSION,
    HELP,
    CONFIGURATION,
    SET_DEC_SLOW,
    SET_DEC_FAST,
    SET_DEC_AUTO,
    SET_MODE_FULL,
    SET_MODE_HALF,
    SET_MODE_QUATER,
    SET_MODE_8,
    SET_MODE_16,
    SET_MODE_32,
    SET_ACC,
    SET_MAX_SPEED,
    SET_R_MAX_SPD,
    SET_R_ACC,
    GET_ACC,
    GET_MAX_SPEED,
    GET_R_MAX_SPD,
    GET_R_ACC,
    ENABLE,
    DISABLE,
    GOTO,
    STOPN,
    STOPF,
    CONSTSPD,
    STATUS
  
};



char get_cmd_id(char *);
const char *get_cmd_name(char);


#endif /* INTERPRETER_H_ */
