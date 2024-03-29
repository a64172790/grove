// demo of Starter Kit V2.0


const int potentiometer = A0;                    // rotary angle sensor connect to A0
const int pinLed = 3;

void setup()
{
    Serial.begin(9600);                         // set the serial communication frequency at 9600 bits per sec
    pinMode(potentiometer, INPUT);
    pinMode(pinLed, OUTPUT);
}

void loop()
{
    int value = analogRead(potentiometer);
    analogWrite(pinLed, map(value,0,1024,0,255));
    
    Serial.println(value);                      // pirnt the value on the serial monitor screen
    delay(100);                                  // wait 1000ms before printing next value
}
