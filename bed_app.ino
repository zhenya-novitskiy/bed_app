#include <FastLED.h>
#include <BH1750.h>
#include <Wire.h>
#include <Button.h>
#include <DoorSensor.h>

#include "ParalelTimer.h"
#include <TimeClock.h>
#include <ColorTransaction.h>
#include "ProjectorPower.h"
#include "SparkFun_External_EEPROM.h" 

#define TRANSACTION_STEPS 30

#define COOLING  55
#define SPARKING 120

BH1750 lightMeter;
ExternalEEPROM externalMemory;

#define BED_LEDS_NUMBER 360
#define BEDSIDE_TABLE_LEDS_NUMBER 97
#define NIGHT_LIGHT_LEDS_NUMBER 55

#define BED_TEMPLATES_NUMBER 3


#pragma region led strip sections

  #define BED_START_INDEX 0
  #define BED_END_INDEX 20

  #define RIGHT_B_DRAWER1_START_INDEX 270
  #define RIGHT_B_DRAWER1_END_INDEX 315

  #define RIGHT_B_DRAWER2_START_INDEX 241
  #define RIGHT_B_DRAWER2_END_INDEX 264

  #define LEFT_B_DRAWER1_START_INDEX 10
  #define LEFT_B_DRAWER1_END_INDEX 55

  #define LEFT_B_DRAWER2_START_INDEX 61
  #define LEFT_B_DRAWER2_END_INDEX 84

  #define T_DRAWER1_START_INDEX 56
  #define T_DRAWER1_END_INDEX 76

  #define T_DRAWER2_START_INDEX 77
  #define T_DRAWER2_END_INDEX 97




#pragma endregion led strip sections

#pragma region pinouts

  #define DATA_PIN 8 
 
  #define LEFT_T_NIGHT_LIGHT_BUTTON 38
  #define LEFT_T_FUNC_BUTTON1 42
  #define LEFT_T_FUNC_BUTTON2 36
  #define LEFT_T_DRAWER1 44
  #define LEFT_T_DRAWER2 40

  #define LEFT_B_DRAWER1 48
  #define LEFT_B_DRAWER2 50

  #define RIGHT_T_NIGHT_LIGHT_BUTTON 26
  #define RIGHT_T_FUNC_BUTTON1 30
  #define RIGHT_T_FUNC_BUTTON2 24
  #define RIGHT_T_DRAWER1 32
  #define RIGHT_T_DRAWER2 28

  #define RIGHT_B_DRAWER1 52 
  #define RIGHT_B_DRAWER2 46


#pragma endregion pinouts

#define CURRENT_BED_TEMPLATE_ADDRESS 0
#define CURRENT_NIGHT_LIGHT_TEMPLATE_ADDRESS 1

ProjectorPower  ProjectorSwitch;

ParalelTimer timer = ParalelTimer(2);

ColorTransaction bedTransactions[BED_LEDS_NUMBER];
ColorTransaction rightBedsideTableTransactions[BEDSIDE_TABLE_LEDS_NUMBER];
ColorTransaction leftBedsideTableTransactions[BEDSIDE_TABLE_LEDS_NUMBER];

TimeClock timeClock = TimeClock(0, 9);

// Define the array of leds
CRGB bedLeds[BED_LEDS_NUMBER];
CRGB bedLedsBuffer[BED_LEDS_NUMBER];
CRGB rightBedsideTableLeds[BEDSIDE_TABLE_LEDS_NUMBER];
CRGB leftBedsideTableLeds[BEDSIDE_TABLE_LEDS_NUMBER];

Button LeftTFuncButton1 = Button(LEFT_T_FUNC_BUTTON1,PULLUP);
Button RightTFuncButton1 = Button(RIGHT_T_FUNC_BUTTON1,PULLUP);

Button LeftTFuncButton2 = Button(LEFT_T_FUNC_BUTTON2,PULLUP);
Button RightTFuncButton2 = Button(RIGHT_T_FUNC_BUTTON2,PULLUP);

DoorSensor Left_B_Drawer1 = DoorSensor(LEFT_B_DRAWER1,PULLUP);
DoorSensor Right_B_Drawer1 = DoorSensor(RIGHT_B_DRAWER1,PULLUP);

