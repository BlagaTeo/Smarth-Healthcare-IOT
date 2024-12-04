
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>       
#include <Adafruit_SSD1306.h>               
#include "MAX30105.h"            
#include "heartRate.h" 
#include "spo2_algorithm.h"
#include <Adafruit_MLX90614.h> 

//Your Domain name with URL path or IP address with path
const char* serverName1 = "http://iotsmarthealthcare.com/a-esp-post-data.php";
const char* serverName2 = "http://iotsmarthealthcare.com/b-esp-post-data.php";
const char* serverName3 = "http://iotsmarthealthcare.com/c-esp-post-data.php";
const char* serverName4 = "http://iotsmarthealthcare.com/d-esp-post-data.php";
const char* serverName11 = "http://iotsmarthealthcare.com/User_1a-esp-post-data.php";
const char* serverName22 = "http://iotsmarthealthcare.com/User_2a-esp-post-data.php";
const char* serverName33 = "http://iotsmarthealthcare.com/User_3a-esp-post-data.php";
const char* serverName44 = "http://iotsmarthealthcare.com/User_4a-esp-post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /esp-post-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";

MAX30105 particleSensor;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//const byte RATE_SIZE = 4;        // Increase this for more averaging. 4 is good.
//byte rates[RATE_SIZE];           // Array of heart rates
//byte rateSpot = 0;
//long lastBeat = 0;               // Time at which the last beat occurred
//float beatsPerMinute;
//int beatAvg=0;
//pentru hearthrate/oximeter
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data
int bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid
int i=0;
int red=0, ir=0;
int heartRate_1= 70;
int heartRate_2= 0;
int spo2_1= 99;
int spo2_2= 0;
int d=0, b=0, c=0;
int aa=1,bb=0;
int ECG[3001];

float t=0;
int p=0;
int o=0;

int ledcount=0;

#define LED 13       //pinul LED-ului rosu
#define SCREEN_WIDTH 128         // OLED display width, in pixels
#define SCREEN_HEIGHT 64         // OLED display height, in pixels
#define OLED_RESET    -1         // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int button; //buton pentru reset wifi
int button1; //buton pentru start/stop masurare
int button2; //buton pentru selectare cont
int button3; //buton pentru bifare cont

