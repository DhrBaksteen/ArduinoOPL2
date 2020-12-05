# An in-depth look at programming the OPL2 and OPL3 
Producing sound on the YM3812 (OPL2) or YMF262 (OPL3) sound chip is not a trivial task. There are many registers that need to be set up just to produce some very simple tones. However with the right control over all of its registers the YM3812 and YMF262 are capable of producing some very impressive audio.

In this document we will have an in-depth look at all of the registers of the OPL2 and OPL3.

## Channels, Operators and registers
---
The YM3812 has 9 channels that can each produce a sound independently from the other channels. The sound of each channel is produced by two operators. Operator 1 and operator 2, or the modulator and the carrier.

## Synthesizer configuration
---
The two operators of each channel can be configured to work in one of two ways; in additive synthesis (AM) mode where the output of the two operators is simply added together. Or in frequency modulation (FM) mode where the output of operator 1, the modulator, influences the frequency op operator 2, the carrier. The synthesizer type is set in bit 0 of base register 0xC0. Setting this bit to 0 will put the channel in FM mode, setting it to 1 puts the channel in AM mode.

##### OPL3 4-operator synthesizer mode

## Audio Frequencies
---
Per channel there are two registers to control the base frequencie of the oscillators for the channel's operators. These are the registers at base address 0xA0 and 0xB0 that hold the frequency block and the F-number.

The F-number is a value from 0 to 1023 that together with the current block number sets the channel's oscillator frequency. We will first look at the frequency blocks, because the block number defines the frequency range of the F-numbers. The block number can be set to a value from 0 to 7 and this results in the following frequency ranges:
| Block # | F-num = 1 | F-num = 1023 | F-num step |
| --- | --- | --- | --- |
| 0 | 0.047 Hz | 48.503 Hz | 0.048 Hz |
| 1 | 0.094 Hz | 97.006 Hz | 0.095 Hz |
| 2 |  0.189 Hz | 194.013 Hz | 0.190 Hz |
| 3 | 0.379 Hz | 388.026 Hz | 0.379 Hz |
| 4 | 0.758 Hz | 776.053 Hz | 0.759 Hz |
| 5 | 1.517 Hz | 1552.107 Hz | 1.517 Hz |
| 6 | 3.034 Hz | 3104.215 Hz | 3.034 Hz |
| 7 | 6.068 Hz | 6208.431 Hz | 6.069 Hz |

Thus by setting the frequency block of a channel we know the difference between two F-numbers and this we can use to work out the F-number needed to get a specific frequency. Setting the F-number to 0 will alwyas result in a frequency of 0 Hz, regardless of the block. If it's required to approach the desired frequency as close as possible then it's important to choose the right block for the frequency. However this is not all there is to calculating the oscillator frequency. The block and F-number will only determine the base frequency for the channel. Below we will see how the eventual frequency of each operator can be altered by a multiplication factor.

The block number is set in bits 2, 3 and 4 of base register 0xB0. The F-number is spread over two registers. The lower 8 bits of the F-number are stored in base register 0xA0 and the upper 2 bits are stored in bits 0 and 1 of base rigister 0xB0.

##### Frequency multiplication factor
Besides the block and the F-number that we can set per channel there is also a setting per operator that influences the frequency of each operator oscillator. This is the frequency multiplication factor. The frequency multiplication factor does just as its name implies; it multiplies the channel frequency with a given factor to get the operator's oscillator frequency. So to know the output frequencies of the operators for a channel we multiply the channel base frequency by the operator's multiplication factor.

The multiplier can be set to a value from 0 to 15 and this will set the multiplier to the given value, thus giving us a maximum multiplication factor of 15. Setting the multiplier to 0 will result in a multiplication factor of 0.5. The frequency multiplier is set in the lower 4 bits of base register 0x20.

##### Playing musical notes
If you are familiar with the frequencies of musical notes you may have noticed that there is a nice property to the block numbers that we can use to our advantage when we want the OPL chip to play music. Below is a table that defines the frequencies of the musical notes for 8 octaves. Notice how when we go up one octave the frequency doubles and when we step down an octave the frequency halves for each note.

