# Raspberry Pi Datalogging Software

This is a piece of software that will interact with the Arduino
ADC/DAC and logg the values in a CSV file. It is based loosley off of 
the program Dr. John Stout did durring his PhD research in the 
department of electrical and computer engineering at Brigham
Young University.

The commands that the raspberry pi can send are:

1. Request data from the Arduino.
2. Send an update to the Arduino to change the relays

The Relay command is followed by 8 1's and 0's indicating
which relays you want on or off from 0 to 7.

It is a project run by Dr. Ryan Steward in the Plants and Wildlife Sciences Department
at Brigham Young University. The main contributer of this code is Samuel Lahti
