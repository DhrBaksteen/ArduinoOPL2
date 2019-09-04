# Getting started
This guide will tell you step by step in detail how to assemble and get started with your OPL2 Audio Board. If you bought a preassembled board you can skip the assembly guide and jump straight to [section 2](#2-connecting-the-opl2-audio-board) to install the software and get going!

## 1. Assembling the OPL2 Audio Board

### 1.1 Inventory
Before we start let's make sure that you have all the parts needed to assemble the OPL2 Audio Board. When your kit was packaged care was taken to make sure that you receive all the necessary parts. Still mistakes can happen, so if any part is missing please contact me to get you the missing part.

| Part  | Description | Quantity | Markings / Notes |
| ----- | ----------- | -------- | ----- |
|            | PCB                          | 1 | |
| C1 ... C4  | 10μF capacitor               | 4 | 106 |
| C5         | 0.1μF capacitor              | 1 | 104 |
| C6 ... C8  | 10pF capacitor               | 3 | 10 |
| C9         | 47μF electrolytic capacitor  | 1 | Mind negative pin marking |
| C10        | 220μF electrolytic capacitor | 1 | Mind negative pin marking |
| IC1        | YM3812                       | 1 | Mind position of the notch or dot that marks pin 1 |
| IC2        | Y3014B                       | 1 | Mind the dot that marks pin 1 |
| IC3        | LM358                        | 1 | Mind position of the notch |
| IC4        | LM386                        | 1 | Mind position of the notch |
| IC5        | 7414                         | 1 | Mind position of the notch |
| IC6        | 74595                        | 1 | Mind position of the notch |
| J1         | 7-pin male header            | 1 ||
| J2         | 3.5mm jack plug socket or 3-pin speaker terminal | 1 | You can use either one |
| R1         | 15kΩ resistor               | 1 | ![](https://placehold.it/15/A06000/000000?text=+) Brown, ![](https://placehold.it/15/00A000/000000?text=+) Green, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/FF0000/000000?text=+) Red |
| R2         | 100Ω resistor               | 1 | ![](https://placehold.it/15/A06000/000000?text=+) Brown, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black |
| R3         | 1MΩ resistor                | 1 | ![](https://placehold.it/15/A06000/000000?text=+) Brown, ![](https://placehold.it/15/F0F000/000000?text=+) Yellow, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black |
| R4         | 1kΩ resistor               | 1 | ![](https://placehold.it/15/A06000/000000?text=+) Brown, ![](https://placehold.it/15/A06000/000000?text=+) Brown, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black |
| R5         | 10kΩ trimmer potentiometer  | 1 | 103 |
| XTAL1      | 3.579 MHz crystal            | 1 ||
|| 24-pin DIP socket | 1 | Optional socket for IC1 |
|| 8-pin DIP socket  | 3 | Optional socket for IC2 .. IC4 |
|| 14-pin DIP socket | 1 | Optional socket for IC5 |
|| 18-pin DIP socket | 1 | Optional socket for IC6 |
 
Besides these parts you are going to need a soldering iron, some solder, flush cutters, needle-nose pliers and a small screwdriver.

### 1.2 Assembly instructions
##### 1.2.1. Before we begin
We will assemble the board in the order of component height working from smallest to tallest. Before starting to assemble the board think about how you are going to use it. Are you going to use headphones or are you attaching a loose speaker? Depending on your answer you should either use the socket for a jack plug or the terminal block to simply connect some wires for a speaker.

##### 1.2.2. Assembling the resistors
We will start with resistors R1 through R4. Bend their legs 90 degrees and stick them throught the PCB. Polarity does not matter. Bend the legs backwards a little so the resistors won't fall out when you flip the board over to solder them in. Once soldered clip off the excess leads.

##### 1.2.3. Pin header
We will continue with the pin header J1. In order to solder it nice and flush to the board the best way is to first solder just one pin. Then while heating the pin again with your iron move the socket from the other end so it sits right on the board. Once your happy with the placement of the sochet apply solder to the remaining pins.

##### 1.2.4. The crystal
Now we will attach the 3.579 MHz crystal XTAL1 which will provide the clock signal for the OPL2. Stick the legs of the crystal through the holes (orientation doesn't matter) and just like you did with the resistors before bed the legs out a little so the part won't fall out. After soldering it down clip off the leads.

##### 1.2.5. Capacitors (part 1)
Next it's time for capacitors C1 through C8. For these orientation doesn't matter. Give them the same treatment as the resistors and the crystal.

##### 1.2.6. Sockets or ICs
Now you will have to make a choice. Do you want to solder the ICs straight onto the board or do you want to place them into sockets. Either way is fine, but I would recommend to use sockets since it will make it much easier should you ever want to replace an IC.

Place the sockets (or ICs) for IC1 through IC6 on the board and mind the orientation of the cutouts! To make sure you assemble your sockts or chips flat an easy trick is to solder just one pin. Then heat the pin back up and give the other end of the component a little push until it sits flat on the board. Once the solder joint has cooled down solder the remaining pins and you will have a great result.

If you went with the sockets don't insert the ICs yet! We will do that last.

##### 1.2.7. Headphone jack
Time to make another choice. If you want to use the 3.5 mm jack plug socket then solder it in at J2. If you want to attach speakers using a wire then you should use the 3-pin terminal block and skip this step. 

To mount the socket flat onto the board you can use the same trick as we did for the IC sockets. First solder the center pin, reheat it and apply some preasure from the top of the socket to make it sit flat and then solder the remaining pins.

##### 1.2.8. Volume adjustment pot
Next we will continue with the volume adjustment pot R5. Insert its pins and solder it down.

##### 1.2.9. Capacitors (part 2)
We still have two capacitors left C9 and C10. Mind their values and the marking of the negative pin as you insert them. The negative band goes toward the hole with the minus symbol or the square pads. Ben the pins out a little, solder them down and clip off excess leads.

##### 1.2.10. Speaker terminal
If you decided to skip step 6 then this is the time to solder the speaker terminal at J2, otherwise you can skip this step

##### 1.2.12. Placing the ICs
Finally insert the ICs into their sockets. Make sure you line-up the notches on the ICs with the notches on the board. Some ICs have a little round cut-out in the upper left corner. This cut out must face the notch.

Be careful when inserting the chips to not bend their pins. If an IC's pins do not line up with the socket then carfully bend the pins inward. A good way to do this is to lie the chip on its side on your bench and carefully roll it slightly to bend one row of pins inward. Repeat this process on both sides but be careful not to over bend the pins!

All done! :)

### 1.3. Inspect your work
Now you've assembled the board carefully inspect the solder joints and make sure that they're all looking nice and clean and that no blobs of solder have spread between components. 

## 2. Connecting the OPL2 Audio Board
Connect some speakers or headphones to the board. To attach the OPL2 Audio Board to your favorite development board see the tables below:

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

## 3. Installing the software
#### Arduino
The easiest way to install the library it do download it through the Arduino Library Manager. Open the Library Manager from your Arduino IDE's Sketch > Include Library > Library Manager menu. A new window will open that allows you to search for a library. Search for 'Arduino OPL2' and it should show this libarary. Select the library, click the install button and you're good to go.

Alternatively you can clone this repo in the `libraries` folder of the Arduino IDE. or you can download the zip file and extract it in the libraries folder.

For more information see the [instructions](https://www.arduino.cc/en/Guide/Libraries) on the Arduino website.

#### Raspberry Pi / Orange Pi
To install the library onto your Pi clone this repo and run `sudo ./build`. The OPL2 library requires [WiringPi](http://wiringpi.com/) to be installed on your Pi. Normally this library is already installed, but if it's missing then the installation script can install it for you.

# 4. Testing
It's finally time to hear that sweet OPL2 FM-synth from your board for the first time. If you don't hear anything or if the audio is distorted use a small screwdriver to adjust trimmer R5 until the audio sounds right.

#### Arduino
After installing the library and restarting the Arduino IDE you will find a number of examples under the File > Examples > ArduinoOPL2 menu. These examples can be found in the `examples` folder of the library. Open the `DemoTune` example and send it to your Arduino. You will hear a short piano tune play. 

For a more advance use of the OPL2 AUdio Board refer to the `SerialIface` example. This is a fully fledged game audio player that plays various music formats through the serial port of your computer.

#### Raspberry Pi / Orange Pi
After building the library you will find a number of examples in the `examples_pi` folder. The examples have been compiled during installation of the library, so you should be ready to dive right in! Try running `sudo ./demotune` for example from the `examples_pi/demotune` folder. It will play a short piano tune.

You can also try `examples_pi/opl2play`, which can play various music files for OPL2 by running `sudo ./opl2play [song1] [song2] ...`. Simply run the application without any arguments or `--help` to see the command line options.

When compiling your own code using the OPL2 library don't forget to specify the library using the `-lOPL2` argument e.g. `gcc -std=c++11 -Wall -o my_program my_program.cpp -lOPL2`
