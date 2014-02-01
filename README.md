# Ardustick

Ardustick by [Mathias Djärv](http://google.com/+MathiasDjärv)

This project uses an Arduino combined with a cheap 433MHz transmitter to control
Nexa-compatible devices using commands over the serial port.

![Ardustick](https://raw2.github.com/mdjarv/ardustick/master/ardustick.jpg "Ardustick")

## Hardware
My setup consists of two parts, note however that this would probably work on any kind of arduino. I chose the Nano v3.0 for the formfactor and the integrated USB port.

* Arduino Nano v3.0
* FS1000A RF 433MHz transmitter

The Arduino Nano v4.0 can also be used however it is worth noting that it has a newer usb-serial chipset (Profilic PL-2303HX) which can impact compatibility depending on what you connect it to. I had a v3.0 (which uses the old FTDI FT232RL serial chip) connected to my Qnap NAS with no problem, while v4.0 was not showing up because of missing kernel modules.

The FS1000A is a very cheap 433MHz transmitter that are usually sold in transmitter/reciever pairs for about US $1 a pair.

## Schematic

The schematic is as simple as can be, connect VCC and GND on the FS1000A to the matching pins on the Arduino and connect the ATAD pin (signal) to Arduino pin 2.

Connect the Arduino to a computer and send the commands over the serial port. 

![Schematic](https://raw2.github.com/mdjarv/ardustick/master/schematic.png "Schematic of the ardustick")

## Software

This sketch requires the [NexaCtrl](https://github.com/calle-gunnarsson/NexaCtrl) library for arduino
The Arduino listens for strings in the following format:

    SET,remote,device,on/off\n

Breakdown:

* `SET` - prefix for command
* remote - the remote id to use, my Nexa remote had an 8-digit number which could be a good reference
* device - device number, a reference to what remote-button to simulate
* on/off - `1` for on, `0` for off
* `\n` - line ending terminating the command

I used some examples found through the [Arduino HomeEasy](http://playground.arduino.cc/Code/HomeEasy) page to read my remote id, but an easier way is to just make up a number here, and then train your recievers using this sketch as it simulates the remote keypresses.

### Examples

*Remote 12345678, device 2, on*

    SET,12345678,2,1

*Remote 12345678, device 1, off*

    SET,12345678,1,0
