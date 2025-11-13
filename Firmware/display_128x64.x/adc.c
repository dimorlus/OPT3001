/*
 * File:   adc.c
 * Author: Dmitry Orlov
 *
 * Created on September 28, 2014, 10:49 AM
 */

#include <plib.h>
#include <xc.h>
#include "types.h"

//#define _AD_EE_

#ifdef _AD_EE_
#include "eeproms.h"
#include "seeprom.h"
#define _AD_FLT_
#endif /*_AD_EE_*/

#include "adc.h"

s32 volatile ad_res[ad_num];

#ifdef _AD_FLT_
static volatile s32 ad_adj[ad_num];
static volatile s32 ad_fk[ad_num];
#endif /*_AD_FLT_*/

/* ADC ISR */
/* Specify ADC Interrupt Routine, Priority Level = 6 */
void __ISR (_ADC_VECTOR, ipl3) ADCHandler (void)
{
  // Continually automatic scanning all used A/D channels. Once per 1 ms
  // (on timer interrupt) enable ADC interrupt. In the ADC interrupt reads
  // AD values from buffer and disable ADC interrupt until next timer (1ms)
  // interrupt occurs.
  mAD1IntEnable(0); // Disable ADC interrupt
  
  idx8 ch;
  #ifdef _AD_FLT_
  s32 dd;
  s32 adRes;
  #endif /*_AD_FLT_*/
  for (ch = 0; ch < ad_num; ch++)
    {
      // Read the result of A/D conversion from the idle buffer
      // digital low pass filter
      #ifdef _AD_FLT_
      adRes = ReadADC10(ch);
      dd = ((512U-128U+ad_adj[ch])*adRes)/1024U; /* Adjust */
      ad_res[ch] += (dd - ad_res[ch])/ad_fk[ch];
      #else /*_AD_FLT_*/
      ad_res[ch] = ReadADC10(ch); 
      #endif /*_AD_FLT_*/
    }
  // clear the interrupt flag
  mPORTBClearBits(BIT_5); // Pin 14
  mAD1ClearIntFlag();
}

void adc_rd_ee(idx8 addr)
{
#ifdef _AD_EE_
  if ((addr == 255) || ((addr >= eeAD0_adj) && (addr <= eeAD5_FK)))
    {
      idx8 i;
      for(i = 0; i < ad_num; i++)
        {
          ad_res[i] = 0;
#ifdef _AD_FLT_
          ad_adj[i] = ee_8(eeAD0_adj + i);
          ad_fk[i] = ee_8(eeAD0_FK + i);
#endif /*_AD_FLT_*/
        }
    }
#endif /*_AD_EE_*/
#ifdef _AD_FLT_
      for(i = 0; i < ad_num; i++) // set default values if it zero.
       {
        if (ad_fk[i]==0) ad_fk[i] = 8;
        if (ad_adj[i]==0) ad_adj[i] = 128;
       }
#endif /*_AD_FLT_*/
}

void adc_init(void)
{
  // configure ADC
  adc_rd_ee(255);
  mPORTASetPinsAnalogIn(BIT_0);
  mPORTASetPinsAnalogIn(BIT_1);
  mPORTBSetPinsAnalogIn(BIT_0);

  CloseADC10(); // Ensure the ADC is off before setting the configuration
  // define setup parameters for OpenADC10
  //             Turn module on | ouput in integer | trigger mode auto | enable autosample
  #ifdef _AD_FLT_
  #define PARAM1  ADC_MODULE_ON | ADC_FORMAT_FRACT  | ADC_CLK_AUTO      | ADC_AUTO_SAMPLING_ON
  #else /*_AD_FLT_*/
  #define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG  | ADC_CLK_AUTO      | ADC_AUTO_SAMPLING_ON
  #endif /*_AD_FLT_*/
  //                ADC ref external | disable offset test    | enable scan mode | perform 5 samples     | use dual buffers | use only mux A
  #define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON      | ADC_SAMPLES_PER_INT_3 | ADC_ALT_BUF_OFF   | ADC_ALT_INPUT_OFF

  //                use ADC internal clock | set sample time
  #define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_31

  //  set inputs to analog
  #define PARAM4  ENABLE_AN0_ANA |ENABLE_AN1_ANA |ENABLE_AN2_ANA

  // only scan An0 to An1
  #define PARAM5  SKIP_SCAN_AN3  |SKIP_SCAN_AN4  | \
                  SKIP_SCAN_AN5  |SKIP_SCAN_AN6  |SKIP_SCAN_AN7  | \
                  SKIP_SCAN_AN8  |SKIP_SCAN_AN9  |SKIP_SCAN_AN10 | \
                  SKIP_SCAN_AN11 |SKIP_SCAN_AN12 |SKIP_SCAN_AN13 | \
                  SKIP_SCAN_AN14 |SKIP_SCAN_AN15

  // set negative reference to Vref for Mux A
  SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF);
  // open the ADC
  OpenADC10(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);
  // -- configure the ADC interrupt, priority level 2 --
  // NOTE: the C32 compiler manual uses ADC_INT_ENABLE for the enable flag
  //       this is wrong, use ADC_INT_ON / ADC_INT_OFF
  ConfigIntADC10(ADC_INT_PRI_3 | ADC_INT_SUB_PRI_2 | ADC_INT_ON);
  // clear the interrupt flag
  mAD1ClearIntFlag();
  // Enable the ADC
  EnableADC10();
}
