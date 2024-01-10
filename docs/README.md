# Tutorial overview

## Introduction

Have you ever dreamed of making your own device? Do you think it's a difficult thing and requires at least a PhD in electronics?
Well, we no longer live in *Stone Age* and there is no need to chisel transistors out of a silicon wafer. There are way better solutions on the market nowadays.

This tutorial presents an affordable way to your very first robotics. For ~€20-25 you can build your own robot that actually can fight! 

> [!IMPORTANT]
> This tutorial assumes you have basic familiarity with one of C-like languages: C, C++, Java, JavaScript, Python.

This tutorial shall follow DRY principle: Don't Repeat Yourself. If there's a good resource, I'll simply point you there.

## Table of contents 
* [Hello World](chapters/0_hello_world.md)
  * Never seen an IDE? Want to test your set up? Come here!
* [I/O handling](chapters/1_io_handling.md)
  *  How to control pins in your board? Start here!
* [Analog input, Pulse Width Modulation](chapters/2_analog_and_pwm.md)
  * Life in between 0V and 5V
* [UART](chapters/3_uart.md)
  * Data transfer between PC and microcontroller
* [Controlling Stepper Motors](chapters/4_stepper_motor.md)
* [Controlling Servos](chapters/5_servo.md)
* [Integrating knowledge - Building a robot!](chapters/6_robot_integration.md)

Your best friend during the course - [Atmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)!
## Bill of materials


## 3D printing

## Integrated development environment

You need to install an integrated development environment to start developing your programs ASAP. It's the easiest and fastest way (of course you can use a standalone AVR Toolchain[^1] and AVRDude [^2]... it's your choice but make sure you have plenty of time and tenacity).

Since it's a quick way to get you started I suggest you install both, yes both, environments:

* VSCode/VSCodium + PlatformIO plugin. This is how you can do it: [Installation tutorial](https://docs.platformio.org/en/latest/integration/ide/vscode.html#ide-vscode)
* Arduino IDE - [download page here](https://www.arduino.cc/en/software)

Why two IDEs? Well, Arduino IDE is so simple that it's a very nice place to start. Lots of pre-installed examples that you can simply run. All toolchains install automatically (AVR, ARM, etc.). It provides an easy to use terminal to see UART data (refer to [UART](chapters/3_uart.md)). However, anything bigger than one file tends to be more and more difficult to maintain. 

VSCode is an advanced text editor that can work quite nicely as an IDE if you configure it properly. *PlatformIO* provides support for most of development boards available on the market. It also handles external dependencies quite well, so you don't need to download 3rd party libraries on your own. It all comes with a greater complexity and is slightly more difficult to use. Just slightly.

Now, sometimes it's easier to compile software in Arduino IDE than in VSCode. You download a lib, compile, and run. VSCode can sometimes be a pain in this aspect. So Arduino IDE is also a good tool for any quick debugging you may need.


## TL;DR; Skip tutorial, give me a robot

If you are impatient and want the robot now without following the tutorial,
here are all the steps you need to create your robot.

* 3d-print all elements you find in [`3d_models`](/3d_models/README.md) directory. Use \*.stl files
  in your favorite slicer software
* Follow the pinout as shown in Fritzing diagram
* Install the software on your Cytron board. Use README file distributed alongside
  the source code



### References

[^1]: [GCC AVR Toolchain](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers)

[^2]: [AVRDude](https://github.com/avrdudes/avrdude)
