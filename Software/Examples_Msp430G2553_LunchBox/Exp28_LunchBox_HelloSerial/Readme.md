# Exp 27

## LunchBox Hello Serial

___

### Description

This example code provides function and register settings for enabling serial communication with UART on LunchBox. Baud Rate for UART communication is set to 9600. It utilises on board CH340, which acts as USB-to-UART bridge. While recieving UART data, J1 and J2  selection should be on side 2.

### Course link

Covered in lecture number - Lecture 33

### Hardware needed

NA

### Schematic image

NA


### Connections

For using UART, Jumper J1 and J2 needs to be changed from Programming mode to UART mode.

### Output

An incremented character will be recieved on putty (or any serial terminal) wrt whatever you will send.