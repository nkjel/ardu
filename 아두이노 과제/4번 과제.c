/*
4.LED 알고리즘1
사용 부품 : R-LED 5개, 330옴 저항 5개, 버튼 1개, 10K옴 저항 1개, 조이스틱 1개
●	버튼을 누를 때마다 모드가 변경됨
●	모드 1 :  1 -> 2 -> 3 -> 4 -> 5 -> 1 -> 2 이렇게 순차적으로 LED가 옮겨지면서 켜진다. (배열 또는 큐를 사용할 것)
●	모드 2 : 조이스틱 x축을 사용하여 오른쪽으로 움직이면 1 -> 1,2 -> 1,2,3 이렇게 채워지고 왼쪽으로 움직이면 5 -> 4,5 -> 3,4,5 이렇게 채워지고 조이스틱을 움직이지 않을 땐 모든불이 꺼져있다.
*/

#define buttonPin 2 //interrupt
#define joystickX A2
#define joystickY A1
#define RIGHT 2
#define LEFT 3

#define JOYSTICKSW 8

const int LEDRIGHT[5] = {4,7,8,12,13};
const int LEDLEFT[5] = {13,12,8,7,4};

int idx = 4;
bool flag = true;
int joystick_LR_flag = 0;
int pre_flag = 0;
long temp = 0;
void setup() {
  Serial.begin(9600);
  for(int i = 0; i<5;i++){
    pinMode(LEDRIGHT[i],OUTPUT);
  }
  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonRelesed, RISING);
  
}

void buttonRelesed(){
  flag = !flag;
  ledClear();
  idx = 0;
}

long getJoysticX(){
  return analogRead(joystickX);
}
long getJoysticY(){
  return analogRead(joystickY);
}

void ledClear(){
  for(int i = 0; i<5;i++){
    digitalWrite(LEDRIGHT[i],LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if(flag){
    digitalWrite(LEDRIGHT[idx],LOW);
    idx = (idx+1)%5;
    digitalWrite(LEDRIGHT[idx],HIGH);
    delay(1000);
  }
  else{
    //조이스틱 중심값 (512,512)
    //조이스틱 왼쪽 값 0~511
    //조이스틱 오른쪽 값 513~1023
    temp = getJoysticX();
    Serial.println(temp);
    if(!(joystick_LR_flag&&pre_flag)){
        idx = 0;
        ledClear();
     }
    if(temp>=0&&temp<500){
      joystick_LR_flag = LEFT;
      digitalWrite(LEDLEFT[idx],HIGH);
      idx = (idx+1)%5;
      pre_flag = LEFT;
      delay(100);
    }
    else if(temp>530&&temp<1024){
      joystick_LR_flag = RIGHT;
      digitalWrite(LEDRIGHT[idx],HIGH);
      idx = (idx+1)%5;
      pre_flag = RIGHT;
      delay(100);
    }
    else{
      //ledClear();
      pre_flag = 0;
      
    }
  }
  
  
}
