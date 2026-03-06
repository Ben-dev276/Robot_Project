#include <IRremote.h>
#include <Servo.h>

// === Définition des broches moteurs ===
#define Lpwm_pin  5
#define Rpwm_pin  6
int pinLB = 2;
int pinLF = 4;
int pinRB = 7;
int pinRF = 8;

// === Capteur à ultrasons ===
#define trigPin A1
#define echoPin A0
Servo myservo;

// === Capteurs IR de ligne noire ===
#define left_IR_pin 9
#define right_IR_pin 10
volatile int left_IR_Sensor = 0;
volatile int right_IR_Sensor = 0;

// === Télécommande IR ===
int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

// === Codes télécommande ===
#define IR_Forward  0x00FF18E7
#define IR_Back     0x00FF4AB5
#define IR_Left     0x00FF10EF
#define IR_Right    0x00FF5AA5
#define IR_Stop     0x00FF38C7
#define IR_Mode     0x00FFA25D // CH

// === Variables globales ===
unsigned char Lpwm_val = 150;
unsigned char Rpwm_val = 150;
volatile int DL, DM, DR;
int mode = 0;

// === Fonctions moteurs ===
void M_Control_IO_config() {
  pinMode(pinLB, OUTPUT); pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT); pinMode(pinRF, OUTPUT);
  pinMode(Lpwm_pin, OUTPUT); pinMode(Rpwm_pin, OUTPUT);
}

void Set_Speed(unsigned char pwm) {
  analogWrite(Lpwm_pin, pwm);
  analogWrite(Rpwm_pin, pwm);
}

void advance() {
  digitalWrite(pinRB, LOW); digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, LOW); digitalWrite(pinLF, HIGH);
}

void back() {
  digitalWrite(pinRB, HIGH); digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, HIGH); digitalWrite(pinLF, LOW);
}

void turnL() {
  digitalWrite(pinRB, HIGH); digitalWrite(pinRF, LOW);
  digitalWrite(pinLB, LOW);  digitalWrite(pinLF, HIGH);
}

void turnR() {
  digitalWrite(pinRB, LOW);  digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH); digitalWrite(pinLF, LOW);
}

void stopp() {
  digitalWrite(pinRB, HIGH); digitalWrite(pinRF, HIGH);
  digitalWrite(pinLB, HIGH); digitalWrite(pinLF, HIGH);
}

// === Distance par ultrasons ===
float checkdistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float distance = pulseIn(echoPin, HIGH) / 58.0;
  delay(10);
  return distance;
}

void Detect_obstacle_distance() {
  myservo.write(160); delay(500);
  DL = checkdistance();
  myservo.write(20); delay(500);
  DR = checkdistance();
  myservo.write(90);
}

// === Mode autonome : ultrasons ===
void Auto_Ultrasound() {
  DM = checkdistance();
  if (DM < 20) {
    stopp(); delay(500);
    Detect_obstacle_distance();
    if (DL > DR) {
      turnL(); delay(300);
    } else {
      turnR(); delay(300);
    }
  } else {
    advance();
  }
  Set_Speed(100);
}

// === Mode autonome : IR ligne noire ===
void Auto_IR_Line() {
  left_IR_Sensor = digitalRead(left_IR_pin);
  right_IR_Sensor = digitalRead(right_IR_pin);

  if (left_IR_Sensor == 0 && right_IR_Sensor == 0) {
    stopp(); delay(300);
    back(); delay(500);
    if (random(1, 10) > 5) turnL(); else turnR();
    delay(300);
  } else if (left_IR_Sensor == 0 && right_IR_Sensor == 1) {
    back(); delay(500); turnR(); delay(300);
  } else if (left_IR_Sensor == 1 && right_IR_Sensor == 0) {
    back(); delay(500); turnL(); delay(300);
  } else {
    advance();
  }
  Set_Speed(100);
}

// === Mode manuel avec sécurité IR + Ultrasons ===
void IR_Control() {
  // === Surveillance constante en mode manuel ===
  if (mode == 0) {
    float distance = checkdistance();
    if (distance < 15) {
      stopp();
      Serial.println("🚧 Obstacle détecté ! Arrêt d'urgence (manuel)");
    }    
    left_IR_Sensor = digitalRead(left_IR_pin);
    right_IR_Sensor = digitalRead(right_IR_pin);

    if (left_IR_Sensor == 0 && right_IR_Sensor == 0) {
      stopp(); delay(300);
      back(); delay(500);
      if (random(1, 10) > 5) turnL(); else turnR();
      delay(300);
    } else if (left_IR_Sensor == 0 && right_IR_Sensor == 1) {
      stopp();
    } else if (left_IR_Sensor == 1 && right_IR_Sensor == 0) {
      stopp();
    }
    Set_Speed(100);
  }

  // === Lecture des capteurs de ligne en manuel ===
  if (mode == 0) {
    left_IR_Sensor = digitalRead(left_IR_pin);
    right_IR_Sensor = digitalRead(right_IR_pin);

    if (left_IR_Sensor == 0 && right_IR_Sensor == 0) {
      stopp(); delay(300);
      back(); delay(500);
      if (random(1, 10) > 5) turnL(); else turnR();
      delay(300); stopp();
      return;
    } else if (left_IR_Sensor == 0 && right_IR_Sensor == 1) {
      back(); delay(500); turnR(); delay(300); stopp();
      return;
    } else if (left_IR_Sensor == 1 && right_IR_Sensor == 0) {
      back(); delay(500); turnL(); delay(300); stopp();
      return;
    }
  }

  // === Lecture de la télécommande ===
  if (irrecv.decode(&results)) {
    unsigned long Key = results.value;

    if (Key == IR_Mode) {
      mode = (mode + 1) % 3;
      stopp();
      delay(500);
    }

    if (mode == 0) {
      switch (Key) {
        
        default: 
          break; 
          
        case IR_Forward:
          if (checkdistance() < 15) {
            stopp();
            Serial.println("Obstacle détecté ! Avance bloquée.");
          } else {
            advance();
          }
          break;

        case IR_Back:
          back(); break;

        case IR_Left:
          turnL(); delay(600); stopp(); break;

        case IR_Right:
          turnR(); delay(600); stopp(); break;

        case IR_Stop:
          stopp(); break;
 
      }
    }

    irrecv.resume(); // Prêt pour prochaine commande
  }
}


// === Setup ===
void setup() {
  Serial.begin(9600);
  M_Control_IO_config();
  Set_Speed(Lpwm_val);
  irrecv.enableIRIn();
  stopp();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(A2);
  myservo.write(90);

  pinMode(left_IR_pin, INPUT);
  pinMode(right_IR_pin, INPUT);
  randomSeed(analogRead(A3));
}

// === Loop ===
void loop() {
  IR_Control();
  if (mode == 1) Auto_Ultrasound();
  else if (mode == 2) Auto_IR_Line();
}