//int h=0;  //variabila pentru afisare "welcome"
int a=4;
int u=0; //pentru user
int v=0; //pentru miscare sageata la selectare user
int vv=0;//pentru miscare sageata la selectare ECG
int s=0; //ca sa scap de primul afisaj care apare la select user
int ss=0;//ca sa scap de al doilea afisaj care apare cu userii
int sss=0;//pentru a folosi un clear desplay o data...
int y=0; //pentru selectare user (repair)
int yy=0;//pentru selectare ECG la user
int f=0; //pentru offline
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float readBME280Puls() {

  long irValue = particleSensor.getIR();       // Reading the IR value it will permit us to know if there's a finger on the sensor or not
        
  if (irValue > 100000)
  {
      button = digitalRead(23);
      if(button == LOW){
        //Serial.println("Button is pushed.");
        RestartWifi();
      }
      if(a>=1)
        a=a-1;
      if(a>=1){
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(11, 5);
        display.print("MEASURING");
        display.setCursor(17, 23);
        display.print("READY IN");
        display.setCursor(58, 40);
        display.print(a);
        display.display();
      }
      /*
      long delta = millis() - lastBeat;                 //Measure duration between two beats
      lastBeat = millis();
      beatsPerMinute = 60 / (delta / 1000.0) + 15;           //Calculating the BPM
      
      if (beatsPerMinute < 100 && beatsPerMinute > 40)  //To calculate the average we strore some values (4) then do some math to calculate the average
      {
        rates[rateSpot++] = (byte)beatsPerMinute;       //Store this reading in the array
        rateSpot %= RATE_SIZE;                          //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
        */
        bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

        //read the first 100 samples, and determine the signal range
        for (i = 0 ; i < bufferLength ; i++){
          while (particleSensor.available() == false) //do we have new data?
            particleSensor.check(); //Check the sensor for new data

          if(i%7==0 && (i%14 != 0))
            digitalWrite(LED,HIGH);
                      
          if(i%14==0)
            digitalWrite(LED,LOW);
          
          
          redBuffer[i] = particleSensor.getRed();
          irBuffer[i] = particleSensor.getIR();
          particleSensor.nextSample(); //We're finished with this sample so move to next sample

          
          if(redBuffer[i]>100000){
            if(red==0)
              red=redBuffer[i];
            else
              red=(red+redBuffer[i])/2;
          }
          
          if(irBuffer[i]>100000){
            if(red==0)
              ir=irBuffer[i];
            else
              ir=(ir+irBuffer[i])/2;
          }
            
          Serial.print(".");
          maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
          
          if((heartRate>60 && heartRate<80) || (spo2>85 && spo2<101)){
      
            if(heartRate>60 && heartRate<80){
              // Serial.print(", HR=");
              // Serial.print(heartRate);
              if(heartRate != b)
                heartRate_1 =(heartRate_1 + heartRate)/2;
            }
      
            if(spo2>85 && spo2<101){
              //Serial.print(", SPO2=");
              //Serial.print(spo2);
              if(spo2 != c)
                spo2_1 =(spo2_1 + spo2+2)/2;
            }
            b=heartRate;
            c=spo2;
          }
    
        }
        if(spo2_2==0)
          heartRate_2=heartRate_1;
        else
          heartRate_2=(heartRate_2+heartRate_1)/2;
    
        if(spo2_2==0)
          spo2_2=spo2_1;
        else
          spo2_2=(spo2_2+spo2_1)/2;
        d=d+1;
        Serial.println();
        
        if(d>=4){
          Serial.print("red=");
          Serial.print(red);
          Serial.print(", ir=");
          Serial.print(ir);
          Serial.print(", HR=");
          Serial.print(heartRate_2);
          Serial.print(", SPO2=");
          Serial.print(spo2_2);
          Serial.println("%");
        }
        
        if(a==0){
          display.clearDisplay();
          display.setTextSize(2);
          display.setCursor(0, 3);
          display.print("BPM:");
          display.print(heartRate_2);
          display.setCursor(0, 23);
          display.print("SPO2:");
          display.print(spo2_2);
          display.setCursor(85, 23);
          display.print("%");
          display.display();
        }
  }
  else
  {
    button = digitalRead(23);
    if(button == LOW){
      RestartWifi();
    }
    a=4;
    //beatAvg=0;
    display.clearDisplay();
    display.setCursor(0, 20);
    display.setTextSize(1);
    display.print("Place your fingers on ");
    display.setCursor(35, 30);
    display.print("the sensors");
    display.display();
  }
  p= heartRate_2;
  o= spo2_2;
  //
  //if (isnan(p)) {    
    //Serial.println("Failed to read from Puls sensor!");
    //return "";
  //}
  //else {
    Serial.println(p);
    return p;
    Serial.println(o);
    return o;
  //}
}
//---------------------------------------------------------------------------------------------------------------------------------

