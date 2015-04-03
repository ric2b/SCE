#include "LCD.h"

void updateScreen(void)
{
  char twoDigitBuffer[3] = {0,0,0};
  char S2, S3 = 0;
  long int count = 0;
  int adc_result = 0;

  if(update_seconds & !configModeUpdated)
  {
    update_seconds = 0;
    while(BusyXLCD());
    SetDDRamAddr(0x06);
    int_to_str(clock.seconds, twoDigitBuffer);
    putsXLCD(twoDigitBuffer);

    if(update_minutes)
    {
      update_minutes = 0;
      while(BusyXLCD());
      SetDDRamAddr(0x03);
      int_to_str(clock.minutes, twoDigitBuffer);
      putsXLCD(twoDigitBuffer);

      if(update_hours)
      {
        update_hours = 0;
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        int_to_str(clock.hours, twoDigitBuffer);
        putsXLCD(twoDigitBuffer);

      }
    }
  }

  if(update_alt)
  {
    update_alt = 0;
    SetDDRamAddr(0x09);
    putrsXLCD("ATL");
  }

  if(update_a)
  {
    update_a = 0;
    SetDDRamAddr(0x0d);
    putcXLCD('a');
  }

  if(update_P)
  {
    update_P = 0;
    SetDDRamAddr(0x0f);
    putcXLCD('P');
  }

  if(update_temp)
  {
    update_temp = 0;
    while(BusyXLCD());
    SetDDRamAddr(0x40);
    readTemperature(twoDigitBuffer);
    putsXLCD(twoDigitBuffer);
  }

  if(update_M)
  {
    update_M = 0;
    while(BusyXLCD());
    SetDDRamAddr(0x47);
    putcXLCD('M');
  }

#ifdef debug
  if(configMode)
  {
    while(BusyXLCD());
    SetDDRamAddr(0x49);
    putcXLCD('M');
  }
#endif

  if(update_lumus)
  {
    update_lumus = 0;
    SetDDRamAddr(0x4f);	// Second line; first column
    ConvertADC();	//perform ADC conversion
    while(BusyADC());	//wait for result
    adc_result = ReadADC() >> 6;	//get ADC result
    putcXLCD(adc_result/204+'0');	// will be on [0,5]
  }
}
