/*
 * interpreter.c
 *
 *  Created on: 28.07.2010
 *      Author: Biba
 */

//TODO Konstante Strings ins Flasch.
//http://www.rn-wissen.de/index.php/Avr-gcc#Optimierungen.2C_Tipps_.26_Tricks
#include <string.h>
#include "interpreter.h"

struct cmd_struct
{
	signed char id;
	char const *name;
} cmd_tbl[] = {
	{ VERSION, "version?\n" },
	{ HELP, "help?\n" },
        { CHECK_STEPS, "steps?\n"},
        { CHECK_ACCELERATION, "acc?\n"},
        { CHECK_MAX_SPEED, "maxspd?\n"},
        { CHECK_DELAY, "delay?\n"},
        { GET_STATE, "s\n"},
        { LED_ON, "ledon\n"},
        { LED_OFF, "ledoff\n"},
        { DISABLE, "d\n"},
        { GOTO1, "g1\n"},
        { AUTO, "c\n"},
        { AUTO1, "f\n"},
        { HOME1, "h\n"},
        { HOME_CLOCK, "hc\n"},
        { HOME_AGAINST, "ha\n"},
	{ -1, NULL }
};

/**
 *
 * @param name
 * @return
 */
signed char get_cmd_id( const char *name )
{
	unsigned char i = 0;

	if ( *name == 0 )
		return -2;

	for ( i = 0; cmd_tbl[i].id != -1; i++ )
		if ( !strcmp( name, cmd_tbl[i].name ) )
			return cmd_tbl[i].id;

	return -1;
}

/**
 *
 * @param id
 * @return command name or NULL if not found
 */
char const * get_cmd_name( signed char id )
{
	unsigned char i;

	for ( i = 0; cmd_tbl[i].id != -1; i++ )
		if ( id == cmd_tbl[i].id )
			return cmd_tbl[i].name;

	return NULL;
}

/**
 *
 * @param name
 * @return
 */
char validate_string( char *name )
{
	unsigned char i = 0;

	for ( i = 1; i < 9; i++ )
	{
		if ( ( name[i] < '0' ) || ( name[i] > 'g' ) )
		{
			return 0;
		}
	}

	return 1;
}
