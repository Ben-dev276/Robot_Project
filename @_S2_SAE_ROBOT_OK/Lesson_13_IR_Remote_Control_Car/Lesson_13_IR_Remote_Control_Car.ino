#include <IRremote.h>
#include <Servo.h>

// Définition des broches
#define RECV_PIN 12
#define Lpwm_pin 5
#define Rpwm_pin 6
#define pinLB 2
#define pinLF 4
#define pinRB 7
#define pinRF 8
#define A1 11
#define A0 10

// Définition des codes IR
#define IR_Go    0x00FF18E7
#define IR_Back  0x00FF4AB5
#define IR_Left  0x00FF10EF
#define IR_Right 0x00FF5AA5
#define IR_Stop  0x00FF38C7

IRrecv irrecv(RECV_PIN);
decode_results results;
Servo myservo;

unsigned char Lpwm_val = 150;
unsigned char Rpwm_val = 150;
volatile int DL, DM, DR;
bool isStopped = false;  // Variable d'état pour suivre l'arrêt du robot

void M_Control_IO_config(void) {
  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT);
  pinMode(Rpwm_pin, OUTPUT);
}

void Set_Speed(unsigned char Left, unsigned char Right) {
  analogWrite(Lpwm_pin, Left);
  analogWrite(Rpwm_pin, Right);
}

void advance() {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
}

void turnR() {
  digitalWrite(pinRB, LOW);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
}

void turnL() {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, LOW);
  digitalWrite(pinLF, HIGH);
}

void stopp() {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, HIGH);
}

void back() {
  digitalWrite(pinRB, HIGH);
  digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, HIGH);
  digitalWrite(pinLF, LOW);
}

float checkdistance() {
  digitalWrite(A1, LOW);
  delayMicroseconds(2);
  digitalWrite(A1, HIGH);
  delayMicroseconds(10);
  digitalWrite(A1, LOW);
  float distance = pulseIn(A0, HIGH) / 58.00;
  delay(10);
  return distance;
}

void Detect_obstacle_distance() {
  myservo.write(160);
  for (int i = 1; i <= 3; i++) {
    DL = checkdistance();
    delay(100);
  }
  myservo.write(20);
  for (int i = 1; i <= 3; i++) {
    DR = checkdistance();
    delay(100);
  }
}

void IR_Control(void) {
  unsigned long Key;
  if (irrecv.decode(&results)) {
    Key = results.value;
    switch (Key) {
      case IR_Go: advance(); break;
      case IR_Back: back(); break;
      case IR_Left: turnL(); break;
      case IR_Right: turnR(); break;
      case IR_Stop: stopp(); break;
      default: break;
    }
    irrecv.resume();
  }
}

void US_Control() {
  DM = checkdistance();
  if (DM < 20 && !isStopped) {
    stopp();
    Set_Speed(0, 0);
    isStopped = true;
    delay(1000);
    Detect_obstacle_distance();
    if (DL < 50 || DR < 50) {
      if (DL > DR) {
        myservo.write(90);
        turnL();
        Set_Speed(100, 100);
        delay(200);
        advance();
        Set_Speed(100, 100);
      } else {
        myservo.write(90);
        turnR();
        Set_Speed(100, 100);
        delay(200);
        advance();
        Set_Speed(100, 100);
      }
    } else {
      if (random(1, 10) > 5) {
        myservo.write(90);
        turnL();
        Set_Speed(100, 100);
        delay(200);
        advance();
        Set_Speed(100, 100);
      } else {
        myservo.write(90);
        turnR();
        Set_Speed(100, 100);
        delay(200);
        advance();
        Set_Speed(100, 100);
      }
    }
  } else if (DM >= 20 && isStopped) {
    isStopped = false;
    advance();
    Set_Speed(100, 100);
  } else {
    advance();
    Set_Speed(100, 100);
  }
}

void setup() {
  myservo.attach(A2);
  pinMode(A1, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT);
  pinMode(Rpwm_pin, OUTPUT);
  DL = 0;
  DM = 0;
  DR = 0;
  myservo.write(90);
  M_Control_IO_config();
  Set_Speed(Lpwm_val, Rpwm_val);
  irrecv.enableIRIn();
  Serial.begin(9600);
  stopp();
}

void loop() {
  IR_Control();
  US_Control();
}
