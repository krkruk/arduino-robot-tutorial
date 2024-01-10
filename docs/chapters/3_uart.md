# Universal Asynchronous Receiver-Transmitter
**Contents**
* toc
{:toc}
So far, you've been blinking LEDs in one way or another. It's time to let your board talk to your PC too!.

See *19. USART0* chapter in Atmega328p Datasheet for very detailed UART tech specification [^1].

## Introduction

Serial port, [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter) or 
[Universal Asynchronous Receiver-Transmitter](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter)
 is a way that allows you to connect easily your device with PC and exchange data.

Asynchronous means that receiver/transmitter does not have to run at the same pace. Clocks of both devices can run at different frequencies or be out of phase. UART uses start and stop bits to 
determine when the transmission is over. This gives a great advantage. The protocol can work with no 
difficult clock synchronization techniques and technologies. 
Therefore, UART can be used at longer distances, speaking more of meters (yards) rather than 
centimeters (inches).

There are two industry wide standards of UART: [RS-232](https://en.wikipedia.org/wiki/RS-232) 
and [RS-485](https://en.wikipedia.org/wiki/RS-485). We are going to use 2 other cases: TTL 
(Transistor-to-Transistor Logic) and connection via UART-USB bridge. In other words, you'll use Atmega integrated circuit, connect it to a 3rd party silicone chip (such as CH340, PL2303, FT232RL and many other) and plug a USB connector to your PC. You can even use another 
Atmega to host a USB (for instance Atmega16u4 with hardware USB support [^2])

Good news is your Arduino board (either Cytron's or the genuine one) comes with UART-USB converter. From coding perspective, you don't need to do anything.

A traditional UART tutorial presents and explains a data frame - a sequence of bits that form a single portion of a message... Let's get it over now (image source: [Wiki - by Cody Hyman](https://pl.wikipedia.org/wiki/Plik:Serial_frame.png)):

![UART data frame](https://upload.wikimedia.org/wikipedia/commons/b/b0/Serial_frame.png)

Isn't it nice? Well, let me explain it from a perspective of typical parameters one typically uses:

* [baudrate](https://en.wikipedia.org/wiki/Baud) (popular: 9600, 115200) - originally, baud rate is a number of symbols/pulses emitted in a unit of time. In context of UART communication, think `baudrate=bitrate=bits per second` [^3]. Good enough, we are not purists here. And one more thing, the bigger the number, the faster is data transmission.
* dataframe (popular choice: 8 bits, 7-9 possible) - simply how many data bits shall be placed between the start bit and parity/end bits
* parity (set to no-parity) - a checksum bit, you can use it or simply ignore it. I prefer to ignore it
* stop bits (popular choice: one; other options: one and a half, two bits)
* flow control (Arduino choice: No Flow Control) - UART supports two leads: RTS (Request to Send) and CTS (Clear to Send). Arduino uses TX and RX pins only, so there is no need to bother.

For a more detailed explanation, please refer to external sources such as this one: [UART - A hardware communication protocol](https://www.analog.com/en/analog-dialogue/articles/uart-a-hardware-communication-protocol.html) and other.

If you are going to connect your UART device to another device, make sure that:
* Baudrate must be the same for both a receiver and a transmitter
* Voltage levels are acceptable - 5V-5V in both devices. Your device uses 5V as it's V<sub>cc</sub> voltage. If you connect to a device that uses 3.3V, you'll likely fry it. Hence, a [logic level converters/level shifter](https://en.wikipedia.org/wiki/Level_shifter) should be introduced.
* Cross your comm lines: `TX` (transmit line) should be connected to `RX` (receive line)
* NEVER FORGET ABOUT THE GROUND!!! GROUND IT!!! Actually, `GND` line should be the first line you connect and the first line you check if something is wrong. Seriously, you'll save lots of time by following this tip!

`TX1` pin is your transmitter and `RX0` is the receiver in your board. Take a look at it. Of course, you can simply use a USB cable if you plan to use PC comm only.

### Serial Monitors

You can use your serial device with a 3rd party tool or your custom software. If you decide to simply
browse or even plot data you can use some of these tools.

Windows:
  * [GUI] Arduino IDE - embedded serial monitor
  * [GUI] PlatformIO - embedded serial monitor
  * [GUI] [PuTTY](https://www.putty.org/)
  * [CLI] [Tabby](https://tabby.sh/)

Linux:
  * [GUI] Arduino IDE - embedded serial monitor
  * [GUI] PlatformIO - embedded serial monitor
  * [CLI] [minicom](https://en.wikipedia.org/wiki/Minicom)
  * [CLI] [Tabby](https://tabby.sh/)

If you are interested in writing your own software, you need a library or [follow a Lord's blessed path](https://en.wikipedia.org/wiki/TempleOS) and write your own library. It's up to you. Personally, I worked with the following:

* C++: 
  * Qt Framework, [Qt Serial Port](https://doc.qt.io/qt-6/qtserialport-index.html)
  * Boost Asio, [Serial Port](https://beta.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/serial_port.html)
* python: [pyserial](https://pyserial.readthedocs.io/en/latest/pyserial.html)
* Java: [jSerialComm](https://fazecast.github.io/jSerialComm/)

## SerDes - Serialization and Deserialization

Data *serialization* is a process of converting data into a format that can be transmitted over 
wire/air and decoded at a destination point. The reverse process is called *deserialization*.

The keyword is **format**. You can choose two options: text or binary. The text option is usually human readable, supported by formats such as CSV, JSON, HTML, anything custom. The binary format 
converts data into a series of bytes, sometimes applying compression. You need to use the same 
serialization/deserialization technique in both receiver and transmitter to successfully deliver a 
message. Otherwise, this can be random bytes.

Which one is better? Well, it depends. The binary format usually comes with big frameworks that can 
convert a *schema* into your C or C++ code. All you need to do is to compile a schema file and link 
generated files to your program. IMO, the greatest disadvantage of the binary format is difficulty to 
handle data packets. Since all your data is binary, you don't really know when the message stops and 
another message begins. A byte of binary data is really an 8-bit number, which translates to 0-255(dec). Microcontroller is not aware of what the byte means. The binary formats of your choice usually 
provides libraries to capture a whole message and deserialize it, such as 
[protobuf](https://github.com/protocolbuffers/protobuf) or [msgpack](https://msgpack.org/), [ROS Messages](https://wiki.ros.org/Messages). Of course, you can implement something custom too - see the note below.

You can use text format too! It has several advantages: 1) it's human readable, 2) easy to find the last character, usually a new line character `\n`. Text format narrows down a range of values we send 
in a single byte from the binary data perspective. [ASCII](https://pl.wikipedia.org/wiki/ASCII) encodes human readable numbers as characters starting from a value 32 (in decimal, space ` `) up until 126 (dec, `~` character). Numbers between 0-31 and 127 
are special characters, such as *new line* character `\n` 10 (decimal). Thanks to this trick, a 
microcontroller can listen and cache raw bytes incoming from another UART device until it 
receives a byte of value 10 (dec, a new line character). You received a full line, a message that can be processed!

> [!NOTE]
> Long time ago, I was playing with robotics and European Rover Challenge. We didn't know anything
> about electronics and coding back then. We needed to control a robotic arm with 8 effectors. We 
> developed a binary format -> one byte to control all 8 devices: AAAV VVVV. `AAA` - 3 address bits, 
> `VVVVV` - a value between 0-31. This technique allowed us to control a 6-DOF arm without really any 
> worries about a data package start and stop positions. All we needed to do was to read just one byte 
> to rule it all :)!

In my experience, hobby projects work very nicely with JSON (text) encoding. Simply, read a line of 
text data and deserialize JSON into individual variables. I recommend ArduinoJson[^4].

ArduinoJson deserialization example (source: [JsonParserExample](https://github.com/bblanchon/ArduinoJson/blob/7.x/examples/JsonParserExample/JsonParserExample.ino))

```
// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2024, Benoit BLANCHON
// MIT License
//
// This example shows how to deserialize a JSON document with ArduinoJson.
//
// https://arduinojson.org/v7/example/parser/

#include <ArduinoJson.h>

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial)
    continue;

  // Allocate the JSON document
  JsonDocument doc;

  // JSON input string.
  const char* json =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch the values
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print the values
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}

void loop() {
  // not used in this example
}
```


## Arduino Framework

### Write data

### Read data

## UART, Atmega, Registers

### Write data

### Read data

## Other communication protocols

We are not limited to only one protocol. There are many others. I prepared a list of the most popular 
ones:

* [I2C](https://en.wikipedia.org/wiki/I%C2%B2C) also known as TWI (Two Wire Interface) in Atmega realm. You can read more in [I2C Arduino Tutorial](https://docs.arduino.cc/learn/communication/wire)
* [1-Wire](https://en.wikipedia.org/wiki/1-Wire). You can read more in [1-Wire Arduino tutorial](https://docs.arduino.cc/learn/communication/one-wire)
* [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface). You can read more in [SPI Arduino tutorial](https://docs.arduino.cc/learn/communication/spi)
* [CAN](https://en.wikipedia.org/wiki/CAN_bus). You can read more about CAN here: [CAN protocol](https://embedclogic.com/can-protocol/), [Introduction to the Controller Area Network (CAN)](https://www.ti.com/lit/an/sloa101b/sloa101b.pdf?ts=1704879860080), [CAN Arduino Due library](https://www.arduino.cc/reference/en/libraries/due_can/). CAN is a standard comm protocol in automotive. Certainly, it's worth investing some time to know it better.
* SpaceWire :) - see ESA's [docs](https://www.esa.int/Enabling_Support/Space_Engineering_Technology/Onboard_Computers_and_Data_Handling/SpaceWire>)


# References

[^1]: [Atmega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
[^2]: [Atmega16u4 Datasheet](https://ww1.microchip.com/downloads/en/devicedoc/atmel-7766-8-bit-avr-atmega16u4-32u4_datasheet.pdf)
[^3]: [What is the difference between baud rate and bit rate](https://stackoverflow.com/questions/20534417/what-is-the-difference-between-baud-rate-and-bit-rate)
[^4]: [ArduinoJson page](https://arduinojson.org/)