#include  <AntaresESP32HTTP.h>   //ESP32 board
#include <ESP32_Servo.h> 
#define ACCESSKEY "yourAntaresAppAccessKey"
#define WIFISSID "youwifissid"         // WIFI SSID
#define PASSWORD "yourwifipass"     //  WIFI PASS

#define APPNAME "yourAppName" //App Name of project
#define DEVICENAME_1 "yourDeviceName"   //device to monitoring
#define DEVICENAME_2 "yourDeviceName_2"  //device to controlling

AntaresESP32HTTP antares(ACCESSKEY);

#define PINSLOT1 23
#define PINSLOT2 22
#define PINSLOT3 21

int count,mobil;
const int servoPin=19;

Servo gate;

void setup() {
  Serial.begin(9600);
  antares.setDebug(false); //set true if you want see log in serial monitor
  antares.wifiConnection(WIFISSID,PASSWORD);  // connecting to wifi
  pinMode(PINSLOT2, INPUT);
  pinMode(PINSLOT3, INPUT);
  gate.attach(servoPin);
  gate.write(0);
}

void loop() {
  antares.get(APPNAME, DEVICENAME_2);

  // Check if we're actually getting data
  if(antares.getSuccess()) {
    int gatestate = antares.getInt("gateState");
    if(gatestate==1){
      gate.write(180);
      delay(100);
    }else if(gatestate==0){
      gate.write(0);
      delay(100);
    }
  }
  
  int s1 = digitalRead(PINSLOT1);
  int s2 = digitalRead(PINSLOT2);
  int s3 = digitalRead(PINSLOT3);
  int FirstSlot,SecondSlot,ThirdSlot;
    
  //detecting slot 1    
  if(!s1){
    FirstSlot=1;
  }else{
    FirstSlot=0;
  }

  //detecting slot 2    
  if(!s2){
    SecondSlot=1;
  }else{
    SecondSlot=0;
  }

  //detecting slot 3    
  if(!s3){
    ThirdSlot=1;
  }else{
    ThirdSlot=0;
  }    

  mobil=FirstSlot+SecondSlot+ThirdSlot;
  count=3-mobil;
  
  // parse data
  // Serial.print("slot1:");Serial.println(s1);
  // Serial.print("slot2:");Serial.println(s2);
  // Serial.print("slot3:");Serial.println(s3);
  antares.add("slot1", s1);
  antares.add("slot2", s2);
  antares.add("slot3", s3);
  antares.add("status", count);

  // POST data to antares platform
  antares.send(APPNAME, DEVICENAME_1);
  delay(300);
}