DoorSensor Left_B_Drawer2 = DoorSensor(LEFT_B_DRAWER2,PULLUP);
DoorSensor Right_B_Drawer2 = DoorSensor(RIGHT_B_DRAWER2,PULLUP);

DoorSensor Left_T_Drawer1 = DoorSensor(LEFT_T_DRAWER1,PULLUP);
DoorSensor Right_T_Drawer1 = DoorSensor(RIGHT_T_DRAWER1,PULLUP);

DoorSensor Left_T_Drawer2 = DoorSensor(LEFT_T_DRAWER2,PULLUP);
DoorSensor Right_T_Drawer2 = DoorSensor(RIGHT_T_DRAWER2,PULLUP);

uint32_t currentBedTemplate[BED_LEDS_NUMBER];

int currentBedTemplateIndex = 4;
const uint32_t bedTemplates[][BED_LEDS_NUMBER] PROGMEM =
{
     {0xFF0000, 0xF51400, 0xEB2800, 0xE13C00, 0xD75000, 0xCD6400, 0xC37800, 0xB98C00, 0xAFA000, 0xA5B400, 0x9BC800, 0x91DC00, 0x87F000, 0x7D0400, 0x731800, 0x692C00, 0x5F4000, 0x555400, 0x4B6800, 0x417C00},
     {0xFF7D, 0x14F57D, 0x28EB7D, 0x3CE17D, 0x50D77D, 0x64CD7D, 0x78C37D, 0x8CB97D, 0xA0AF7D, 0xB4A57D, 0xC89B7D, 0xDC917D, 0xF0877D, 0x47D7D, 0x18737D, 0x2C697D, 0x405F7D, 0x54557D, 0x684B7D, 0x7C417D},
     {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}
};

float currentLux;

int currentNightLightTemplateIndex = 0;
Button LeftTNLButton = Button(LEFT_T_NIGHT_LIGHT_BUTTON, PULLUP);
uint32_t currentLeftNightLightTemplate[NIGHT_LIGHT_LEDS_NUMBER];
bool leftTNLButtonState = false;

Button RightTNLButton = Button(RIGHT_T_NIGHT_LIGHT_BUTTON, PULLUP);
uint32_t currentRightNightLightTemplate[NIGHT_LIGHT_LEDS_NUMBER];
bool rightTNLButtonState = false;

bool isMax = false;
bool isHardOff = false;
bool isHardOn = false;

// const uint32_t nighLightTemplates[][NIGHT_LIGHT_LEDS_NUMBER] PROGMEM =
// {
//      {0xFF0000, 0x00FF00, 0xFF0000},
//      {0xFF00FF, 0x00DDFF, 0xFF00FF},
//      {0x00DDFF, 0x00DDFF, 0x00DDFF}
// };

const uint32_t nighLightTemplates[][NIGHT_LIGHT_LEDS_NUMBER] PROGMEM =
{
     {0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000,0xFF0000, 0x00FF00, 0xFF0000, 0x00FF00, 0xFF0000},
     {0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF, 0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF,0xFF00FF, 0x00DDFF, 0xFF00FF, 0x00DDFF, 0xFF00FF},
     {0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF,0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF, 0x00DDFF}
};

DoorSensor PIR1 = DoorSensor(40,PULLDOWN);
DoorSensor PIR2 = DoorSensor(41,PULLDOWN);

CRGBPalette16 gPal;

