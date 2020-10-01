#include "HID-Project.h" // https://github.com/NicoHood/HID

const int fwdButton = 4;
const int backButton = 5;
const int playButton = 6;
const int volUpButton = 7;
const int volDwnButton = 8;
const int muteButton = 9;

void setup() {
  pinMode(playButton, INPUT_PULLUP);
  pinMode(fwdButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(volUpButton, INPUT_PULLUP);
  pinMode(volDwnButton, INPUT_PULLUP);
  pinMode(muteButton, INPUT_PULLUP);
  Consumer.begin();
}

void loop() {
  if (!digitalRead(playButton)) {
    Consumer.write(MEDIA_PLAY_PAUSE);
    delay(500);
  }
  if (!digitalRead(fwdButton)) {
    Consumer.write(MEDIA_NEXT);
    delay(500);
  }
  if (!digitalRead(backButton)) {
    Consumer.write(MEDIA_PREVIOUS);
    delay(500);
  }
  if (!digitalRead(volUpButton)) {
    Consumer.write(MEDIA_VOLUME_UP);
    delay(500);
  }
  if (!digitalRead(volDwnButton)) {
    Consumer.write(MEDIA_VOLUME_DOWN);
    delay(500);
  }
  if (!digitalRead(muteButton)) {
    Consumer.write(MEDIA_VOLUME_MUTE);
    delay(500);
  }
}
