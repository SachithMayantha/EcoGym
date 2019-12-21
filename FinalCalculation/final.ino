#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //defining lcd pins

const int dataIN = 2; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference
unsigned long lcdrefresh; // To store time for lcd to refresh

float rpm = 0.0, rpm2 = 0.0; // RPM value

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan
float voltage;
float rev = 0.0;
int oldTime = 0;
int time = 0;
float cal = 0.0;
float cal2 = 0.0;
int ledPin = 11;
int buckPin;
int state1 = 0;
int state2 = 0;
float eng = 0.0;

void setup()
{
  pinMode(ledPin, OUTPUT);
  //pinMode(buckPin, OUTPUT);
  //pinMode(buckPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  //digitalWrite(buckPin, LOW);
  Serial.begin(38400);
  //Serial.begin(9600);
  pinMode(dataIN, INPUT);
  pinMode(A1, INPUT);
  lcd.begin(16, 2);
  prevmillis = 0;
  prevstate = LOW;
  attachInterrupt(0, isr, RISING);
}

void loop()
{

  delay(1000);
  buckPin = analogRead(A0);
  //Serial.println(buckPin);

  voltage = analogRead(A1);
  detachInterrupt(0);
  time = millis() - oldTime;
  rpm = (rev / time) * 60000;

  oldTime = millis();
  rev = 0.0;

  if (Serial.available()) { // Checks whether data is comming from the serial port
    state1 = Serial.read();
    state2 = Serial.read();// Reads the data from the serial port
    //Serial.println(state1);
  }
  if (state1 == 0) {
    digitalWrite(ledPin, LOW); // Turn LED OFF
    //Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
    //state1 = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calories = ");
    lcd.print(0.0);
    lcd.setCursor(0, 1);
    lcd.println("Not Charging");
    cal = 0.0;
    eng = 0.0;
    cal2=0.0;

  }
  else if (state1 == 120) {
    digitalWrite(ledPin, HIGH);
    //Serial.println("LED: ON");
    //state1 = 0;
    rpm2 = rpm / 3.25;
    cal += 0.0008 * rpm2;
    if (buckPin > 900)
    {
      //Serial.println("ON");
      eng += 5.3 * 1.0 * time / 1000.0;
      cal2+= 0.0008 * rpm2;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Calories = ");
      lcd.print(cal);
      //lcd.print(" kcl");
      lcd.setCursor(0, 1);
      lcd.println("Energy = ");
      lcd.print(eng);
    }
    //eng += 5.3 * 1.0 * time / 1000.0;
    //voltage = voltage * 2.0 * 8.00 / 1024 ;
    //delay(1000);
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Calories = ");
      lcd.print(cal);
      //lcd.print(" kcl");
      lcd.setCursor(0, 1);
      lcd.println("Not Charging");
      //lcd.print(eng);
    }

    //lcd.print(" J");
    //Serial.print(rpm2);
    //Serial.print("  ");
    Serial.print(cal *4184);
    Serial.print(" J");
    Serial.print("   ");
    Serial.print(cal2 *4184);
    Serial.print(" J");
    Serial.print("   ");
    Serial.print(eng);
    Serial.print(" J");
    Serial.print("   ");
    Serial.print(eng*100/(cal2*4184));
    Serial.println(" %");
    //Serial.println();
    attachInterrupt(0, isr, RISING);
  }

  //if ( ( millis() - lcdrefresh ) >= 100 )
  //{

  //delay(100);
  //lcdrefresh = millis();

  //}
}

void isr()
{
  rev++;
}
