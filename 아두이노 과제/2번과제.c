/*2.서보모터를 활용한 모드 표시기
●	버튼을 누르면 조도, 거리센서, 조이스틱 모드로 변경 
○	변경시 변경됨을 알 수 있는 장치 구성(LED, serial 등)
●	각 입력 장치에 따라 서보모터를 0~180 사이를 맵핑하여 적용
*/

#include <Servo.h>
#define lightSensor A0
#define joystickX A2
#define joystickY A1

#define JOYSTICKSW 8
#define SERVO 10
#define trigPin 11
#define echoPin 12

#define buttonPin 3 //interrupt



Servo servo;

const int led[5] = {5,13,4}; //5번은 pwm
enum sel{LIGHT,DISTANCE,JOYSTICK};
long duration = 0;
long distance = 0;

long preAvg = 0; //이전 평균 값
long curAvg = 0; //
long k = 0; 

int led_state = LOW;
bool led_state_changed = false;
int state = 0;

long temp = 0;

void setup() {
  // put your setup code here, to run once:
  servo.attach(SERVO);
  servo.write(0);
  Serial.begin(9600);
  for(int i = 0; i<3;i++){
    pinMode(led[i],OUTPUT);
  }
  pinMode(JOYSTICKSW,INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(trigPin, OUTPUT); //초음파 발사
  pinMode(echoPin, INPUT); //초음파가 들어왔을때 값 

  digitalWrite(led[state],HIGH);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, RISING);
}

void buttonPressed(){
  digitalWrite(led[state],LOW);
  state = (state+1)%3;
  digitalWrite(led[state],HIGH);
}

void runTriger(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2); // 쓰레기 값 지우기 위해 (초기화)
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}
long getbrightness(){
  return analogRead(lightSensor);
}

long getJoysticX(){
  return analogRead(joystickX);
}
long getJoysticY(){
  return analogRead(joystickY);
}

long distanceWithAvgFilter(){
  runTriger();
  duration = pulseIn(echoPin, HIGH); //센서 값을 저장
  distance = (duration / 58.2); 
  k = k+1;
  curAvg = (((k-1)*preAvg)+(distance))/k;
  preAvg = curAvg;
  return curAvg;
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(state){
    case LIGHT:
      //Serial.println("LIGHT");
      temp = getbrightness();
      temp = temp>>2;
      servo.write(temp);
      break;
    case DISTANCE:
      //Serial.println("DISTANCE");
      temp = distanceWithAvgFilter();
      temp = temp>>4; //최대 5000 서보 180
      servo.write(temp);
      break;
     case JOYSTICK:
//      Serial.println("JOYSTICK");
      temp = getJoysticX();
      temp = temp/6; //최대 1024 서보 180
      servo.write(temp);
      break;
  }
}
