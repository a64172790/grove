#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int button = 3;
const int rotary = 3;
const int LED = 5;
const int touch = 4;
const int buttonpressed = HIGH;
const int buzzer = 8;
int last_button_state;
int last_touch_state;
long user = 0L;
long game = 0L;
int count;
int present = 0;
int score = 0;
int level = 0;
int levels[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
int start = 0;

int length = 5; // the number of notes
char notes[] = "cdefg"; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1};
int tempo = 100;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(buzzer, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzer, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

byte armsDown[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b01010
};

byte armsUp[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b10101,
    0b01110,
    0b00100,
    0b00100,
    0b01010
};

byte smiley[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000
};

void setup() 
{
    Serial.begin(9600);
    last_button_state = !buttonpressed;
    last_touch_state = !buttonpressed;
    randomSeed(analogRead(0));
    lcd.begin(16, 2);
    lcd.createChar(0, armsDown);
    lcd.createChar(1, armsUp);
    lcd.createChar(2, smiley);
    lcd.setCursor(2, 0);
    lcd.print("Welcome to");
    lcd.setCursor(3, 1);
    lcd.print("FitMemory");
    pinMode(rotary, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(touch, INPUT);
    pinMode(button, INPUT);
    pinMode(buzzer, OUTPUT);
    while (!getButtonPress()) { delay(100); }
    lcd.clear();
    count = 0;
    score = 0;
    level = 1;
    start = 1;
}

void loop()
{
  if(start) {
     for (int i = 0; i < length; i++) {
      playNote(notes[i], beats[i]*tempo);
      delay(tempo/2);
     }
    lcd.clear();
    lcd.print("Press Again to");
    lcd.setCursor(0, 1);
    lcd.print("Start");
    lcd.setCursor(5, 1);
    lcd.write((uint8_t)0);
    delay(100);
    lcd.setCursor(5, 1);
    lcd.write((uint8_t)1);
    delay(100);
    
  }

    if (getButtonPress()) {
      start = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      if (user == game && game != 0 && score < 10) {
        lcd.print("You Are Correct");
        score++;
        lcd.setCursor(0, 1);
        lcd.print("Score:");
        lcd.setCursor(6, 1);
        lcd.print(score);
        delay(500);
        level = levels[score];
        Serial.println(level);
      }
      else if (game != 0 && user != game) {
        lcd.print("You Lost");
        Serial.println(user);
        lcd.setCursor(0, 1);
        lcd.print("Score:");
        lcd.setCursor(6, 1);
        lcd.print(score);
        score = 0;
        level = 1;
        delay(1000);
      }
      
      if (score == 10) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("YOU WON");
        lcd.setCursor(12, 0);
        lcd.write((uint8_t)2);
        score = 0;
        //Add Animation
      }
      delay(100);
      lcd.clear();
      count = 0;
      user = 0;
      game = random(powlong(10L, 2+level), powlong(10L, 3+level));
      lcd.setCursor(0, 0);
      lcd.print("Number:");
      lcd.setCursor(7, 0);
      lcd.print(game);
      lcd.setCursor(0, 1);
      lcd.print("Level:");
      lcd.setCursor(6, 1);
      lcd.print(level);
      for (int i = 0; i < 3; i++) {
        analogWrite(LED, 255);
        delay(200);
        analogWrite(LED, 0);
        delay(200);
      }
      lcd.clear();
      lcd.setCursor(0,0);
    }
    
      if (getTouchPress()) {
        user += ((long)present) * powlong(10L, 2+level-count);
        count++;
        lcd.setCursor(count, 0);
      }
      
      else {
        if (game != 0) {
          lcd.setCursor(0,1);
          lcd.print("Score:");
          lcd.setCursor(6,1);
          lcd.print(score);
          lcd.setCursor(0,0);
          lcd.print("Input:");
          lcd.setCursor(6 + count, 0);
          present = getDigit();
          lcd.print(present);
        }
      }
    
    delay(100);
  
}

int getButtonPress() {
  int button_state = digitalRead(button);
  int button_change = button_state!=last_button_state;
  last_button_state = button_state;
  return button_change && (button_state==buttonpressed);
}

int getTouchPress() {
  int touch_state = digitalRead(touch);
  int touch_change = touch_state!=last_touch_state;
  last_touch_state = touch_state;
  return touch_change && (touch_state==buttonpressed);
}

int getDigit() {
  return (int)analogRead(rotary)/103;
}

long powlong(long x, int y)
{
  if (y==0)
    return(1);
  else
    return(powlong(x,y-1)*x);
}

