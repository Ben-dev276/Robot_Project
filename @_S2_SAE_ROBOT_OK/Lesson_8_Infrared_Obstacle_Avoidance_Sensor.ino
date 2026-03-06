volatile int left_IR_Sensor;
volatile int right_IR_Sensor;

void setup(){
  Serial.begin(9600);
  left_IR_Sensor = 0;
  right_IR_Sensor = 0;
  pinMode(9, INPUT);
  pinMode(10, INPUT);

}

void loop(){
  left_IR_Sensor = digitalRead(9);
  right_IR_Sensor = digitalRead(10);
  Serial.print("left_light_value = ");
  Serial.println(left_IR_Sensor);
  Serial.print("right_light_value = ");
  Serial.println(right_IR_Sensor);
  delay(500);
}
