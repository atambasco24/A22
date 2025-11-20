
#include "ssd1306.h"
#include <Arduino.h>
#include <Servo.h>
//install from searching ssd1306 in libraries by alexy dynda
//notable files include 
//ssd1306_1bit.h
//ssd1306_fonts.h
//ssd1306_generic.h 
//and everything in the direct draw folder

//github.com/lexus2k/ssd1306/blob/master/src/

const int triggerPin = 2;
const int noidPin = 4;
const int fireControlPin = 5;
const int reloadPin = 8;

unsigned long lastDebounceTimes[14];
volatile int lastPinStates[14];
volatile int pinStates[14]; //stores the state of each pin
unsigned long debounceDelay = 37;

const uint8_t magMax = 18;
uint8_t currentAmmoCount = 18;

char semiStr[2] = "S";
char burstStr[2] = "B";
char autoStr[2] = "A";
int fireMode = 1;
                      //these are toggled when a button is pressed, preventing buttons from activating twice accidentally and accounting for debounce. 
bool readyToFire = true;
bool readyFireMode = true;
bool reloadReady = true; // may need to be an interrupt on pin 3

bool getPinState(const int pinNum)
{
  bool reading = digitalRead(pinNum);
  if (reading != lastPinStates[pinNum])
  {
    lastDebounceTimes[pinNum] = millis();
  }
  if (millis() - lastDebounceTimes[pinNum] > debounceDelay)
  {
    if (reading != pinStates[pinNum]) {
      pinStates[pinNum] = reading;
    }
  }
  lastPinStates[pinNum] = reading;
  return pinStates[pinNum];
}

void setup() {
  // put your setup code here, to run once:
  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(noidPin, OUTPUT);
  pinMode(fireControlPin, INPUT_PULLUP);
  pinMode(reloadPin, INPUT_PULLUP);
    /*OLED SETUP*/
  getPinState(triggerPin);
  getPinState(noidPin);
  getPinState(fireControlPin);
  getPinState(reloadPin);
   /* Replace the line below with ssd1306_128x32_i2c_init() if you need to use 128x32 display */
    ssd1306_128x64_i2c_init();
    ssd1306_fillScreen(0x00);
    ssd1306_setFixedFont(courier_new_font11x16_digits);
    //ssd1306_setFixedFont(ssd1306xled_font8x16);
    updateAmmoCount(); //init ammo count
    //ssd1306_printFixedN(42, 16, u8"18", STYLE_BOLD, 1);
    ssd1306_charF6x8(0, 0, semiStr, STYLE_NORMAL);
    
} //end setup


void loop() {
  // put your main code here, to run repeatedly:

  if(getPinState(fireControlPin)==LOW && readyFireMode){ //
    fireMode++;
    if(fireMode > 3){
      fireMode = 1;
    }
    readyFireMode = false;
    if(fireMode == 1){
      //update screen to reflect fire mode
      ssd1306_charF6x8(0, 0, semiStr, STYLE_NORMAL);
    }
    else if(fireMode == 2){
      ssd1306_charF6x8(0, 0, burstStr, STYLE_NORMAL);
    }
    else if(fireMode == 3){
      ssd1306_charF6x8(0, 0, autoStr, STYLE_NORMAL);
    }
    else{
      //do something
    }
  }

  if(getPinState(fireControlPin)== HIGH){
    readyFireMode = true;
  }
  if(getPinState(reloadPin) == LOW && reloadReady){
    currentAmmoCount = magMax;
    updateAmmoCount();
    reloadReady = false;
  }
  if(getPinState(reloadPin) == HIGH){
    reloadReady = true;
  }

  if(getPinState(triggerPin) == LOW && readyToFire){ //if the trigger is pressed
      if(fireMode == 1){ //SEMI AUTO
          digitalWrite(noidPin, HIGH);
          delay(50);
          digitalWrite(noidPin, LOW);
          updateAmmoCount();
          readyToFire = false; //after firing, make sure the blaster cannot fire again until trigger is released
      }
      else if(fireMode == 2){ //BURST
            digitalWrite(noidPin, HIGH);
          delay(50);
          digitalWrite(noidPin, LOW);
          updateAmmoCount();
          delay(50);
          digitalWrite(noidPin, HIGH);
          delay(50);
          digitalWrite(noidPin, LOW);
          updateAmmoCount();
          delay(50);
          digitalWrite(noidPin, HIGH);
          delay(50);
          digitalWrite(noidPin, LOW);
          updateAmmoCount();
          delay(50);
          readyToFire = false; //after firing, make sure the blaster cannot fire again until trigger is released
      }
      else if(fireMode == 3){ //FULL AUTO
        digitalWrite(noidPin, HIGH);
        delay(50);
        digitalWrite(noidPin, LOW);
        updateAmmoCount();
        delay(50);
      }
       
    }

   if(getPinState(triggerPin)==HIGH){ //if the trigger is released
      readyToFire = true;  //blaster is ready to fire
    }    


} //end loop

void updateAmmoCount(){
  char ammoStr[3] = "00";
  ammoStr[0] = '0' + currentAmmoCount / 10;
  ammoStr[1] = '0' + currentAmmoCount % 10;
  
  if(currentAmmoCount > 0){
  currentAmmoCount--;
    ssd1306_printFixedN(42, 16, ammoStr, STYLE_BOLD, 1);
  } 
  else{
    ssd1306_printFixedN(42, 16, ammoStr, STYLE_BOLD, 1);
  }
}
