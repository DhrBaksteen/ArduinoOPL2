# OPL2 Audio Board
This repository contains the OPL2 Audio Board library for Arduino, Teensy, Raspberry Pi and compatible boards as well as a number of examples on how the board can be used. You can use the OPL2 Audio Board to:
* Experiment with the YM3812 chip
* Build your own synthesizer
* Play your own OPL2 tunes
* Play exported OPL2 music (DRO, IMF, VGM) or Reality Adlib Tracker music files
* Use the board as a MIDI synthesizer (Teensy++ 2.0 and later example included)
* Emulation with DosBox you can use the board to output MIDI music (Teensy++ 2.0 and later)

Current library version is 1.4.2

To obtain your own OPL2 Audio Board visit the [Tindie store](https://www.tindie.com/products/DhrBaksteen/opl2-audio-board/).

For more info visit the [Hackaday project page](https://hackaday.io/project/18995-opl2-audio-board-for-arduino-raspberry-pi).

![](https://raw.githubusercontent.com/DhrBaksteen/ArduinoOPL2/master/extra/OPL2_board.jpg)


## 1. Assembling the kit
For a step by step guide on how to assemble the kit please read the [Assembly guide](https://github.com/DhrBaksteen/ArduinoOPL2/blob/master/assembling.md).

**IMPORTANT!**
Please be aware that due to some last minute changes IC5 will be marked as 7404 on the board and in the brief assembly instructions, while you should use the included 7414!


## 2. Installing the library
#### Arduino / Teensy
The easiest way to install the library it do download it through the Arduino Library Manager. Open the Library Manager from your Arduino IDE's Sketch > Include Library > Library Manager menu. A new window will open that allows you to search for a library. Search for 'Arduino OPL2' and it should show this libarary. Select the library, click the install button and you're good to go.

Alternatively you can clone this repo in the `libraries` folder of the Arduino IDE. or you can download the zip file and extract it in the libraries folder.

For more information see the [instructions](https://www.arduino.cc/en/Guide/Libraries) on the Arduino website.

#### Raspberry Pi / Orange Pi and compatibles
To install the library onto your Pi clone this repo and run `./build`. The OPL2 library requires [WiringPi](http://wiringpi.com/) to be installed on your Pi. Normally this library is already installed, but if this is not the case then the build script can install it for you.


## 3. Connecting the OPL2 Audio Board
You can connect the OPL2 Audio Board directly to a speaker, however for the best results I recommend you connect some active (amplified) speakers. Headphones or line level recording will also work great. See the table below how to connect the OPL2 Audio Board to an Arduino. [Visit the wiki](https://github.com/DhrBaksteen/ArduinoOPL2/wiki/Connecting) to learn how to connect the OPL2 AUdio Board to your favorite development platform.

#### Arduino
| OPL2 Board | Arduino Pin |
|------------|-------------|
| A0 | 9 |
| Data | 11 |
| Latch | 10 |
| Shift | 13 |
| Reset | 8 |


## 4. Using the OPL2 Audio Board
#### Arduino
After installing the library and restarting the Arduino IDE you will find a number of examples under the File > Examples > ArduinoOPL2 menu. These examples can be found in the `examples` folder of the library.

#### Teensy
Most Arduino examples will also work directly on a Teensy. The examples that require an SD card (PlayDRO, PlayIMG and PlayRAD) are Teensy specific when you use the Teensy's on board SD card slot. Teensy++ 2.0 and later can be configured as MIDI device. An example for this can be found among the Teensy specific examples in the `examples/teensy` folder.

#### Raspberry Pi / Orange Pi
After building the library you will find a number of examples in the `examples_pi` folder. The examples have been compiled during installation of the library, so you should be ready to dive right in! Try running `sudo ./demotune` for example from the `examples_pi/demotune` folder.

When compiling your own code using the OPL2 library don't forget to specify the library using the `-lOPL2` argument e.g. `gcc -std=c++11 -Wall -o my_program my_program.cpp -lOPL2 -lwiringPi`