void setup() { 
  
  // Uncomment/edit one of the following lines for your leds arrangement.
  // ## Clockless types ##
  FastLED.addLeds<WS2812, 5, GRB>(bedLeds, BED_LEDS_NUMBER).setCorrection( TypicalLEDStrip );  // GRB ordering is assumed
  FastLED.addLeds<WS2812, 4, GRB>(rightBedsideTableLeds, BEDSIDE_TABLE_LEDS_NUMBER).setCorrection( TypicalLEDStrip );  // GRB ordering is assumed
  FastLED.addLeds<WS2812, 2, GRB>(leftBedsideTableLeds, BEDSIDE_TABLE_LEDS_NUMBER).setCorrection( TypicalLEDStrip );  // GRB ordering is assumed
  
  FastLED.setBrightness(255);
    
  gPal = HeatColors_p;
  
  Wire.begin();

  if (externalMemory.begin() == false)
  {
    Serial.println("No memory detected. Freezing.");
    while (1)
      ;
  }

  ProjectorSwitch.projectorOn(hardOff);
  ProjectorSwitch.projectorOff(projectorOff);
  
  ProjectorSwitch.init();
//
//  #pragma region ButtonEvents
//
  RightTNLButton.subscribeOnClick(RightNightLightSwitch);
  LeftTNLButton.subscribeOnClick(LeftNightLightSwitch);
  
  RightTNLButton.subscribeOnDoubleClick(changeNightLightTemplate);
  LeftTNLButton.subscribeOnDoubleClick(changeNightLightTemplate);

  RightTNLButton.subscribeOnPressedMoreThan(1000, SwithToMax);
  LeftTNLButton.subscribeOnPressedMoreThan(1000, SwithToMax);

  RightTFuncButton1.subscribeOnPressedMoreThan(2000, hardSwitch);
  LeftTFuncButton1.subscribeOnPressedMoreThan(2000, hardSwitch);

  RightTFuncButton1.subscribeOnDoubleClick(resetHardSwitch);
  LeftTFuncButton1.subscribeOnDoubleClick(resetHardSwitch);

  RightTFuncButton2.subscribeOnClick(changeBedTemplate);
  LeftTFuncButton2.subscribeOnClick(changeBedTemplate);
  
  timeClock.nightStarted(nightStarted);
  timeClock.morningStarted(morningStarted);

  Left_B_Drawer1.doorClose(Left_B_Drawer1_Close);
  Left_B_Drawer1.doorOpen(Left_B_Drawer1_Open);
  Left_B_Drawer2.doorClose(Left_B_Drawer2_Close);
  Left_B_Drawer2.doorOpen(Left_B_Drawer2_Open);

  Right_B_Drawer1.doorClose(Right_B_Drawer1_Close);
  Right_B_Drawer1.doorOpen(Right_B_Drawer1_Open);
  Right_B_Drawer2.doorClose(Right_B_Drawer2_Close);
  Right_B_Drawer2.doorOpen(Right_B_Drawer2_Open);

  Left_T_Drawer1.doorClose(Left_T_Drawer1_Close);
  Left_T_Drawer1.doorOpen(Left_T_Drawer1_Open);
  Left_T_Drawer2.doorClose(Left_T_Drawer2_Close);
  Left_T_Drawer2.doorOpen(Left_T_Drawer2_Open);

  Right_T_Drawer1.doorClose(Right_T_Drawer1_Close);
  Right_T_Drawer1.doorOpen(Right_T_Drawer1_Open);
  Right_T_Drawer2.doorClose(Right_T_Drawer2_Close);
  Right_T_Drawer2.doorOpen(Right_T_Drawer2_Open);

  PIR1.doorClose(Left_PIR_Detected);
  PIR1.doorOpen(Left_PIR_Clear);

  PIR2.doorClose(Right_PIR_Detected);
  PIR2.doorOpen(Right_PIR_Clear);

  #pragma endregion ButtonEvents

  timer.Miliseconds = 8;

  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);

  for(int i=22; i<=55; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
 
  #pragma region Setup initial strip state 

  currentBedTemplateIndex = externalMemory.read(CURRENT_BED_TEMPLATE_ADDRESS);;
  
  for (int i = 0; i < BED_LEDS_NUMBER-1; i++) 
  {
    currentBedTemplate[i] = pgm_read_dword(&bedTemplates[currentBedTemplateIndex][i]);

    setbedLeds(i, currentBedTemplate[i]);

    bedTransactions[i].CurrentColor = bedLeds[i];
    //bedTransactions[i].TransactionSteps = 50;
  }

  currentNightLightTemplateIndex = externalMemory.read(CURRENT_NIGHT_LIGHT_TEMPLATE_ADDRESS);;
  
  for (int i = 0; i < NIGHT_LIGHT_LEDS_NUMBER; i++) 
  {
        //leftBedsideTableTransactions[i].TransactionSteps = 50;
    //rightBedsideTableTransactions[i].TransactionSteps = 50;
    currentLeftNightLightTemplate[i] = pgm_read_dword(&nighLightTemplates[currentNightLightTemplateIndex][i]);
    currentRightNightLightTemplate[i] = pgm_read_dword(&nighLightTemplates[currentNightLightTemplateIndex][i]);

  }

  FastLED.show();
  
//  Left_B_Drawer1.init();
//  Left_B_Drawer2.init();
//
//  Right_B_Drawer1.init();
//  Right_B_Drawer2.init();

  Left_T_Drawer1.init();
  Left_T_Drawer2.init();

  Right_T_Drawer1.init();
  Right_T_Drawer2.init();

  #pragma endregion Setup initial strip state

  Serial.begin(9600);
}

