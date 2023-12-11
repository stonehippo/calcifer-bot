# calcifer-bot

A simple HomeKit-powered bot to control our pellet stove.

If you want to read a bit about why I built this, and how the build went, check out out [my designer's notes](DESIGNER_NOTES.md).

## Dependencies

- [Arduino ESP8266 Core](https://github.com/esp8266/Arduino)
- [Arduino Servo Libary](https://github.com/arduino-libraries/Servo)
- [Arduino-HomeKit-ESP8266](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266)

You'll need [the Arduino IDE](https://www.arduino.cc/en/software) or [Arduino CLI](https://arduino.github.io/arduino-cli/latest/). I prefer the Arduino CLI these days, but either will work.

## Building

To build the firmware, first clone this repo. If you're using the IDE, it's probably best to clone it into your Sketchbook folder (usually something like `~/Arduino/`). If you're using the CLI, you can put it pretty much wherever you like.

You'll need to install the dependencies above, via the board and library managers.

Next, you will need to edit `wifi_config.h`, updating it to use your SSID and password:

```
#define MY_SSID		"ssid"
#define MY_PASSWORD	"password"
```

*Note: the ESP8266 doesn't support 5GHz WiFi, so make sure that you're using a network that support 2.4GHz!*

To build the firmware:

```
cd [path to where you cloned it]/calcifer-bot/
arduino-cli compile -b esp8266:esp8266:huzzah:xtal=160
```

Note that the board fully qualified name includes the parameters to run the ESP8266 at 160MHz. This is important, because without it the HomeKit crypto will probably cause timeouts when running at the default 80MHz. If you're using the Arduino IDE to compile this firmware, make sure to select the 160MHz frequency (see the Arduino-Homekit-ESP8266 README for full details on recommended Arduino IDE/CLI settings).

To upload the code once built, first put the device in bootloader mode (on the Huzzah, this means holding down the GPIO0 button while pressing the RESET button; other devices, such as the Adafruit Feather ESP8266 Huzzah, may not require this). Then run:

```
arduino-cli upload . -b esp8266:esp8266:huzzah:xtal=160 -p [your device's port]

```

You can compile and upload in one shot with this command (don't forget the bootloader mode first!):

```
arduino-cli compile -b esp8266:esp8266:huzzah:xtal=160 -u -p [your device's port]
```

## Hardware

I implemented my bot with the following components:

- [Adafruit ESP8266 Huzzah breakout board](https://www.adafruit.com/product/2471) - the brains of the operation
- [Micro servo](https://www.adafruit.com/product/169) - Drives calcifer-bot's arm
- [Red Candle Flicker LED](https://shop.evilmadscientist.com/productsmenu/partsmenu/576) - for ambiance! Oh, also indicates status, I suppose

If building this yourself, you can substitute pretty much any ESP8266 board out there, except perhaps the early ESP-01, which typically had only 512KB of RAM. You could also use just about any standard RC hobby servo.

# Configuring with HomeKit

First, power on the device and wait a minute or so while it connects to your Wifi.

Next, **make sure that your iOS device (iPad or iPhone) is connected to the same 2.4GHz WiFi network as the bot**. If you don't do this, you might still see the device, but set up won't complete. In my case, I had to switch my phone from the default 5GHz.

Open the Home app, and click the `+` button at the top of the view, then select **Add Accessory**. In the dialog, select `I Don't Have a Code or Cannot Scan`. The app will scan for your device, and when it's shown, tap on it. You'll be prompted to enter a code; by default it is '021-40-210'. You should then be asked how you'd like the device to be presented: I choose Switch. You also get to select which room the device appears in.

Once that's done, you should be able to control the device with On and Off states in the home app, or control it with Siri with commands like "Hey Siri, turn on Calcifer!".
