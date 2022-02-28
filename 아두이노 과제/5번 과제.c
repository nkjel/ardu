/*
사용 부품 : R-LED 5개, 330옴 저항 5개, 버튼 2개, 10K 옴 저항 2개
●	버튼 1 : 
○	한번 누를 땐 초기화 (LED 1번 on)
○	두번 연속 클릭 = 모드 변경 (일반/홀수) 
●	버튼 2 : 
○	모드에 따라 매 클릭시 불빛의 이동 순서 적용
○	일반 모드: 1 -> 2 -> 3 -> … 순서로 이동
○	홀수 모드: 1 -> 3 -> 5 -> 7(2) -> 9(4) -> 11(1) 순서로 이동
■	괄호안에 2, 4, 1은 LED가 총 5개이기에 표시되는 순서


*/


#define buttonPinOne 2 //interrupt
#define buttonPinTwo 3 //interrupt

const int LEDRIGHT[5] = {4,7,8,12,13};

int sel[5] = {0,2,4,1,3};
unsigned int clickTime = 0;
int mod = 0; //0은 일반 , 1은 홀수 
int idx = 0;
int selection = 0;
bool button_one_clicked = false;
bool mod_changed = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 0; i<5;i++){
    pinMode(LEDRIGHT[i],OUTPUT);
  }
  pinMode(buttonPinOne, INPUT);
  pinMode(buttonPinTwo, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPinOne), buttonOneClicked, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPinTwo), buttonTwoClicked, RISING);
}

void ledClear(){
  for(int i = 0; i<5;i++){
    digitalWrite(LEDRIGHT[i],LOW);
  }
}

void buttonOneClicked(){
  
  if (!mod_changed){
    clickTime = millis();
    mod_changed = true;
    ledClear();
    digitalWrite(LEDRIGHT[0],HIGH);
    Serial.println("moood");
  }
  else {
    unsigned int two_click = millis();
    Serial.println(two_click);
    if((two_click - clickTime)<1000){
      mod = (mod+1)%2;
      idx = 0;
    }
    mod_changed = false;
  }
}

void buttonTwoClicked(){
  switch (mod){
    case 0 : 
      digitalWrite(LEDRIGHT[idx],HIGH);
      idx = (idx +1)%5;
      break;
    case 1 : 
      selection = sel[idx];
      digitalWrite(LEDRIGHT[selection],HIGH);
      idx = (idx +1)%5;
      break;
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
