# Car-System
The project is a remote control car that has 3 main features; passive lane detection, sound system, Warning Indicators and Power Mirrors. 
Lane detection: warning if the car is getting out of the lane. 
Sound System: a sound system using an mp3 and 3 buttons to control it. 
Warning indicators: warnings if the fuel level is low, if there is rain and if the seat belt is not buckled. All the warnings are through an lcd display however the seatbelt has an additional audible warning. 
Power mirrors: allows you to rotate the mirrors of the car and indicating that the mirror’s being rotated through an lcd. 

# Components and functionality: 

# LDW feature: 
1. Line tracking sensor:  senses whenever the colour of the ground changes from white to black and vice versa.
2. LED: turns on when switching lanes.
3. Buzzer: turns on when switching lanes.  


# Rain Detection:
1. Rain sensor module consisting of a sensor and a raining board whenever water drops fall on the raining board the sensor senses it and a led is turned on indicating rainfall.
2. LCD: displays rain when there is rain. 

# Fuel level: 
1. Water level sensor: outputs 0 when dry and outputs a positive number when it is dumped under water, the number increases according to the water level. 
2. LCD: displays L when fuel level is low. 

# Power mirrors: 
1. Servo motor: rotates clockwise and anticlockwise to simulate rotating the mirror.
2. Joystick: move it horizontally to control the servo motor.
3. LCD: displays mirror to indicate that the mirror is rotating. 

# Seat-belt: 
1. Button: simulates the belt buckle, when pressed means the belt is buckled.
2. Buzzer: turns when if belt is not buckled as a warning. 
3. LCD: displays seat belt to inform the driver to buckle up their seat belt. 

# Sound System: 
1. Df player: it is an mp3 player module. 
2. Speaker: outputs sound. 
3. Three Buttons: previous, next and play/pause buttons. 
==================================================================================

# Libraries:
1. Arduino_FreeRTOS: included to be able to use scheduling. 
2. Semphr: included to be able to use semaphores. 
3. LiquidCrystal_I2C: included to be able to use the lcd. 
4. Servo: included to be able to use the servo motor for the mirror 
5. AFMotor: included to be able to control the motors. 

===================================================================================

# INPUTS: 
How do you take and handle the inputs? 

# LDW FEATURE: 
Input is taken from the line tracking sensor that is connected to pin 27 in the Arduino, when switching lanes the sensor (digitalRead) returns high. 
# Rain detection: 
The rain detection sensor is connected to pin 24 in the Arduino, when rain is detected the sensor (digitalRead) returns LOW. 
# Fuel Level: 
The water level sensor is connected to pin A9 in the Arduino, when there is no water analogRead(A9) returns 0 else it returns a positive number according to the water level. 
# Power mirror: 
The joystick (horizontal direction only) is connected to pin A8 in the Arduino, whenever the joystick is moved in the X direction (horizontally), analogRead(A8) returns values from 0 to 1008, when the joystick is in the middle the value is 500. 
# Seat-belt: 
The button used to simulate the seat-belt is connected to pin 23, when the button is pressed digitalRead (23) returns 1 else it returns 0. 
# Sound System feature: 
Three buttons for previous, next and play/pause, they are connected to pins 45, 51 and 47 respectively. When any of the buttons are pressed digitalRead returns 1 else it returns 0.   

=================================================================================

# OUTPUTS: 

# LDW feature: 
The LED is connected to pin 28 and the buzzer is connected to pin 29, when the line tracking sensor returns HIGH, the signal of both the led and the buzzer are driven to high using digitalWrite. 
# Warning Indicators and Power Mirrors: 
The LCD is connected to the scl and sda in the Arduino and of course  
To the gnd and vcc. To be able to use the LCD we first write LiquidCrystal_I2C lcd(0x27,20,4);  to set the LCD address to 0x27 for a 16 chars and 2 line display then lcd.init(); and to write something on the lcd we first need to set the position of the output using lcd.setCursor(row,col); and then to print we use lcd.print(" output "); 
# Seat-belt: 
In addition to the use of the lcd a buzzer is used, it is connected to pin 22, when the seat belt button is not pushed we write high to pin 22 else we write LOW. 
# Sound System: 
A speaker is used to output the sound, it is connected to the dfplayer. 

==================================================================================

# FreeRTOS: 
Features were divided into tasks according to their importance and priority. We decided that the LDW feature is the most important so it is placed in a separate task and its priority is set to be the highest, next the rain and fuel level detection modules were placed in a task and the seat belt and mirror power modules were placed in another task and their priority are set to the next highest priority. Finally the sound system is placed in another task with the least priority. 
