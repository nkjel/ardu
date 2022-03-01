/*
7.문제 제출 / 맞추기 게임 봇 만들기
사용 부품 : 버튼 4개, 10K옴 4개, 모터 드라이버 1개, 모터 1개, RGB LED 1개, 부저 1개
●  버튼 1 : 문제 제출 모드(빨간 불 2hz점멸) , 정답 제출 모드 변경(파란불 2hz 점멸)
● 버튼 2 : 방향 전환 버튼 
● 버튼 3 : 속도 5단계 전환 버튼 
● 버튼 4 : 1번 누를 땐 reset(리셋 되면 녹색 불 2hz 점별 2회), 2번 연속 누를 땐 제출 완료(녹색 불 4hz 점멸 5회)
문제를 제출하면  모터는 계속해서 반복적으로 움직임 (반복되는 사이에는 녹색 불 1hz 3회 점멸의 시간 구분이 들어감)
맞추면 알록달록 불빛, 틀리면 부저 굉음

즉, 문제 만드는게 버튼 2,3번 
 */

/*
버튼 1,4는 인터럽트로 구현
버튼은 풀다운 
*/
#define MOTORAA 5 //PWM
#define MOTORAB 6 //PWM
#define BUTTONONE 2 //INTERRUPT
#define BUTTONTWO 7
#define BUTTONTHREE 8
#define BUTTONFOUR 3 //INTERRUPT
#define REDPIN 9 //pwm
#define GREENPIN 10 //pwm
#define BLUEPIN 11 //pwm
#define value 51

bool CW = true;
bool QUS = true; //문제 제출모드 true else 정답 제출모드 


void setup() {
  // put your setup code here, to run once:
  pinMode(MOTORAA,OUTPUT);
  pinMode(MOTORAB,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(TRIGPIN,OUTPUT);
  pinMode(BUTTONONE,INPUT);

  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTONONE), buttonClicked, RISING);
}

void buttonClicked(){
  QUS = !QUS;
}

void setColor(int red, int green, int blue)
{
  analogWrite(REDPIN,red);
  analogWrite(GREENPIN,green);
  analogWrite(BLUEPIN,blue);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int rotation = digitalRead(BUTTONTWO);
  static int changespeed = digitalRead(BUTTONTHREE);
  static int speedlevel = 1;
  if (rotation == HIGH)
    CW = !CW;
  if (changespeed == HIGH){
    speedlevel = (speedlevel+1)%6
  }
  
  if(QUS){
    setColor(255,0,0); //red 
  }
  else{
    setColor(0,0,255); //blue
  }


  static int motorspeed = speedlevel * value; //0~255 0, 51, 102, 153, 204, 255 
  
  if(CW){ //정회전
    analogWrite(MOTORAA,motorspeed); //0~255
    digitalWrite(MOTORAB,LOW);
  }
  else
  {
    analogWrite(MOTORAB,motorspeed); //0~255
    digitalWrite(MOTORAA,LOW);
  }
  

}