| Octave | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **C**  | 16.35 Hz | 32.70 Hz | 65.41 Hz | 130.81 Hz | 261.63 Hz | 523.25 Hz | 1046.50 Hz | 2093.00 Hz |
| **C#** | 17.32 Hz | 34.65 Hz | 69.30 Hz | 138.59 Hz | 277.18 Hz | 554.37 Hz | 1108.73 Hz | 2217.46 Hz |
| **D**  | 18.35 Hz | 36.71 Hz | 73.42 Hz | 146.83 Hz | 293.66 Hz | 587.33 Hz | 1174.66 Hz | 2349.32 Hz |
| **D#** | 19.45 Hz | 38.89 Hz | 77.78 Hz | 155.56 Hz | 311.13 Hz | 622.25 Hz | 1244.51 Hz | 2489.02 Hz |
| **E**  | 20.60 Hz | 41.20 Hz | 82.41 Hz | 164.81 Hz | 329.63 Hz | 659.25 Hz | 1318.51 Hz | 2637.02 Hz |
| **F**  | 21.83 Hz | 43.65 Hz | 87.31 Hz | 174.61 Hz | 349.23 Hz | 698.46 Hz | 1396.91 Hz | 2793.83 Hz |
| **F#** | 23.12 Hz | 46.25 Hz | 92.50 Hz | 185.00 Hz | 369.99 Hz | 739.99 Hz | 1479.98 Hz | 2959.96 Hz |
| **G**  | 24.50 Hz | 49.00 Hz | 98.00 Hz | 196.00 Hz | 392.00 Hz | 783.99 Hz | 1567.98 Hz | 3135.96 Hz |
| **G#** | 25.96 Hz | 51.91 Hz | 103.83 Hz | 207.65 Hz | 415.30 Hz | 830.61 Hz | 1661.22 Hz | 3322.44 Hz |
| **A**  | 27.50 Hz | 55.00 Hz | 110.00 Hz | 220.00 Hz | 440.00 Hz | 880.00 Hz | 1760.00 Hz | 3520.00 Hz |
| **A#** | 29.14 Hz | 58.27 Hz | 116.54 Hz | 233.08 Hz | 466.16 Hz | 932.33 Hz | 1864.66 Hz | 3729.31 Hz |
| **B**  | 30.87 Hz | 61.74 Hz | 123.47 Hz | 246.94 Hz | 493.88 Hz | 987.77 Hz | 1975.53 Hz | 3951.07 Hz |

Now that we know of this property and we look back at the table that defines the frequency ranges for the block numbers you will notice that it does the same. When we step up a block number the step in frequency per F-number doubles and when we step down a block it halves. Well, more or less, but it's close enough for us to treat the block number as the octave. This way we can use a fixed set of 12 F-numbers to define the notes of the musical scale as follows.

| Note | C | C# | D | D# | E | F |F# | G | G# | A | A# | B |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **F-number** | 342 | 363 | 385 | 408 | 432 | 458 | 485 | 514 | 544 | 577 | 611 | 647 |

The OPL2 library has these note F-number defined as the `noteFNumbers` array.

##### Relevant library functions
```c++
void setBlock(byte channel, byte block);
void setFNumber(byte channel, short fNumber);
void setFrequency(byte channel, float frequency);

byte getBlock(byte channel);
short getFNumber(byte channel);
float getFrequency(byte channel);

void playNote(byte channel, byte octave, byte note);
byte getFrequencyBlock(float frequency);
short getFrequencyFNumber(byte channel, float frequency);
short getNoteFNumber(byte note);
float getFrequencyStep(byte channel);
```

## Changing Volume
---
Per operator there is one register that controls the volume. This is the register at base address 0x40 and it defines the key scale level and the output level. 

Changing volume is, again, not a straight forward task and it may be a bit counter intuitive. It depends on the synthesis mode of the channel and for the OPL3 it also depends on whether the channel operates in 2-operator or 4-operator mode. Let's first look at the simplest case where we have a channel in AM (additive synth) mode. To change volume for this channel we have to set the output levels of both operators 1 and 2. When the channel is set to FM (frequency modulation) mode only the output level of the carrier (operator 2) should be changed. Changing the modulator's output level will result in the frequency range of the carrier being changed for FM channels and that may not be what you want, but it can result in interesting sounds.

