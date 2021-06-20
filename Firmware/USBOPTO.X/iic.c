/*
 * File:   iic.c
 * Author: Dmitry Orlov
 *
 * Created on August 31, 2014, 3:06 PM
 */

#include <plib.h>
#include <xc.h>
#include "types.h"
#include "iic.h"

#define FORCED_SCL  /* switch SCL pin to output on the rise time */
#define WAIT     0  /* 5ms delay after write operation. Polling ACK if 0 */

//#define MACRO_FN    //Macro SDA/SCL operation

#ifdef FORCED_SCL
#undef MACRO_FN
#endif /*FORCED_SCL*/

/*RB9 SDA operations*/
#define SDA_PIN     BIT_9
#define SDA_SET     TRISBSET = SDA_PIN
#define SDA_CLR     TRISBCLR = SDA_PIN
#define IS_SDA      ((PORTB & SDA_PIN) != 0)

/*RB8 SCL operations*/
#define SCL_PIN     BIT_8
#define SCL_SET     TRISBSET = SCL_PIN
#define SCL_CLR     TRISBCLR = SCL_PIN
#define IS_SCL      ((PORTB & SCL_PIN) != 0)

static idx8 iic_1ms_timeout;
//---------------------------------------------------------------------------

// calls from the timer 1ms interrupt
void iic_timer_1ms(void)
{
 if (iic_1ms_timeout > 0) iic_1ms_timeout--;
}
//---------------------------------------------------------------------------

/* iic routines */
static void delay(void)
{
 Nop();
 Nop();
 Nop();
 Nop();
 ClearWDT();
}
//---------------------------------------------------------------------------

static void wait(void)
{
#if (WAIT > 0)
 while(iic_1ms_timeout) delay();
#endif /*WAIT*/
}
//---------------------------------------------------------------------------

static void Delay(void) //5 - is about 300kHz SCL
{
  int i;
  for(i = 0; i < 5; i++) delay();
}
//---------------------------------------------------------------------------

#ifdef MACRO_FN
/* SDA operations */
#define is_sda()    IS_SDA
#define sda_set()   SDA_SET
#define sda_clr()   SDA_CLR
/* SCL operations */
#define scl_set()   SCL_SET
#define scl_clr()   SCL_CLR
#else /*MACRO_FN*/
//---------------------------------------------------------------------------

/* SDA operations */
static BOOL is_sda()
{
  return (PORTB & SDA_PIN)?true:false;
}
//---------------------------------------------------------------------------

static void sda_set(void)
{
  TRISBSET = SDA_PIN;
}
//---------------------------------------------------------------------------

static void sda_clr(void)
{
  TRISBCLR = SDA_PIN;
}
//---------------------------------------------------------------------------

/* SCL operations */
static void scl_set(void)
{
#ifdef FORCED_SCL
  LATBSET  = SCL_PIN;
  TRISBCLR = SCL_PIN;
  TRISBSET = SCL_PIN;
  LATBCLR  = SCL_PIN;
#else  /*FORCED_SCL*/
  TRISBSET = SCL_PIN;
#endif /*FORCED_SCL*/
}
//---------------------------------------------------------------------------

static void scl_clr(void)
{
  TRISBCLR = SCL_PIN;
}
#endif /*MACRO_FN*/
//---------------------------------------------------------------------------

/* init pins */
void iic_init(void)
{
  CNPUBSET = SDA_PIN;     //sda pullup set
  CNPUBSET = SCL_PIN;     //scl pullup set
  TRISBSET = SDA_PIN;     //sda pin TRIS set (input, pin high)
  TRISBSET = SCL_PIN;     //scl pin TRIS set (input)
  LATBCLR  = SDA_PIN;     //sda LAT clear (prepare to clear pin if TRIS = 0)
  LATBCLR  = SCL_PIN;     //scl LAT clear
  Delay();
}
//---------------------------------------------------------------------------