bool isInAnimation = false;

void loop() 
{ 

   if (lightMeter.measurementReady()) 
   {
       currentLux = lightMeter.readLightLevel();
   }


  #pragma region Button Loops 

  ProjectorSwitch.loop();
  timeClock.loop();

  RightTFuncButton1.loop();
  RightTFuncButton2.loop();
  RightTNLButton.loop();
  
  LeftTFuncButton1.loop();
  LeftTFuncButton2.loop();
  LeftTNLButton.loop();
  
  Left_B_Drawer1.loop();
  Left_B_Drawer2.loop();
  Right_B_Drawer1.loop();
  Right_B_Drawer2.loop();

  Left_T_Drawer1.loop();
  Left_T_Drawer2.loop();
  Right_T_Drawer1.loop();
  Right_T_Drawer2.loop();
  
  PIR1.loop();
  PIR2.loop();

  #pragma endregion ButtonEvents

  #pragma region Update transactions
    

  if (timer.ItsTime())
  {
    isInAnimation = false;

    for (int i = 0; i < BED_LEDS_NUMBER; i++) 
    {
      if (bedTransactions[i].InAnimation())
      {
          isInAnimation = true;
          bedTransactions[i].NextFrame();
          setbedLeds(i, bedTransactions[i].CurrentColor);
      }
    }

    for (int i = 0; i < BEDSIDE_TABLE_LEDS_NUMBER; i++) 
    {
      if (leftBedsideTableTransactions[i].InAnimation())
      {
          //Serial.println("leftBedsideTableTransactionsInAnimation");
          isInAnimation = true;
          leftBedsideTableTransactions[i].NextFrame();
          leftBedsideTableLeds[i] = leftBedsideTableTransactions[i].CurrentColor;
      }
    }

    for (int i = 0; i < BEDSIDE_TABLE_LEDS_NUMBER; i++) 
    {
      if (rightBedsideTableTransactions[i].InAnimation())
      {
          isInAnimation = true;
          rightBedsideTableTransactions[i].NextFrame();
          rightBedsideTableLeds[i] = rightBedsideTableTransactions[i].CurrentColor;
      }
    }
  
   
  }

  #pragma endregion Update transactions 


  if (currentBedTemplateIndex >= BED_TEMPLATES_NUMBER)
  {
      if (currentBedTemplateIndex == BED_TEMPLATES_NUMBER)
      {
        random16_add_entropy( random(65535));
        pride();
      }

      if (currentBedTemplateIndex == BED_TEMPLATES_NUMBER + 1)
      {
        pacifica_loop();
      }

      if (currentBedTemplateIndex == BED_TEMPLATES_NUMBER + 2)
      {
        aatemplate();
      }


      if (currentBedTemplateIndex == BED_TEMPLATES_NUMBER + 3)
      {
        mover();
        
      }

//      for (int index = 0; index < BED_LEDS_NUMBER; index++) 
//      {
//        if ((!Left_B_Drawer1.isPressed() && index >= LEFT_B_DRAWER1_START_INDEX && index <= LEFT_B_DRAWER1_END_INDEX)||
//            (!Left_B_Drawer2.isPressed() && index >= LEFT_B_DRAWER2_START_INDEX && index <= LEFT_B_DRAWER2_END_INDEX)||
//            (!Right_B_Drawer1.isPressed() && index >= RIGHT_B_DRAWER1_START_INDEX && index <= RIGHT_B_DRAWER1_END_INDEX)||
//            (!Right_B_Drawer2.isPressed() && index >= RIGHT_B_DRAWER2_START_INDEX && index <= RIGHT_B_DRAWER2_END_INDEX)) 
//        {
//            bedLeds[index] = CRGB::White;
//        }
//      }

      isInAnimation = true;
  }

  if (isInAnimation)
  {
    //  for (int i = 0; i < BED_LEDS_NUMBER; i++) 
    //   {
    //     bedLedsBuffer[i] = bedLeds[i];

    //     if (currentLux >= 5 && currentLux < 12 && i % 3 == 0)
    //     {
    //       bedLeds[i] = CRGB::Black;
    //     }

    //     if (currentLux < 5 && i % 2 == 0)
    //     {
    //       bedLeds[i] = CRGB::Black;
    //     }
    //   }
    FastLED.show();

    // for (int i = 0; i < BED_LEDS_NUMBER; i++) 
    // {
    //   bedLeds[i] = bedLedsBuffer[i];
    // }
  }
  
//for(int i=22; i<=40; i++)
//{
//
//   buttonState = digitalRead(i);
//
//   if (buttonState == LOW) 
//   {
//     //Serial.println(i);
//    }
//}
}

