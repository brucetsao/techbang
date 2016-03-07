const int ledcount = 16;
int ledPin[] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 , 42, 44, 46,48,50,52 };
void setup()
{
    Serial.begin(9600);
    for (int thisLed = 0 ; thisLed < ledcount ; thisLed++ )
    {
        pinMode( ledPin[ thisLed ], OUTPUT );
    }
    /*
  Serial.begin(9600);
  int i ;
  for(i = 0 ; i< 10;  i++)
    {
        pinMode(led[i], OUTPUT) ;
        digitalWrite(led[i],LOW);
        Serial.print("Now is set ");
        Serial.print(i);
          Serial.print("pin pinmode and Low ");  
  }
*/
}

void loop()
{
  for ( int thisLed = 0 ; thisLed < ledcount ; thisLed++ )
    {
        digitalWrite( ledPin[ thisLed ], HIGH );      
        Serial.print("led ");
        Serial.print(thisLed);
        Serial.print("IS Light ");
        
        delay(80);
        digitalWrite( ledPin[ thisLed ], LOW );

    }

    for ( int thisLed = ledcount-1 ; thisLed >= 0 ; thisLed-- )
    {
        digitalWrite( ledPin[ thisLed ], HIGH );
        Serial.print("led ");
        Serial.print(thisLed);
        Serial.print("IS Light ");        
        delay(80);
        digitalWrite( ledPin[ thisLed ], LOW);
    }
  
  
}

