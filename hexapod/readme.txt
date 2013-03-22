
hexapod documentation (crude)
-----------------------------

The raspberry-pi will DHCP for an address when powered up;  it'll give up after
a short while, so make sure a server is there for it!  Hardware address is:

    b8:27:eb:fe:6b:72

Standard raspbian login, username "pi" password "raspberry".  If you want root
(which you'll need to access the serial port), just "sudo /bin/bash -".

In here is "hexapod-v2.c", the code that runs on the Arduino.  To re-flash the
device, you'd need to plug something into the USB port on the Arduino (accessible
near the front-left leg).  But before doing that, disconnect the serial link
between the Arduino and the Raspberry Pi's interface board.


hardware notes (crude)
----------------------

Beware power and I/O connections: there is little to no protection between
the 5v used by the Arduino and the 3.3v used by the Raspberry Pi, save for
a diode to prevent the Arduino dumping 5v into the Pi (which will DESTROY its
interface circuitry).  The serial connection has "Rx/Tx" on the flying lead,
and "Tx-pi/Rx-pi" on the interface boards.  These should be connected OPPOSITE,
such that the lead's "Tx" goes to the Pi's "Rx-pi" (writing on the flying lead
connector facing into the board, pin-clip-holes facing out).

The PSU (cheapy Maplin one) will deliver about 3A it recons;  I'm sure the
board may try and draw more than this in extremis, so try not to let it strain
too much!  The regulator that powers the Raspberry Pi has a heatsink attached
to it -- this may get warm/hot!

The red switch delivers juice to the control boards (Arduino, SSC-32, Raspberry
Pi) and, I'd expect, up to 1-1.5A.  The yellow switch delivers juice to most of
the servos, and can probably draw 3-4A if the robot was restrained in some way.
I've seen an individual servo draw close to 800mA when trying to move to a
position that it physically can't reach.


software notes (crude)
----------------------

Once the robot is powered up, and you can login to it, there are some tools
in ~pi/serial/, e.g.:

-------------------------------------------------------------------------------
pi@raspberrypi:~/serial$ sudo ./dumpserial -b 38400 /dev/ttyAMA0 
03 bytes: 4d310d
03 bytes: 4d310d
03 bytes: 4d300d
03 bytes: 4d310d
03 bytes: 4d300d
32 bytes: 5220303330302030333030203033303020303230302030323030203033303020
20 bytes: 303230302030313030203032303020303130300d
-------------------------------------------------------------------------------

^^^ the above just dumps the output from the serial port, which looks framed
right, but the analog data its reporting looks wrong to me (need to debug this
some more).

To send commands to the device (prior to you folks implementing something
proper!) just:

-------------------------------------------------------------------------------
root@raspberrypi:/home/pi/serial# ./sendserial -b 38400 /dev/ttyAMA0 'M1\x0d'
root@raspberrypi:/home/pi/serial# ./sendserial -b 38400 /dev/ttyAMA0 'M0\x0d'
root@raspberrypi:/home/pi/serial# ./sendserial -b 38400 /dev/ttyAMA0 'R\x0d'
root@raspberrypi:/home/pi/serial# ./sendserial -b 38400 /dev/ttyAMA0 'S#1 P1500 #5 P1500 #9 P1500 #17 P1500 #21 P1500 #25 P1500\x0d'

-------------------------------------------------------------------------------

The command structure can be found in the "hexapod-v2.c" source file near the
top.  I've not tested all of this by any means, but forwarding commands through
to the SSC-32 board (as the last line here does) seems to be sane :).


SSC-32 docs
-----------

The SSC-32 servo controller board is manufactured by lynxmotion, documentation
for that (and other hexapod stuff) can be found here:

    http://www.lynxmotion.com/images/html/build136.htm