float readBME280Temperature() {
  // Read temperature as Celsius (the default)
  float t1= mlx.readObjectTempC();

  if(t==0)
    t=t1;
  else
    t= (t+t1)/2.0;

  while(t>38)
    t--;
  while(t<36)
    t++;
  
  if(a==0){
    display.setTextSize(2);
    display.setCursor(0, 43);
    display.print("Temp:");
    display.print(t);
    display.display();
  }
    
  if (isnan(t)) {    
    Serial.println("Failed to read from Temperature sensor!");
    //return "";
  }
  else {
    Serial.println(t);
    return t;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);

  pinMode(LED,OUTPUT);
  pinMode(23, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(32, INPUT); // Setup for leads off detection LO +
  pinMode(33, INPUT); // Setup for leads off detection LO -
  Wire.setClock(400000);                         // Set I2C speed to 400kHz
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);     // Start the OLED display
  display.setTextColor(WHITE);
  // (you can also pass in a Wire library object like &Wire2)
  bool status = mlx.begin();
  if (!status) {
    Serial.println("Could not find a valid MLX sensor, check wiring or change I2C address!");
    while (1);
  }

  //
  particleSensor.begin(Wire, I2C_SPEED_FAST);    // Use default I2C port, 400kHz speed
  particleSensor.setup();                        // Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);     // Turn Red LED to low to indicate sensor is running
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 23);
  display.print("WELCOME!");
  display.display();
  delay(5000);

  display.clearDisplay();
  display.setCursor(1, 23);
  display.print("CONNECTING");
  display.display();
  delay(1000);
  display.setCursor(35, 40);
  display.print(".");
  display.display();
  delay(1000);
  display.setCursor(55, 40);
  display.print(".");
  display.display();
  delay(1000);
  display.setCursor(75, 40);
  display.print(".");
  display.display();
  delay(1000);

  button1 = digitalRead(19); //Verifica daca butonul nu e apasat si se conecteaza la net
  if(button1 == HIGH){
  ConnectWifi();
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(5, 23);
  display.print("CONNECTED!");
  display.display();
  delay(5000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(35, 15);
  display.print("PRESS");
  display.setCursor(17, 33);
  display.print("TO START");
  display.display();  
  }
  
  else{ //daca butonul era apasat te anunta si trebuie apasat si repornit aparatul
  display.clearDisplay();
  display.setCursor(1, 23);
  display.setTextSize(1);
  display.print("Button 1 is pressed!");
  display.display();
  delay(1000);
  }
  digitalWrite(LED,LOW);
}

