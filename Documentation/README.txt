
Hexapod / MSc-Project Documentation
-----------------------------------

So..  Robot should start up on its own happily.

For visualisation, need SSH tunnel into the Raspberry Pi, e.g.:

    ssh -N -L 33334:localhost:33334 pi@192.168.16.252
                                       ^^^^^^^^^^^^^^  (IP of the robot)

password is: raspberry


For the joystick control: (make sure it's plugged in) then:

    cd RemoteControl
    ./control 192.168.16.252
              ^^^^^^^^^^^^^^ (IP of the robot)