Now the part that can be counter intuitive is that the value for the output level *reduces* the volume. The output level is a value that ranges from 0 to 63, where 0 is loudest and 63 is softest. The output level is set in bits 0 through 5 of base register 0x40. The reduction in output level is given in the table below. So to reduce the volume by 9 dB we would set an output level of 12.

| Bit | D5 | D4 | D3 | D2 | D1 | D0 |
| --- | --- | --- | --- | --- | --- | --- |
| **Attenuation** | 24 dB | 12 dB | 6 dB | 3 dB | 1.5 dB | 0.75 dB |

Below is a table that shows for which operator(s) the output level should be set to change volume depending on the channel's synthesis mode. For an OPL2 or an OPL3 with a channel in 2-operator mode only the first two rows apply.

| Mode | OP-1 | OP-2 | OP-3 | OP-4 |
| --- | --- | --- | --- | --- |
| **AM** | X | X | N/A| N/A |
| **FM** | | X | N/A | N/A |
| **FM-FM** |||| X |
| **AM-FM** | X ||| X |
| **FM-AM** || X || X |
| **AM-AM** | X || X | X |

The OPL2 library functions `setChannelVolume` and `set4OPChannelVolume` have this logic internally, so the programmer does not have to worry about it. The `setVolume` function can be used to manually change the output level of each operator if so desired.

##### Key scale level
Additional attenuation of the output level of each operator is provided by the key scale level. The key scale level reduces the volume of an operator depending on the current block number of the channel, effectively reducing the volume per octave and this can help to make your instruments sound more natural.

| KSL | 0 | 1 | 2 | 3 |
| --- | --- | --- | --- | --- |
| **Attenuation** | 0 dB | 3.0 dB / block | 1.5 dB / block | 6.0 dB / block |

The key scale level is set in bits 6 and 7 of base register 0x40.

##### Relevant library functions
```c++
void setChannelVolume(byte channel, byte volume);
void set4OPChannelVolume(byte channel4OP, byte volume);
void setVolume(byte channel, byte operatorNum, byte volume);
void setScalingLevel(byte channel, byte operatorNum, byte scaling);

byte getChannelVolume(byte channel);
byte get4OPChannelVolume(byte channel4OP);
byte getVolume(byte channel, byte operatorNum);
byte getScalingLevel(byte channel, byte operatorNum);
```

## Attack, Decay, Sustain and Release
---
For each operator a sound envelope can be configured that shapes the sound as it's played over time. The envelope is part of what makes a piano sound different from a flute. When you stike a key on a piano and carfully listen to the sound you will hear that it is almost instantly at its full volume. When you release the key you will hear the sound decrease in volume over some period of time time, the piano is not instantly silent when you release the key. The opposite is true for the flute when you play a note it takes a little time to reach full volume, but when you stop blowing in to it the sound immediately stops. This characteristic is what we call the envelope.

To set the envelope of an operator we have 4 parameters at our disposal:
##### Attack
 The time it takes from when you set the key on for the volume to climb from complete silence to to maximum as defined by the output level of the operator. Attack is a value from 0 to 15 where the higher the value, the quicker the attack. Attack must be greater than 0, as a value of 0 will cause the sound to never attack. Attack is set in the upper 4 bits of base register 0x60.
   
##### Decay
The time it takes for the volume to drop from maximum to the sustain level. The decay portion of the envelope immediately follows after the attack. Decay is a value from 0 to 15 where a higher value means a shorter decay time. Decay is set in the lower 4 bits of base register 0x60.
  
##### Sustain
The sustain level defines by how much the output level of the sound will be reduced by the decay portion of the envelope after attack. The sound will remain playing at the resulting output level as long as the key is held. Sustain is a value from 0 to 15 that is set in the upper 4 bits of base register 0x80. Each bit reduces the output level as follows. So in order to reduce the output level by 15 dB after attack a sustain of 5 is set.

