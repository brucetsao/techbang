
#define LEDPIN 7     
#define SWPIN 2      


const int ledcount = 7;
int ledPin[] = { 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 , 42, 44, 46,48,50,52 };


byte bitmap[] = {
0b00000000,
0b01000001,
0b01111111,
0b01000001,
0b00000000,
0b00000000,
0b00001110,
0b00011111,
0b00111111,
0b01111110,
0b00111111,
0b00011111,
0b00001110,
0b00000000,
0b00000000,
0b00111111,
0b01000000,
0b01000000,
0b00111111,
0b10000000   
};


unsigned char pos = 0; 
unsigned char i = 0;


void setup() {
  Serial.begin(9600);

for(int pin = 0; pin < ledcount + 7; pin++) {
  pinMode(ledPin[pin], OUTPUT);
}
pinMode(SWPIN, INPUT);      
//digitalWrite(SWPIN, HIGH);  
}

void loop() {

if(digitalRead(SWPIN) == HIGH) {  
    Serial.print("Turn Right  \n ") ;
    
      delay(10);  
      pos = 0;

      while(bitmap[++pos] != 0b10000000) { 
        for(i = 0; i < 7; i++) 
        {  
               digitalWrite(ledPin[i], (bitmap[pos] >> i) & 0b00000001);
        }
        delay(5); 
      }

      for(i = 0; i < ledcount; i++) {  
        digitalWrite(ledPin[i], LOW);
      }
 }
}


