/* 
 * File:   ee_als.h
 * Author: Dmitry Orlov
 *
 * Created on April 8, 2015, 6:12 PM
 */

#ifndef EE_ALS_H
#define	EE_ALS_H


#define eeMasterMul 4

#ifdef	__cplusplus
extern "C"
{
#endif

// Read double value from the onboard EEPROM (24LC02)
// each data stored twice as 64bit float value and checksumed
double read_val(idx8 i);

// Set 16 bit register of OPT3001
bool SetOPTreg(u8 chip, u8 Reg, u16 Data);


// Get 16 bit register from OPT3001
int GetOPTreg(u8 chip, unsigned char Reg);

// Setup OPT3001
bool SetOPTcfg(u8 chip);

// Get light data from OPT3001
double GetOPT(u8 chip);

#ifdef	__cplusplus
}
#endif

#endif	/* EE_ALS_H */

