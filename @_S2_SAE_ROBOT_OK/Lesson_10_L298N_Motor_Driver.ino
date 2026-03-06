#define Lpwm_pin  5     //pin of controlling speed---- ENA of motor driver board
#define Rpwm_pin  6    //pin of controlling speed---- ENB of motor driver board
int pinLB=2;             //pin of controlling turning---- IN1 of motor driver board
int pinLF=4;             //pin of controlling turning---- IN2 of motor driver board
int pinRB=7;            //pin of controlling turning---- IN3 of motor driver board
int pinRF=8;            //pin of controlling turning---- IN4 of motor driver board
void setup()
{
 pinMode(pinLB,OUTPUT); // /pin 2
  pinMode(pinLF,OUTPUT); // pin 4
  pinMode(pinRB,OUTPUT); // pin 7
  pinMode(pinRF,OUTPUT);  // pin 8
  pinMode(Lpwm_pin,OUTPUT);  // pin 5 (PWM) 
  pinMode(Rpwm_pin,OUTPUT);  // pin6(PWM) 
}
void loop()
{
advance();
Set_Speed(200);
delay(2000);
Set_Speed(0);
delay(2000);
back();
Set_Speed(200);
 delay(2000);
Set_Speed(0);
 delay(2000);
}

void Set_Speed(unsigned char pwm) //function of setting speed
{
  analogWrite(Lpwm_pin,pwm);
  analogWrite(Rpwm_pin,pwm);
}

void advance()    //  going forward
    {
     digitalWrite(pinRB,LOW);  // making motor move towards right rear
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);  // making motor move towards left rear
     digitalWrite(pinLF,HIGH); 
   
    }
void turnR()        //turning right(dual wheel)
    {
     digitalWrite(pinRB,LOW);  //making motor move towards right rear
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,LOW);  //making motor move towards left front
  
    }
void turnL()         //turning left(dual wheel)
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,LOW );   //making motor move towards right front
     digitalWrite(pinLB,LOW);   //making motor move towards left rear
     digitalWrite(pinLF,HIGH);
    
    }    
void stopp()        //stop
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,HIGH);
    
    }
void back()         //back up
    {
     digitalWrite(pinRB,HIGH);  //making motor move towards right rear     
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH);  //making motor move towards left rear
     digitalWrite(pinLF,LOW);
      
    }
