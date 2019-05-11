#include <SoftwareSerial.h>
#include <dht.h>
#include <SPI.h>
#include <SD.h>
#include <SharpIR.h>
dht DHT;
#define DHT11_PIN 7
#define IR A3
#define model 430
File myFile;
int motorf = 9;
int motorb = 6;
int xronos = 25;
int sunolomprosta = 4;
int dis;
int fire;
const int trigPin = 11;
const int echoPin = 10;
long duration;
int distance;
int c = 0;
SharpIR SharpIR(IR, model);


void setup() {
  delay(4000);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
  delay(4000);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //Serial.print("\n");
  //Serial.print("KYTION V 1.02 OPERATING");
  
}

void loop() { // run over and over

  metrhsh();
  
  mprosta(xronos);
  
  pisw(xronos,sunolomprosta);
}


void mprosta(int xronos){
  while(c<1000){
    digitalWrite(motorf,HIGH);
    //int front = xronos * 1000;
    //delay(front);
    metrhsh();
    c++;
    delay(5000);
    }
  digitalWrite(motorf,LOW);
  c = 0;
}

void pisw(int xronos,int sunolomprosta){
  digitalWrite(motorb,HIGH);
  //int back = xronos * 1000 * sunolomprosta;
  //delay(back);
  //delay(1000);
  do{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance= duration*0.034/2;
    delay(200);
  } while(distance > 30);
  
  digitalWrite(motorb,LOW);
  delay(120000);
}

void opisthen(){
  do{
      delay(500);  
      digitalWrite(motorb,HIGH); 
  
    unsigned long startTime=millis();  // takes the time before the loop on the library begins
  
    dis=SharpIR.distance();  // this returns the distance to the object you're measuring
  
    // Sharp IR code for Robojax.com
  
    
    //Serial.println(analogRead(A0));
    unsigned long endTime=millis()-startTime;  // the following gives you the time taken to get the measurement
  
       // Sharp IR code for Robojax.com
  }while(dis > 40);
  digitalWrite(motorb,LOW);
  delay(240000);
  
}


void sdcard(){
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  myFile = SD.open("metrhseis.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to metrhseis.txt...");
    myFile.println("Kytion 1, 2, 3.");
    
    myFile.close();
    Serial.println("done.");
  } 
  else {
    Serial.println("error opening metrhseis.txt");
  }

 
  myFile = SD.open("metrhseis.txt");
  if (myFile) {
    Serial.println("metrhseis.txt:");

   
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
   
    myFile.close();
  } else {
 
    Serial.println("error opening metrhseis.txt");
  }
}

int metrhsh(){
  int chk = DHT.read11(DHT11_PIN);
  int rain = analogRead(A1);
  //int fire = analogRead(A0);
  int temp = DHT.temperature;
  int humid = DHT.humidity;
  //Serial.println("KYTION V1.02");
  //Serial.print("\n");
  Serial.print("<Temp");
  if(temp > -997){
    Serial.print(temp);
  }
  else{
    Serial.print("NA");
  }
  Serial.print("Humi");
  if(humid != -999){
    Serial.print(humid);
  }
  else {
    Serial.print("NA");
  }
  Serial.print("Baro1.2");
  //Serial.println(DHT.humidity);
  if(rain < 420){
    Serial.print("Rain>");
  }
  else{
    Serial.print("Nonn>");
  }
  
 
  if(fire < 300){
    Serial.print("Fire");
  }
  else{
    Serial.print("Nonn");
  }
  
  
  //delay(200);
  
  
}
