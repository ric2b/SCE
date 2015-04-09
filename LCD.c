#include "LCD.h"

void updateScreen(void)
{
  char buffer[3] = {0,0,0};
  char S2, S3 = 0;
  long int count = 0;
  int adc_result = 0;

  // awake LCD
  while(BusyXLCD());
  WriteCmdXLCD(CURSOR_OFF);

  if(update_seconds & !configModeUpdated)
  {
    updateClock(clock, buffer);
  }

  if(update_alt)
  {
    update_alt = 0;
    SetDDRamAddr(0x09);
    putrsXLCD("ATL");
  }

  if(update_temp)
  {
    readTemperature(buffer);
    updateTemp(buffer);
  }

#ifdef debug
  if(configMode)
  {
    while(BusyXLCD());
    SetDDRamAddr(0x49);
    putcXLCD('C');
  }
#endif

  if(update_lumus)
  {
    update_lumus = 0;
    SetDDRamAddr(0x4f); // Second line; first column
    ConvertADC(); //perform ADC conversion
    while(BusyADC()); //wait for result
    adc_result = ReadADC() >> 6;  //get ADC result
    lumus = adc_result/204;
    putcXLCD(lumus+'0');  // will be on [0,5]
  }
}

void updateTemp(char * temperature)
{
  update_temp = 0;
  while(BusyXLCD());
  SetDDRamAddr(0x40);
  putsXLCD(temperature);
}

void updateClock(time LCDtime, char * buffer)
{
  update_seconds = 0;
  while(BusyXLCD());
  SetDDRamAddr(0x06);
  int_to_str(LCDtime.seconds, buffer);
  putsXLCD(buffer);

  if(update_minutes)
  {
    update_minutes = 0;
    while(BusyXLCD());
    SetDDRamAddr(0x03);
    int_to_str(LCDtime.minutes, buffer);
    putsXLCD(buffer);

    if(update_hours)
    {
      update_hours = 0;
      while(BusyXLCD());
      SetDDRamAddr(0x00);
      int_to_str(LCDtime.hours, buffer);
      putsXLCD(buffer);

    }
  }
}
