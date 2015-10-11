#include "dht.h"
//dht DHT;
 
#define DHT11_PIN 7
dht DHT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);			//宣告監控畫面的通訊速率

  Serial.println("DHT PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
 // Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
   if ( chkDHT(chk) == 0)    //　簡　查
       {
           Serial.println("ERROR on init DHT Sensor") ;
          while (true) ;
       }
 
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
 
  delay(2000);
}

unsigned int chkDHT( int chk )
{
   switch (chk)
  {
    case DHTLIB_OK: 
                Serial.print("OK,\t");
                return 1;
    case DHTLIB_ERROR_CHECKSUM:
                Serial.print("Checksum error,\t");
                return 0;
    case DHTLIB_ERROR_TIMEOUT:
                Serial.print("Time out error,\t");
               return 0;
    default:
                Serial.print("Unknown error,\t");
               return 0;
  }

}
