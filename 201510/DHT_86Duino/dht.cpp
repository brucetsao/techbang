#include "dht.h"
int dht::read11(uint8_t pin)
{
    // READ VALUES
    int rv = read(pin, DHTLIB_DHT11_WAKEUP);
    if (rv != DHTLIB_OK)
    {
        humidity    = DHTLIB_INVALID_VALUE; // invalid value, or is NaN prefered?
        temperature = DHTLIB_INVALID_VALUE; // invalid value
        return rv;
    }
 
    // CONVERT AND STORE
    humidity    = bits[0];  // bits[1] == 0;
    temperature = bits[2];  // bits[3] == 0;
 
    // TEST CHECKSUM
    // bits[1] && bits[3] both 0
    uint8_t sum = bits[0] + bits[2];
    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
 
    return DHTLIB_OK;
}
 
// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int dht::read21(uint8_t pin)
{
    // dataformat & wakeup identical to DHT22
    return read22(pin);
}
 
// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int dht::read22(uint8_t pin)
{
    // READ VALUES
    int rv = read(pin, DHTLIB_DHT22_WAKEUP);
    if (rv != DHTLIB_OK)
    {
        humidity    = DHTLIB_INVALID_VALUE;  // invalid value, or is NaN prefered?
        temperature = DHTLIB_INVALID_VALUE;  // invalid value
        return rv; // propagate error value
    }
 
    // CONVERT AND STORE
    humidity = word(bits[0], bits[1]) * 0.1;
 
    if (bits[2] & 0x80) // negative temperature
    {
        temperature = -0.1 * word(bits[2] & 0x7F, bits[3]);
    }
    else
    {
        temperature = 0.1 * word(bits[2], bits[3]);
    }
 
    // TEST CHECKSUM
    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
 
    return DHTLIB_OK;
}
 
/////////////////////////////////////////////////////
//
// PRIVATE
//
 
// return values:
// DHTLIB_OK
// DHTLIB_ERROR_TIMEOUT
int dht::read(uint8_t pin, uint8_t wakeupDelay)
{
    // INIT BUFFERVAR TO RECEIVE DATA
    uint8_t mask = 128;
    uint8_t idx = 0;
 
    // EMPTY BUFFER
    for (uint8_t i = 0; i < 5; i++) bits[i] = 0;
 
    // REQUEST SAMPLE
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(wakeupDelay);
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    pinMode(pin, INPUT);
 
    // GET ACKNOWLEDGE or TIMEOUT
    unsigned int loopCnt = TIMEOUT;
    while(digitalRead(pin) == LOW)
    {
        if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
    }
 
    loopCnt = TIMEOUT;
    while(digitalRead(pin) == HIGH)
    {
        if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
    }
 
    // READ THE OUTPUT - 40 BITS => 5 BYTES
    for (uint8_t i = 0; i < 40; i++)
    {
        loopCnt = TIMEOUT;
        while(digitalRead(pin) == LOW)
        {
            if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
        }
 
        unsigned long t = micros();
 
        loopCnt = TIMEOUT;
        while(digitalRead(pin) == HIGH)
        {
            if (--loopCnt == 0) return DHTLIB_ERROR_TIMEOUT;
        }
 
        if ((micros() - t) > 40) bits[idx] |= mask;
        mask >>= 1;
        if (mask == 0)   // next byte?
        {
            mask = 128;
            idx++;
        }
    }
    return DHTLIB_OK;
}
//
// END OF FILE
