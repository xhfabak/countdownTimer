int RXLED = 17;  // The RX LED has a defined Arduino pin;
int BUZZER = 16;  // BUZZER ;
int LCDRW = 4;  // LCD display 'RW' pin;
int BUTPLUS = 15;  // BUTTON (LED light (+));
int BUTNO = 14;  // BUTTON (NO connection);
#include <LiquidCrystal.h>
LiquidCrystal LCD(3, 5, 6, 7, 8, 9);  // (RS, E, D4, D5, D6, D7) LCD display pins;
signed short minutes = 10, seconds = 0, milliseconds = 0;  // Set time for Countdown timer;
char timeline[16];  // Display timer
int count = 0;  // for button to check how long button is pressed;
boolean action = true; // for button and "count";
boolean treset = false;
boolean started = false;
volatile int output = LOW; // Interrupt pin changes state
int i = 0;
unsigned long current_time = 0;  // time
unsigned long next_update = 0;  // time
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
   pinMode(RXLED, OUTPUT);  // Set RX LED as an output  // TX LED is set as an output behind the scenes;
   pinMode(BUZZER, OUTPUT);  // BUZZER
   pinMode(LCDRW, OUTPUT);  // LCD display 'RW' pin
   pinMode(BUTPLUS, OUTPUT); digitalWrite(BUTPLUS, LOW); //  BUTTON (LED light (+)) // BUTTON LED is turned off (LOW);
   pinMode(BUTNO, INPUT_PULLUP);  // BUTTON (NO connection) << PUSH BUTTON >>
   attachInterrupt(digitalPinToInterrupt(2), Interrupt, RISING); // trigger when the pin goes from low to high,
   LCD.begin(16,2); // LCD DISPLAY TEXT
   LCD.setCursor(1,0); LCD.print("FUSSBALL TIMER");
   LCD.setCursor(3,1); LCD.print("LOADING...");
   delay(1500); LCD.clear();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void display_timer() {
  LCD.setCursor(2, 1);
  sprintf(timeline, "%0.2d : %0.2d : %0.2d", minutes, seconds, milliseconds);
  LCD.print(timeline);
  LCD.setCursor(2, 0); LCD.print("MM : SS : MS");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void shortPress()
{
  started = !started;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void longPress()
{
  reset_timer();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void reset_timer() {
  started = false;
  minutes = 10;
  seconds = 0;
  milliseconds = 0;
  buzz(100);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tmer() {
  display_timer();
  if (started) {
    update_timer();
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interrupt() {
    if (count>=1) { }   // Count 1 * 50 = 50ms
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void buzz(int duration) {
  digitalWrite(BUTPLUS, HIGH); // ON - Button yellow LED
  digitalWrite(BUZZER, HIGH);  // ON - buzzer
  delay(duration);
  digitalWrite(BUZZER, LOW);  // OFF - buzzer
  digitalWrite(BUTPLUS, LOW); // OFF - Button yellow led
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void update_timer() {
  if (minutes == 5 && seconds == 1 && milliseconds == 0) {buzz(70);}  
  if (minutes == 3 && seconds == 0 && milliseconds == 2) {buzz(90);}
  if (minutes == 1 && seconds == 0 && milliseconds == 0) {buzz(150);}
  if (minutes == 0 && seconds == 30 && milliseconds == 0) {buzz(200);}
  if (minutes == 0 && seconds <= 10 && milliseconds == 0) {buzz(200);}
  if (minutes == 0 && seconds == 0 && milliseconds == 0) {
    started = false;
    buzz(1000);
    } 

  if (seconds == 0 && minutes != 0)
  {
    seconds = 59;
    minutes --;
  }  
  else if (milliseconds == 0 && seconds != 0) 
  {
    milliseconds = 99;
    seconds --;
  }
  else {
    milliseconds --;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_press(){
   if (digitalRead(BUTNO) == LOW && action) 
   {
    digitalWrite(BUTPLUS, HIGH);  // ON - Button yellow LED
     count+=1;
     delay(50); // ms
   }
   if (digitalRead(BUTNO) == HIGH){
    digitalWrite(BUTPLUS, LOW); // OFF - Button yellow LED
    }
   if (count>=20 && action) {   // Count 20 * 50 = 1000ms (1s)
      longPress();
      count=0;
      action=false;
    }
   if (count>=2 && digitalRead(BUTNO) == HIGH && action) {   // Count 2 * 50 = 100ms
      shortPress();
      count=0;
      action=false;
    }
   if (digitalRead(BUTNO) == HIGH) {
      count=0;
      action=true;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){
  current_time = millis();

  if (current_time >= next_update) {
    tmer();
    next_update = current_time + 10;
  }

  check_press();
}
