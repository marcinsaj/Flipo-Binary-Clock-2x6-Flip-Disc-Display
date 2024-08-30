#include <FlipDisc.h>         // https://github.com/marcinsaj/FlipDisc
#include <RTC_RX8025T.h>      // https://github.com/marcinsaj/RTC_RX8025T
#include <TimeLib.h>          // https://github.com/PaulStoffregen/Time
#include <Wire.h>             // https://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <OneButton.h>        // https://github.com/mathertel/OneButton
#include <EEPROM.h>           // https://www.arduino.cc/en/Reference/EEPROM


// 1. Pin declaration for a dedicated controller
#define EN_PIN  10
#define CH_PIN  A1 
#define PL_PIN  A0


// Buttons - counting from the top
#define B1_PIN A3  // Top button
#define B2_PIN A2   // Middle button

// RTC
#define RTC_PIN 2 // RTC interrupt input

static const uint8_t TOP_ROW = 2;
static const uint8_t BOT_ROW = 1;

static const uint8_t CLR_ALL = 0;
static const uint8_t SET_ALL = 63;

// Aliases for individual option settings
static const uint8_t HR12 = 0;   // Display time in 12 hour format
static const uint8_t HR24 = 1;   // Display time in 24 hour format

// Declare structure that allows convenient access to the time elements:
// - tm.Hour - hours
// - tm.Minute - minutes
tmElements_t tm;

// Initialize a new OneButton instance for a buttons 
// BUTTON_PIN - Input pin for the button
// true      - Button is active low
// true      - Enable internal pull-up resistor
OneButton button1(B1_PIN, true, true);
OneButton button2(B2_PIN, true, true);


// Flags for storing button press status
bool shortPressButton1Status = false;
bool shortPressButton2Status = false;
bool longPressButton1Status = false;
bool longPressButton2Status = false;

// RTC interrupt flag
volatile bool interruptRtcStatus = false;


// The flag specifies whether the currently available time 
// in the tm structure is up to date. If the time is current, 
// there is no need to read the time from the RTC again
bool currentTimeStatus = false;

bool timeToDisplayTime = false;

uint8_t binaryTopRowAR[6];
uint8_t binaryBotRowAR[6];


uint8_t hour_time = 0;
uint8_t minute_time = 0;




// Interrupt from RTC
void rtcInterruptISR(void)
{
  interruptRtcStatus = true;
}



void setup() 
{
  Flip.Pin(EN_PIN, CH_PIN, PL_PIN);
  Flip.Init(D2X6);

  pinMode(RTC_PIN, INPUT_PULLUP);


  // RTC RX8025T initialization
  RTC_RX8025T.init();

  // Time update interrupt initialization. Interrupt generated by RTC (INT output): 
  // "INT_SECOND" - every second,
  // "INT_MINUTE" - every minute.
  RTC_RX8025T.initTUI(INT_MINUTE);

  // "INT_ON" - turn ON interrupt generated by RTC (INT output),
  // "INT_OFF" - turn OFF interrupt.
  RTC_RX8025T.statusTUI(INT_ON);
  
  // Assign an interrupt handler to the RTC output, 
  // an interrupt will be generated every minute to display the time
  attachInterrupt(digitalPinToInterrupt(RTC_PIN), rtcInterruptISR, FALLING);

    // Link the button functions
  button1.attachClick(ShortPressButton1);
  button2.attachClick(ShortPressButton2);
  button1.attachLongPressStart(LongPressButton1);
  button2.attachLongPressStart(LongPressButton2);

  delay(3000);
  DisplayTime();
}


void loop() 
{
  WatchButtons();
  if(interruptRtcStatus == true) DisplayTime();
  if(longPressButton1Status == true) SetTime();
  //if(longPressButton2Status == true) DisplaySequence();
}












void DisplaySequence(void)
{

}





void DisplayTime(void)
{
  GetRtcTime();

  DisplayData(TOP_ROW, hour_time);
  DisplayData(BOT_ROW, minute_time);

  interruptRtcStatus = false;
}


void GetRtcTime(void)
{
  // Get the time from the RTC and save it to the tm structure
  RTC_RX8025T.read(tm);

  hour_time = tm.Hour;
  minute_time = tm.Minute;
}


void DisplayData(uint8_t row, uint8_t data)
{
  if(row == BOT_ROW)
  {
    DecToBinary(data, binaryBotRowAR);
    for(int i = 1; i <= 6; i++) Flip.Disc_2x6(1, i, binaryBotRowAR[6-i]);
  }

  if(row == TOP_ROW)
  {
    DecToBinary(data, binaryTopRowAR);
    for(int i = 1; i <= 6; i++) Flip.Disc_2x6(1, i+6, binaryTopRowAR[6-i]);
  }
}












