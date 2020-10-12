/*
 * Mini Media Keyboard
 */

#include "HID-Project.h" // https://github.com/NicoHood/HID - arduino-cli lib install "HID-Project"
// #include <Encoder.h> // https://github.com/PaulStoffregen/Encoder - arduino-cli lib install "Encoder"
#include <ClickEncoder.h> // https://github.com/0xPIT/encoder - git clone git@github.com:0xPIT/encoder.git $HOME/Documents/Arduino/libraries/ClickEncoder
#include <TimerOne.h> // https://github.com/PaulStoffregen/TimerOne - arduino-cli lib install "TimerOne"

const int button1 = 9;
const int button2 = 8;
const int button3 = 7;
const int button4 = 6;

template<typename T>
struct TimeStampedValue {
  explicit TimeStampedValue(T value) : _value(value), _timeStamp(0) {}
  void set(const T& value) { _value = value; touch(); }
  operator const T&() const { return _value; }
  void touch() { _timeStamp = millis(); }
  unsigned long getTimeStamp() const { return _timeStamp; }

private:
  T _value;
  unsigned long _timeStamp;
};

const int rotaryStepsPerNotch = 4; // without this one turn will increment/decrement 4 times
const int rotaryButton = A2;
const int rotaryOutputA = A1;
const int rotaryOutputB = A0;
ClickEncoder encoder(rotaryOutputA, rotaryOutputB, rotaryButton, rotaryStepsPerNotch);
TimeStampedValue<int16_t> value(0);
int16_t currentRotaryPosition = 0;

void timerIsr() {
  encoder.service();
}

void setup() {
  Serial.begin(9600); // Starts the serial communication

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  Consumer.begin();
}

void loop() {
  if (!digitalRead(button1)) {
    Consumer.write(MEDIA_PLAY_PAUSE);
    Serial.println("MEDIA_PLAY_PAUSE");
    delay(500);
  }
  if (!digitalRead(button2)) {
    Consumer.write(MEDIA_PREVIOUS);
    Serial.println("MEDIA_PREVIOUS");
    delay(500);
  }
  if (!digitalRead(button3)) {
    Consumer.write(MEDIA_NEXT);
    Serial.println("MEDIA_NEXT");
    delay(500);
  }
  if (!digitalRead(button4)) {
    Consumer.write(GAMEPAD_DPAD_RIGHT);
    Serial.println("GAMEPAD_DPAD_RIGHT");
    delay(500);
  }

  // handle rotary encoder knob
  currentRotaryPosition += encoder.getValue();
  auto diff = currentRotaryPosition - value;
  if (diff != 0) {
    Serial.println("Encoder Diff: ");
    Serial.println(diff);

    if (diff < 0) {
      Serial.println("MEDIA_VOL_UP");
      Consumer.write(MEDIA_VOL_UP);
    }
    else {
      Serial.println("MEDIA_VOL_DOWN");
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
    value.set(currentRotaryPosition);
  }

  // handle rotary encoder button
  ClickEncoder::Button b = encoder.getButton();
  if (b != ClickEncoder::Open) {
    Serial.println("Button");
    switch (b) {
      case ClickEncoder::Clicked:
        Serial.println("MEDIA_VOL_MUTE");
        Consumer.write(MEDIA_VOL_MUTE);
        value.touch();
        break;
    }
  }
}
