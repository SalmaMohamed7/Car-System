#include <Arduino_FreeRTOS.h> 
#include <semphr.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


#include "SoftwareSerial.h"
SoftwareSerial mySerial(52, 53);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

# define ACTIVATED LOW

int buttonNext = 51;
int buttonPause = 47;
int buttonPrevious = 45;
boolean isPlaying = false;
SemaphoreHandle_t xSemaphore;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


const int beltButtonPin = 23;     // the number of the pushbutton pin 5=>22
const int beltBuzzerPin = 22;      // the number of the LED pin 3=>24

int buttonState = 0;         // variable for reading the pushbutton status

int sensor = 24; //rain drop sensor 4=>28
#define led 26 //11=>26

Servo servo1;//for mirrors P6 => p30
int joyX = A8; //A0=>A8
int servoVal;

int waterLevelPin = A9;

int laneDetection = 27;
#define LDWled 28
const int LDWbuzzer = 29;      // the number of the LED pin 3=>24

void Task1( void *pvParameters); 
void Task2( void *pvParameters);
void Task3( void *pvParameters); 
void Task4( void *pvParameters);



void setup() {
  // initialize the LED pin as an output:
  pinMode(beltBuzzerPin, OUTPUT);
    pinMode(LDWbuzzer, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(beltButtonPin, INPUT);
   pinMode(led,OUTPUT);
   pinMode(LDWled,OUTPUT);
   pinMode(waterLevelPin,INPUT);
   pinMode(laneDetection,INPUT);


  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  
  Serial.begin(9600);
   servo1.attach(25); //6=>30
  pinMode(joyX,INPUT);
  pinMode(buttonPause, INPUT);
digitalWrite(buttonPause,HIGH);
pinMode(buttonNext, INPUT);
digitalWrite(buttonNext,HIGH);
pinMode(buttonPrevious, INPUT);
digitalWrite(buttonPrevious,HIGH);

xTaskCreate (Task1, "laneDetection", 128, NULL, 3, NULL); //4
xTaskCreate (Task2, "fuel/seatBelt", 128, NULL, 4, NULL); //3
//xTaskCreate (Task4, "speaker", 128, NULL, 5, NULL);//2

//xTaskCreate (Task3, "mirror/rain", 128, NULL, 3, NULL);
//xTaskCreate (Task5, "Motor", 128, NULL, 5, NULL);


mySerial.begin (9600);
delay(1000);
  xSemaphore = xSemaphoreCreateBinary();
//playFirst();
isPlaying = true;  // da haytghayar to a semaphore
}

void loop() {
  // read the state of the pushbutton value:


}

void playFirst()
{
  execute_CMD(0x3F, 0, 0);
  delay(500);
  setVolume(30);
  delay(500);
  execute_CMD(0x11,0,1); 
  delay(500);
}

void pause()
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1); 
  delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);
  delay(500);
}

void playPrevious()
{
  execute_CMD(0x02,0,1);
  delay(500);
}