void nightStarted()
{
    hardOff();
}


void morningStarted()
{
    resetHardSwitch();
}


void hardSwitch()
{
  isHardOff = !isHardOff;
  isHardOn = !isHardOff;

  if (isHardOff)
  {
      hardOff();
  }else
  {
      hardOn();
  }
}

void hardOff()
{
  Serial.println("projectorOn");
    isHardOff = true;
    isHardOn = false;
    for (int i = 0; i < BED_LEDS_NUMBER; i++) 
    {
      bedLeds[i] = CRGB::Black;
    }
    FastLED.show();
}


void hardOn()
{
    isHardOff = false;
    isHardOn = true;
    currentBedTemplateIndex--;
    changeBedTemplate();
    
}


void projectorOff()
{
  Serial.println("projectorOff");
    if (!timeClock.isNightNow())
    {
      resetHardSwitch();
    }
}

void resetHardSwitch()
{
  Serial.println("resetHardSwitch");
 
    isHardOn = isHardOff = false;
    currentBedTemplateIndex--;
    changeBedTemplate();
}

void setbedLeds(int index, CRGB value)
{
    if (!isHardOff || isHardOn)
    {
      bedLeds[index] = value;
    }
}

void changeBedTemplate()
{
Serial.println("changeBedTemplate");
   
    currentBedTemplateIndex++;
    
    if (currentBedTemplateIndex > BED_TEMPLATES_NUMBER + 3) currentBedTemplateIndex = 0;
    
    if (currentBedTemplateIndex < BED_TEMPLATES_NUMBER)
    {

      Left_B_Drawer1.init();
      Left_B_Drawer2.init();

      Right_B_Drawer1.init();
      Right_B_Drawer2.init();

      FastLED.setBrightness(255 );
  
      for (int i = 0; i < BED_LEDS_NUMBER; i++) 
      {
        currentBedTemplate[i] = pgm_read_dword(&bedTemplates[currentBedTemplateIndex][i]);
      }
      
      ApplyBedCurrentTemplate();
    }else
    {
      FastLED.setBrightness(255);
    }

    //externalMemory.write(CURRENT_BED_TEMPLATE_ADDRESS, currentBedTemplateIndex);
}

void changeNightLightTemplate()
{
    isMax = false;
    currentNightLightTemplateIndex++;
    if (currentNightLightTemplateIndex>2) currentNightLightTemplateIndex = 0;

    for (int i = 0; i < NIGHT_LIGHT_LEDS_NUMBER; i++) 
    {
        currentLeftNightLightTemplate[i] = pgm_read_dword(&nighLightTemplates[currentNightLightTemplateIndex][i]);
        currentRightNightLightTemplate[i] = pgm_read_dword(&nighLightTemplates[currentNightLightTemplateIndex][i]);
    }

    for (int i = 0; i < NIGHT_LIGHT_LEDS_NUMBER; i++) 
    {
        leftBedsideTableTransactions[i].ToColor(leftBedsideTableLeds[i], CRGB(currentLeftNightLightTemplate[i]), TRANSACTION_STEPS);
        rightBedsideTableTransactions[i].ToColor(rightBedsideTableLeds[i], CRGB(currentRightNightLightTemplate[i]) , TRANSACTION_STEPS);
    }

    //externalMemory.write(CURRENT_NIGHT_LIGHT_TEMPLATE_ADDRESS, currentNightLightTemplateIndex);

    leftTNLButtonState = true;
    rightTNLButtonState = true;
}

