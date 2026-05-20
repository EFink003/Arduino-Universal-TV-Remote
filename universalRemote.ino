//This was done using an Arduino Mega 2560, an IR LED, an IR reciever, and a A2222 NPN transistor
//Author Evan Fink
//Last edited 05-20-2026
//IR LED = Pin 3, IR Reciever = Pin 11, Button1 = Pin 10, Button2 = Pin 9, Button3 = Pin 8, Button4 = Pin 7, Button5 = Pin 6

#include <IRremote.hpp>
#define IR_SEND_PIN 3
#define IR_RECEIVE_PIN 11
IRrecv irrecv(IR_RECEIVE_PIN);
uint32_t last_decodedRawData = 0;
// NEC codes TV SPECIFIC
#define KEY_POWER      0xF708FB04 //Vizio Codes
#define KEY_VOL_UP     0xFD02FB04
#define KEY_VOL_DWN    0xFC03FB04

#define KEY_MUTE       0x61A0708F //Insignia Codes
#define KEY_UP         0x61A042BD
#define KEY_DOWN       0x61A0C23D
#define KEY_LEFT       0x61A06897
#define KEY_RIGHT      0x61A0A857
#define KEY_ENTER      0x61A018E7
void translateIR() //for recieving IR signals
{
    if (irrecv.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) // for handling repeated or held signals
    {
        irrecv.decodedIRData.decodedRawData = last_decodedRawData;
        Serial.println("REPEAT!");
    }
    else
    {
        Serial.print("IR code: 0x");
        Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
    }
    switch (irrecv.decodedIRData.decodedRawData)//list of possible buttons to read in
    {
        case KEY_POWER:
            Serial.println("POWER");
            break;
        case KEY_VOL_UP:
            Serial.println("VOL UP");
            break;
        case KEY_VOL_DWN:
            Serial.println("VOL DOWN");
            break;
        case KEY_MUTE:
            Serial.println("MUTE");
            break;
        case KEY_UP:
            Serial.println("UP");
            break;
        case KEY_DOWN:
            Serial.println("DOWN");
            break;
        case KEY_LEFT:
            Serial.println("LEFT");
            break;
        case KEY_RIGHT:
            Serial.println("RIGHT");
            break;
        case KEY_ENTER:
            Serial.println("ENTER");
            break;
        default:
            Serial.println("OTHER BUTTON");
            break;
    }
    last_decodedRawData = irrecv.decodedIRData.decodedRawData;
}
void setup()
{
    Serial.begin(9600);
    IrSender.begin(IR_SEND_PIN);
    irrecv.enableIRIn();
    for (int pin = 6; pin <= 10; pin++)
    {
        pinMode(pin, INPUT_PULLUP);
    }
    Serial.println("IR Transmitter / Receiver Ready");
}
void loop()
{
    int activePin = -1;
    uint32_t activeButton = 0;
    for (int pin = 6; pin <= 10; pin++)
    {
        if (digitalRead(pin) == LOW)
        {
            activePin = pin;
            break;
        }
    }
    switch (activePin) //physical button pressed is linked to desired signal
    {
        case 10:
            activeButton = KEY_POWER;
            break;
        case 9:
            activeButton = KEY_VOL_UP;
            break;
        case 8:
            activeButton = KEY_VOL_DWN;
            break;
        case 7:
            activeButton = KEY_MUTE;
            break;
        case 6:
            activeButton = KEY_ENTER;
            break;
    }
    if (activeButton != 0)
    {
        Serial.print("Sending: 0x");
        Serial.println(activeButton, HEX);
        IrSender.sendNECRaw(activeButton, 32);
        delay(200);
    }
    if (irrecv.decode())
    {
        translateIR();
        irrecv.resume();
    }
    delay(500);
}
