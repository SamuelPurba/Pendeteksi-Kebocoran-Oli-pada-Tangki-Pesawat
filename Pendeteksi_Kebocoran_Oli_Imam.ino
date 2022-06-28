/*Library LCD*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

/*XKC-Y25 (Sensor Non-Contact Liquid Level )*/    
int potPin = 2;   // select the input pin for the potentiometer
#define Non 3// Non-Contant Liquid Level Sensor

#define ledRed 4       
#define ledGreen 5       

#define buzzer 7       

/*TCS3200 (Sensor Warna)*/
#define s0 8       
#define s1 9
#define s2 10
#define s3 11
#define out 12

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int val = 0;       // variable to store the value coming from the sensor
int val_max;

int RGB_max;

int data=0;        //This is where we're going to stock our values
int Liquid_level=0;

void setup() 
{  
   lcd.init();                      // initialize the lcd 
   lcd.backlight();

   pinMode(s0, OUTPUT);    //pin modes
   pinMode(s1, OUTPUT);
   pinMode(s2, OUTPUT);
   pinMode(s3, OUTPUT);
   pinMode(out, INPUT);

   pinMode(buzzer, OUTPUT);
   pinMode(ledRed, OUTPUT);
   pinMode(ledGreen, OUTPUT);
   
   pinMode(Non, INPUT);
   //pinMode(potPin, INPUT);
   
   digitalWrite(buzzer, LOW);
   digitalWrite(ledRed, LOW);
   digitalWrite(ledGreen, LOW);
   
   digitalWrite(Non, LOW);
   //analogWrite(potPin, LOW);
   
   Serial.begin(9600);   //intialize the serial monitor baud rate
   
   digitalWrite(s0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
   digitalWrite(s1,HIGH); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
   
}

void TCS()
{
   lcd.setCursor(0, 1);
   
   digitalWrite(s2,LOW);        //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
   digitalWrite(s3,LOW);
   Serial.print("Red value= "); 
   lcd.print("R= "); 
   GetData();

   digitalWrite(s2,HIGH);
   digitalWrite(s3,HIGH);
   Serial.print("Green value= ");
   lcd.print("G= "); 
   GetData();

   digitalWrite(s2,LOW);
   digitalWrite(s3,HIGH);
   Serial.print("Blue value= ");
   lcd.print("B= "); 
   GetData();
   Serial.println();

   delay(500);
  
}

void loop()                  //Every 2s we select a photodiodes set and read its data
{
  Potensio();
  TCS();
  logika_void();
}

void Potensio()
{
  val = analogRead(potPin);    // read the value from the sensor
  val_max = 117;
  RGB_max = 1023;
}

void Sensor_Non()
{
      Liquid_level = digitalRead(Non);
      if(Liquid_level != 0)
      {
        digitalWrite(buzzer, HIGH);
      }else
      {
        digitalWrite(buzzer, LOW);
      }
      
      Serial.print("Liquid_level= ");
      Serial.print(Liquid_level,DEC);
      Serial.print(" || ");
      
      lcd.setCursor(0, 0);
      
      lcd.print("Liquid_level= ");
      lcd.print(Liquid_level,DEC);
      lcd.print(" || ");
      
      TCS();
     
   if(data < 62)
   {
      logika_selain_hitam();
      Sensor_Non();
   }else if(data >= 62)
   {
      logika_hitam();
      Sensor_Non();
   }
}

void logika_hitam()
{
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
      
      digitalWrite(Non, HIGH);
}

void logika_selain_hitam()
{
      digitalWrite(ledRed, LOW);
      digitalWrite(ledGreen, HIGH);
      
      digitalWrite(Non, LOW);
}

void logika_void()
{
   if(data < 62)
   {
      logika_selain_hitam();
      Sensor_Non();
   }else if(data >= 62)
   {
      logika_hitam();
      Sensor_Non();
   }
}

void GetData(){
   data=pulseIn(out,LOW);       //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again
   Serial.print(data);          //it's a time duration measured, which is related to frequency as the sensor gives a frequency depending on the color
   Serial.print("\t");          //The higher the frequency the lower the duration
   
   lcd.print(data);          //it's a time duration measured, which is related to frequency as the sensor gives a frequency depending on the color
   lcd.print("\t");          //The higher the frequency the lower the duration
   
   delay(20);
}
