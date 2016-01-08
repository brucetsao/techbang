#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {

    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (Y/M/D) = ");
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tm.Day);
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the Set-Time");
      Serial.println("example to initialize the time and begin run-ning.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the cir-cuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
// 簡便的工具函式，格式化輸出字串
void pf(const char *fmt, ... ){
    char tmp[128]; // resulting string limited to 128 chars
    va_list args;
    va_start (args, fmt );
    vsnprintf(tmp, 128, fmt, args);
    va_end (args);
    Serial.print(tmp);
}
