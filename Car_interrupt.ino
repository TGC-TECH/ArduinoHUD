#include <avr/wdt.h>

//Settings-Need to Set
float radius = 2; //radius of tire

//Pinout
int hall_pin = 2; // Hall sensor
int LED = LED_BUILTIN;// Set LED pin













//Variables
volatile byte counter = 0;
unsigned int rpm = 0; //unsigned gives only positive values
float circumference;
float magic;
float mph;
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
  magic = 0.68181818*circumference;
  //Print Variables
  Serial.print("Circumference=");
  Serial.println(circumference);
  Serial.print("Magic=");
  Serial.println(magic);

  //Hall Pin
  pinMode(hall_pin, INPUT_PULLUP); //Enables Sensor Pin
  attachInterrupt(digitalPinToInterrupt(hall_pin), hall_count, RISING); //Interrupt on rising edge

  //Led Pin
  pinMode(LED, OUTPUT);

  //Timer 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  Serial.println("Initalized Timer");


  //Enable Interrupts
  interrupts();//allow interrupts
  Serial.println("Enabled Interrupts");



}


//1Hz Timer
//Runs ever second
ISR(TIMER1_COMPA_vect) {
  noInterrupts();
  calc_rpm();
  blinker();
  wdt_reset(); //Resets Watchdog
  interrupts();

}


//Called on hall_interupt
void hall_count()
{
  counter++;
}


//Called every 1s
void calc_rpm()
{
  rpm = counter * 60;
  counter = 0; //Resets the counter
  mph = (rpm*magic);
  Serial.print("RPM=");
  Serial.println(rpm); //Calculated values are displayed
  Serial.print("MPH=");
  Serial.println(mph);
}

void blinker()
{
  //1Hz/2
  if (toggle) {
    digitalWrite(LED, HIGH);
    toggle = 0;
  }
  else {
    digitalWrite(LED, LOW);
    toggle = 1;
  }
}


void loop()
{

}
