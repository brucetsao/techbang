#include <Arduino.h>
#include <stdint.h>

#define DHT_LIB_VERSION "0.1.10"
 
#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2
#define DHTLIB_INVALID_VALUE    -999
 
#define DHTLIB_DHT11_WAKEUP     18
#define DHTLIB_DHT22_WAKEUP     1
#define TIMEOUT (F_CPU/40000)

class dht
{
public:
    int read11(uint8_t pin);
    int read21(uint8_t pin);
    int read22(uint8_t pin);
 
    double humidity;
    double temperature;
 
private:
    uint8_t bits[5];  // buffer to receive data
    int read(uint8_t pin, uint8_t wakeupDelay);
};
 
