# Analog input, Pulse Width Modulation

**Contents**
* toc
{:toc}
This chapter shall introduce you to a read and write analog inputs/outputs. Digital electronics and 
analog values? Yes, exactly!

## Analog input

Atmega328p comes with a 10-bit Analog-to-Digital-Converter, *ADC* for short. Note the singular form of ADC. There is just one ADC, yet supported in multiple Atmega leads. This can be done by simply
multiplexing the signal. It means the circuit iterates over ADC-ready pins and reads values sequentially, on one pin after another. The diagram represents the circuit design (source [^1]):

![ADC Schematic - extracted from Atmega328p datasheet](./assets/images/chapter_2/1_adc_block_schematic.png)

You need one more bit to understand basics of ADC. So far we know it's a 10-bit ADC, which means 0-1023 values, and a multiplexer that we need to handle in one way or another. Good, but still we need a reference point, don't we?

You have several options:
* provide your own reference voltage
* use V<sub>cc</sub> as a reference (see `AVCC` in the diagram)
* use built-in 1.1V reference (you can change it with [`analogReference()`](https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/) function, or setting 
`ADMUX |= (1<<REFS1) | 1<<REFS0)`)

There is also a limit on how fast you can measure the voltage: 10kHz according to [`analogRead()`](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/) documentation. You can also use ADC as a comparator, which can become handy to implement one of the proposed robot features.

Alright, let's take a look at some code. You need to build a circuit like the one below. All you need is your board, some wires and a photoresistor.

![ADC Schematic - extracted from Atmega328p datasheet](./assets/images/chapter_2/2_photoresistor_circuit.png)


## Analog output - PWM

Let's face it, you cannot have an analog output without a DAC (Digital to Analog Converter)... You can get a really good approximation, though!

# References

[^1]: [Atmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)