void SetTime(void)
{
  ClearPressButtonFlags();
  GetRtcTime();
  // get time format from eeprom H12/HR24

  uint8_t time_settings_level = 1;
  bool updateDisplay = true;
  bool timeFormat = true;

  do // Stay in settings until all values are set
  {
    WatchButtons();

    if(shortPressButton1Status == true || shortPressButton2Status == true)
    {
      if(time_settings_level == 2) timeFormat = !timeFormat;
    
      if(shortPressButton1Status == true)
      {
        if(time_settings_level == 4) hour_time++;
        if(time_settings_level == 6) minute_time++;
      }

      if(shortPressButton2Status == true)
      {
        if(time_settings_level == 4) hour_time--;
        if(time_settings_level == 6) minute_time--; 
      }
      
      ClearPressButtonFlags();
      updateDisplay = true;
    }

    if(longPressButton1Status == true)
    {      
      time_settings_level++;      
      if(time_settings_level >  6) time_settings_level = 0;  // Exit settings
      if(time_settings_level <= 6) 
      {
        updateDisplay = true;
      }

      ClearPressButtonFlags();
    }


    if(updateDisplay == true)
    {
      if(time_settings_level == 1)
      {
        DisplayData(TOP_ROW, CLR_ALL);
        DisplayData(BOT_ROW, CLR_ALL);
        delay(300);
                DisplayData(TOP_ROW, SET_ALL);
        DisplayData(BOT_ROW, SET_ALL);
        delay(1000);

        for(int i = 0; i < 6; i++)
        {
          Flip.Disc_2x6(1, i+1,  1);
          Flip.Disc_2x6(1, 12-i, 1);
          delay(200);
        }

        for(int i = 0; i < 6; i++)
        {
          Flip.Disc_2x6(1, i+1,  0);
          Flip.Disc_2x6(1, 12-i, 0);
          delay(200);
        }    

        DisplayData(BOT_ROW, SET_ALL);
        time_settings_level = 2;
      }

      if(time_settings_level == 2)
      {
        if(timeFormat == HR12) DisplayData(TOP_ROW, 12);
        if(timeFormat == HR24) DisplayData(TOP_ROW, 24);
      }   

      if(time_settings_level == 3)
      {
        DisplayData(TOP_ROW, CLR_ALL);
        DisplayData(BOT_ROW, CLR_ALL);
        delay(300);

        for(int i = 7; i <= 12; i++)
        {
          Flip.Disc_2x6(1, i,  1);
          delay(200);
        }

        for(int i = 7; i <= 12; i++)
        {
          Flip.Disc_2x6(1, i,  0);
          delay(200);
        }

        delay(100);
        time_settings_level = 4;
      }

      if(time_settings_level == 4)
      {
        DisplayData(TOP_ROW, hour_time);
      }

      if(time_settings_level == 5)
      {
        DisplayData(TOP_ROW, CLR_ALL);
        DisplayData(BOT_ROW, CLR_ALL);
        delay(300);

        for(int i = 1; i <= 6; i++)
        {
          Flip.Disc_2x6(1, i,  1);
          delay(200);
        }

        for(int i = 1; i <= 6; i++)
        {
          Flip.Disc_2x6(1, i,  0);
          delay(200);
        }
        
        delay(100);
        time_settings_level = 6;
      }      

      if(time_settings_level == 6)
      {
        DisplayData(BOT_ROW, minute_time);
      }

      updateDisplay = false;
    }



  } while(time_settings_level > 0);


  // setTime(hh, mm, ss, day, month, year) 
  // The date is skipped and the seconds are set by default to 0
  // We are only interested in hours and minutes
  setTime(hour_time, minute_time, 0, 0, 0, 0);

  // Set the RTC from the system time
  RTC_RX8025T.set(now());

  DisplayTime();
}


















// Keep watching the buttons
void WatchButtons(void)
{
  button1.tick();
  button2.tick();
}

// Button flags clearing function
void ClearPressButtonFlags(void)
{
  shortPressButton1Status = false;
  shortPressButton2Status = false;
  longPressButton1Status = false;
  longPressButton2Status = false;
}

// Button press handling functions
void ShortPressButton1(void){shortPressButton1Status = true;}
void ShortPressButton2(void){shortPressButton2Status = true;}
void LongPressButton1(void){longPressButton1Status = true;}
void LongPressButton2(void){longPressButton2Status = true;}

void DecToBinary(uint8_t decValue, uint8_t binaryArray[]) 
{
  for(int i = 0; i < 6; i++)
  {
    binaryArray[i] = decValue & B00000001;
    decValue = decValue >> 1;
  }
}
