# Arduino ADC/DAC

This is the code for the Arduino ADC/DAC. It runs a simple state machine
that takes the voltage on its input 60 times a minute and then returns the
average every minute. It will communicate with a raspberry Pi through an SPI
connection. The Raspberry Pi will send it instructions and it will either
change its current output or return a data value.

A more detailed description of the system can be found [here](https://docs.google.com/document/d/1yu_FLf-eIkDXZ3RdOsL7_ZVePAdN1JockzXvX2gE-KU/edit?usp=sharing).
## Contributors
It is a project run by Dr. Ryan Steward in the Plants and Wildlife Sciences Department
at Brigham Young University. The main contributer of this code is Samuel Lahti.