void loop() {
    button1 = digitalRead(19);
    button2 = digitalRead(17);
    button3 = digitalRead(18);
    button = digitalRead(23);
    
    if(button == LOW){
      RestartWifi();
    }
    Serial.print("1");
    if(button1 == LOW){
      Serial.println("Button is pushed.");
    
      if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;

      digitalWrite(LED,HIGH);
        while(u==0){
          if(s==0){
            display.clearDisplay();
            display.display();
            display.setTextSize(2);
            display.setCursor(30, 15);
            display.print("SELECT");
            display.setCursor(37, 33);
            display.print("USER:");
            display.display();
            delay(2000);
          }
        
          button2 = digitalRead(17);
          if(v==0 && button2 == LOW){
            display.clearDisplay();
            display.setTextSize(1);
            display.setCursor(18, 25);
            display.print("User_1");
            display.setCursor(78, 25);
            display.print("User_2");
            display.setCursor(18, 45);
            display.print("User_3");
            display.setCursor(78, 45);
            display.print("User_4");
            display.display();
            delay(1000);
            button2 = digitalRead(17);
            v=1;
            s=1;
            Serial.print("3");
          }
          
          if(v==1 && button2 == LOW && ss==0){
            display.setCursor(95, 35);
            display.setTextColor(BLACK, BLACK);
            display.print("V");
            display.setCursor(30, 15);
            display.setTextColor(WHITE, BLACK);
            display.print("V");
            display.display();
            delay(1000);
            button2 = digitalRead(17);
            v=2;
            Serial.print("4");
          }
          if(v==2 && button2 == LOW && ss==0){
            display.setCursor(30, 15);
            display.setTextColor(BLACK, BLACK);
            display.print("V");
            display.setCursor(95, 15);
            display.setTextColor(WHITE, BLACK);
            display.print("V");
            display.display();
            delay(1000);
            button2 = digitalRead(17);
            v=3;
          }
          if(v==3 && button2 == LOW && ss==0){
            display.setCursor(95, 15);
            display.setTextColor(BLACK, BLACK);
            display.print("V");
            display.setCursor(30, 35);
            display.setTextColor(WHITE, BLACK);
            display.print("V");
            display.display();
            delay(1000);
            button2 = digitalRead(17);
            v=4;
          }
          if(v==4 && button2 == LOW && ss==0){
            display.setCursor(30, 35);
            display.setTextColor(BLACK, BLACK);
            display.print("V");
            display.setCursor(95, 35);
            display.setTextColor(WHITE, BLACK);
            display.print("V");
            display.display();
            delay(1000);
            button2 = digitalRead(17);
            v=1;
          }
      
          button3 = digitalRead(18);
          //Serial.print(".");
          
//-----------------------------------------------------------------------
          if(v==2 && (button3 == LOW || ss==1)){
            if(sss==0){
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(16, 15);
              display.print("SELECTED");
              display.setCursor(30, 33);
              display.print("USER_1");
              display.display();
              delay(2000);
              display.clearDisplay();
              display.setTextSize(1);
              display.setCursor(23, 10);
              display.print("DO YOU WISH TO ");
              display.setCursor(28, 23);
              display.print("MEASURE ECG?");
              display.setCursor(30, 50);
              display.print("Yes");
              display.setCursor(83, 50);
              display.print("No");
              display.display();
              sss=1;
            }
            
            delay(300);
            button2 = digitalRead(17);
            ss=1;
            Serial.print("6");
            
            if(vv==0 && button2 == LOW  && ss==1){
              Serial.print("7");
              display.setCursor(85, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(38, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=1;
            }
            if(vv==1 && button2 == LOW && ss==1){
              Serial.print("8");
              display.setCursor(38, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(85, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=0;
            }
            button3 = digitalRead(18);

            if(vv==1 && button3 == LOW&& ss==1){
              y=1; //select user 1
              yy=1;//select yes to ECG
              u=1; //opreste loop
              Serial.print("9");
            }
            if(vv==0 && button3 == LOW&& ss==1){
              y=1; //select user 1
              //yy=2;//select no to ECG
              u=1; //opreste loop
              Serial.println("10");
            }
            
          }
          
//-----------------------------------------------------------------------
          if(v==3 && (button3 == LOW || ss==1)){
            if(sss==0){
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(16, 15);
              display.print("SELECTED");
              display.setCursor(30, 33);
              display.print("USER_2");
              display.display();
              delay(2000);
              display.clearDisplay();
              display.setTextSize(1);
              display.setCursor(23, 10);
              display.print("DO YOU WISH TO ");
              display.setCursor(28, 23);
              display.print("MEASURE ECG?");
              display.setCursor(30, 50);
              display.print("Yes");
              display.setCursor(83, 50);
              display.print("No");
              display.display();
              sss=1;
            }
            
            delay(300);
            button2 = digitalRead(17);
            ss=1;
            Serial.print("6");
            
            if(vv==0 && button2 == LOW  && ss==1){
              Serial.print("7");
              display.setCursor(85, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(38, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=1;
            }
            if(vv==1 && button2 == LOW && ss==1){
              Serial.print("8");
              display.setCursor(38, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(85, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=0;
            }
            button3 = digitalRead(18);

            if(vv==1 && button3 == LOW&& ss==1){
              y=2; //select user 2
              yy=2;//select yes to ECG
              u=1; //opreste loop
              Serial.print("9");
            }
            if(vv==0 && button3 == LOW&& ss==1){
              y=2; //select user 2
              //yy=2;//select no to ECG
              u=1; //opreste loop
              Serial.println("10");
            }
          }
            
//-----------------------------------------------------------------------          
          if(v==4 && (button3 == LOW || ss==1)){
            if(sss==0){
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(16, 15);
              display.print("SELECTED");
              display.setCursor(30, 33);
              display.print("USER_3");
              display.display();
              delay(2000);
              display.clearDisplay();
              display.setTextSize(1);
              display.setCursor(23, 10);
              display.print("DO YOU WISH TO ");
              display.setCursor(28, 23);
              display.print("MEASURE ECG?");
              display.setCursor(30, 50);
              display.print("Yes");
              display.setCursor(83, 50);
              display.print("No");
              display.display();
              sss=1;
            }
            
            delay(300);
            button2 = digitalRead(17);
            ss=1;
            Serial.print("6");
            
            if(vv==0 && button2 == LOW  && ss==1){
              Serial.print("7");
              display.setCursor(85, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(38, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=1;
            }
            if(vv==1 && button2 == LOW && ss==1){
              Serial.print("8");
              display.setCursor(38, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(85, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=0;
            }
            button3 = digitalRead(18);

            if(vv==1 && button3 == LOW&& ss==1){
              y=3; //select user 2
              yy=3;//select yes to ECG
              u=1; //opreste loop
              Serial.print("9");
            }
            if(vv==0 && button3 == LOW&& ss==1){
              y=3; //select user 2
              //yy=2;//select no to ECG
              u=1; //opreste loop
              Serial.println("10");
            }
          }
//-----------------------------------------------------------------------         
          if(v==1 && (button3 == LOW || ss==1)){
            if(sss==0){
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(16, 15);
              display.print("SELECTED");
              display.setCursor(30, 33);
              display.print("USER_4");
              display.display();
              delay(2000);
              display.clearDisplay();
              display.setTextSize(1);
              display.setCursor(23, 10);
              display.print("DO YOU WISH TO ");
              display.setCursor(28, 23);
              display.print("MEASURE ECG?");
              display.setCursor(30, 50);
              display.print("Yes");
              display.setCursor(83, 50);
              display.print("No");
              display.display();
              sss=1;
            }
            
            delay(300);
            button2 = digitalRead(17);
            ss=1;
            Serial.print("6");
            
            if(vv==0 && button2 == LOW  && ss==1){
              Serial.print("7");
              display.setCursor(85, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(38, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=1;
            }
            if(vv==1 && button2 == LOW && ss==1){
              Serial.print("8");
              display.setCursor(38, 40);
              display.setTextColor(BLACK, BLACK);
              display.print("V");
              display.setCursor(85, 40);
              display.setTextColor(WHITE, BLACK);
              display.print("V");
              display.display();
              delay(1000);
              button2 = digitalRead(17);
              vv=0;
            }
            button3 = digitalRead(18);

            if(vv==1 && button3 == LOW&& ss==1){
              y=4; //select user 2
              yy=4;//select yes to ECG
              u=1; //opreste loop
              Serial.print("9");
            }
            if(vv==0 && button3 == LOW&& ss==1){
              y=4; //select user 2
              //yy=2;//select no to ECG
              u=1; //opreste loop
              Serial.println("10");
            }
          }
        }
      
        if(y==1)
          http.begin(client, serverName1);
        if(y==2)
          http.begin(client, serverName2);
        if(y==3)
          http.begin(client, serverName3);
        if(y==4)
          http.begin(client, serverName4);

          
        if(y==1 || y==2 || y==3 || y==4){
        Serial.println("Puls:");
        readBME280Puls();
        Serial.println("Temperatura:");
        readBME280Temperature();
        }

        if(a==0){
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          // Prepare your HTTP POST request data
          String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(t)
                            + "&value2=" + String(p) + "&value3=" + String(o) + "";
          Serial.print("httpRequestData: ");
          Serial.println(httpRequestData);
      
          // Send HTTP POST request
          int httpResponseCode = http.POST(httpRequestData);
          
          //dupa ce se trimit datele astepta 5 secunde si afiseaza 
          if (httpResponseCode>0){ 
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            delay(10000);
            display.clearDisplay();
            display.setTextSize(2);
            display.setCursor(10, 10);
            display.print("DATA SENT");
            display.display();
            delay(2000);
          }
          else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
          }
      
          while(button1 == LOW && yy==0){
            display.setTextSize(1);
            display.setCursor(0, 44);
            display.print("PRESS BUTTON AGAIN TO");
            display.setCursor(45, 55);
            display.print("STOP");
            display.display();
            button1 = digitalRead(19);
            delay(1000);
          }
          if(button1 == HIGH && yy==0){
             display.clearDisplay();
             display.setTextSize(2);
             display.setCursor(5, 5);
             display.print("Thank you!");
             display.setCursor(15, 23);
             display.print("You can");
             display.setCursor(10, 40);
             display.print("power off");
             display.display();
             delay(1000);
          }
        }
        
        if((yy==1 || yy==2 || yy==3 || yy==4) && a==0){
          display.clearDisplay();
          display.setTextSize(2);
          display.setCursor(10, 10);
          display.print("Measuring");
          display.setCursor(50, 28);
          display.print("ECG");
          display.display();
          delay(1000);
          
          while(aa==1){
            ECG[bb]=analogRead(34);
  
            //if(bb>2500)
              //Serial.println(ECG[bb]);
            //if(bb<2500)
            //  Serial.println(ECG[bb]);
              //Serial.println(".");

            if(bb%30==0 && (bb%60 != 0))
              digitalWrite(LED,LOW);
            if(bb%60==0)
              digitalWrite(LED,HIGH);

            if(ECG[bb]>4000)
              ECG[bb]=ECG[bb-1];
              
            bb++;
            
            if(bb==3000){
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(20, 10);
              display.print("SENDING");
              display.setCursor(40, 28);
              display.print("DATA");
              display.display();
              delay(1000);
              aa=0;
            }
            //Serial.println(ECG[i]);
            delay(10); 
          }
        }

        if((yy==1 || yy==2 || yy==3 || yy==4) && a==0){                   //pentru ECG
          for(int i=2800; i<3000; i++){

            if(yy==1){
              http.begin(client, serverName11);
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            }
            if(yy==2){
              http.begin(client, serverName22);
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            }
            if(yy==3){
              http.begin(client, serverName33);
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            }
            if(yy==4){
              http.begin(client, serverName44);
              http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            }
            if(i%2==0){
            String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(ECG[i])
                            + "&value2=" + String(1) + "&value3=" + String(1) + "";
            Serial.print("httpRequestData: ");
            Serial.println(httpRequestData);
            // Send HTTP POST request
            int httpResponseCode = http.POST(httpRequestData);

              if (httpResponseCode>0){ 
              }
              else {
                //Serial.print("Error code: ");
                //Serial.println(httpResponseCode);
              }
            }

            if(i%2!=0){
            String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(ECG[i])
                            + "&value2=" + String(1) + "&value3=" + String(1) + "";
            Serial.print("httpRequestData: ");
            Serial.println(httpRequestData);
            // Send HTTP POST request
            int httpResponseCode = http.POST(httpRequestData);
            
              if (httpResponseCode>0){ 
              }
              else {
                //Serial.print("Error code: ");
                //Serial.println(httpResponseCode);
              }
            }

            Serial.println(ECG[i]);
            
            delay(1200);
            if(i==2999){
              //http.end(); //asta cred ca trebuie folosit doar la finalizarea connecsiunii
              while(button1 == LOW){
                display.clearDisplay();
                display.setTextSize(2);
                display.setCursor(10, 10);
                display.print("DATA SENT");
                display.display();
                delay(10);
                display.setTextSize(1);
                display.setCursor(0, 47);
                display.print("PRESS BUTTON AGAIN TO");
                display.setCursor(45, 55);
                display.print("STOP");
                display.display();
                button1 = digitalRead(19);
                delay(1000);
              }
              if(button1 == HIGH){
                display.clearDisplay();
                display.setTextSize(2);
                display.setCursor(5, 5);
                display.print("Thank you!");
                display.setCursor(15, 23);
                display.print("You can");
                display.setCursor(10, 40);
                display.print("power off");
                display.display();
                delay(1000);
              }
            }
            //delay(1000);
          }
        }
      // Free resources
      http.end();
      }
      else {
        if(f==0){
          Serial.println("WiFi Disconnected");
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(20, 5);
          display.print("DISCONNECTED!");
          display.display();
          delay(1000);
        }
        /*
        button3 = digitalRead(17);
        if(button3 == LOW){
          ConnectWifi();
          delay(1000);
        }
        */
        button1 = digitalRead(19);
        if(button1 == LOW){
          if(f==0){
          Serial.println("Go offline");
          }
          Serial.println("Puls:");
          readBME280Puls();
          Serial.println("Temperatura:");
          readBME280Temperature();
          f=1;
          delay(1000);
        }
      }
    }
    delay(1000);
}

void ConnectWifi(){
  WiFiManager wm;

  bool res;
  res = wm.autoConnect("AutoConnectAP"); 
  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart(); //cred ca restarteaza ESP daca nu se poate conecta?
  }
}

void RestartWifi(){
  WiFiManager wm;
  Serial.println("Apasat");
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 5);
  display.print("RESETING");
  display.setCursor(40, 23);
  display.print("WIFI");
  display.setCursor(1, 40);
  display.print("CONNECTION");
  display.display();
  wm.resetSettings();
  //ConnectWifi();
  delay(5000);
}
