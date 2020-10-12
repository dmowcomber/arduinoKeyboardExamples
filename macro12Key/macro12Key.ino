#include "HID-Project.h" // https://github.com/NicoHood/HID - arduino-cli lib install "HID-Project"
#include <ClickEncoder.h> // https://github.com/0xPIT/encoder - git clone git@github.com:0xPIT/encoder.git $HOME/Documents/Arduino/libraries/ClickEncoder
#include <TimerOne.h> // https://github.com/PaulStoffregen/TimerOne - arduino-cli lib install "TimerOne"


template <typename T> struct TimeStampedValue {
  explicit TimeStampedValue(T value) : _value(value), _timeStamp(0) {}
  void set(const T &value) {
    _value = value;
    touch();
  }
  operator const T &() const { return _value; }
  void touch() { _timeStamp = millis(); }
  unsigned long getTimeStamp() const { return _timeStamp; }

private:
  T _value;
  unsigned long _timeStamp;
};

const int rotaryStepsPerNotch = 4; // without this one turn will increment/decrement 4 times
const int rotaryButton = -1; // ignore rotary buttons for now

const int rotary1OutputA = 14;
const int rotary1OutputB = 15;
ClickEncoder RotaryEncoder1(rotary1OutputA, rotary1OutputB, rotaryButton, rotaryStepsPerNotch);

const int rotary2OutputA = 10;
const int rotary2OutputB = 16;
ClickEncoder RotaryEncoder2(rotary2OutputA, rotary2OutputB, rotaryButton, rotaryStepsPerNotch);

TimeStampedValue<int16_t> rotary1TimeStampedValue(0);
TimeStampedValue<int16_t> rotary2TimeStampedValue(0);
int16_t rotary1Position = 0;
int16_t rotary2Position = 0;

void timerIsr() {
  RotaryEncoder1.service();
  RotaryEncoder2.service();
}

void setup() {
  Serial.begin(9600);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
}

void loop() {
  encoder1();
  encoder2();
}

void encoder1() {
  int16_t diff = getEncoderDiff(RotaryEncoder1, rotary1Position, rotary1TimeStampedValue);
  if (diff != 0) {
    if (diff > 0) {
      Serial.println("MEDIA_VOL_UP");
      Consumer.write(MEDIA_VOL_UP);
    } else {
      Serial.println("MEDIA_VOL_DOWN");
      Consumer.write(MEDIA_VOLUME_DOWN);
    }
  }
}

void encoder2() {
  int16_t diff = getEncoderDiff(RotaryEncoder2, rotary2Position, rotary2TimeStampedValue);
  if (diff != 0) {
    if (diff > 0) {
      Serial.println("CONSUMER_BRIGHTNESS_UP");
      Consumer.write(CONSUMER_BRIGHTNESS_UP);
    } else {
      Serial.println("CONSUMER_BRIGHTNESS_DOWN");
      Consumer.write(CONSUMER_BRIGHTNESS_DOWN);
    }
  }
}

// getEncoderDiff passes variables by reference so that it can be reused with multiple encoders
int16_t getEncoderDiff(ClickEncoder &encoder, int16_t &position, TimeStampedValue<int16_t> &timeStampValue) {
  position += encoder.getValue();
  int16_t diff = position - timeStampValue;

  // return if rotary encoder was not turned
  if (diff == 0) {
    return diff;
  }

  Serial.print("position: ");
  Serial.println(position);
  Serial.print("timeStampValue: ");
  Serial.println(timeStampValue);

  timeStampValue.set(position);
  return diff;
}
