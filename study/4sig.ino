#define MAX 10

const unsigned int led[7] = {2,3,4,5,6,7,8};
const unsigned int com[1+4] = {-1,9,10,11,12};

const unsigned int num[10][7] = {
  {1,1,1,1,1,1,0, },
  {0,1,1,0,0,0,0, },
  {1,1,0,1,1,0,1, },
  {1,1,1,1,0,0,1, },
  {0,1,1,0,0,1,1, },
  {1,0,1,1,0,1,1, },
  {1,0,1,1,1,1,1, },
  {1,1,1,0,0,1,0, },
  {1,1,1,1,1,1,1, },
  {1,1,1,1,0,1,1, },
};

int k =0;
int i = 0;
int cnt = 0;
int nsize = 5;

int preTime = 0;
int curTime =0;
int state = 0;
unsigned int text[10] = {1,2,3,4,5,6,7,8,9,0};

void display_init(){
  for(int x = 0;x<7;x++){
    pinMode(led[x],OUTPUT);
  }
  for(int y=1; y<=4; y++){
    pinMode(com[y],OUTPUT);
  }
}

void display_clear(){
  for(int x = 0; x<7;x++){
    digitalWrite(led[x],LOW);
  }
  for(int y =1 ; y<=4;y++){
    digitalWrite(com[y],HIGH);
  }
}

void display_number(unsigned int digit, unsigned int segNum){ //1번 세그먼트에 숫자 출력 
  display_clear();
  //led[x] = a,b,c,d,e,f,g
  if(0<=digit&&digit<=9 &&1<=segNum&&segNum<=4){
    for(int x =0; x<7;x++){
      digitalWrite(led[x],num[digit][x] == 1? HIGH : LOW);
      
    }
    digitalWrite(com[segNum],LOW);
  }
  
}

void setup() {
  // put your setup code here, to run once:
  display_init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  switch (state) {
    case 0 :
      if(Serial.available()){
        Serial.println("1이상 10 이하의 정수를 입력해주세요.");
        nsize = Serial.read();
        if (0<nsize && 11>nsize){
          state = 1;
          break;
        }
      }
      break;
    case 1 : 
      preTime = millis();
      while(true){
        for(i = 1; i<5;i++){
          k = (i+cnt)%nsize;
          display_number(text[k],i);
          delay(1);
        }
        curTime = millis();
        if ((curTime - preTime) >= 500)
          break;
      }
      cnt++;
      break;
  }
}
