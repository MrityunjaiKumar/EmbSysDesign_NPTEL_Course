# Exp 13

## LunchBox Hello Seven Segment Display

___

### Description

This example code provides function and register settings for increasing count on every press of an external switch. 

### Course link

Covered in lecture number - Lecture 27

### Hardware needed

Seven Segment Display (4 digit SSD) - Common Cathode, Resistors - 270 ohm (quanitity - 8), 1 kohm (quanitity - 1), Toggle Push Button Switch,  Capacitors - 0.1 uF (Ceramic capacitor, quantity - 2), 10 uF (Electrolytic capacitor, quanitity - 1), Connecting wires.

### Schematic image

https://drive.google.com/file/d/1jRcAl5XDVLmvb1X5_1XmrmLOv0bUIz7p/view?usp=sharing

### Connections

One end of switch is connected to Ground and another end is connected to pin 2.3 of Lunchbox, this pin is also connected to Vcc (3.3 V) through a pull-up resistor. 
D1 pin of SSD is connected to Ground of LunchBox, pins a,b,c,d,e,f,g,decimal of SSD are connected to pins 1.0 to 1.7 of LunchBox respectively through series resistor of 270 ohm.

### Output

On every Switch press, the number on SSD will increase from 0 to F. (0 to 9 in decimal and then A to F in Hexadecimal). 
