
#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

#define NUM_BUTTONS  8

const uint8_t button1 = 2;
const uint8_t button2 = 3;
const uint8_t button3 = 4;
const uint8_t button4 = 5;
const uint8_t button5 = 6;
const uint8_t button6 = 7;
const uint8_t button7 = 8;
const uint8_t button8 = 9;

const uint8_t buttons[NUM_BUTTONS] = {button1, button2, button3, button4, button5, button6, button7, button8};
uint8_t buttonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
uint8_t newButtonStates[NUM_BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
const uint8_t keys[NUM_BUTTONS] = {0, 2, 4, 6, 7, 9, 11, 12};



int octavePin = 1;
int velocityPin = 2;
int volPin = 3;
int modPin = 4;

int volVal = 200;
int modVal = 200;




void volume(byte mod) {
  Serial.print("\r\n vol val");
  Serial.print(mod);
  midiEventPacket_t mod_packet = {0x09, 0xb0, 0x01, mod};
  MidiUSB.sendMIDI(mod_packet);
}

void modulate(byte mod) {
  Serial.print("\r\n mod val");
  Serial.print(mod);
  midiEventPacket_t mod_packet = {0x09, 0xb0, 0x07, mod};
  MidiUSB.sendMIDI(mod_packet);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}


int btn2State = HIGH;
int btn3State = HIGH;

int potVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
}




void loop() {
  // put your main code here, to run repeatedly:
   int buttonValue2 = digitalRead(2);
   int buttonValue3 = digitalRead(3);
   int octVal = analogRead(octavePin) >> 7;
   int velocityVal = analogRead(velocityPin) >> 3;    // read the value from the sensor
   int newVolVal = analogRead(volPin) >> 3;    // read the value from the sensor
   int newModVal = analogRead(modPin) >> 3;    // read the value from the sensor



   if(newVolVal != volVal) {
    volVal = newVolVal;
    
    volume(volVal);
    MidiUSB.flush();
   }

   if(newModVal != modVal) {
    modVal = newModVal;
    
    modulate(modVal);
    MidiUSB.flush();
   }


   for (int i = 0; i < NUM_BUTTONS; i++)
  {
    newButtonStates[i] = digitalRead(buttons[i]);

    if (newButtonStates[i]  == LOW && buttonStates[i] == HIGH){
      // If button pushed, turn LED on
      buttonStates[i] = LOW;
      Serial.print("\r\n button down");
      noteOn(0, (12 * octVal) + keys[i] + 17, velocityVal);   // Channel 0, middle C, normal velocity
      MidiUSB.flush();
      delay(5);
      
     } else if(newButtonStates[i] == HIGH && buttonStates[i] == LOW){
       buttonStates[i] = HIGH;
       Serial.print("\r\n button up");
       noteOff(0, (12 * octVal) + keys[i] + 17, velocityVal);  // Channel 0, middle C, normal velocity
       MidiUSB.flush();
       delay(5);
     }
    
  }
   

   delay(5);
   
}
