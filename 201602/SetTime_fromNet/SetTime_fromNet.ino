#include <Time.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

tmElements_t tm;

int Ntpyr,Ntpmon,Ntpday,Ntphr,Ntpmin,Ntpsec ;


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};
IPAddress ip(192, 168, 2, 200);     //fix ip
IPAddress dnServer(168, 95, 1, 1);    //dns ip
// the router's gateway address:
IPAddress gateway(192, 168, 2, 254);    //gateway ip
// the subnet:
IPAddress subnet(255, 255, 255, 0);     //sub mask


unsigned int localPort = 8888;       // local port to listen for UDP packets

char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
// timeZoneOffset = (Time Zone) * 3600L eg. (+8) * 3600L = 28800L for Taipei, Taiwan
const long timeZoneOffset = 28800L; 
// sync to NTP server every "ntpSyncTime" seconds, set to 1 hour or more to be reasonable
unsigned long ntpSyncTime = 21600;
// adjust the sync latency with computer NTP client in seconds 
unsigned int syncLatency = 2;


const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};




void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
  Ethernet.begin(mac, ip, dnServer, gateway, subnet);

  }
  Udp.begin(localPort);
  Serial.begin(9600);
    ntpSyncDS1307() ;
}

void loop() {
  tmElements_t ttm;

  if (RTC.read(ttm)) {

    Serial.print("Ok, Time = ");
     Serial.print(print2digits(ttm.Hour));
    Serial.print(':');
     Serial.print(print2digits(ttm.Minute));
    Serial.print(':');
     Serial.print(print2digits(ttm.Second));
    Serial.print(", Date (Y/M/D) = ");
    Serial.print(print4digits(tmYearToCalendar(ttm.Year)));
    Serial.print('/');
    Serial.print(print2digits(ttm.Month));
    Serial.print('/');
    Serial.print(print2digits(ttm.Day));
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

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(char* address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

String  print2digits(int number) {
  String ttt ;
  if (number >= 0 && number < 10) 
  {
     ttt =String("0")+String(number);
  }
  else
  {
     ttt =String(number);
  }
   return ttt ;
}

String  print4digits(int number) {
  String ttt ;
     ttt =String(number);
   return ttt ;
}


void getInternetDateTime()
{
    sendNTPpacket(timeServer); // send an NTP packet to a time server

  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
  // wait ten seconds before asking for the time again

}

// functions to be called when an alarm triggers:
void ntpSyncDS1307() {
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a replay is available
  delay(1000);
  if (Udp.parsePacket()) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    // the timstamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900)
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    // now convert NTP time into everyday time:
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800L;
    // substract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears + timeZoneOffset + syncLatency;
    setTime(epoch);
    RTC.set(epoch);
   // RTC.Write
    // output time and "Sync OK" message every sync 
    Serial.print(year());
    Serial.print('-');
    Serial.print(month());
    Serial.print('-');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(hour());
    Serial.print(':');
    Serial.print(minute());
    Serial.print(':');
    Serial.print(second());
    Serial.print(' ');
    Serial.println("Sync OK");
  }
}



