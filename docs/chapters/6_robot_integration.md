# Robot integration
**Contents**
* toc
{:toc}
You've learned so much. Let's put that into practice and finally build your own robot!


## Introduction

Alright! You want to build a robot. This is the right place. I can simply provide you some steps
to achieve it. Still, this is not a purpose of this exercise. Let's think a bigger picture - engineering.

Normally, one creates an item, a tangible object to fulfil a need. It can be a minor but still its
goal is to satisfy the need. Well? Why do we need to build a robot? It's fun, for sure. You also
want to familiarize yourself with basics of microcontrollers and coding. You did it in previous 
chapters. You are quite proficient at it! This chapter really integrates the knowledge. IMO, Building
a robot is the coolest way to do it. What sort of goals can we achieve with the robot? Are there any
alternatives? What's the context, etc.?

An engineer must ask a lot of questions to really understand the problem. After all, we try to solve
human problems rather than build cool stuff. (I know, here we intend to build cool stuff. 
I generalize)

So what's our main mission? Say, you can bid a DARPA contract to win a robotic office battle. The
bid comes with a set of requirements you are expected to meet. Also the budget is limited, as DARPA
is willing to pay no more than €30 for a single robot. Specification can look like this:
* **DARPA/GENERAL/OFFICE-BATTLE/REQ/0010**: A device is a mobile platform, dimensions no greater than 20x20x10cm (length x width x height)
* **DARPA/GENERAL/OFFICE-BATTLE/REQ/0020**: The device shall be controlled over wire
* **DARPA/GENERAL/OFFICE-BATTLE/REQ/0030**: The device shall use commonly available off-the-shelf price
* **DARPA/GENERAL/OFFICE-BATTLE/REQ/0040**: The device shall compete non-destructively with other competitors to score points
* **DARPA/GENERAL/OFFICE-BATTLE/REQ/0050**: The device should not be equipped in any agent-dispersing equipment
    * (Consider this requirement as no flamethrowers, no chemical weapons, no shooting)
* and so on

The specification can of course be presented in different forms, formatting and so on (i.e., a few 
blocks of text...). Eventually, it's up to you to identify all possible requirements and assumptions 
to meet your client's needs. You can learn a bit more about it by reviewing some university competitions such as [Rules - European Rover Challenge](https://drive.google.com/file/d/1jZyn9oY7vqMJMvXQyE6EeLh7gRC0LpJh/view?usp=drive_link) or [Rules - University Rover Challenge](https://urc.marssociety.org/home/requirements-guidelines)

Note the language. Verbs such as is/shall/should/may/can/etc., one precise item mentioned in each bullet. The language of such requirements in the specification tries to be very detailed. In reality, even the
most detailed description tends to be barely a draft once you attempt to implement it. There are
simply too many factors to predict. Still, I wish you always get all the specs with no ambiguity.
Note the enumeration too. It's not always the case but usually it helps to navigate in documents
if you provide a decent convention. Imagine you need to talk to your client and discuss 
an unclear requirement. You can say that: on the 3rd page, 8th paragraph, second sentence the
requirement states that... and so on. Lots of words, too many actually. The enumeration helps
tracking this sort of problems in formal procurements as well as in future discussions.

The bid comes with a specification that defines a series of requirements. Now you need to model your further activities. What sort of actions the device, or from now on a robot, shall take? What
are the *rules of engagement*? Should the robot fight automatically? How to strike an opponent
within the given rules? How you can flex those rules, so it gives you market advantage :)?
There is lots of thinking involved... What are your skills (although it's not as important as it
can look like)? What's the budget? And so on...

