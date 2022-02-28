/*
3.거리 측정 장치

사용 부품 : 버튼 1개, 초음파 센서 1개, RGB LED 1개
●	평상 시 : R -> G -> B ->R 순서로 1초마다 색이 변화한다. 
●	버튼을 누르고 있는 동안 : 
○	G 색상이 유지되면서 누르고 있는 동안의 초음파 센서 값의 평균을 지속적으로 구함
○	버튼을 떼는 순간 최종 평균 값을 시리얼 창에 출력
*/


#define buttonPin 2 //interrupt

const int RGBLED[3] = {3,6,9};
const int trig_pin = 11;
const int echo_pin = 12;
long duration = 0;
long distance = 0;
int preAvg = 0; //이전 평균 값
int curAvg = 0; //
int k = 0; 

unsigned int preTime = 0;
unsigned int curTime = 0;

int state = 0;
bool flag = false;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for(int i = 0; i<3;i++){
    pinMode(RGBLED[i],OUTPUT);
  }
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonChanged, CHANGE);
}

void buttonChanged(){
  flag = !flag;
  if (flag){
    k = 0;
    preAvg = 0;
    curAvg = 0;
  }
  else {
    Serial.print("최종 평균 : ");
    Serial.println(curAvg);
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(RGBLED[0],red);
  analogWrite(RGBLED[1],green);
  analogWrite(RGBLED[2],blue);
}

void runTriger(){
  digitalWrite(trig_pin,LOW);
  delayMicroseconds(2); // 쓰레기 값 지우기 위해 (초기화)
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  preTime =  millis();
  while (!flag){
    curTime = millis();
    if ((curTime - preTime) >=1000){
      switch (state){
        case 0 : setColor(255,0,0);
          break;
        case 1 : setColor(0,255,0);
          break;
        case 2 : setColor(0,0,255);
          break;
      }
      state = (state+1)%3;
      break;
    }
  }
  
  while (flag){
    setColor(0,255,0);
    runTriger();
    duration = pulseIn(echo_pin, HIGH); //센서 값을 저장
    distance = (duration / 58.2); 
    k = k+1;
    Serial.println(distance);
    curAvg = (((k-1)*preAvg)+(distance))/k;
    preAvg = curAvg;
    delay(100);
    
  }
  
}
