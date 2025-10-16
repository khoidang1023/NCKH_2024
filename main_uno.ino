#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial GSM(11, 10); 

int button1 = 12;
int led1 = 4;
int button2 = 8;
int led2 = 5;

int buttonState1 = LOW;
int lastButtonState1 = LOW;
int buttonState2 = LOW;
int lastButtonState2 = LOW;

boolean isButtonPressed1 = false;
boolean isButtonPressed2 = false;
boolean button2State = false; 

char phone_no[] = "+84389205207"; 
int cambien1 = 2;    
int cambien2 = 3;
int giatri1, giatri2;
int dem = 0;

int pre1 = HIGH;
int pre2 = HIGH;
boolean called = false;
int pre_dem = 0;

void initModule(String cmd, char *res, int t) {
  while (1) {
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while (GSM.available() > 0) {
      if (GSM.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}

void callUp(char *number) {
  int tempDem = dem; // Lưu trữ giá trị của dem vào biến tạm thời
  GSM.print("ATD"); GSM.print(number); GSM.println(";"); 
  delay(20000);       
  GSM.println("ATH"); 
  delay(100);
  dem = tempDem; // Khôi phục lại giá trị của dem sau khi cuộc gọi kết thúc
}

void setup() {
  pinMode(button1, INPUT);
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  pinMode(button2, INPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);
  Serial.begin(115200);
  GSM.begin(115200);  
  pinMode(cambien1, INPUT);
  pinMode(cambien2, INPUT);
  Serial.println("Initializing....");
  initModule("AT", "OK", 1000);              
}

void loop() {
  giatri1 = digitalRead(cambien1); 
  if (giatri1 == LOW && pre1 == HIGH) {
    dem++;
    delay(1000);
  }
  pre1 = giatri1;
  giatri2 = digitalRead(cambien2);
  if (giatri2 == LOW && pre2 == HIGH) {
    dem--;
    delay(1000);
  }
  pre2 = giatri2;
  if (dem < 0) dem = 0;
  pre_dem = dem;

  if (Serial.availableForWrite()) {
    Serial.println(dem);
  }

  buttonState1 = digitalRead(button1);
  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == HIGH) {
       digitalWrite(led1, HIGH);
       digitalWrite(led2, LOW);
       button2State = false;
      if (!isButtonPressed1 && dem != 0) {
        isButtonPressed1 = true;
        callUp(phone_no);
      }else {
        if (isButtonPressed1) {
          isButtonPressed1 = false;
          button2State = false;
        }
      }
    }
    else {
      isButtonPressed1 = false;
      digitalWrite(led1, LOW);
    }
    delay(50);
  }

  buttonState2 = digitalRead(button2);
  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == HIGH) {
      if (button2State == false) {
        digitalWrite(led2, HIGH); 
        dem = 0;
        pre1 = HIGH;
        pre2 = HIGH;
        called = false;
        pre_dem = 0;
        button2State = true;
      }
      else {
        digitalWrite(led2, LOW); 
        button2State = false;
      }
    }
    delay(50);
  }
  lastButtonState2 = buttonState2;
  lastButtonState1 = buttonState1;
  delay(500);
}