/* I2C BUS operations */
/*
 *
 *    -------------+         +---------+         +---------+         +---------+         +---------------------
SCL                |         |         |         |         |         |         |         |
 *                 +---------+         +---------+         +---------+         +---------+
SDA   --------+       +--------- - - - - - - - - - - - - - - - - - - - - - - - - - - -       +-----------------
 *            |       |          |||||||||||||||||||||||||||||||||||||||||||||||||||||       |
 *            +-------+          - - - - - - - - - - - - - - - - - - - - - - - - - - - ------+
 *          START                                                                           STOP
 */
//---------------------------------------------------------------------------

static void iic_start(void) //start condition
{
  sda_set();     //SDA = 1
  scl_set();     //SCL = 1
  Delay();
  sda_clr();     //SDA = 0
  Delay();
  scl_clr();     //SCL = 0
  Delay();
}
//---------------------------------------------------------------------------

static void iic_stop(void) //stop condition
{
  sda_clr();     //SDA = 0
  scl_clr();     //SCL = 0
  Delay();
  scl_set();     //SCL = 1
  Delay();
  sda_set();     //SDA = 1;
  Delay();
}
//---------------------------------------------------------------------------

static BOOL iic_ack(void) //get ack (nack_mcu)
{
  BOOL b;

  scl_set();        //SCL = 1;
  Delay();
  b = is_sda();
  scl_clr();        //SCL = 0;
  Delay();
  return b;
}
//---------------------------------------------------------------------------

static void iic_ack_clr(void) //clr ack (ack_mcu)
{
  sda_clr();        //SDA = 0;
  Delay();
  scl_set();        //SCL = 1;
  Delay();
  scl_clr();        //SCL = 0;
  Delay();
  sda_set();
  Delay();
}
//---------------------------------------------------------------------------

static void iic_put(u8 b) //send byte
 {
  idx8 i;

  sda_clr();      //SDA = 0;
  scl_clr();      //SCL = 0;
  for (i=0; i < 8; i++)
   {
    if (b & 0x80) sda_set();   //SDA = 1;
    else sda_clr();   //SDA = 0;
    b <<= 1;
    Delay();
    scl_set();    //SCL = 1
    Delay();
    scl_clr();    //SCL = 0
    Delay();
   }
  sda_set();      //SDA = 1;
  Delay();
 }
//---------------------------------------------------------------------------

static u8 iic_get(void) //get byte
 {
  idx8 i;
  u8 b = 0;

  sda_set();     //SDA = 1;
  for (i=0; i < 8; i++)
   {
    scl_set();        //SCL = 1;
    Delay();
    b <<= 1;
    b |= is_sda();
    scl_clr();        //SCL = 0;
    Delay();
   }
  return b;
 }
//---------------------------------------------------------------------------

/*****************************************************************************
 * 24XXxxx EEPROM service
 *****************************************************************************/
//---------------------------------------------------------------------------

static BOOL put_dsc(u8 Dsc) //send control byte with ACK polling
{
 int tryes = 0;
 do
  {
   iic_start();
   iic_put(Dsc); //0b101000x0
   ClearWDT();
   if (++tryes > 150) return false;
  }
 while (iic_ack());
 return true;
}
//---------------------------------------------------------------------------

BOOL pwr_11(u8 Ctl, u8 Size, u16 Addr, const u8 *Data) //page write
{
 idx8 i;
 u8 Dsc;
 BOOL res = false;

 iic_init();
 wait();
 iic_start();
 Dsc = Ctl&0xf0;
 Dsc |= ((Addr / 256U) << 1)&0b00001110;
 if (!put_dsc(Dsc)) goto stop;
 iic_put(Addr % 256U);
 if (iic_ack()) goto stop;
 for(i = 0; i < Size; i++)
   {
     iic_put(Data[i]); /* put value */
     if (iic_ack()) goto stop;
   }
 res = true;
stop:
 iic_stop();
 iic_1ms_timeout = WAIT;
 return res;
}
//---------------------------------------------------------------------------

