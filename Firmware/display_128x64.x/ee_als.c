#include <plib.h>
#include <xc.h>
#include "types.h"
#include "iic.h"
//---------------------------------------------------------------------------

typedef union udbl
 {
  long double d; //64 bit float
  unsigned char cd[sizeof(long double)];
 } tudbl;
//---------------------------------------------------------------------------

// Read double value from the onboard EEPROM (24LC02)
// each data stored twice as 64bit float value and checksumed
//---------------------------------------------------------------------------

double read_val(idx8 i)
 {
  tudbl d[2];
  u16 eeAddr = i*2*sizeof(tudbl);
  if (srd_11(0xa0, sizeof(d), eeAddr, (u8*)&d))
   {
    unsigned char chk = 0;
    idx8 i;
    for(i=0; i < sizeof(tudbl); i++) chk += (d[0].cd[i] - d[1].cd[i] + 85)&255;
    if (chk) return -1.0;
    else return (double)d[0].d;
   }
  else return -1.0;
 }

//---------------------------------------------------------------------------
// Set 16 bit register of OPT3001
//---------------------------------------------------------------------------
bool SetOPTreg(u8 chip, u8 Reg, u16 Data)
{
 u8 dat[2];
 dat[0] = Data/256;
 dat[1] = Data%256;
 return pwr_11(0x80, 2, (u16)(4+(chip&3))*256+Reg, &dat[0]);
}
//---------------------------------------------------------------------------
// Get 16 bit register from OPT3001
//---------------------------------------------------------------------------
int GetOPTreg(u8 chip, u8 Reg)
{
 u8 dat[2];
 if (srd_11(0x80, 2, (u16)(4+(chip&3))*256+Reg, &dat[0])) return dat[0]*256+dat[1];
 else return -1;
}
//---------------------------------------------------------------------------
// Setup OPT3001
//---------------------------------------------------------------------------
bool SetOPTcfg(u8 chip)
{
 u8 dat[2];
// u16 CFG =     // 0xcc00
// (0x0c << 12)+ //RN[3:0]   Range number field (automatic full-scale-range)
// (0x01 << 11)+ //CT        Conversion time field (800 ms)
// (0x02 <<  9)+ //M[1:0]    Mode of conversion operation field (Continuous conversions)
// (0x00 <<  8)+ //OVF       Overflow flag field (read-only)
// (0x00 <<  7)+ //CRF       Conversion ready field (read-only)
// (0x00 <<  6)+ //FH        Flag high field (read-only)
// (0x00 <<  5)+ //FL        Flag low field (read-only)
// (0x00 <<  4)+ //L         Latch field
// (0x00 <<  3)+ //POL       Polarity field
// (0x00 <<  2)+ //ME        Mask exponent field
// (0x00 <<  0); //FC[1:0]   Fault count field

 dat[0] =
 (0x0c << 12-8)+ //RN[3:0]   Range number field (automatic full-scale-range)
 (0x01 << 11-8)+ //CT        Conversion time field (800 ms)
 (0x02 <<  9-8)+ //M[1:0]    Mode of conversion operation field (Continuous conversions)
 (0x00 <<  8-8); //OVF       Overflow flag field (read-only)

 dat[1] =
 (0x00 <<  7)+ //CRF       Conversion ready field (read-only)
 (0x00 <<  6)+ //FH        Flag high field (read-only)
 (0x00 <<  5)+ //FL        Flag low field (read-only)
 (0x00 <<  4)+ //L         Latch field
 (0x00 <<  3)+ //POL       Polarity field
 (0x00 <<  2)+ //ME        Mask exponent field
 (0x00 <<  0); //FC[1:0]   Fault count field

 return pwr_11(0x80, 2, (u16)(4+(chip&3))*256+1, &dat[0]);
}
//---------------------------------------------------------------------------
// Get light data from OPT3001
//---------------------------------------------------------------------------
double GetOPT(u8 chip)
{
 u8 dat[2];

 if (srd_11(0x80, 2, (u16)(4+(chip&3))*256, &dat[0]))
  {
   int Data = dat[0]*256+dat[1];
   int  mantissa = Data & 0x0fff;
   int  exponent = (Data & 0xf000)>>12;
   double Res = 0.01*(double)mantissa*(1<<exponent);
   return Res;
  }
 else return -1.0;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

