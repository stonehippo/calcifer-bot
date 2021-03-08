# calcifer-bot Designer's Notes

## What is calcifer-bot?

One of the ways we heat our home is with a really nifty Italian-made pellet stove. We love this
stove: it's heats the house so nicely, and gives us little flames that create a lovely ambiance
on cold nights. After we installed in it our homes a few years ago, and fired it up for the first
time, it reminded of something. And so we nicknamed our pellet stove Calcifer.

Now Calcifer is amazing and all, but like it's namesake, it's not perfect. No big flaws, of course,
but a couple of little things that we'd love to fix.

And that's where calcifer-bot comes in.

I wanted to be able to bring Calcifer to life on command, to speak a magic word, and fire on!
Of course, we already have a sort of demon for just this purpose: Siri, courtesy of the several
Apple devices around the place, including a HomePod.

This got me thinking that perhaps I could meld the two, bringing Siri and Calcifer together to
fulfill my dreams of speaking a spell to ignite (or douse) the flames in my little steel friend.

### Some design constraints

There are a few constraints that I wanted to place on the project.

- No hacking on Calcifer itself; it's too precious to break!
- Must not interfere with normal, manual operation
- Must work with Apple HomeKit

And I wanted to be build calcifer-bot with tools and tech that I already had on hand, if possible. 

### The solution

The solution to my problem is to create a static robot, able to carry out the task of turning on and off
the pellet stove at command, and controllable via HomeKit.

calcifer-bot is a simple robotic device. At it's heart is a standard RC hobby micro servo. This is
the actuator that does the hard work of pressing the button that turns Calcifer on and off.
The servo is driven by an ESP8266, a nice, cheap little microcontroller that has built-in Wifi.
This allows me to run a HomeKit Accessory server right on the device. I could have chosen to do something
lighter, such a run MQTT on the ESP8266, and connect that to something more powerful, like a
Raspberry Pi run an MQTT broker and HAP-js (probably orchestrated by Node-RED). But for this project,
I wanted the direct approach.

## Some stumbling blocks

One issue I ran into is that the standard Arduino Servo library doesn't work well with the ARM Cortex-MO+/M4
that is the heart of the SAMD21/SAM51-based boards. This wasn't a problem for my final implementation, but
it did cause a bit of headache when I was testing this concept during the design phase.

When I was first thinking about how to build calcifer-bot, I spent some time on what kind of mechanism
would be best to push the button on the pellet stove. The button seems to be a momentary switch, but
you have to press and hold it for a few seconds to start or stop the stove. I needed to know if
calcifer-bot would be able to apply enough force to press and hold that button.

My initial thought for how to achieve this was with a solenoid, since it's push/pull motion is
very much like pushing down with a finger. However, I wanted to complete calcifer-bot with parts
I already have around, and sadly, no solenoids available at the moment. Looking in my big box
of motors, I have various DC motors, steppers of various sizes and strengths, and a few hobby servos.
After thinking about what it would take to hook up the dc motors or steppers, I decided that a servo
would be the simplest first choice, since I can control the position or speed of very simply in code.

But which servo to use?

I rejected the various continuous rotation servos, since it seems like calcifer-bot's "finger" only needs
to be pressing, or it the released position, and that's easy enough to do by passing in a desired angle.
Now the question was micro servo or something beefier.

To test things out, I grabbed a SparkFun Qwiic Twist (a really nifty little I2C controlled rotary encoder)
and an Adafruit OLED featherwing, and hooked them up to a SparkFun SAMD51 Thing Plus. I have a lot of
dev boards on hand, but that on happens to have a built-in Qwiic/STEMMA QT port on it, making hooking
up I2C peripherals a breeze. With these parts, I could hook up the servo, use the encoder to set angle,
and see what the angle was on the OLED. All in all, a nice way to try out pressing the button.

And that's where I stumbled. While putting together test code based on this hardware, I found that the
standard Arduino Servo library doesn't work with the ARM-based cores. Turns out ladyada submitted a PR
to the library to address this quite a while ago, but for some reason, it's never been merged in. 

Anyhow, to get around it, a made a local library based on the PR, and voila, I had a test unit that worked!

To wrap it up, it turns out the smaller servo could apply plenty of force to press the button when braced
properly. With a successful test of the first prototype, I was able to move on, swapping out the SAMD51
board for an ESP8266