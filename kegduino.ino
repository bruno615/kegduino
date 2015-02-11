//Kegerator Temp display.  Turns compressor on above target and off below target

#include <LiquidCrystal.h>
//float temp;
int tempPin = 0;
float targetTemp = 80;
float lowTemp = 77;
float chillTime = 30;
float restTime = 60;
boolean compressorStatus = false;
unsigned long startCompressorTime = 0;
unsigned long startCoolDownTime = 0;
unsigned long secondsON = 0;
unsigned long secondsOFF = 0;
string message = "Bruno's Kegduino" //String to display during operation

// Setup the LCD Display
// Attach to pins    12 13 14  15  16
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

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
  // Read the temperature
  // explained here: http://www.instructables.com/id/ARDUINO-TEMPERATURE-SENSOR-LM35/
  float reading = analogRead(tempPin);
  float tempC = reading * (5.0 * 1000 / 1024) / 10;
  float tempF = tempC * 9 / 5 + 32;

  //Remove this Serial part after debugging
  Serial.print(tempF);
  Serial.println("*F  ");

  // Write Temperature and Message to LCD Screen
  lcd.clear();
  printMessage(message)
  printTemp(tempF);

  // Count the seconds - Check this for flaws
  secondsON = round((millis()-startCompressorTime)/1000);
  secondsOFF = round((millis()-startCoolDownTime)/1000);

 // Main Loop
  if (!compressorStatus && tempF > targetTemp && secondsOFF > restTime) {
    // Turn Compressor on if OFF & above target temp & rested
      compressorON();
  }
  else if (compressorStatus && tempF <= targetTemp && secondsON > chillTime)  {
    // Turn Compressor off if ON & below target temp & chilled long enough
      compressorOFF();
  }
  else {
    // No change
  }
  delay(1000);
}


//Trigger activated, cooling mode
//      lcd.print("Compressor ");
//      lcd.setCursor(11,1);
//      lcd.print(seconds);

//Functions////////////////////////////////////////////////////////////

void compressorON() {
  digitalWrite(5, HIGH);//Turn on the compressor
  compressorStatus = true;
  startCompressorTime = millis();  //
  //lcd.write("Compressor Go!!!");
}

void compressorOFF() {
  digitalWrite(5, LOW); //Shut off compressor
  compressorStatus = false;
  startCoolDownTime = millis();
  //lcd.write("All Better!");
}

void printTemp(float tempF){
  lcd.setCursor(0,1);
  lcd.write("Temp:");
  lcd.setCursor(6,1);
  lcd.print(round(tempF));
  lcd.setCursor(8,1);
  lcd.write("*F");
}

void printMessage(string message) {
  lcd.setCursor(0,0);
  lcd.write(message)
}
