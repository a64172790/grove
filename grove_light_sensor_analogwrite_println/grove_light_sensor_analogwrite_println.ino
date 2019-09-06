

const int pinLight = A0;
const int pinLed   = 3;

void setup()
{

      Serial.begin(9600);
      pinMode(pinLed, OUTPUT);            
}

void loop()
{
    int sensorValue = analogRead(pinLight);    
   
    analogWrite(pinLed, map(sensorValue,0,760,255,0));
  
    Serial.println(sensorValue);
    delay(100);
}
