/*
 * config.h
 *
 *  Created on: 31.07.2010
 *      Author: Biba
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define set_bit(byte, bit) byte |= (1 << bit)
#define clr_bit(byte, bit) byte &= ~(1 << bit)
#define check_bit(byte, bit)	(byte & (1 << bit)) != 0 ? 1:0

/** \def set_as_output
 * Write logical one bit \c DDxn in IO register \c DDRx.
 * This will configure Pxn as an output pin. */
#define set_as_output(DDRx, DDxn) DDRx |= (1 << DDxn)

/** \def set_as_input
 * Write logical zero bit \c DDxn in IO register \c DDRx.
 * This will configure Pxn as an input pin. */
#define set_as_input(DDRx, DDxn) DDRx &= ~(1 << DDxn)

/** \def set_port
 * Write logic one bit \c PORTxn in IO register \c PORTx.
 * When the pin is configured as an output pin,
 * the port pin is driven high (one).*/
#define set_port(PORTx, PORTxn) PORTx |= (1 << PORTxn)

/** \def clr_port
 * Write logic zero bit \c PORTxn in IO register \c PORTx.
 * When the pin is configured as an output pin,
 * the port pin is driven low (zero).*/
#define clr_port(PORTx, PORTxn) PORTx &= ~(1 << PORTxn)

/** \def check_pin
 * Test whether bit \c PINxn in IO register \c PINx is set.
 * This will return a 0 if the port pin is driven low, and non-zero
 * if the pin is driven high. */
#define check_pin(PINx, PINxn) (PINx & (1 << PINxn))

#define CHECKPIN(x,y)	((x&(1<<y))!=0?1:0)


#define TRUE 1==1
#define FALSE 1!=1

//Head status
//#define STATE_NOT_READY         0
//#define STATE_READY             1
//#define STATE_MOVING_HEAD_DOWN  2
//#define STATE_MOVING_HEAD_UP    3
//#define STATE_WAIT_RED_BUTTON   4
//#define STATE_MOVE_HEAD_UP	5
//#define STATE_HEAD_DOWN         6
//#define STATE_WAIT_RED_BUTTON1  7
//#define STATE_WAIT_RED_BIN      8
//#define STATE_WAIT_RED_BIN1     9
//#define STATE_UNKNOWN           10

//Inputs
#define BUTTON1               PB0
#define BUTTON2               PB1
#define EMERGECY_STOP         PB4
#define HOME                  PB5
//#define BUTTON2             PB6
//#define EMERGECY_STOP       PB7
//#define LED2                PD2
//#define LED3                PD3
//#define LOCK                PD4
//#define UNLOCK              PD5
//#define PRESENS             PD6
//#define REDBIN              PD7
//#define INIT                PE2
//#define DOOR_SEN            PC1

//Outputs
#define LED1                    PA0
#define LED2                    PA1
#define LED3                    PA2
//#define NAILING               PA3
//#define MARK                  PA4
//#define LOCK                  PA5
//#define MARK                  PA6
//#define COUNTER               PA7
//#define LED_PASS              PC7
//#define LED_PROCESS           PC6
//#define LED_FAIL              PC5
//#define DOOR                  PC4
//#define HIGH_PRESSURE         PC3


//PORT input defines
#define PBUTTON1               PORTB
#define PBUTTON2               PORTB
#define PEMERGECY_STOP         PORTB
#define PHOME                  PORTB
//#define PBUTTON2             PORTB
//#define PEMERGECY_STOP       PORTB
//#define PLED2                PORTD
//#define PLED3                PORTD
//#define PLOCK                PORTD
//#define PUNLOCK              PORTD
//#define PPRESENS             PORTD
//#define PREDBIN              PORTD
//#define PINIT                PORTE
//#define PDOOR_SEN            PORTC

        
//PORT output defines
#define PLED1                  PORTA
#define PLED2                  PORTA
#define PLED3                  PORTA
//#define PNAILING             PORTA
//#define PMARK                PORTA
//#define PLOCK                PORTA
//#define MARK                 PA6
//#define PCOUNTER             PORTA
//#define PLED_PASS            PORTC
//#define PLED_PROCESS         PORTC
//#define PLED_FAIL            PORTC
//#define PDOOR                PORTC
//#define PHIGH_PRESSURE       PORTC

//DIRECTION input defines
#define DBUTTON1               DDRB
#define DBUTTON2               DDRB
#define DEMERGECY_STOP         DDRB
#define DHOME                  DDRB
//#define DBUTTON2             DDRB
//#define DEMERGECY_STOP       DDRB
//#define DLED2                DDRD
//#define DLED3                DDRD
//#define DLOCK                DDRD
//#define DUNLOCK              DDRD
//#define DPRESENS             DDRD
//#define DREDBIN              DDRD
//#define DINIT                DDRE
//#define DDOOR_SEN            DDRC

//DIRECTIN output defines 
        
#define DLED1                  DDRA
#define DLED2                  DDRA
#define DLED3                  DDRA
//#define DNAILING             DDRA
//#define DMARK                DDRA
//#define DLOCK                DDRA
//#define MARK                 PA6
//#define DCOUNTER             DDRA
//#define DLED_PASS            DDRC
//#define DLED_PROCESS         DDRC
//#define DLED_FAIL            DDRC
//#define DDOOR                DDRC
//#define DHIGH_PRESSURE       DDRC

//INPUT input defines
#define IBUTTON1               PINB
#define IBUTTON2               PINB
#define IEMERGECY_STOP         PINB
#define IHOME                  PINB
//#define IBUTTON2             PINB
//#define IEMERGECY_STOP       PINB
//#define ILED2                PIND
//#define ILED3                PIND
//#define ILOCK                PIND
//#define IUNLOCK              PIND
//#define IPRESENS             PIND
//#define IREDBIN              PIND
//#define IINIT                PINE
//#define IDOOR_SEN            PINC
        

//INPUT output defines
#define ILED1                PINA
#define ILED2                PINA
#define ILED3                PINA
/*
#define INAILING             PINA
#define IMARK                PINA
#define ILOCK                PINA
//#define MARK               PA6
#define ICOUNTER             PINA
#define ILED_PASS            PINC
#define ILED_PROCESS         PINC
#define ILED_FAIL            PINC
//#define IDOOR              PINC
//#define IHIGH_PRESSURE     PINC
*/
#define ADD_SERVO
#define ADD_STEPPER



#endif /* CONFIG_H_ */