void SwithToMax()
{
    if (!isMax)
    {
      leftTNLButtonState = true;
      rightTNLButtonState = true;

      for (int i = 0; i < NIGHT_LIGHT_LEDS_NUMBER; i++) 
      {
          leftBedsideTableTransactions[i].ToColor(leftBedsideTableLeds[i], CRGB::White , TRANSACTION_STEPS);
          rightBedsideTableTransactions[i].ToColor(rightBedsideTableLeds[i], CRGB::White , TRANSACTION_STEPS);
      }
      
      isMax = true;
    }else
    {
      leftTNLButtonState = false;
      rightTNLButtonState = false;

      for (int i = 0; i < NIGHT_LIGHT_LEDS_NUMBER; i++) 
      {
          leftBedsideTableTransactions[i].ToColor(leftBedsideTableLeds[i], CRGB::Black , TRANSACTION_STEPS);
          rightBedsideTableTransactions[i].ToColor(rightBedsideTableLeds[i], CRGB::Black , TRANSACTION_STEPS);
      }

      isMax = false;
    }

}


void ApplyBedCurrentTemplate()
{
    for (int i = BED_START_INDEX; i < BED_END_INDEX; i++) 
    {
      bedTransactions[i].ToColor(bedLeds[i], CRGB(currentBedTemplate[i-BED_START_INDEX]) , TRANSACTION_STEPS);
    }
}

// Night light

void RightNightLightSwitch()
{
    isMax = false;
    rightTNLButtonState = !rightTNLButtonState;
    
    for (int i = 0; i < NIGHT_LIGHT_LEDS_NUMBER; i++) 
    {
      if (rightTNLButtonState)
      {
        rightBedsideTableTransactions[i].ToColor(CRGB::Black, CRGB(currentRightNightLightTemplate[i]) , TRANSACTION_STEPS);
      }
      else{
        rightBedsideTableTransactions[i].ToColor(rightBedsideTableLeds[i], CRGB::Black , TRANSACTION_STEPS);
      }
    }
}
void LeftNightLightSwitch()
{
    isMax = false;
    leftTNLButtonState = !leftTNLButtonState;

    for (int i = 0; i < NIGHT_LIGHT_LEDS_NUMBER; i++) 
    {
      if (leftTNLButtonState)
      {
        leftBedsideTableTransactions[i].ToColor(CRGB::Black, CRGB(currentLeftNightLightTemplate[i]) , TRANSACTION_STEPS);
      }
      else{
        leftBedsideTableTransactions[i].ToColor(leftBedsideTableLeds[i], CRGB::Black , TRANSACTION_STEPS);
      }
    }
}

// Drawers Bed

void Left_B_Drawer1_Open()
{
  
  for (int i = LEFT_B_DRAWER1_START_INDEX; i <= LEFT_B_DRAWER1_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB::White , TRANSACTION_STEPS);
    bedTransactions[i].Lock();
  }
}
void Left_B_Drawer1_Close()
{
  for (int i = LEFT_B_DRAWER1_START_INDEX; i <= LEFT_B_DRAWER1_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB(currentBedTemplate[i-BED_START_INDEX]) , TRANSACTION_STEPS);
  }
}

void Left_B_Drawer2_Open()
{
  
  for (int i = LEFT_B_DRAWER2_START_INDEX; i <= LEFT_B_DRAWER2_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB::White , TRANSACTION_STEPS);
    bedTransactions[i].Lock();
  }
}
void Left_B_Drawer2_Close()
{
  for (int i = LEFT_B_DRAWER2_START_INDEX; i <= LEFT_B_DRAWER2_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB(currentBedTemplate[i-BED_START_INDEX]) , TRANSACTION_STEPS);
  }
}

void Right_B_Drawer1_Open()
{
  
  for (int i = RIGHT_B_DRAWER1_START_INDEX; i <= RIGHT_B_DRAWER1_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB::White , TRANSACTION_STEPS);
    bedTransactions[i].Lock();
  }
}
void Right_B_Drawer1_Close()
{
  for (int i = RIGHT_B_DRAWER1_START_INDEX; i <= RIGHT_B_DRAWER1_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB(currentBedTemplate[i-BED_START_INDEX]) , TRANSACTION_STEPS);
  }
}

