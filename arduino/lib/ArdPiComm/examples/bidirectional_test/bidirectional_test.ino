/* Library to implement the communication protocol between the Arduino and
*  the Raspberry Pi in Roberto over a Serial port.
*  
*  The communication protocol is based on HLDLC with ARQ-StopAndWait error control.
*
*  Note: Works only on version 1.0 and above of Arduino IDE. (only arduino.h)
*
*  Author:  Francisco Miguel Moreno
*  Email:   butakus@gmail.com
*  Version: 0.2
*  Date:    February 2017
*/

#include <ArdPiComm.h>

ArdPiComm comms;
uint8_t payload[64];
uint8_t arr[4] = {5, 48, 100, 150};

void setup()
{
    Serial.begin(BAUDRATE);
    comms.begin(&Serial);

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);

    comms.send(0x03);

    delay(1000);

    comms.send(0x06, 4, arr);
}

void loop()
{
    // Read
    if (comms.read())
    {
        uint8_t command = comms.get_command();
        uint8_t payload_size = comms.get_payload(payload);

    }
    //delay(10);
    // Write
    uint8_t error = comms.send(0x07, 4, arr);
    // Handle error
    // Blink a led if there was an error in the communication
    switch (error)
    {
        case TIMEOUT_ERROR: blink(1000); break;
        case ACK_ERROR: blink(2000); break;
        default: break;
    }
    //delay(10);
}

void blink(unsigned long t)
{
    digitalWrite(13, HIGH);
    delay(t);
    digitalWrite(13, LOW);
    delay(t);
}
