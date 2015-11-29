#define SensorPin A1
#define RelayPin 13
#define turnon HIGH
#define turnoff LOW
#define MidValue 524
#define ReadMaxCount 200
char getdata ;  // 儲存接收資料的變數
double Ampdata ;  // 儲存接收資料的變數

void setup() {
    pinMode(RelayPin , OUTPUT) ;  
    digitalWrite(RelayPin , turnon) ;  
 Serial.begin(9600);
 Serial1.begin(9600) ;
// while(!Serial) ;
 Serial.println("Program Start") ;
}

//String analogValue0 = String(lightMeter.readLightLevel(), DEC);
void loop()
{
    
  getdata = '@'; 
  // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
  if (Serial1.available()) {
    getdata = Serial1.read();
  
    Serial.print("(");
    Serial.print(getdata, DEC);
    Serial.print(")\n");

  }

    if (getdata == 67)
        {
          digitalWrite(RelayPin, turnoff) ;
        }

    if (getdata == 79)
        {
          digitalWrite(RelayPin, turnon) ;
        }

    if (getdata == 81)
        {
            Ampdata = ReadCurrentAverage() ;
            Serial1.print("@");
            Serial1.print(String(Ampdata,5) );
            Serial.print("(") ;
            Serial.print(Ampdata) ;
            Serial.print("/") ;
            Serial.print(String(Ampdata,5)) ;
            Serial.print(")\n") ;
            
        }

  
      delay(200) ;
  
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
