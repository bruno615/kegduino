//Kegerator Temp display.  Turns compressor on above target and off below target

#include <LiquidCrystal.h>

// Setup the LCD Display
// Attach to pins    12 13 14  15  16
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Specify the temp sensor input pin (A0)
int tempPin = 0;

// Kegerator Cooling settings
float targetTemp = 77;
float maxTemp = 80;
float restTime = 60;  //In Seconds (Default = 300s)

// Declare Variables
boolean isCompressorActive = false;
unsigned long startCompressorTime = 0;
unsigned long startCoolDownTime = 0;
unsigned long secondsON = 100;
unsigned long secondsOFF = 100; 

unsigned long counterStart = 0;
unsigned long seconds = 0;

void setup() {
  //For debugging
  Serial.begin(9600);

  //Setup for led/compressor
  pinMode(5, OUTPUT);

  //Setup for LCD screen size - (X, Y)
  lcd.begin(16, 2);

  //Display initial message
  lcd.setCursor(0,0);
  lcd.write("Kegduino Loading");
  lcd.setCursor(0,1);
  lcd.write("bruno615/kegduino");

  delay(2000);

}

void loop() {
  // Read the temperature using a LM35 Sensor
  // explained here: http://www.instructables.com/id/ARDUINO-TEMPERATURE-SENSOR-LM35/
  float reading = analogRead(tempPin);
  float tempC = reading * (5.0 * 1000 / 1024) / 10;
  float tempF = tempC * 9 / 5 + 32; // Standard temp conversion

  //Remove this Serial part after debugging
  Serial.print(tempF);
  Serial.println("*F  ");
  Serial.println(isCompressorActive);
  Serial.println(secondsON);
  Serial.println(secondsOFF);

  // Figure out current status
  boolean isCool = tempF <= targetTemp;
  boolean isTooHot = tempF > maxTemp;
  boolean isCompressorRested = secondsOFF > restTime;


 // Main Loop
  if (!isCompressorActive && isTooHot && isCompressorRested) {
    // Turn Compressor ON if OFF & above max temp & rested
      turnCompressorON();
      printScreen(tempF, "Compressor On");
      delay(2000);
  } else if (isCompressorActive && isCool)  {
    // Turn Compressor OFF if ON & below target temp
      turnCompressorOFF();
      printScreen(tempF, "Compressor Off");
      delay(2000);
  } else {
    // Write Temperature and Message to LCD Screen
    printScreen(tempF, "CitraSaison 6.4%");
    
    // Count the seconds - Check this for flaws
    secondsON = round((millis()-startCompressorTime)/1000);
    secondsOFF = round((millis()-startCoolDownTime)/1000);
  }
  
  //Blink if compressor is active
//  if (isCompressorActive) {
//    lcd.setCursor(16,1);
//    lcd.blink();
//    Serial.println("blink");
//  } else {
//    lcd.noBlink();
//  }
  delay(1000);
}

//Functions////////////////////////////////////////////////////////////

void turnCompressorON() {
  digitalWrite(5, HIGH);//Turn on the compressor
  isCompressorActive = true;
  startCompressorTime = millis();  //
  //lcd.write("Compressor Go!!!");
}

void turnCompressorOFF() {
  digitalWrite(5, LOW); //Shut off compressor
  isCompressorActive = false;
  startCoolDownTime = millis();
  //lcd.write("All Better!");
}

void printScreen(float tempF, const char* message) {
  lcd.clear();
  lcd.print(message);
  lcd.setCursor(0,1);
  lcd.write("Temp:");
  lcd.setCursor(6,1);
  lcd.print(round(tempF));
  lcd.setCursor(8,1);
  lcd.write("*F");
}
