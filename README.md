# Pong
Custom Implemenation (both hardware &amp; software) of the Classic "Pong" game.

This was my the first programming project I did which actually took some time-investment to figure out.

I like to call the approach I took here "Logical Brute-Forcing" because I basically wrote the code in such a way
as to try and cover all logical possibilities.  This was done in August of 2017, and is clearly before I learned any
proper algorithmic-coding.

The program is far longer than it should be, but the gameplay is extremely smooth, I've never actually played Pong before, 
and being that I used absolutely zero resources to figure out how to write the game (except for using the graphics library), 
I'm fairly proud of the results.

Looking back at code one writes over a lifetime is educational and fun to think about how one might write a better version,
using ideas and techniques unbeknownst to one's self at original coding time.

The setup for this game will require some hardware, and here's a list:
  (1) An Arduino Uno - if not using an Uno, some of the pin variables will likely need to be changed,
                       but it should be fairly trivial to use a different board.
  (2) Two Potentiometers - they function as the controllers for the paddles
  (3) One OLED Screen - any screen should suffice, but major modifications to the source code will need to be made.
                        As mentioned before, I basically coded all logical consequences manually, so the dimensions of the
                        screen are pivotal to the game working properly.  The screen dimensions are 128x64 (width by heighth).
  (4) A breadboard to connect all the wiring
  (5) 20 male-to-male wires
  (6) Two piezo speakers - these are optional, as the sound output is only done when a player wins, and the choice of whether or
                           not to include sound in the game is left up to the developer.
                           
 ![alt text](https://raw.githubusercontent.com/Connor-Wilhoit/Pong/master/temp3.jpg)