The modelled design shall be presented in a form of a proposal. Yet again, there are many ways
to skin a cat... If it's a public institution, you might be given some clues, like [here (ESA)](https://www.esa.int/About_Us/Business_with_ESA/How_to_do/Preparing_a_technical_proposal)
or [here (NASA)](https://www.nasa.gov/wp-content/uploads/2022/02/2021_ed._nasa_guidebook_for_proposers.pdf).

Let me model that for you:
* **RB-GENERAL-REQ/0010**: The robot shall be a two wheel construction
* **RB-GENERAL-REQ/0020**: The robot shall be [controlled differentially](https://en.wikipedia.org/wiki/Differential_wheeled_robot)
* **RB-GENERAL-REQ/0030**: The robot shall be controlled at max distance of 1.5m from its control station (PC)
* etc.
* **RB-COMBAT-REQ/0010**: The robot shall engage at max distance of 12cm
* **RB-COMBAT-REQ/0020**: The robot shall shall strike an opponent by hitting it from top
* **RB-COMBAT-REQ/0030**: The robot shall shall perform at least 1 strike per second
* **RB-COMBAT-REQ/0040**: The robot may count received hits and announce opponent's score by
emitting a sound signal

These requirements shall convert into the following assumptions:
* **RB-GENERAL-IMPL/0010/REF:RB-GENERAL-REQ/0010**: The robot shall utilize 28BYJ-48 stepper motors to drive
* **RB-GENERAL-IMPL/0020/REF:RB-GENERAL-REQ/0010**: The robot shall be equipped with wheel 7-7.5cm in diameter, 3D-printed in PLA, rubberized tread
* **RB-GENERAL-IMPL/0030/REF:RB-GENERAL-REQ/0020**: The drive model shall be implemented within microcontroller
* **RB-GENERAL-IMPL/0031/REF:RB-GENERAL-REQ/0020**: The drive model shall comprise 4 commands: FORWARD, BACKWARD, TURN_LEFT, TURN_RIGHT. Turning shall be performed in-place. Power setting of min.0-max.255 shall be issued along with the drive command.
* **RB-GENERAL-IMPL/0040/REF:RB-GENERAL-REQ/0030**: The robot shall use microUSB cable and be supplied
with power output provided by USB3.0 (max. 900mA).
    * Comment: Energy budget is important! Each stepper consumes roughly 100-200mA @5V, servo in between 10mA-650mA[^1]. Arduino consumes 50-100mA. It means, the budget of 2x150mA + 300mA + 100mA ~= 700mA is under the total power output provided by USB3.0. We should be good here

* **RB-COMBAT-IMPL/0010/REF:RB-COMBAT-REQ/0010-0020**: The robot shall be equipped with a 12cm PLA-printed arm, driven by a servo mechanism
* **RB-COMBAT-IMPL/0020/REF:RB-COMBAT-REQ/0020-0030**: The combat arm shall be driven by SG-92R servo
* **RB-COMBAT-IMPL/0020/REF:RB-COMBAT-REQ/0040**: The robot should be equipped with a photoresistor
to count every opponent's successful strike

Note how precise this list is! IT IS INCOMPLETE! I didn't specified any programming languages nor
desired microcontrollers. It takes lots of work to complete the documentation. It takes even more
details to recreate a solution described in such a specification... I haven't even provided any 
drawings yet! Testing, maybe? Operational scenarios (a set of typical procedures to follow) and 
alternative scenarios ([FUBAR](https://en.wikipedia.org/wiki/List_of_military_slang_terms#FUBAR) cases).

> INSERT a CAD shot

Anyway, you should have now a general feeling over how the robot is going to look like: 2-wheels 
controlled with stepper motors and an arm that hits opponent robots from top.

This section was meant to introduce you to an engineering process that you may encounter in your
future work. Quite often, the documentation is the key to success. You can read more on project
management on [Wiki - Project Management](https://en.wikipedia.org/wiki/Project_management).

## Pinout

>> Here comes a pinout table and a Fritzing drawing

## Assembly

Make sure you printed all elements listed in Bill Of Materials!

Full assembly instructions:

![Rear - wall assembly](./assets/images/chapter_6/01_rear.jpg)
<br />Figure: Rear axle: Wall assembly

1. Assemble  `rear_wall.stl` with `chassis_rear_beam_with_airsoft_bearing.stl`. Mind the notches.
The support beams in `rear_wall.stl` should point outwards. See the next figures for more details

![Rear - bearing assembly](./assets/images/chapter_6/02_rear_airsoft_ball.jpg)
<br />Figure: Rear axle: Bearing assembly

2. Insert a 6mm polymer ball into the notch in `chassis_rear_beam_with_airsoft_bearing.stl` at the
bottom

![Insert rear axle](./assets/images/chapter_6/03_rear_breadboard.jpg)
<br />Figure: Install rear axle

3. Insert the breadboard into the axle as shown in the picture. This should fit tight. You can use 
a zip tie to tight the side walls: mind the openings in the sides: you can use it to insert the zip
tie


![Motor assembly](./assets/images/chapter_6/04_motor.jpg)
<br />Figure: Motor assembly

4. Make sure you hold the stepper so the shaft is at the top. Insert the left piece 
into `stepper_motor_case.stl`. The print is elastic, you can bend it to accommodate for the necessary room. With a counterclockwise movement, insert the right piece of the stepper into the slot. Use some force as its very tight by design so no screw/bolts are needed. Perform this step twice

![Wheel and stepper](./assets/images/chapter_6/05_motor_and_wheel.jpg)
<br />Figure: Install wheel

5. Glue a piece of rubber on top of the wheel. A typical window gasket/seal is good enough
6. Insert the stepper shaft into the slot. It is up to you to pick the wheel facing side


![Front axle](./assets/images/chapter_6/06_front_axle.jpg)
<br /> Figure: Front axle assembly

7. Push each wheel onto `chassis_front_beam.stl`
8. Thread the stepper leads through the bottom opening
9. Install `front_wall.stl` with the support beams facing front. Mind the cables. You should
achieve the overall result as shown in the Figure

![Install Servo](./assets/images/chapter_6/07_servo_into_driver_tower.jpg)
<br />Figure: Install Servo

10. Insert the servo into `servo_holder.stl`. There are two opening for the cabling. Make
sure you lay the cables through the openings
11. Insert the combined element into the middle drawer of `driver_tower_with_servo.stl`
12. Assemble the arm by gently installing `servo_arm_effector.stl` onto `servo_arm.stl`
13. Insert an asymmetric handle/needle/rudder into the assembled arm (no photos). The handle
comes with the servo in the same package
14. Push the assembled arm onto servo shaft. Do it gently as you need to calibrate the servo later.

![Front axle and driver tower](./assets/images/chapter_6/08_driver_tower_and_front_axle.jpg)
<br />Figure: Install front axle and the driver tower

15. Install the driver tower, so the small openings face forward
16. Install the front axle
17. Thread the stepper leads through the top and bottom notches. You should achieve the result 
as shown in the picture

![Wiring](./assets/images/chapter_6/09_wiring.jpg)
<br />Figure: Wiring

18. Connect all leads as suggested in the pinout table
19. Lay the cables in a way they have very low chances to interfere with the arm. Use zip ties,
thread, tape to order the cabling

20. We're done here. Move to the coding part :)!

## Coding

I chose more structural approach to write firmware for the robot. It's simply easier to
follow for unexperienced developers. If you familiar with C++ and OOP, you can develop
a hierarchy of classes if you want. Just allocate such objects on stack rather than heap.

### JSON contract

See **RB-GENERAL-IMPL/0031/REF:RB-GENERAL-REQ/0020**

### Servo integration and calibration

See the [Controlling Servos](chapters/5_servo.md) chapter to calibrate the servo.

As the **RB-COMBAT-REQ/0020** assumption suggests, the arm should hit from top to bottom. This can be 
interpreted as keeping the arm in a vertical position. The hit would be *fast* move to a 
horizontal position. We can even overshoot a bit for the best *shock and awe* effect :D!.

Once you calibrate the servo you should now the 0deg and 180deg positions. You can now adjust
the arm location at either of these positions and consider it as your default state. So say, the arm is vertical (or slightly tilted towards the hitting direction). Then, by applying 0deg or 180 deg
position, the servo performs a hit. It's a question of how you inserted the servo into
the driver tower box. For the configuration as presented in the assembly pictures, I can provide
these control code:

```
some code
```

See the `loop()` function. It's a temporary set of commands to simply test the servo. You want
to integrate one thing at a time so you minimize opportunities for errors.


### Stepper integration


### JSON Contract integration

### Serial Monitor tests




# References

[^1]: [Motor Micro SG92R](https://protosupplies.com/product/servo-motor-micro-sg92r/)