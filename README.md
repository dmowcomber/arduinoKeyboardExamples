# arduinoMediaKeyboard
An Arduino keyboard examples for the Arduino Pro Micro

# Compile an example
```
arduino-cli compile --fqbn arduino:avr:leonardo
```

# Get Arduino USB device
```
usbDevice=$(arduino-cli board list | grep usb | awk '{print $1}')
```

# Upload to the Arduino
```
arduino-cli upload -p $usbDevice --fqbn arduino:avr:leonardo
```