void Right_B_Drawer2_Open()
{
  
  for (int i = RIGHT_B_DRAWER2_START_INDEX; i <= RIGHT_B_DRAWER2_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB::White , TRANSACTION_STEPS);
    bedTransactions[i].Lock();
  }
}
void Right_B_Drawer2_Close()
{
  for (int i = RIGHT_B_DRAWER2_START_INDEX; i <= RIGHT_B_DRAWER2_END_INDEX; i++) 
  {
    bedTransactions[i].UnLock();
    bedTransactions[i].ToColor(bedLeds[i], CRGB(currentBedTemplate[i-BED_START_INDEX]) , TRANSACTION_STEPS);
  }
}

// Drawers T

void Left_T_Drawer1_Open()
{
  //Serial.println("Left_T_Drawer1_Open");
  
  for (int i = T_DRAWER1_START_INDEX; i <= T_DRAWER1_END_INDEX; i++) 
  {
    
    leftBedsideTableTransactions[i].UnLock();
    leftBedsideTableTransactions[i].ToColor(CRGB::Black, CRGB::White , TRANSACTION_STEPS);
    leftBedsideTableTransactions[i].Lock();
  }
}
void Left_T_Drawer1_Close()
{
  //Serial.println("Left_T_Drawer1_Close");
  for (int i = T_DRAWER1_START_INDEX; i <= T_DRAWER1_END_INDEX; i++) 
  {
    leftBedsideTableTransactions[i].UnLock();
    leftBedsideTableTransactions[i].ToColor(CRGB::White, CRGB::Black , TRANSACTION_STEPS);
  }
}

void Left_T_Drawer2_Open()
{
  //Serial.println("Left_T_Drawer2_Open");
  
  for (int i = T_DRAWER2_START_INDEX; i <= T_DRAWER2_END_INDEX; i++) 
  {
    leftBedsideTableTransactions[i].UnLock();
    leftBedsideTableTransactions[i].ToColor(CRGB::Black, CRGB::White , TRANSACTION_STEPS);
    leftBedsideTableTransactions[i].Lock();
  }
}
void Left_T_Drawer2_Close()
{

  //Serial.println("Left_T_Drawer2_Close");
  
  for (int i = T_DRAWER2_START_INDEX; i <= T_DRAWER2_END_INDEX; i++) 
  {
    leftBedsideTableTransactions[i].UnLock();
    leftBedsideTableTransactions[i].ToColor(CRGB::White, CRGB::Black , TRANSACTION_STEPS);
  }
}

void Right_T_Drawer1_Open()
{
  Serial.println("Right_T_Drawer1_Open");
  
  for (int i = T_DRAWER1_START_INDEX; i <= T_DRAWER1_END_INDEX; i++) 
  {
    //rightBedsideTableTransactions[i].UnLock();
    rightBedsideTableTransactions[i].ToColor(CRGB::Black, CRGB::White , TRANSACTION_STEPS);
    //rightBedsideTableTransactions[i].Lock();
  }
}
void Right_T_Drawer1_Close()
{
  Serial.println("Right_T_Drawer1_Close");
  
  for (int i = T_DRAWER1_START_INDEX; i <= T_DRAWER1_END_INDEX; i++) 
  {
    //rightBedsideTableTransactions[i].UnLock();
    rightBedsideTableTransactions[i].ToColor(CRGB::White, CRGB::Black , TRANSACTION_STEPS);
  }
}

void Right_T_Drawer2_Open()
{
  Serial.println("Right_T_Drawer2_Open");
  
  for (int i = T_DRAWER2_START_INDEX; i <= T_DRAWER2_END_INDEX; i++) 
  {
    //rightBedsideTableTransactions[i].UnLock();
    rightBedsideTableTransactions[i].ToColor(CRGB::Black, CRGB::White , TRANSACTION_STEPS);
    //rightBedsideTableTransactions[i].Lock();
  }
}
void Right_T_Drawer2_Close()
{
  Serial.println("Right_T_Drawer2_Close");
  
  for (int i = T_DRAWER2_START_INDEX; i <= T_DRAWER2_END_INDEX; i++) 
  {
    //rightBedsideTableTransactions[i].UnLock();
    rightBedsideTableTransactions[i].ToColor(CRGB::White, CRGB::Black , TRANSACTION_STEPS);
  }
}


// Detectors

