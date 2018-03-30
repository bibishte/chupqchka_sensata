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
    CHECK_STEPS,
    CHECK_ACCELERATION,
    CHECK_MAX_SPEED,
    CHECK_DELAY,
    GET_STATE,
    LED_ON,
    LED_OFF,
    DISABLE,
    GOTO1,
    AUTO,
    AUTO1,
    HOME1,
    HOME_CLOCK,
    HOME_AGAINST
};



signed char get_cmd_id(const char *);
char const *get_cmd_name(signed char);
char validate_string(char *);

#endif /* INTERPRETER_H_ */