void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}
//LANE DETECTION TASK
void Task1( void *pvParameters ){
  volatile uint32_t ul; 
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while(1){
    Serial.println("task1");

    if(digitalRead(laneDetection)==HIGH){
        digitalWrite(LDWbuzzer, HIGH);
        digitalWrite(LDWled, HIGH);
  }
  else{
        digitalWrite(LDWbuzzer, LOW);
        digitalWrite(LDWled, LOW);
  }
  if (digitalRead(buttonPause) == ACTIVATED)
  {
    Serial.println("uygiyg");
        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        pause();
     //  xSemaphoreGive(xSemaphore);
    
    else{
        play();
        xSemaphoreGive(xSemaphore);
    }
    
  }
  

 if (digitalRead(buttonNext) == ACTIVATED)
  {       
    Serial.println("nnnn");
    xSemaphoreTake(xSemaphore , portMAX_DELAY);
    playNext();
    xSemaphoreGive(xSemaphore);

  }

   if (digitalRead(buttonPrevious) == ACTIVATED)
  { 
    Serial.println("pppppp");
    xSemaphoreTake(xSemaphore , portMAX_DELAY);
    playPrevious();
    xSemaphoreGive(xSemaphore);
       
  }
 // delay(700);//
  vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 50 ) );//250//50
  }
}
//FUEL LEVEL AND SEAT BELT
void Task2(void *pvParameters){
  volatile uint32_t ul;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  while(1){
    Serial.println("task2222222");

     buttonState = digitalRead(beltButtonPin);

Serial.print(buttonState);
//seatbelt feature
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  
  if (buttonState == HIGH) {
    digitalWrite(beltBuzzerPin, LOW);
     lcd.setCursor(0,0);
     lcd.print("         ");
  } else {
    digitalWrite(beltBuzzerPin, HIGH);
     lcd.setCursor(0,0);
     lcd.print("seat belt");
 
  }
  Serial.println(analogRead(waterLevelPin));

  if(analogRead(waterLevelPin) <=200 && analogRead(waterLevelPin)>10){
   lcd.setCursor(10,0);
   lcd.print("L");
   Serial.println("Low"); 
  }
  else if(analogRead(waterLevelPin)>200){
    lcd.setCursor(10,0);
    lcd.print(" ");
}
 if(digitalRead(24)==LOW){
  lcd.setCursor(0,1);
  lcd.print("rain");
  digitalWrite(led,HIGH);
}
else{
    digitalWrite(led,LOW);
    lcd.setCursor(0,1);
     lcd.print("      ");
   // Serial.println("rain not");
}
servoVal = analogRead(joyX);
servoVal = map(servoVal, 0, 1023, 0, 180);
servo1.write(servoVal);
Serial.println(servoVal);
if(servoVal>=80 && servoVal<=90){
  lcd.setCursor(6,1);
   lcd.print("      ");
 
}
else{
  lcd.setCursor(6,1);
  lcd.print("mirror");
}

//delay(500);//450
 vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 700 ) );//300--500
  }
}
////MIRROR AND RAIN
//void Task3(void *pvParameters){
//   
//  volatile uint32_t ul;
//  while(1){
//     if(digitalRead(24)==LOW){
//  lcd.setCursor(0,1);
//  lcd.print("rain");
//  digitalWrite(led,HIGH);
//}
//else{
//    digitalWrite(led,LOW);
//    lcd.setCursor(0,1);
//     lcd.print("      ");
//   // Serial.println("rain not");
//}
//servoVal = analogRead(joyX);
//servoVal = map(servoVal, 0, 1023, 0, 180);
//servo1.write(servoVal);
//Serial.println(servoVal);
//if(servoVal>=80 && servoVal<=90){
//  lcd.setCursor(6,1);
//   lcd.print("      ");
// 
//}
//else{
//  lcd.setCursor(6,1);
//  lcd.print("mirror");
//}
//delay(20);
//  }
//}
//void Task4(void *pvParamters){
////   const char *pcTaskName = “My FOURTH TASK\n";
//  volatile uint32_t ul;
//  TickType_t xLastWakeTime;
//  xLastWakeTime = xTaskGetTickCount();
////zawedt
//Serial.println("task4");
// // xSemaphore = SemaphoreCreateBinary();//zawedt
//  while(1){
//    Serial.println("taskkkkk");
//    if (digitalRead(buttonPause) == ACTIVATED)
//  {
//    Serial.println("uygiyg");
//        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
//        pause();
//     //  xSemaphoreGive(xSemaphore);
//    
//    else{
//        play();
//        xSemaphoreGive(xSemaphore);
//    }
//    
//  }
//  
//
// if (digitalRead(buttonNext) == ACTIVATED)
//  {       
//    Serial.println("nnnn");
//    xSemaphoreTake(xSemaphore , portMAX_DELAY);
//    playNext();
//    xSemaphoreGive(xSemaphore);
//
//  }
//
//   if (digitalRead(buttonPrevious) == ACTIVATED)
//  { 
//    Serial.println("pppppp");
//    xSemaphoreTake(xSemaphore , portMAX_DELAY);
//    playPrevious();
//    xSemaphoreGive(xSemaphore);
//       
//  }
// // delay(200);
//  vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 200 ) );//300
//
//  
//  }
//}
//SPEAKER
//void Task4(void *pvParamters){
////   const char *pcTaskName = “My FOURTH TASK\n";
//  volatile uint32_t ul;
//  //SemaphoreHandle_t xSemaphore;  //zawedt
////  xSemaphore = SemaphoreCreateBinary();//zawedt
//  while(1){
//    if (digitalRead(buttonPause) == ACTIVATED)
//  {
//    if(isPlaying)
//    {
//      pause();
//      isPlaying = false;
//    }else
//    {
//      isPlaying = true;
//      play();
//    }
//  }
//
//
// if (digitalRead(buttonNext) == ACTIVATED)
//  {
//    if(isPlaying)
//    {
//      playNext();
//    }
//  }
//
//   if (digitalRead(buttonPrevious) == ACTIVATED)
//  {
//    if(isPlaying)
//    {
//      playPrevious();
//    }
//  }
//  delay(200);
//  }
//}