BOOL fill_11(u8 Ctl, u8 Size, u16 Addr, const u8 Data) //page write
{
 idx8 i;
 u8 Dsc;
 BOOL res = false;

 iic_init();
 wait();
 iic_start();
 Dsc = Ctl&0xf0;
 Dsc |= ((Addr / 256U) << 1)&0b00001110;
 if (!put_dsc(Dsc)) goto stop;
 iic_put(Addr % 256U);
 if (iic_ack()) goto stop;
 for(i = 0; i < Size; i++)
   {
     iic_put(Data); /* put value */
     if (iic_ack()) goto stop;
   }
 res = true;
stop:
 iic_stop();
 iic_1ms_timeout = WAIT;
 return res;
}
//---------------------------------------------------------------------------

BOOL srd_11(u8 Ctl, u8 Size, u16 Addr, u8 *Data) //sequence read
{
 u8 Dsc;
 idx8 i;
 BOOL res = false;

 iic_init();
 wait();
 iic_start();
 Dsc = Ctl&0xf0;                            //Ctl = Buf[1]
 Dsc |= ((Addr / 256U) << 1)&0b00001110;    //Addr/256 = Buf[3]
 if (!put_dsc(Dsc)) goto stop;
 iic_put(Addr%256);                         //Addr%256 = Buf[2]
 if (iic_ack()) goto stop;
 iic_start();
 iic_put(Dsc|1); //RD
 if (iic_ack()) goto stop;
 for(i = 0; i < Size; i++)
  {
   Data[i] = iic_get();
   if (i < Size - 1) iic_ack_clr();
   else iic_ack();
  }
 res = true;
stop:
 iic_stop();
 return res;
}
//---------------------------------------------------------------------------

BOOL srd_16(u8 Ctl, u8 Size, u16 Addr, u8 *Data) //sequence read
{
 u8 Dsc;
 idx8 i;
 BOOL res = false;

 iic_init();
 wait();
 iic_start();

 Dsc = Ctl&0xfe;
 if (!put_dsc(Dsc)) goto stop;
 iic_put(Addr / 256U);
 if (iic_ack()) goto stop;
 iic_put(Addr % 256U);
 if (iic_ack()) goto stop;

 iic_start();
 iic_put(Dsc|1); //RD
 if (iic_ack()) goto stop;
 for(i = 0; i < Size; i++)
  {
   Data[i] = iic_get();
   if (i < Size - 1) iic_ack_clr();
   else iic_ack();
  }
 res = true;
stop:
 iic_stop();
 return res;
}
//---------------------------------------------------------------------------

BOOL pwr_16(u8 Ctl, u8 Size, u16 Addr, const u8 *Data) //page write
{
 idx8 i;
 u8 Dsc;
 BOOL res = false;

 iic_init();
 wait();
 iic_start();
 Dsc = Ctl & 0xfe;
 if (!put_dsc(Dsc)) goto stop;
 iic_put(Addr / 256U);
 if (iic_ack()) goto stop;
 iic_put(Addr % 256U);
 if (iic_ack()) goto stop;
 for(i = 0; i < Size; i++)
   {
     iic_put(Data[i]); /* put value */
     if (iic_ack()) goto stop;
   }
 res = true;
stop:
 iic_stop();
 iic_1ms_timeout = WAIT;
 return res;
}
//---------------------------------------------------------------------------

BOOL fill_16(u8 Ctl, u8 Size, u16 Addr, const u8 Data) //page write
{
 idx8 i;
 u8 Dsc;
 BOOL res = false;

 iic_init();
 wait();
 iic_start();
 Dsc = Ctl & 0xfe;
 if (!put_dsc(Dsc)) goto stop;
 iic_put(Addr / 256U);
 if (iic_ack()) goto stop;
 iic_put(Addr % 256U);
 if (iic_ack()) goto stop;
 for(i = 0; i < Size; i++)
   {
     iic_put(Data); /* put value */
     if (iic_ack()) goto stop;
   }
 res = true;
stop:
 iic_stop();
 iic_1ms_timeout = WAIT;
 return res;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
