# TinkerTie
Code designed to run on Atom Computer's Tinker Tie. http://atomcomputer.us/tinkertie/

My Tinker Tie is modified to have an extra digital button connected to pins 10 (ground), 12 (read), 13 (3.3V)
Pins 12, 13 are used instead of power and ground pins since pins 10, 13 are closer to the location of the extra button.
The extra button is used to change the colour profile without it the colour profile must be set at compile-time.

To use this code with an unmodified Tinker Tie do the following in TinkerTie.ino:

1. remove the pinMode/digitalWrites for the extra button in setup()
2. remove the call to colourProfileButton in loop()
3. (optional) remove the #define for buttonPin, buttonPowerPin, groundPin and the definition of colourProfileButton()
