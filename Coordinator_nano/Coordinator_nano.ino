#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "fir-demo-19284.firebaseio.com"
#define FIREBASE_AUTH "h88016gY5z5V6frYDU8CqaXXGm1dyNJTI8lqhz7B"
#define WIFI_SSID "Mpetos"
#define WIFI_PASSWORD "1111111111"
#define water D6
//TP-LINK_941D6A
//mame6240
//COSMOTE-82AED6
//dbqVphVcfh4SejCe
bool started= false;//True: Message is strated
bool ended   = false;//True: Message is finished 
char incomingByte ; //Variable to store the incoming byte
char msg[23];    //Message - array from 0 to 2 (3 values - PWM - e.g. 240)
byte indexx;     //Index of array
char xbee;
int count = 0;
int er = 0;
String temp = "00";
String humid = "00";
// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(9600);
  pinMode(water,OUTPUT);
  digitalWrite(water,HIGH);
	// initialize the LCD
	lcd.init();

	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.print("KYTION V1.02");
  lcd.setCursor(0,1);
  delay(2000);
  lcd.print("Server Operating");
  delay(3000);
  lcd.init();
  lcd.print("Searching for");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("KYTION...");
  delay(3000);
  lcd.init();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    lcd.print(".");
    delay(500);
  }
  lcd.setCursor(0,1);
  lcd.print("connected: ");
  lcd.setCursor(0,2);
  lcd.println(WiFi.localIP());
  delay(1000);
  lcd.init();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}



void loop(){
  

  firebase();

  //delay(100);

  ReceivePackage();
    
}
   
    
  

void firebase(){
  lcd.setCursor(0,3);
  lcd.print(Firebase.getString("test/hlektrovana"));
    
  if(Firebase.getString("test/hlektrovana") == "water"){
    digitalWrite(water,LOW);
    if (Firebase.failed()) {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());  
        return;
    }
  }
  else{
    digitalWrite(water,HIGH);
  }
}



void xwater(){
  if(xbee == 'w'){
      int i = 10;
      lcd.clear();
      lcd.print("STARTED WATERING");
      lcd.setCursor(0,1);
      lcd.print("for 10sec");
      digitalWrite(water,LOW);
      while(i>0){
        lcd.init();
        lcd.print("remaining time: ");
        lcd.print(i);
        i--;
        delay(500);
        
      }
      digitalWrite(water,HIGH);
      lcd.init();
    }
}





int check(){
  if (Serial.available()>0){
      xbee = Serial.read();
      if((xbee >= '1' and xbee <= '9') || (xbee >= 'a' and xbee <= 'z') || (xbee >= 'A' and xbee <= 'Z') || xbee == ' ' || xbee == '0'){
        //lcd.print(xbee);
        count++;
        return(1);
      }
    }
  return(0);
}
void ReceivePackage(){
    while (Serial.available()>0){
    //Read the incoming byte
    incomingByte = Serial.read();
    //Start the message when the '<' symbol is received
    if(incomingByte == '<')
    {
     started = true;
     indexx = 0;
     msg[indexx] = '\0'; // Throw away any incomplete packet
   }
   //End the message when the '>' symbol is received
   else if(incomingByte == '>')
   {
     ended = true;
     break; // Done reading - exit from while loop!
   }
   //Read the message!
   else
   {
     if(indexx < 24) // Make sure there is room
     {
       msg[indexx] = incomingByte; // Add char to array
       indexx++;
       msg[indexx] = '\0'; // Add NULL to end
     }
   }
   
 }
 
 if(started && ended)
 {
   Serial.println(msg);
   PushToFirebase();
   //Serial.println(value); //Only for debugging
   indexx = 0;
   msg[indexx] = '\0';
   started = false;
   ended = false;
 }
 
}


void PushToFirebase(){
  
              lcd.init();
              lcd.setCursor(0,0);
              lcd.print("Temperature: ");
              lcd.print(msg[4]);
              temp[0] = msg[4];
              lcd.print(msg[5]);
              temp[1] = msg[5];
              Firebase.setString("items/-LeJ55erYlplphRz4M73/body/",temp);
              lcd.setCursor(0,1);
              lcd.print("Humidity: ");
              lcd.print(msg[10]);
              humid[0] = msg[10];
              lcd.print(msg[11]);
              humid[1] = msg[11];
              Firebase.setString("items/-LeJFlE9QUezTw9SgWRZ/body/",humid);
              //Serial.println(humid);
              if(msg[19] == 'R'){
                Firebase.setString("items/-LeWB5WZQSVHcEO6I6o7/body/","Raining F stp water");
              }
              else{
                Firebase.setString("items/-LeWB5WZQSVHcEO6I6o7/body/","...");
              }
              //if(msg[23] == 'F'){
              //  Firebase.setString("items/-LeWB5WZQSVHcEO6I6o7/body/","Fire");
              //}
              
              lcd.setCursor(0,2);
              lcd.print("Barometer: 1.2 c: ");
              er++;
              lcd.print(er);
              
}


  
