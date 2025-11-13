/* 
 * File:   types.h
 * Author: Dmitry Orlov
 *
 * Created on August 29, 2014, 2:53 PM
 */

#ifndef TYPES_H
#define	TYPES_H

#include <stdbool.h>
#include <stdint.h>

//#define persistent
//#define _persistent_
#define _persistent_ __attribute__ ((persistent))

/* types definition */
//typedef enum _BOOL { FALSE = 0, TRUE } BOOL;
//typedef enum _BIT { CLEAR = 0, SET } BIT;
//typedef enum _bit { clr = 0, set } bit;

//typedef bit BOOL;
typedef BIT bit;

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;
typedef union {u16 w; u8 b[2];} u16u;
typedef union {u32 dw; u8 b[4]; u16 w[2];} u32u;

enum order {Lo, Hi, HiLo, HiHi}; /* Byte ordering (LSB) */

#define max_u32     0xffffffffL
#define min_u32     0L
#define max_i32     0x7fffffffL
#define min_i32     (-max_i32-1)

#ifdef __PIC32MX__
typedef u32 idx8;  //use u8 for 8 bit CPU
typedef u32 idx16; //use u16 for 8 bit CPU
#endif /*__PIC32MX__*/
#ifdef _PIC18
typedef u8 idx8;  //use u8 for 8 bit CPU
typedef u16 idx16; //use u16 for 8 bit CPU
#endif /*_PIC18*/
#ifdef _PIC14
typedef u8 idx8;  //use u8 for 8 bit CPU
typedef u16 idx16; //use u16 for 8 bit CPU
#endif /*_PIC14*/

#endif	/* TYPES_H */

