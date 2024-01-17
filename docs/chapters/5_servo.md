# Servo control
**Contents**
* toc
{:toc}
This chapter is all about servomechanisms, servos for short. You will learn: 
* What is a servo? How does it work?
* How to connect a servo?
* How to control a servo?
* Different types of servos

## Theory

A servomechanism (servo) is a feedback system that uses a closed-loop controller to set
and actively adjust its position and/or velocity as an actuator[^1], [^2]. There are many types of
servos i.e., hydraulic, pneumatic, electric [^1]. In this tutorial, we shall use a typical
positional rotation DC motor servos, often used in RC control models.

Let's quickly decipher the definition to fit our context:
* servo does rotate
* servo does keep its position
* servo can be controller with a microcontroller

Internally, a servo is an effector (here: a DC motor) wrapped with electronics that read and control its position. To read a position you need some sort of [*encoder*](https://en.wikipedia.org/wiki/Encoder_(position)). You also need a control mechanism/circuitry to read the position and correct it.
Often, it's a microcontroller with custom software, usually a [PID controller](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller). Finally, the servo needs to
actuate in the real world. 5V/20mA is barely enough to light up an LED. The DC servo comes with
an [H-bridge](https://en.wikipedia.org/wiki/H-bridge) to control position and power in the DC motor. The servo also may need gears (gearbox) to increase output torque. All of it comes in a small and usually affordable package!

Hopefully, you bought SG92(R) servo. It's a small servo that offers a decent torque in a small package. There are three wire leads: brown, red, orange. Colors may vary in tinge. The middle one, the
red one is a positive power supply. You'll connect 5V there. The brown one is GND. The orange one
is PWM signal, which controls the position of the servo. Make sure you connect it to PB1 pin.
The pinout of the servo is pretty smart. 5V lead in the middle effectively prevents you from applying
wrong polarity. If you connect the PWM lead to GND by mistake, nothing will happen! Of course, try
always to connect your hardware correctly.

| Servo  | Arduino Nano | Atmega Pinout |
|--------|--------------|---------------|
| Orange | D9           | PB1           |
| Red    | 5V           | N/A           |
| Brown  | GND          | N/A           |


There are many tutorials in the Web. You can google them or simply use one of the below links:
* [Arduino - Basic servo control](https://docs.arduino.cc/tutorials/generic/basic-servo-control/)
* [Adafruit - Servo tutorial](https://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors/overview)

Now, how to control it?
* 50Hz PWM signal
* 5%-10% duty cycle, to control its position between its maximum ranges 
(often 0 to 180deg)[^4]. There are lots of variations on it[^5]. From my experience, this is a good
approximation but still it's better to calibrate the values on your own. 
* Power supply and GND are also welcomed :)

## Servo library

Arduino IDE comes with *Servo* library included[^3]. The library uses 16-bit timer to provide
precise timing to control servos.

## Hardware-based servo control


# References
[^1]: [Wiki - Servomechanism](https://en.wikipedia.org/wiki/Servomechanism)
[^2]: [ScienceDirect - Servomechanism](https://www.sciencedirect.com/topics/engineering/servomechanism)
[^3]: [Arduino - Servo library](https://www.arduino.cc/reference/en/libraries/servo/)
[^4]: [SparkFun - PWM](https://learn.sparkfun.com/tutorials/pulse-width-modulation/all)
[^5]: [Wiki - Servo Control](https://en.wikipedia.org/wiki/Servo_control)