/* 
 * File:   adc.h
 * Author: Dmitry Orlov
 *
 * Created on September 28, 2014, 10:49 AM
 */

#ifndef ADC_H
#define	ADC_H

enum ad_ch
{
  ad0 = 0,
  ad1 = 1,
  ad_num
};

extern volatile s32 ad_res[ad_num];

void adc_init(void);
void adc_rd_ee(idx8 addr);

#endif	/* ADC_H */

