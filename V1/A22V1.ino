#include "ssd1306.h"
//install from searching ssd1306 in libraries by alexy dynda
//notable files include 
//ssd1306_1bit.h
//ssd1306_fonts.h
//and everything in the direct draw folder

//github.com/lexus2k/ssd1306/blob/master/src/

const int triggerPin = 2;
const int revPin = 7;
const int noidPin = 4;
const int motorPin = 6;
const int fireControlPin = 5;

const int magPin = 3;
const int reloadPin = 8;

unsigned long timeOfLastCall = 0;
unsigned long fireDelay = 100;

const uint8_t magMax = 18;
uint8_t currentAmmoCount = 18;


int fireMode = 1;
bool readyToFire = true;
bool readyFireMode = true;
bool reloadReady = true; // may need to be an interrupt on pin 3

void setup() {
  // put your setup code here, to run once:
  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(revPin, INPUT_PULLUP);
  pinMode(noidPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(magPin, INPUT_PULLUP);
  pinMode(fireControlPin, INPUT_PULLUP);
  pinMode(reloadPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(triggerPin), checkFire, FALLING);
  //attachInterrupt(digitalPinToInterrupt(magPin), reload, FALLING);

    /*OLED SETUP*/
    
   /* Replace the line below with ssd1306_128x32_i2c_init() if you need to use 128x32 display */
    ssd1306_128x64_i2c_init();
    ssd1306_fillScreen(0x00);
    ssd1306_setFixedFont(courier_new_font11x16_digits);
    ssd1306_printFixedN(42, 16, u8"18", STYLE_BOLD, 1);
    
} //end setup


void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(revPin)==LOW){
      digitalWrite(motorPin, HIGH);
    }
    
  if(digitalRead(revPin)== HIGH){
      digitalWrite(motorPin, LOW);
     }

  if(digitalRead(fireControlPin)==LOW && readyFireMode){
    fireMode++;
    if(fireMode > 3){
      fireMode = 1;
    }
    readyFireMode = false;
  }

  if(digitalRead(fireControlPin)== HIGH){
    readyFireMode = true;
  }
  if(digitalRead(reloadPin) == LOW && reloadReady){
    currentAmmoCount = magMax;
    updateAmmoCount();
    reloadReady = false;
  }
  if(digitalRead(reloadPin) == HIGH){
    reloadReady = true;
  }

  if(digitalRead(triggerPin) == LOW && readyToFire){ //if the trigger is pressed
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


    
   if(digitalRead(triggerPin)==HIGH){ //if the trigger is released
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
