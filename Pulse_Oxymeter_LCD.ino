

#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
const int rs = 14, en = 16, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
//R/W to ground
//For Arduino Micro pro
//SDA to 2
//SCL to 3
int flag=0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define REPORTING_PERIOD_MS     1000
byte smiley[8] ={
                0b00000,
                0b01010,
                0b11111,
                0b11111,
                0b11111,
                0b01110,
                0b00100,
                0b00000};

PulseOximeter pox;
int pulse,spo2,pulseold=70;
uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
  //  Serial.println("Beat!");
}

void setup()
{   lcd.createChar(0, smiley);
    lcd.begin(16, 2);
     //Serial.begin(115200);
    //Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    // Register a callback for the beat detection
   // pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
     //lcd.clear(); 
     pulse=pox.getHeartRate();
     spo2=pox.getSpO2();
     tsLastReport = millis();
     if((pulse>50) && (pulse<150) && (pulse-pulseold)<2 && (spo2>20))
     { 
        lcd.clear();
        lcd.setCursor(0,0);      
        lcd.write(byte(0));
        lcd.print("Heart rate:");  
        lcd.print(pulse);
        lcd.write(byte(0));
        lcd.setCursor(4,1);
        lcd.print("SpO2:");
        lcd.print(spo2);
        lcd.print("%");
     }
     else if((pulse<50) || (pulse>150))
     {
      lcd.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Please wait !");      
      }
     pulseold=pulse;
     pulse=0;    
    }
}
