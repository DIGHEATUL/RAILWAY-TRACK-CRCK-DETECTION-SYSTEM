
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial gsm(2, 3);


TinyGPS gps;

#define IR1 A1
#define IR2 A4
#define E1 10
#define I11 A2
#define I12 A3
#define E2 11
#define I21 A5
#define I22 A6
#define BUZZ A0

const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int IRVAL1, IRVAL2;


void setup() {

  Serial.begin(9600);
  gsm.begin(9600);

  pinMode(E1, OUTPUT);
  pinMode(I11, OUTPUT);
  pinMode(I12, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(I21, OUTPUT);
  pinMode(I22, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  digitalWrite (BUZZ, HIGH);
  delay(2000);
  digitalWrite (BUZZ, LOW);

  lcd.begin(16, 2);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Railway Track");
  lcd.setCursor(0, 1);
  lcd.print("Crack Detection");
  delay(5000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Using");
  lcd.setCursor(0, 1);
  lcd.print("   GSM & GPS   ");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gsm Initialising");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Searching For");
  lcd.setCursor(4, 1);
  lcd.print("Network");
  delay(8000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gsm Ready !!");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gps Initialising");
  delay(15000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gps Ready !!");
  delay(3000);

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("All Ok !!");

}

void loop() {

  IRVAL1 = digitalRead(IR1);
  IRVAL2 = digitalRead(IR2);

  if (IRVAL2 == HIGH)
  {
    digitalWrite(BUZZ, HIGH);
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    digitalWrite(I11, LOW);
    digitalWrite(I12, LOW);
    digitalWrite(I21, LOW);
    digitalWrite(I22, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CRACK DETECTED");
    delay(2000);

    digitalWrite(BUZZ, LOW);

    loc();

  }
  if (IRVAL1 == HIGH)
  {
    digitalWrite(BUZZ, HIGH);
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    digitalWrite(I11, LOW);
    digitalWrite(I12, LOW);
    digitalWrite(I21, LOW);
    digitalWrite(I22, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CRACK DETECTED");
    delay(2000);

    digitalWrite(BUZZ, LOW);

    loc();

  }

  else
  {

    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("All Ok !!");
    delay(1000);


    analogWrite(E1, 195);
    analogWrite(E2, 195);
    digitalWrite(I11, HIGH);
    digitalWrite(I12, LOW);
    digitalWrite(I21, HIGH);
    digitalWrite(I22, LOW);
  }
}


void loc()
{
  if (Serial.available())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    Getting    ");
    lcd.setCursor(0, 1);
    lcd.print("    Location    ");
    delay(2200);

    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;

    // For one second we parse GPS data and report some key values
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
      while (Serial.available())
      {
        char c = Serial.read();
        Serial.print(c);
        if (gps.encode(c))
          newData = true;
      }
    }

    if (newData)      //If newData is true
    {
      float flat, flon;
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("    Location    ");
      lcd.setCursor(0, 1);
      lcd.print("   Collected   ");
      delay(3200);


      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sending SMS !!");
      delay(2000);

      gsm.print("AT+CMGF=1\r");
      delay(1000);

      gsm.print("AT+CMGS=\"+919766817901\"\r");
      delay(1000);

      gsm.println("CRACK DETECTED !!");
      delay(500);
      gsm.println("TAKE ACTION");
      delay(500);
      gsm.println("Location Link- ");
      delay(500);
      gsm.print("https://www.google.com/maps/?q=");
      gsm.print(flat, 6);
      gsm.print(",");
      gsm.println(flon, 6);
      delay(500);


      gsm.write(0x1A);
      delay(1000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SMS Sent !!");
      delay(3000);

    } else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Getting Location");
      lcd.setCursor(0, 1);
      lcd.print("     Failed     ");
      delay(4000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please Check for");
      lcd.setCursor(0, 1);
      lcd.print("   GPS Network  ");
      delay(4000);


  }
}
}