| Bit | SL3 (D7) | SL2 (D6) | SL1 (D5) | SL0 (D4) |
| --- | --- | --- | --- | --- |
| **Attenuation** | -24 dB | -12 dB | -6 dB | -3 dB |

A sustain value of 0 results in the loudest sustain where the output level is not reduced. A value of 15 results in the biggest reduction and is a special case where the output level is reduced by 93 dB. Notice that sustain must be enabled separately by setting the 'maintain sustain' bit 5 of base register 0x20. If this bit is not enabled then there will be no sustain and the envelope will proceed to the release portion immediately after its decay.

##### Release
The time it takes from when the key is release for the sound to die down to be silent. Release is defined as a value from 0 to 15 where a higher value results in a shorter release time. If the release time is set to 0 then the sound will never release and remain playing. The release rate is set in the lower 4 bits of base register 0x80.

##### Envelope scaling
There is one last setting for each operator that we can use to scale the durations of attack, decay and release depending on the frequency of the operator's oscillator. This is the 'key scale rate' (KSR). When enabled it will make the duaraion of the envelope shorter as the oscillator frequency rises and this makes your instruments sound more natural. The KSR is enabled by setting bit 4 of base register 0x20.

##### Relevant library functions:
```c++
void setAttack(byte channel, byte operatorNum, byte attack);
void setDecay(byte channel, byte operatorNum, byte decay);
void setSustain(byte channel, byte operatorNum, byte sustain);
void setRelease(byte channel, byte operatorNum, byte release);
void setMaintainSustain(byte channel, byte operatorNum, bool enable);
void setEnvelopeScaling(byte channel, byte operatorNum, bool enable);

byte getAttack(byte channel, byte operatorNum);
byte getDecay(byte channel, byte operatorNum);
byte getSustain(byte channel, byte operatorNum);
byte getRelease(byte channel, byte operatorNum);
bool getMaintainSustain(byte channel, byte operatorNum);
bool getEnvelopeScaling(byte channel, byte operatorNum);
```

## Waveforms
---
For each operator we can select the waveform that the oscillator will produce. On the OPL2 we have 4 different waveforms at our disposal and on the OPL3 we have a total of 8 we can choose from. The operator waveform is set in bits 0, 1 (and 2 on an OPL3) of base register 0xE0. The following wave forms are available. Waveforms 4 through 7 only apply to OPL3 chips.

##### OPL2 Specifics
On the OPL2, before we can change operator waveforms we first need to enable waveform selection (WSE) on the chip. This is done by setting bit 5 of register 0x01. If waveform selection is disabled on the OPL2 then all operator oscillators will produce a sine wave regardless of the waveform that was set.

##### OPL3 Specifics
On the OPL3 the waveform selection enable bit is not implemented. The OPL3 can always select waveforms 0 through 3. However, when the OPL3 is reset it will always start in OPL2 compatible mode. In order to select wave forms 4 through 7 the chip must first be put into OPL3 mode by setting bit 0 of register 0x105. See below for more details on OPL3 specific functions.

##### Relevant library functions
```c++
void setWaveForm(byte channel, byte operatorNum, byte waveForm);
void setWaveFormSelect(bool enable);

byte getWaveForm(byte channel, byte operatorNum);
bool getWaveFormSelect();
```


## Vibrato & Tremolo
---
Vibrato an the effect that changes or modulates the frequency of an oscillator over time by a sine wave to produce a small regular variation in pitch. This effect is available to all operators and can be enabled by setting bit 6 of base register 0x20. When vibrato is enabled the output frequency of the operator's oscillator will fluctuate with 7/100th of a semi-tone. We can also enable a deep vibrato of 14/100th of a semi-tone by setting bit 6 of register 0xBD, but enabling the deep vibrato is a chip wide setting, thus it will affect all operators that have vibrato enabled.

