/*1.밝기에 따라서 조절되는 LED
●	밝기에 따라 연속적으로 변경
*/

const int lightDipin = 7; //조도센서 디지털 핀 
const int led = 3; //LED 아날로그 핀 

void setup(){
  pinMode(7,INPUT); 
  pinMode(A0,INPUT); //조도 센서 아날로그 인 
  Serial.begin(9600);
}

void loop(){
  
  int a_value = analogRead(A0); //조도센서 아날로그 값 저장
  int d_value = digitalRead(7); //조도센서 디지털 값 저장 
  Serial.print("A : ");
  Serial.print(a_value);
  Serial.print(" D : ");
  Serial.println(d_value);
 
 // 조도센서 아날로그 값을 측정한 결과
 // 0~ 580 사이 분포를 가짐
 // 0은 가장 밝을 때 , 580은 가장 어두울 때
 // analogWrite() 함수는 정수형 0~255 값을 사각 파형으로 내보낸다
 // 0~580 의 값을 0~255 스케일로 변환 
 // a_value>>1 
 
  int temp = a_value>>1; //스케일 낮추기 
  if(temp>255) // led 최대값
    temp = 255;
    
  // 0일때 가장 밝으므로 밝을 때 LED를 가장 밝게 만들기 위해 
  // 255에서 temp를 뺌
  analogWrite(led, 255-(temp));
  delay(100);
  
}
