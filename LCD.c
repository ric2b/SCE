#include <xlcd.h>
#include "sensors.h"
#include "LCD.h"
#include "alarms.h"

void updateScreen(void)
{
  char buffer[3] = {0,0,0};
  char S2, S3 = 0;
  long int count = 0;
  int adc_result = 0;

  // awake LCD
  if(!sleeping)
  {
    while(BusyXLCD());
    WriteCmdXLCD(CURSOR_OFF);
  }

  if(update_seconds & !configModeUpdated)
  {
    updateClock(clock, buffer);
  }

  if(update_temp)
  {
    readTemperature(buffer);
    updateTemp(buffer);
  }

  if(update_lumus)
  {
    update_lumus = 0;
    SetDDRamAddr(0x4f); // Second line; first column
    ConvertADC(); //perform ADC conversion
    while(BusyADC()); //wait for result
    adc_result = ReadADC() >> 6;  //get ADC result
    lumus = adc_result/204;
    putcXLCD(lumus+'0');  // will be on [0,5]

    if(lumus == lumus_treshold && (alarmMask & 0b00000001))
      fireLumusAlarm();
  }
}

void updateTemp(char * temperature)
{
  update_temp = 0;
  while(BusyXLCD());
  SetDDRamAddr(0x40);
  putsXLCD(temperature);

  if(temp == temperature_treshold && (alarmMask & 0b00000010))
    fireTempAlarm();
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
