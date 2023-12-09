# Input/Output handling

Hopefully, you own and Arduino Nano-compatible board, just like the Cytron's one mentioned in the *Bill of materials* page:

[<img alt="Cytron Maker Nano pinout" src="https://www.cytron.io/image/catalog/products/maker-nano/maker-nano-pinout.png" />](https://www.cytron.io/p-maker-nano-simplifying-arduino-for-projects)

> [!IMPORTANT] I strongly support original Arduino to support the the idea behind it! Arduino enabled thousands of people to develop hardware reducing the entry barrier. I strongly encourage to buy yourself one!

The project can be completed using a standard Arduino Nano board if you exclude a buzzer. The Cytron's board adds new features such as LEDs in all GPIO, a programmable button, and a buzzer. It's a nice touch if you want to reduce wiring in your final project.

> [!NOTE] The board uses CH340C as a UART-USB converter. It's may not be compatible with your operating system ot of the box. In *Ubuntu 22.04.3 LTS*, you'll need to configure it by uninstalling a package from your system (it disables braille display): `sudo apt remove brltty` [^1]. MS Windows may require similar steps.

The Cytron's board uses Atmega328P as it's main microcontroller in the TQFP package. Luckily, you don't need to follow PCB paths correspond to which PIN. The pinout diagram above does it perfectly! Still, if it happens one day you need create your own PCB, it's worth to know physical dimensions and properties of your microcontroller. You also want make yourself familiar with the original datasheet: [Atmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf). The datasheet provides all necessary information you need to start coding your IC (integrated circuit) and tech specs that you should follow

## PORTs, PINs and power output

What a port, then? Well, a port in a realm of microcontroller is a module that connects ALU (Arithmetic Logic Unit) with a world by receiving/generating electric signals on a set of pins.
A pin is a physical connector that one can solder onto a PCB. Pins are the true interface that the IC uses to interact with the world

Atmega328p has 3 ports (PORTB, PORTC, PORTD) with built-in pull-up resistors. The 
microcontroller can therefore support up to 23 input/output pins 
(PORTC exposes 7 pins, the rest supports 8 pins). To visualize the overall Atemga architecture,
please refer to the documentation (chapter: ). The block diagram (extracted from the datasheet *Chapter 2.1 Block diagram*):

![Source: Atmega 328p datasheet, Chapter 2.1 Block diagram](./assets/images/chapter_1/1_io_atmega_block_diagram.png)

That's a theory. You are not soldering your own PCB, you bought an off-the-shelf one. It means, 
you need to map pins defined in Atmega datasheet to your board outputs. Your Cytron's Nano is 
compatible with the original Arduino Nano, therefore you can re-use all resources known in 
Arduino to your advantage. Compare Cytron's pinout (the very first image in this page) with the 
table and the Arduino pinout below.

| PORT | Ardunio Nano | Cytron Maker Nano | Atmega328P |
|------|--------------|-------------------|------------|
| DDRB | D8           | D8                | PB0        |
| DDRB | D9           | D9                | PB1        |
| DDRB | D10          | D10               | PB2        |
| DDRB | D11          | D11               | PB3        |
| DDRB | D12          | D12               | PB4        |
| DDRB | D13          | D13               | PB5        |

![<img alt="Arduino Nano Pinout" src="https://www.electronicshub.org/arduino-nano-pinout/" />](https://www.electronicshub.org/arduino-nano-pinout/)

`D\d` (for instance: `D8`, `D13`) is an alias for a given pin Arduino ecosystem. You don't need to
memorize anything. Your board has all the markings printed on the PCB solder mask itself. You will use the very same aliases such as `D8`, `D13` in your code if you decide to use Arduino framework. The Atmega328P names correspond to aliases found in Atmega toolchain and are simply a shorthand for underlying pin addresses (i.e., `0x00`). You definitely want to use these aliases for readability.

If you want to know more about Nano board, you can refer to this page [^2].

## Digital output

> [!IMPORTANT] Atmega328p pins are limited to 20mA (milli amps) per pin and up to 100/150mA for 
a port. You should not connect anything more amp-consuming to a port directly. Use a transistor 
to amplify a digital signal to an output that can handle bigger loads. You can read more on 
amplifiers in [Wiki](https://en.wikipedia.org/wiki/Category:Single-stage_transistor_amplifiers). Please, see also *28. Electrical Characteristics* in your Atmega datasheet [^3].

One more thing, Arduino Nano works in 5V logic. It means the high state (also referred as *1*) is 
5V and the low state is 0V (you guessed it! It's referred as *0*). That's the ideal condition, of course. In the real world, *0* state can be considered as 1/3 VDD or lower of your voltage supply. Similarly, the high state is 2/3 VDD or higher [^4]. You can also refer to *28.2 DC Characteristics * in datasheet for more details on input/out (high|low) voltages [^3].

Lots of theory, and no code so far. It's time to change it. Let's take a closer look to the blinking example we had in the previous chapter:

```
// Source: https://github.com/arduino/arduino-examples/blob/main/examples/01.Basics/Blink/Blink.ino

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
```

The `setup` function initializes hardware. It includes setting up a desired state on a pin, interrupts, serial devices. Effectively, anything you need to run your main application code in a loop.

The `loop` function is effectively anything you want to run indefinitely as your firmware. This involves all ALU operations you want to run with help of your peripherals such as I/O pins, UART etc.

In order to blink an LED you always need to configure your port by telling a direction of a given pin, whether it's an input or output. Possible options: `OUTPUT`, `INPUT`, `INPUT_PULLUP`.

Once this is done, you can then either write or read a pin state, depending on your direction configuration. To do so, you can use `digitalWrite(<<pin alias>>, <<state>>)` function. *Pin alias* can be a `uint8_t` value or a label such as `LED_BUILTIN`. *State* can be either `LOW` or `HIGH`.

Let's connect your own LED... You need an LED and a resistor. You should never connect an LED to a DC output as you will likely fry it. 


## Digital input and pull-up resistors

## Interrupts

## Analog input

## Analog output - PWM

Digital electronics, as your Atmega328p, is all about 0 and ones. There are no values in between,
ideally. Thus, it's a rather hard task to talk about analog values in between 0-5V. Luckily, we are not exactly bound to 2 states only. We can change a pin state fast enough to reduce output power in a given timespan - this is called PWM, Pulse
Width Modulation. It's a rather important aspect of embedded programming, therefore
please read the next chapter that handles PWM in details.

There are also Arduino boards that support DAC - Digital-to-Analog converters, such
as [Arduino Due](https://store.arduino.cc/products/arduino-due). Unfortunately,
Atmega 328p, as the one in your board, does not support DAC. For more details,
take a look at Wiki: [Digital-to-analog converter](https://en.wikipedia.org/wiki/Digital-to-analog_converter).


[^1]: [CH340 Configuration in Ubuntu 22.04](https://askubuntu.com/questions/1403705/dev-ttyusb0-not-present-in-ubuntu-22-04)
[^2]: [Electronics Hub - Arduino Nano Pinout](https://www.electronicshub.org/arduino-nano-pinout/)
[^3]: [Atmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
[^4]: [Wiki - Logic Levels](https://en.wikipedia.org/wiki/Logic_level#Logic_voltage_levels)