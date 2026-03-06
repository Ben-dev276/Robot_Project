#define Lpwm_pin  5     //pin of controlling speed---- ENA of motor driver board
#define Rpwm_pin  6    //pin of controlling speed---- ENB of motor driver board
int pinLB=2;             //pin of controlling turning---- IN1 of motor driver board
int pinLF=4;             //pin of controlling turning---- IN2 of motor driver board
int pinRB=7;            //pin of controlling turning---- IN3 of motor driver board
int pinRF=8;            //pin of controlling turning---- IN4 of motor driver board


volatile int left_IR_Sensor;
volatile int right_IR_Sensor;

void setup(){
  Serial.begin(9600);
  left_IR_Sensor = 0;
  right_IR_Sensor = 0;
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(pinLB,OUTPUT); // /pin 2
  pinMode(pinLF,OUTPUT); // pin 4
  pinMode(pinRB,OUTPUT); // pin 7
  pinMode(pinRF,OUTPUT);  // pin 8
  pinMode(Lpwm_pin,OUTPUT);  // pin 5 (PWM) 
  pinMode(Rpwm_pin,OUTPUT);  // pin6(PWM) 

}

void loop(){
  left_IR_Sensor = digitalRead(9);
  right_IR_Sensor = digitalRead(10);
//  Serial.print("left_light_value = ");
//  Serial.println(left_IR_Sensor);
//  Serial.print("right_light_value = ");
//  Serial.println(right_IR_Sensor);
  if (left_IR_Sensor == 0 && right_IR_Sensor == 0) {
    stopp();
    Set_Speed(100);
    delay(300);
    back();
    Set_Speed(100);
    delay(500);
    if (random(1, 10) > 5) {
      turnL();
      Set_Speed(100);
    } else {
      turnR();
      Set_Speed(100);

    }
    delay(300);

  } else if (left_IR_Sensor == 0 && right_IR_Sensor == 1) {
     back();
    Set_Speed(100);
    delay(500);
     turnR();
      Set_Speed(100);
    delay(300);
  } else if (left_IR_Sensor == 1 && right_IR_Sensor == 0) {
    back();
    Set_Speed(100);
    delay(500);
    turnL();
      Set_Speed(100);
    delay(300);
  } else {
    advance();
    Set_Speed(100);

  }

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
