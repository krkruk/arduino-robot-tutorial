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

> [!NOTE]
> If you take a closer look at the circuit, you notice it's a [voltage divider](https://en.wikipedia.org/wiki/Voltage_divider)! You can control ADC value at-rest by modifying the resistor value. Try it yourself with 1k, 10k and 100k resistors.

And you can use the following Arduino code (source - [Analog Read with Serial Monitor](./assets/code/chapter_2/2_analog_read_serial_monitor/2_analog_read_serial_monitor.ino)):
```
static constexpr const uint8_t ANALOG_PIN_PHOTORESISTOR = A0; 

static uint16_t photoresistorValue = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
}

void loop() {
  photoresistorValue = analogRead(ANALOG_PIN_PHOTORESISTOR);

  Serial.print("photoresistor ADC (0-1023) = ");
  Serial.println(photoresistorValue);

  delay(50);
}

```

Let's dismiss explanation for `Serial` prefixed calls. It's UART and shall be formally introduced in the next chapter. All you need to know about it is UART can transfer data onto your PC and display it in a serial monitor (in Arduino IDE: Tools -> Serial Monitor, make sure `baud` is set to **115200**):

![Serial monitor](./assets/images/chapter_2/2_serial_monitor_photoresistor.png)

Use your flashlight or a phone LED to shed a beam of light onto the photoresistor. You'll see how the 
values change. By default, if no light reaches the photoresistor, it's effectively a break in a 
circuit, which translate to high resistance (think megaohms). On the other hand, the brighter the 
area, the less resistance can be observed.

From the code perspective, Arduino offers a function called `analogRead(<<ANALOG PIN>>)`. It returns a value between 0-1023, that corresponds to 0-5V in real world. It's up to you, how you map it. You can, obviously, use a function `map()` to map things. Say you want convert ADC value into a PWM value 0-1023 -> 0-255, you can type `uint8_t result = map(0, 1023, 0, 255, <<adc value>>);`

Note the `delay` at the end of processing. It's linked with how multiplexer really works. It takes 
time to iterate over each analog pin. Still, `analogRead()` is pretty good if you simply want to get 
an analog value quickly.

How would Atmega code look like? Well, it's more complicated than one simply function call (source [Analog Read - Registers](./assets/code/chapter_2/2_2_analog_read_atmega_registers_adc0/2_2_analog_read_atmega_registers_adc0.ino)):

```
static uint16_t photoresistorValue = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  ADCSRA = (1 << ADEN)  // enable ADC
    | ((1 << ADPS2) 
    | (1 << ADPS1) 
    | (1 << ADPS0));     // prescaler 128
  ADMUX = (1 << REFS0); // MUX3..0 bits are set to 0, which is ADC0/PC0/A0 pin
}

void loop() {  
  ADCSRA |= (1 << ADSC);            // start ADC convertion
  while (!(ADCSRA & (1 << ADIF)));  // wait until ADC completes its job
  photoresistorValue = ADC;         // provide ADC value present on A0 pin

  Serial.print("photoresistor ADC (0-1023) = ");
  Serial.println(photoresistorValue);

  delay(50);
}
```

There are 3 registers you need play with (see *23.9 Register Description* [^1]):
* `ADCSRA` - ADC Control and Status register, enables/disables ADC, triggers a conversion and sets up basic parameters
* `ADMUX` - ADC Multiplexer Selection Registers - allows choosing a given channel in an ADC multiplexer
* `ADC` - combined result of `ADCH` (high register) and `ADCL` (low register) in 16bit variable - expect 10-bit value

So the first step is to enable ADC hardware and set up a prescaler to reduce MUX processing speed 
(16MHz, default clock in your board) by dividing it by 128, so the hardware can really measure a 
value. Then, you choose a channel from which you intend to read. Finally, you perform a blocking wait 
`while (!(ADCSRA & (1 << ADIF)))` until the channel is ready to provide an analog value measured on a pin.

Of course, if you remember your interrupt tutorial, you can use it here as well. You need to implement an `ISR(ADC_vect)` and start ADC in an interrupt mode by enabling `ADCSRA |= 1 << ADIE` bit. Of course, you cannot forget about enabling global interrupts `sei()` (source - [Analog Read with Interrupt](./assets/code/chapter_2/2_3_analog_read_atmega_interrupts/2_3_analog_read_atmega_interrupts.ino)):

```
volatile uint16_t photoresistorValue = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  ADCSRA = (1 << ADEN)  // enable ADC
    | ((1 << ADPS2) 
    | (1 << ADPS1) 
    | (1 << ADPS0))     // prescaler 128
    | (1 << ADIE);      // enable interrupts

  ADMUX = (1 << REFS0); // MUX3..0 bits are set to 0, which is ADC0/PC0/A0 pin
  ADCSRA |= 1 << ADSC;  // start ADC, handle a response in the ISR routine
  sei();
}

void loop() {  
  Serial.print("photoresistor ADC (0-1023) = ");
  Serial.println(photoresistorValue);

  delay(50);
}

ISR(ADC_vect) {
  photoresistorValue = ADC;
  ADCSRA |= 1 << ADSC;      // run ADC/ch:adc0 immediately again
}
```

I suggest you to review documentation in details if you need to provide accurate measurements. For the sake of this tutorial, a coarse value is enough. The documentation introduces to things such as ADC Noise canceller and available techniques, ADC accuracy and measurement errors etc.

## Analog output - PWM

Let's face it, you cannot have an analog output without a DAC (Digital to Analog Converter)... You can get a really good approximation, though!

# References

[^1]: [Atmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)