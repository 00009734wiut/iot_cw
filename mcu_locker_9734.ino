#define BLYNK_TEMPLATE_ID "TMPLDsvUJQck"
#define BLYNK_DEVICE_NAME "Locker"
#define BLYNK_AUTH_TOKEN "MlY5q9jSzoqtxvE8m_q1l_BD8OoDYZK1"

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <string>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// wifi
char ssid[] = "AndroidAP52";
char pass[] = "1987654320";

char auth[] = "MlY5q9jSzoqtxvE8m_q1l_BD8OoDYZK1";

//lcd
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

// servo
Servo myservo;
int servo_pos = 0;

//leds
const int redLed = D8;
const int greenLed = D9;

//keypad
String password = "1234";
String typing = String();

const byte rows = 3;
const byte columns = 4;
// Keypad pin map
char hexaKeys[rows][columns] = {
  {'1', '2', '3'},
  {'4', '5', '6'}
};
// Initializing pins for keypad
// byte row_pins[rows] = {D0, D3, D4};
byte row_pins[rows] = {D0, D3};
byte column_pins[columns] = {D4, D6, D7};

// Create instance for keypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void grant_access(int notification = 1){
  Blynk.virtualWrite(V2, "Opened");
  if (notification){
    Blynk.virtualWrite(V4, "Access granted");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Access granted");
  digitalWrite(greenLed, HIGH);
  myservo.write(180);              // tell servo to go to position in variable 'pos'  
  delay(5000);
  
  Blynk.virtualWrite(V2, "Closed");
  // Blynk.virtualWrite(V1, pinValue);  
  myservo.write(servo_pos);              // tell servo to go to position in variable 'pos'  
  digitalWrite(greenLed, LOW);
  def_lcd();
  typing = String();  
}

void deny_access(){
  Blynk.virtualWrite(V2, "Red");
  Blynk.virtualWrite(V4, "Access denied");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wrong password");
  digitalWrite(redLed, HIGH);
  delay(1000);

  Blynk.virtualWrite(V2, "Closed");
  digitalWrite(redLed, LOW);
  def_lcd();
  typing = String();    
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  if (pinValue){

  grant_access(0);
  Blynk.virtualWrite(V1, 0);
  }

}

void setup() {
  myservo.attach(D5);
  myservo.write(0);
  Serial.begin(9600);
  Blynk.config(auth);
  Blynk.connectWiFi(ssid, pass);
  
  // lcd
  lcd.init();
  def_lcd();

  deny_access();  

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void loop() {
  // Serial.println("Started");
  Blynk.run();
  char myKey = keypad_key.getKey();

  if (myKey != NULL){
    typing += myKey;
    if (typing.length() == 4){
      if (typing == password){
        grant_access();
      } else {
        deny_access();
      }
      def_lcd();
      typing = String();      
    } else {
      lcd.print('*');                    
    }
  }
}



void def_lcd(){
  lcd.clear();        
  lcd.backlight();   
  lcd.setCursor(0,0);   //Move cursor to character 2 on line 1
  lcd.print("Enter password");
  lcd.setCursor(0,1);
}

