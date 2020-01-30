int  val;
long  last=0;
int stat=LOW;
int stat2;
int contar=0;
int displayrpm;
int sens=75;    //  this  value indicates the limit reading between dark  and light,
                            //  it  has to  be  tested  as  it  may change  acording  on  the 
                            //  distance  the leds  are placed.
int nPalas=1; //  the number  of  blades  of  the propeller
int milisegundos=500; //  the time  it  takes each  reading
#include  <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5,  4,  3,  2);
void  setup()
{
    Serial.begin(9600);
    pinMode(13,OUTPUT);
    lcd.begin(16,2);
}
void  loop()
{
    val=analogRead(0);
    if(val<sens)
        stat=LOW;
      else
        stat=HIGH;
      digitalWrite(13,stat);  //as  iR  light is  invisible for us, the led on  pin 13  
                                                    //indicate  the state of  the circuit.
      if(stat2!=stat){    //counts  when  the state change, thats from  (dark to  light)  or  
                                          //from  (light  to  dark),  remmember that  IR  light is  invisible for us.
          contar++;
          stat2=stat;
      }
      if(millis()-last>=milisegundos){
          double  rps=((double)contar/nPalas)/2.0*1000.0/milisegundos;
          double  rpm=((double)contar/nPalas)/2.0*60000.0/(milisegundos);
          displayrpm=rpm;
          Serial.print((contar/2.0));Serial.print("  RPS  ");Serial.print(rps);
          Serial.print("  RPM");Serial.print(rpm);Serial.print("    VAL ");Serial.println(val);
          contar=0;
          last=millis();
          Serial.print("      MPH   "); 
          Serial.print(rpm*0.0080622311);
          
          LCDPrint  ();
      }
}
void  LCDPrint  ()
{
lcd.clear ();
lcd.setCursor (0,0);
lcd.print("MPH");
lcd.setCursor (0,1);
lcd.print(displayrpm*0.0080622311     );
lcd.setCursor (8,0);
lcd.print("RPM");
lcd.setCursor (8,1);
lcd.print(displayrpm);
return;
}
