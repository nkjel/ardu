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
#define BUTTONONE 13 //INTERRUPT
#define BUTTONTWO 7
#define BUTTONTHREE 8
#define BUTTONFOUR 2 
#define REDPIN 9 //pwm
#define GREENPIN 10 //pwm
#define BLUEPIN 11 //pwm
#define BUZZER 3 //pwm


bool CW = true;
bool QUS = false; //문제 제출모드 true else 정답 제출모드 
bool isOne = false;
bool summit = false;
bool qus_summit_compl = false;
int answer[2] = {0,0}; //1번이 방향, 2번이 속도 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MOTORAA,OUTPUT);
  pinMode(MOTORAB,OUTPUT);

  pinMode(BUTTONONE,INPUT);
  pinMode(BUTTONTWO,INPUT);
  pinMode(BUTTONTHREE,INPUT);
  pinMode(BUTTONFOUR,INPUT);

  pinMode(REDPIN,OUTPUT);
  pinMode(GREENPIN,OUTPUT);
  pinMode(BLUEPIN,OUTPUT);

}

void mortor(int _speedlevel)
{
  static int motorspeed = 100 + (_speedlevel * 30); //0~255 0, 51, 102, 153, 204, 255 
  
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

void setColor(int red, int green, int blue)
{
  analogWrite(REDPIN,red);
  analogWrite(GREENPIN,green);
  analogWrite(BLUEPIN,blue);
}

void setColorwithhz(int red, int green, int blue,int hz)
{
  static bool on_off_flag = true;
  static unsigned long pre_time = 0;
  static unsigned long cur_time = 0;
  static int cnt;

  cnt = 1000/(hz*2);
  cur_time = millis();
  if((cur_time - pre_time) >= cnt){
    on_off_flag = !on_off_flag;
    pre_time = cur_time;
  }
  
  if (on_off_flag){
    analogWrite(REDPIN,red);
    analogWrite(GREENPIN,green);
    analogWrite(BLUEPIN,blue);

  }
  else {
    analogWrite(REDPIN,0);
    analogWrite(GREENPIN,0);
    analogWrite(BLUEPIN,0);   
  }
  
}

void trueorfalse(int r, int sp){
  int correct = (r&answer[0]) & (sp&answer[1]);
  if (correct){
    setColor(255,255,255);
    delay(100);
    setColor(255,0,0);
    delay(100);
    setColor(0,255,0);
    delay(100);
    setColor(0,0,255);
    delay(100);
  }
  else{
    tone(BUZZER,262);
    delay(500);
    noTone(BUZZER);
  }
}

void delaycolor(int red, int green, int blue, int hz, int k){

    int d = 1000/hz;
    for (int i = 0; i<k;i++){
      analogWrite(REDPIN,red);
      analogWrite(GREENPIN,green);
      analogWrite(BLUEPIN,blue);
      delay(d); 
      analogWrite(REDPIN,0);
      analogWrite(GREENPIN,0);
      analogWrite(BLUEPIN,0);
      delay(d);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  static int bt1_is_pushed = false;
  static int bt2_is_pushed = false;
  static int bt3_is_pushed = false;
  static int bt4_is_pushed = false;
  
  static int pre_bt1 = false;
  static int pre_bt2 = false;
  static int pre_bt3 = false;
  static int pre_bt4 = false;
  
  static int isQues = false;
  static int rotation = false;
  static int bt4_is_clicked = false;
  static int changespeed = false;
  static int wait = true;
  static int speedlevel = 0;

  static int pretime;
  static int curtime;
  
  
  
  bt1_is_pushed = digitalRead(BUTTONONE);
  bt2_is_pushed = digitalRead(BUTTONTWO);
  bt3_is_pushed = digitalRead(BUTTONTHREE);
  bt4_is_pushed = digitalRead(BUTTONFOUR);

  if((bt1_is_pushed == HIGH)&&(pre_bt1 == LOW)){ // bt1_is_pushed - pre_bt1 == 1
    isQues = true;
    wait = false;
  }
  pre_bt1 = bt1_is_pushed;
  
  if((bt2_is_pushed == HIGH)&&(pre_bt2 == LOW)){
    rotation = true;
  }
  pre_bt2 = bt2_is_pushed;

  if((bt3_is_pushed == HIGH)&&(pre_bt3 == LOW)){
    changespeed = true;
  }
  pre_bt3 = bt3_is_pushed;
  if(bt4_is_clicked){
    curtime = millis();
    if(curtime-pretime<500){
      if((bt4_is_pushed == HIGH)&&(pre_bt4 == LOW)){  
        summit = true;
        delaycolor(0,255,0,4,5);
        bt4_is_clicked = false;
      }
    }
    else{ // 시간 초과 
      CW = true;
      speedlevel = 0;
      digitalWrite(MOTORAB,LOW);
      digitalWrite(MOTORAA,LOW);
      qus_summit_compl = false;
      delaycolor(0,255,0,2,2);
      bt4_is_clicked = false;
    } 
  }
  else{
  if((bt4_is_pushed == HIGH)&&(pre_bt4 == LOW)){
      bt4_is_clicked = true;
      pretime = millis();
    }//end of bt4 
  }//end of bt4_is_clicked
  pre_bt4 = bt4_is_pushed;


////////////////////////////
  
  if (qus_summit_compl){
    if(wait){
     mortor(1);
     setColorwithhz(0,255,0,3);
    }
    else {
      //digitalWrite(MOTORAB,LOW);
      //digitalWrite(MOTORAA,LOW);
      if (isQues){
        QUS = !QUS;
        isQues = false;
       }//버튼 1 모드 체크 
      if(QUS){
        //setColor(255,0,0);
        setColorwithhz(255,0,0,2); // 2hz red
        if (changespeed){
          speedlevel = (speedlevel+1)%6;
          changespeed = false;
        }// end of speed
        if (rotation){
          CW = !CW;
          rotation = false;
        }
        if (summit){
          answer[0] = CW;
          answer[1] = speedlevel;
          CW = true;
          speedlevel = 0;
          summit = false;
          qus_summit_compl = true;
          wait = true;
          }//end of summit
        }//end of qus if 
      else{
       setColorwithhz(0,0,255,2); //2hz blue
       if (changespeed){
         speedlevel = (speedlevel+1)%6;
         changespeed = false;
        }//end of speed
       if (rotation){
          CW = !CW;
          rotation = false;
        }//end of rotation
       if (summit){
          trueorfalse(CW,speedlevel);
          CW = true;
          speedlevel = 0;
          summit = false;
          wait = true;
       }
      }//end of  qus else 
    } //대기상태 아님 
  }// end of qus_summit
  else{
    if(!wait){
    if (isQues){
     QUS = !QUS;
     setColor(0,0,0);
     isQues = false;
    }
    if(QUS){
      setColorwithhz(255,0,0,2); // 2hz red
      if (changespeed){
        speedlevel = (speedlevel+1)%6;
        changespeed = false;
      }// end of speed
      if (rotation){
        CW = !CW;
        rotation = false;
      }
      if (summit){
        answer[0] = CW;
        answer[1] = speedlevel;
        summit = false;
        qus_summit_compl = true;
        wait = true;
        }//end of summit
      }//end of qus if 

   } //not wait
  } // end of else qus_summit 
  
}// end of loop()