void Left_PIR_Detected()
{
    if (timeClock.isNightNow() && !isHardOn)
    {
      for (int i = BED_START_INDEX; i < BED_END_INDEX/2; i++) 
      {
        bedTransactions[i].ToColor(CRGB::Black, CRGB::White , TRANSACTION_STEPS);
      }
    }
}
void Left_PIR_Clear()
{
    if (timeClock.isNightNow() && !isHardOn)
    {
      for (int i = BED_START_INDEX; i < BED_END_INDEX/2; i++) 
      {
        bedTransactions[i].ToColor(CRGB::White, CRGB::Black , TRANSACTION_STEPS);
      }
    }
}

void Right_PIR_Detected()
{
    if (timeClock.isNightNow() && !isHardOn)
    {
      for (int i = BED_END_INDEX/2; i < BED_END_INDEX; i++) 
      {
        bedTransactions[i].ToColor(CRGB::Black, CRGB::White , TRANSACTION_STEPS);
      }
    }
}
void Right_PIR_Clear()
{
    if (timeClock.isNightNow() && !isHardOn)
    {
      for (int i = BED_END_INDEX/2; i < BED_END_INDEX; i++) 
      {
        bedTransactions[i].ToColor(CRGB::White, CRGB::Black , TRANSACTION_STEPS);
      }
    }
}

#pragma region Animations

void pride()
{
 static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < BED_LEDS_NUMBER; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (BED_LEDS_NUMBER-1) - pixelnumber;
    
    CRGB temp = bedLeds[pixelnumber];  
    nblend( temp, newcolor, 64);
    setbedLeds(pixelnumber, temp);
  }
}



CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };


void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( bedLeds, BED_LEDS_NUMBER, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < BED_LEDS_NUMBER; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = bedLeds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      setbedLeds(i, bedLeds[i] + CRGB( overage, overage2, qadd8( overage2, overage2)));
    }
  }

  // Deepen the blues and greens a bit
  for( uint16_t i = 0; i < BED_LEDS_NUMBER; i++) {
    auto temp = bedLeds[i];

    temp.blue = scale8( temp.blue,  145); 
    temp.green= scale8( temp.green, 200); 
    temp |= CRGB( 2, 5, 7);

    setbedLeds(i, temp);
  }
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < BED_LEDS_NUMBER; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);

    setbedLeds(i, bedLeds[i] + c);
  }
}

// Palette definitions
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// Define variables used by the sequences.
int      twinkrate = 100;                                     // The higher the value, the lower the number of twinkles.
uint8_t  thisdelay =  10;                                     // A delay value for the sequence(s).
uint8_t   thisfade =   8;                                     // How quickly does it fade? Lower = slower fade rate.
uint8_t    thishue =  50;                                     // The hue.
uint8_t    thissat = 255;                                     // The saturation, where 255 = brilliant colours.
uint8_t    thisbri = 255;                                     // Brightness of a sequence.
bool       randhue =   1;                                     // Do we want random colours all the time? 1 = yes.

void aatemplate()
{
  uint8_t secondHand = (millis() / 1000) % 10;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case 0: thisdelay = 10; randhue = 1; thissat=255; thisfade=8; twinkrate=10; break;  // You can change values here, one at a time , or altogether.
      case 5: thisdelay = 100; randhue = 0;  thishue=random8(); thisfade=2; twinkrate=20; break;
      case 10: break;
    }
  }                                              // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    if (random8() < twinkrate) 
    {
      auto i = random16(BED_LEDS_NUMBER);

      setbedLeds(i, bedLeds[i] + ColorFromPalette(currentPalette, (randhue ? random8() : thishue), 255, currentBlending));
    }
    fadeToBlackBy(bedLeds, BED_LEDS_NUMBER, thisfade);
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
}

static int16_t dist;                                          // A moving location for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.

uint8_t maxChanges = 24;                                      // Value for blending between palettes.

void mover()
{
    EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
    
    uint8_t locn = inoise8(xscale, dist+yscale) % 255;          // Get a new pixel location from moving noise.
    uint8_t pixlen = map(locn,0,255,0,BED_LEDS_NUMBER);                // Map that to the length of the strand.
    setbedLeds(pixlen, ColorFromPalette(currentPalette, pixlen, 255, LINEARBLEND));   // Use that value for both the location as well as the palette index colour for the pixel.

    dist += beatsin8(10,1,4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.                                             
                                         // Update the LED array with noise at the new location
    fadeToBlackBy(bedLeds, BED_LEDS_NUMBER, 4);     
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
}

#pragma endregion Animations
