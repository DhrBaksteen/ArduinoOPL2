# Assembling the OPL2 Audio Board
This guide will tell you step by step how to assemble and get started with your OPL2 Audio Board.

### Inventory
Before we start let's make sure that you have all the parts needed to assemble the OPL2 Audio Board. When your kit was packaged care was taken to make sure that you receive all the necessary parts. Still mistakes can happen, so if any part is missing please contact me to get you the missing part.

| Part  | Description | Quantity | Markings / Notes |
| ----- | ----------- | -------- | ----- |
|            | PCB                          | 1 | |
| C1         | 4.7nF capacitor              | 1 | 472 |
| C2         | 0.1μF capacitor              | 1 | 104 |
| C3 ... C6  | 10μF electrolytic capacitor  | 4 | Mind negative pin marking |
| C7         | 10pF capacitor               | 1 | 10 |
| C8         | 10nF capacitor               | 1 | 102 |
| C9         | 100μF electrolytic capacitor | 1 | Mind negative pin marking |
| IC1        | YM3812                       | 1 | Mind position of the notch |
| IC2        | Y3014B                       | 1 | Mind the dot that marks pin 1 |
| IC3        | LM358                        | 1 | Mind position of the notch |
| IC4        | LM386                        | 1 | Mind position of the notch |
| IC5        | 7404                         | 1 | Mind position of the notch |
| IC6        | 74595                        | 1 | Mind position of the notch |
| J1         | 7-pin male header            | 1 ||
| J2         | 3.5mm jack plug socket       | 1 ||
| R1, R2     | 470Ω  resistor              | 2 | ![](https://placehold.it/15/F0F000/000000?text=+) Yellow, ![](https://placehold.it/15/A000A0/000000?text=+) Violet, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black |
| R3         | 10kΩ trimmer potentiometer  | 1 | 103 |
| R4         | 100Ω resistor               | 1 | ![](https://placehold.it/15/A06000/000000?text=+) Brown, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black |
| R5         | 15kΩ resistor               | 1 | ![](https://placehold.it/15/A06000/000000?text=+) Brown, ![](https://placehold.it/15/00A000/000000?text=+) Green, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/FF0000/000000?text=+) Red |
| R6         | 220Ω resistor               | 1 | ![](https://placehold.it/15/FF0000/000000?text=+) Red, ![](https://placehold.it/15/FF0000/000000?text=+) Red, ![](https://placehold.it/15/000000/000000?text=+) Black, ![](https://placehold.it/15/000000/000000?text=+) Black |
| XTAL1      | 3.579 MHz crystal            | 1 ||
 
Besides these parts you are going to need a soldering iron, some solder, flush cutters, needle-nose pliers and a small screwdriver.

### Before we begin
One more important note before we begin to assemble the board. You can drive an 8Ω speaker directly from the board, although it will not provide great volume. It is recommended to use the board with active (amplified) speakers, headphones, or use it for line level audio recording. If you do wish to connect a speaker directly you can omit C8 and replace R6 with a jumper wire. These components normally provide noice reduction, but are not needed when directly driving a speaker.  

### Assembling the resistors
To assemble the resistorsR5 and R6 bend their legs 90 degrees and stick them throught the PCB. Polarity does not matter. Bend the legs backwards a little so the resistors won't fall out when you flip the board over to solder them in. 

Resistor R6 is optional, though recommended. It is used to reduce the noice on the output from your Arduino or Raspberry Pi, but at the same time it also reduced the audio volume. If you want to drive a small non-amplified speaker directly from the board you may consider leaving out R6 and replacing it by a jumper wire. You can salvage a cut off resistor or capacitor leg for this.

For the upright resistors R1, R2 and R4 bend one of the legs so it becomes parallel to the other leg. You may want to add these last to make it easier t assemble the other components by working from smallest to tallest.

### Assembling the capacitors
Insert the capacitors into the board just like you did with the resistors and bend their legs outward a little to stop them from falling out when you flip to board over. For C1, C2, C7 and C8 the polarity does not matter. Remember that C8 is optional, though recommended.

For the other electrolytic capacitors C3 through C6 and C9 make sure you place them according to their polarity markings. Each electrolytic capacitor has a bright colored band on its side that marks which pin is negative. The negative leg of the capacitor should go into the hole with a square pad, also marked with a '-' sign on the pcb. Like the upright resistors you may want to add these components last to make soldering easier.

### Crystal, trimmer, pin header and audio jack
Now let's assemble the 3.579 MHz crystal XTAL1, the pin header, Trimmer resistor R3 and the audio jack. Polarity of XTAL1 doesn't matter. YOu can bend the legs outward a little to stop it from falling out when flipping the board over.

### Assembling the chips
When placing the ICs on the board make sure that the notches (or dot) of the ICs face toward the gaps in the markings on the PCB. Notice that for all ICs except IC6 the notches should face up.

You may have to bend the legs of some of the ICs inward a little for them to align with the holes in the board. An easy way to do this is by laying the chip on its side on the table and carefully and very slightly bending it toward the end of the legs. This way all pins will bend evenly.

# Inspect your work
Now you've assembled the board carefully inspect the solder joints and make sure that they're all looking nice and clean and that no blobs of solder have spread between components. 

# Connecting the OPL2 Audio Board
Connect some speakers or headphones to the 3.5mm jack. See the tables below how to connect the pin header on the board to your development platform.

#### Arduino
| OPL2 Board | Arduino Pin |
|------------|-------------|
| A0 | 9 |
| Data | 11 |
| Latch | 10 |
| Shift | 13 |
| Reset | 8 |

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

# Installing the software
#### Arduino
The easiest way to install the library is to clone this repo in the `libraries` folder of the Arduino IDE. Alternatively you can download the zip file and extract it in the libraries folder.

For more information see the [instructions](https://www.arduino.cc/en/Guide/Libraries) on the Arduino website.

#### Raspberry Pi / Orange Pi
To install the library onto your Pi clone this repo and run `./build`. You can specify the type of board you are using as a command line agrument. Use `./build -opi` for Orange Pi or `./build -rpi` for Raspberry Pi. If you do not specify a board type the build will assume you're building the library for a Raspberry Pi.

The OPL2 library requires [WiringPi](http://wiringpi.com/) to be installed on your Pi. Normally this library is already installed.

# Testing
It's finally time to hear that sweet OPL2 FM-synth from your board for the first time. If you don't hear anything or if the audio is distorted use a small screwdriver to adjust trimmer R3 until the audio sounds right.

#### Arduino
After installing the library and restarting the Arduino IDE you will find a number of examples under the File > Examples > ArduinoOPL2 menu. These examples can be found in the `examples` folder of the library. Open the `DemoTune` example and send it to your Arduino. You will hear a short piano tune play. 

For a more advance use of the OPL2 AUdio Board refer to the `SerialIface` example. This is a fully fledged game audio player that plays various music formats through the serial port of your computer.

#### Raspberry Pi / Orange Pi
After building the library you will find a number of examples in the `examples_pi` folder. The examples have been compiled during installation of the library, so you should be ready to dive right in! Try running `sudo ./demotune` for example from the `examples_pi/demotune` folder. It will play a short piano tune.

You can also try `examples_pi/opl2play`, which can play various music files for OPL2 by running `sudo ./opl2play [song1] [song2] ...`. Simply run the application without any arguments or `--help` to see the command line options.

When compiling your own code using the OPL2 library don't forget to specify the library using the `-lOPL2` argument e.g. `gcc -std=c++11 -Wall -o my_program my_program.cpp -lOPL2`