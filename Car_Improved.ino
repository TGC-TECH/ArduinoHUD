#include <avr/wdt.h>

//Settings-Need to Set
float radius = 2; //radius of to magnet in inches

//Pinout
int hall_pin = 2; // Hall sensor

//Speed
int fast = 50; //Speed where speed gets averaged







//Variables
float circumference;

volatile int time_new;
volatile int time_old;

volatile unsigned int elapsed;

volatile byte counter = 0;
unsigned int rpm = 0; //unsigned gives only positive values

volatile float inms = 0;//inches per millisecond
float mph;
float mph_old;

float avg_mph;

volatile boolean toggle = 0;


void setup() {

  //Disable Inturupts
  noInterrupts();//stop interrupts

  //Enable Watchdog
  //wdt_enable(WDTO_2S);//2sec
  //wdt_enable(WDTO_4S);//4sec
  wdt_enable(WDTO_8S);//8sec

  //Initalize Serial
  Serial.begin(9600);
  Serial.println("Initalized Serial");

  //Init Variables
  circumference = 2 * 3.14 * radius;

  //Print Variables
  Serial.print("Circumference=");
  Serial.println(circumference);

  //Hall Pin
  pinMode(hall_pin, INPUT_PULLUP); //Enables Sensor Pin
  attachInterrupt(digitalPinToInterrupt(hall_pin), hall_int, RISING); //Interrupt on rising edge

  //Enable Interrupts
  interrupts();//allow interrupts
  Serial.println("Enabled Interrupts");



}


void hall_int() {

  //Time Elaped
  time_new = millis();
  elapsed = (time_new - time_old);
  time_old = time_new;

  //Calculate inch/milisecond
  inms = circumference / elapsed;
}


void loop() {
  delay(50);


  //Calc MPH
  mph = inms * 56.8182;

  //Print MPH
  Serial.print("MPH=");
  Serial.println(mph);

  //Avg MPH
  if (mph < fast) {
    avg_mph = ((mph + mph_old) / 2);
  }
  else {
    avg_mph = mph;
  }
  mph_old = mph;

  //Print AVG MPH
  Serial.print("AVG MPH=");
  Serial.println(avg_mph);

  wdt_reset(); //Resets Watchdog
}