Tremolo is the same kind of effect as vibrato, but applied to the amplitude of the waveform generated by an operator's oscillator. This effect is also available to all operators and it is enabled by setting bit 7 of base register 0x20. When enabled tremolo will make the output level of the operator fluctuate by 1.0 dB. Like vibrator there is also a deep tremolo that can be enabled by setting bit 7 of register 0xBD. The deep tremolo makes the output level fluctuate by 4.8 dB. Enabling deep tremolo is a chip wide setting and will affect all operators that have tremolo enabled.

##### Relevant library functions
```c++
void setVibrato(byte channel, byte operatorNum, bool enable);
void setTremolo(byte channel, byte operatorNum, bool enable);
void setDeepVibrato(bool enable);
void setDeepTremolo(bool enable);

bool getVibrato(byte channel, byte operatorNum);
bool getTremolo(byte channel, byte operatorNum);
bool getDeepVibrato();
bool getDeepTremolo();
```

## Feedback
---
Base reg 0xC0

## Making a Sound
---
Now that we've seen mose of the registers on the OPL2 and OPL3 chips and we know their function let's apply our knowledge to produce a simple sound. The following code will setup a brass instrument. Not all registers are needed to produce this sound. Any register that is not changed after a reset of the OPL2 will be 0. 

```c++
opl2.begin();

opl2.setAttack(0, OPERATOR1, 7);
opl2.setDecay(0, OPERATOR1, 2);
opl2.setSustain(0, OPERATOR1, 2);
opl2.setRelease(0, OPERATOR1, 2);
opl2.setMaintainSustain(0, OPERATOR1, true);
opl2.setVolume(0, OPERATOR1, 22);
opl2.setMultiplier(0, OPERATOR1, 1);
opl2.setVibrato(0, OPERATOR1, true);

opl2.setAttack(0, OPERATOR2, 7);
opl2.setDecay(0, OPERATOR2, 15);
opl2.setSustain(0, OPERATOR2, 0);
opl2.setRelease(0, OPERATOR2, 3);
opl2.setMaintainSustain(0, OPERATOR2, true);
opl2.setVolume(0, OPERATOR2, 5);
opl2.setMultiplier(0, OPERATOR2, 1);

opl2.setFeedback(0, 6);
opl2.setSynthMode(0, false);
```

Once we have the two operators setup we can start to play a note. Let's setup channel 0 to play note C-4:
```c++
opl2.setBlock(0, 4);
opl2.setFNumber(0, 342);
```

To now start playing the note we have to set the key-on bit of channel 0. This is bit 5 of base address 0xB0. Immediately after setting the key-on the note will start playing.
```c++
opl2.setKeyOn(0, true);
```

The note will remain playing as long as the key-on bit is set. If sustain was enabled then the note will be held at sustain level. If sustain was not enabled then the note will start to die down immediately. Since in this example sustain is enabled we'll hold the note for a while before releasing the key to hear the note release.
```c++
delay(2000);
opl2.setKeyOn(0, false);
```

## Percussion Mode
---
Ba-Dum Tsss

## OPL3 special functions
---
The YMF262 OPL3 chip is in essence the same as the YM3812 OPL2. After the chip is reset it will behave exactly the same as the OPL2 with the only exception that it has 18 channels at its disposal instead of the 9 channels the YM3812 has. The OPL3 however has some additional features:
* 18 2-Operator channels
* 6 Of which can be configured as 4-operator channels (leaving 6 2-operator channels)
* 4 Additional wave forms
* Four digital output channels. Regular sound cards and the OPL2 library only use 2 channels to produce stereo sound

To use the features that the OPL3 has to offer you need to first set the OPL3Enable bit. 

* Enabeling OPL3 mode
* 4-op sounds
* Panning

## References
---
1. OPL Chip, Modding Wiki
  [http://www.shikadi.net/moddingwiki/OPL_chip](http://www.shikadi.net/moddingwiki/OPL_chip)
2. Programming the AdLib/Sound Blaster FM Music Chips, Jeffrey S. Lee
  [http://bespin.org/~qz/pc-gpe/adlib.txt](http://bespin.org/~qz/pc-gpe/adlib.txt)
3. YM3812 datasheet, Yamaha
4. YMF262 datasheet, Yamaha
