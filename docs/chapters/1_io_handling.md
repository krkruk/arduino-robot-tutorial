# Input/Output handling

## Digital output

## Digital input

## Pullup

## Interrupts

## Analog input

## Analog output - PWM

Well, this is digital electronics so it's hard to talk about *real* analog
output. Luckily, we are not exactly bound to full 1 or 0 state. We can change
it fast enough to limit the overal power output - this is called PWM, Pulse
Width Modulation. It's a rather important aspect of embedded programming, therefore
please read the next chapter that handles PWM in details.

There are also Arduino boards that support DAC - Digital-to-Analog converters, such
as [Arduino Due](https://store.arduino.cc/products/arduino-due). Unfortunately,
Atmega 328p, as the one in your board, does not support DAC. For more details,
take a look at Wiki: [Digital-to-analog converter](https://en.wikipedia.org/wiki/Digital-to-analog_converter).
