#define SPEED 9600

#define WAIT 48 //char 아스키 값 0 
#define START 49 //char 아스키 값 1

const int trig_pin = 11;
const int echo_pin = 12;
long duration = 0;
long distance = 0;
int preAvg = 0; //이전 평균 값
int curAvg = 0; //
int k = 0; 
volatile int state = WAIT; 

void serialEvent(){
  state = Serial.read();
  //오류처리 넣어야함. 
}

void runTriger(){
  digitalWrite(trig_pin,LOW);
  delayMicroseconds(2); // 쓰레기 값 지우기 위해 (초기화)
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SPEED);
  pinMode(trig_pin, OUTPUT); //초음파 발사
  pinMode(echo_pin, INPUT); //초음파가 들어왔을때 값 
}


void loop() {
  // put your main code here, to run repeatedly:
  //state = START;
  switch (state){
    case WAIT :
      preAvg = 0;
      curAvg = 0;
      k = 0;
      break;
    case START :
      runTriger();
      duration = pulseIn(echo_pin, HIGH); //센서 값을 저장
      distance = (duration/2) / 29.1; 
      k = k+1;
      curAvg = (((k-1)*preAvg)+(distance))/k;
      preAvg = curAvg;
      Serial.print("Avg:");
      Serial.println(curAvg);
      delay(100);
      break;

  }
  
  
}
