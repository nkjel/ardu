/*
 6.거리 반응 모터 변속 알고리즘
사용 부품 : 초음파 센서 1개, 모터드라이버 1개, 모터 1개, 버튼 1개, 10K옴 저항 1개
● 버튼 : 방향 전환 (전진, 후진)
● 초음파 센서 거리에 따라서 속도 변화 
○ 구간을 나누지 않고 4cm~최대측정 구간을 그대로 맵핑할 것
 */
#define MOTORAA 5 //PWM
#define MOTORAB 6 //PWM
#define ECHOPIN 12
#define TRIGPIN 11
#define BUTTON 3 //INTERRUPT


unsigned long duration = 0;
unsigned long distance = 0;
int value = 0;

bool CW = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTORAA,OUTPUT);
  pinMode(MOTORAB,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(TRIGPIN,OUTPUT);
  pinMode(BUTTON,INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonClicked, RISING);
}

void buttonClicked(){
  CW = !CW;
}

void runTriger(){
  digitalWrite(TRIGPIN,LOW);
  delayMicroseconds(2); // 쓰레기 값 지우기 위해 (초기화)
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
}

long getDistance(){
  runTriger();
  duration = pulseIn(ECHOPIN, HIGH); //센서 값을 저장
  distance = (duration / 58); 
  return distance; //cm 4cm ~ 5000cm
}

void loop() {
  // put your main code here, to run repeatedly:
  value = getDistance()/20;
  if(CW){
    analogWrite(MOTORAA,value); //0~255
    digitalWrite(MOTORAB,LOW);
  }
  else
  {
    analogWrite(MOTORAB,value); //0~255
    digitalWrite(MOTORAA,LOW);
  }
  

}
