/*
 * File:   iic.h
 * Author: Dmitry Orlov
 *
 * Created on August 31, 2014, 3:06 PM
 */

#ifndef IIC_H
#define	IIC_H

//#define _24XX_  1024
//#define _PAGE_  128

#define _24XX_  04
#define _PAGE_  16

void iic_timer_1ms(void);
void iic_init(void);


/**
 * @param Ctl
 *   Control byte. For 24xx EEPROM is 0xA0 | Bank < 1
 * @param Addr
 *   Start address
 * @param Data
 *   Data pointer
 * @param Size
 *  Data size
 * for page write operations page size shouldn't exceed hardware defined page
 */

//page write 11bit address
BOOL pwr_11(u8 Ctl, u8 Size, u16 Addr, const u8 *Data);

//sequence read 11bit address
BOOL srd_11(u8 Ctl, u8 Size, u16 Addr, u8 *Data);

//page fill with constant 11bit address
BOOL fill_11(u8 Ctl, u8 Size, u16 Addr, const u8 Data);

//sequence read two address bytes
BOOL srd_16(u8 Ctl, u8 Size, u16 Addr, u8 *Data);

//page write two address bytes
BOOL pwr_16(u8 Ctl, u8 Size, u16 Addr, const u8 *Data);

//page fill with constant two address bytes
BOOL fill_16(u8 Ctl, u8 Size, u16 Addr, const u8 Data);

/**
 *
 * @param chip
 * Chip address (0..3)
 * @param dataaddr
 * register address:
 * Result = 0, Configuration = 1, Low Limit = 2, High Limit = 3
 * Manufacturer ID = 0x7E, Device ID = 0x7F
 * @param data
 * @return
 */
//BOOL opt_put(u8 chip, u8 dataaddr, u16 data);

//BOOL opt_get(u8 chip, u8 dataaddr, u16 *data);
#endif /*IIC_H*/
