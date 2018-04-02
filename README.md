# OPL2 Audio Board
This repository contains the OPL2 Audio Board library for Arduino, Raspberry Pi and Orange Pi as well as a number of examples on how the board can be used. Use this library to control the YM3812 chip and play music, sound effects or have fun with OPL2 FM-synthesis.

Current library version is 1.2.3

To obtain your own OPL2 Audio Board visit the [Tindie store](https://www.tindie.com/products/DhrBaksteen/opl2-audio-board/).

For more info visit the [Hackaday project page](https://hackaday.io/project/18995-opl2-audio-board-for-arduino-raspberry-pi).

![](https://raw.githubusercontent.com/DhrBaksteen/ArduinoOPL2/master/extra/OPL2_board.jpg)

## 1. Assembling the kit
For a step by step guide on how to assemble the kit please read the [Assembly guide](https://github.com/DhrBaksteen/ArduinoOPL2/blob/master/assembling.md).

**IMPORTANT!**
Please be aware that due to some last minute changes IC5 will be marked as 7404 on the board and in the brief assembly instructions, while you should use the included 7414!

## 2. Installing the library
#### Arduino
The easiest way to install the library it do download it through the Arduino Library Manager. Open the Library Manager from your Arduino IDE's Sketch > Include Library > Library Manager menu. A new window will open that allows you to search for a library. Search for 'Arduino OPL2' and it should show this libarary. Select the library, click the install button and you're good to go.

Alternatively you can clone this repo in the `libraries` folder of the Arduino IDE. or you can download the zip file and extract it in the libraries folder.

For more information see the [instructions](https://www.arduino.cc/en/Guide/Libraries) on the Arduino website.

#### Raspberry Pi / Orange Pi
To install the library onto your Pi clone this repo and run `./build`. You can specify the type of board you are using as a command line agrument. Use `./build -rpi` for Raspberry Pi or `./build -opi` for Orange Pi. If you do not specify a board type the build will assume you're building the library for a Raspberry Pi.

The OPL2 library requires [WiringPi](http://wiringpi.com/) to be installed on your Pi. Normally this library is already installed.

## 3. Connecting the OPL2 Audio Board
You can connect the OPL2 Audio Board directly to a speaker, however for the best results I recommend you connect some active (amplified) speakers. Headphones or line level recording will also work great. See the tables below how to connect the OPL2 Audio Board to your development platform.

#### Arduino
| OPL2 Board | Arduino Pin |
|------------|-------------|
| A0 | 9 |
| Data | 11<sup>*</sup> |
| Latch | 10 |
| Shift | 13<sup>*</sup> |
| Reset | 8 |

<sup>*</sup>Use pins 51 and 52 respectively on Arduino Mega, or refer to [https://www.arduino.cc/en/Reference/SPI](https://www.arduino.cc/en/Reference/SPI) for your Arduino board.

#### Raspberry Pi / Orange Pi
| OPL2 Board | GPIO Pin |
|------------|-------------|
| +5v | 2 |
| GND | 6 |
| A0 | 16 |
| Data | 19 |
| Latch | 12 |
| Shift | 23 |
| Reset | 18 |

## 4. Using the OPL2 Audio Board
#### Arduino
After installing the library and restarting the Arduino IDE you will find a number of examples under the File > Examples > ArduinoOPL2 menu. These examples can be found in the `examples` folder of the library.

#### Raspberry Pi / Orange Pi
After building the library you will find a number of examples in the `examples_pi` folder. The examples have been compiled during installation of the library, so you should be ready to dive right in! Try running `sudo ./demotune` for example from the `examples_pi/demotune` folder.

When compiling your own code using the OPL2 library don't forget to specify the library using the `-lOPL2` argument e.g. `gcc -std=c++11 -Wall -o my_program my_program.cpp -lOPL2 -lwiringPi`
