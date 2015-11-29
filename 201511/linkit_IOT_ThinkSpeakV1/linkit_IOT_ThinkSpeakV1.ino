#include <LDateTime.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#define SITE_URL "184.106.153.149"
#define WIFI_AP "linkitone" //請輸入所要連的AP名稱
#define WIFI_PWD "" // 請輸入AP 密碼
LWiFiClient client;
// ThingSpeak Settings
String writeAPIKey = "7FQZ6JEDHQO6QF1H ";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 5000;        // Time interval in milliseconds to update ThingSpeak   

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int resetCounter = 0;
//=================
#define SensorPin A1
#define RelayPin 13
#define turnon HIGH
#define turnoff LOW
#define MidValue 524
#define ReadMaxCount 200
char getdata ;  // 儲存接收資料的變數
double Ampdata ;  // 儲存接收資料的變數
long mAmpdata ;  // 儲存接收資料的變數
//===wifi var
//datatime data object
datetimeInfo t;


void setup() {
    pinMode(RelayPin , OUTPUT) ;  
    digitalWrite(RelayPin , turnon) ;  
 Serial.begin(9600);
 Serial1.begin(9600) ;
// while(!Serial) ;
 Serial.println("Program Start") ;
//wifi start here
 LWiFi.begin();

 Serial.println();
 Serial.print("Connecting to AP...");
// if(LWiFi.connectWEP(WIFI_AP, WIFI_PWD) < 0)
 if(LWiFi.connect(WIFI_AP) < 0)
 {
 Serial.println("FAIL!");
 return;
 }
 Serial.println("ok");
 Serial.print("Connecting to site...");
 if(!client.connect(SITE_URL, 80))
 {
 Serial.println("FAIL!");
 return;
 }
 Serial.println("ok");
 //client.close() ;


}

//String analogValue0 = String(lightMeter.readLightLevel(), DEC);
void loop()
{
    // get date time
        LDateTime.getTime(&t);
        String analogdata = String(analogRead(SensorPin), DEC);
        String tramformdata = String(analogRead(SensorPin), DEC);
        String datedata = String(t.year, DEC)+"/"+String(t.mon, DEC)+"/"+String(t.day, DEC);
        String timedata = String(t.hour, DEC)+":"+String(t.min, DEC)+":"+String(t.sec, DEC);
    // get current data
     Ampdata = ReadCurrentAverage() ;
     mAmpdata =  (long)(Ampdata * 1000) ;
            
      delay(200) ;
    Serial.print("Data is :(");
  Serial.print(mAmpdata);
  Serial.print("/");
  Serial.print(Ampdata);
  Serial.print(")\n");
  
  // Print Update Response to Serial Monitor
  while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
  
  //  updateThingSpeak("field4="+Ampdata);
 //   Serial.println("Update thingspeak is ok");
    
    updateThingSpeak("field1="+datedata+"&field2="+timedata+"&field3="+Ampdata+"&field4="+mAmpdata);
    Serial.println("Update thingspeak is ok");

  
  delay(10000) ;

}
//===============

double ReadCurrent()
{
   int Value = analogRead(SensorPin);
  double Angv= (double)(Value - MidValue) ;
   double Mv = Angv * 5000/1024 ;
   double Amp = Mv /60 ;
   double mAmp = Amp * 1000 ;
 // Serial.print("Aout Value =( ");
  Serial.print(Value);
  Serial.print("  ,  ");
  Serial.print(Angv);
  Serial.print("  ,  ");
  Serial.print(Mv);
  Serial.print("  ,  ");
  Serial.print(Amp) ;
  Serial.print("  ,  ");
  Serial.print(mAmp) ;
//  Serial.print(")\n");
  Serial.print("\n");
 return Amp ;
}


double ReadCurrentAverage()
{
     int Value;
  double Angv;
   double Mv;
   double Amp = 0 ;
   double mAmp ;

  for(int i=0 ;i <ReadMaxCount; i++)
    {
      Value = analogRead(SensorPin);
     Angv= ((double)(Value - MidValue) * 5000/1024) /60  ;
      Amp = Amp + (Angv * Angv) ;
    }
    
    return sqrt(Amp/ReadMaxCount) ;
}


//  thinkspeak functions

//===============
void updateThingSpeak(String tsData)
{
  if (client.connect(SITE_URL, 80))
  { 
    Serial.println("Connected to ThingSpeak...");
    Serial.println();
        
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    
    resetCounter = 0;
    
  }
  else
  {
    Serial.println("Connection Failed.");   
    Serial.println();
    
    resetCounter++;
    
    if (resetCounter >=5 ) {resetEthernetShield();}

    lastConnectionTime = millis(); 
  }
}

void resetEthernetShield()
{
  Serial.println("Resetting Ethernet Shield.");   
  Serial.println();
  
  client.stop();
  //delay(1000);
  
 // Ethernet.begin(mac, ip, gateway, subnet);
  delay(1000);
}





