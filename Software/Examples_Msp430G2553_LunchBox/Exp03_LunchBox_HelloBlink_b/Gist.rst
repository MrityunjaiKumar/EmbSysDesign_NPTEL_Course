Experiment 2 - LunchBox Hello Blink (using pointers)
====================================================

Description
^^^^^^^^^^^^
This example code provides the functions and register settings to toggle the LED present on LunchBox. In this example, we have used direct memory mapped address of register to perform the blinking of led. We can rephrase this statement by saying no #include <MSP430.h> header is used, which contains mapping for register names and addresses. Comments are provided to see the correlation of addresses to register names. These addresses can be verfied from the MSP430G2553 datasheet.

Course link
^^^^^^^^^^^^
Covered in lecture number - Lecture 21

Hardware needed
^^^^^^^^^^^^^^^
NA

Schematic image
^^^^^^^^^^^^^^^
NA

Connections
^^^^^^^^^^^
NA

Output
^^^^^^
On board user led starts blinking

Code
^^^^

.. literalinclude:: ../../../../Software/EmbSysDesign/Software/Examples_Msp430G2553_LunchBox/Exp03_LunchBox_HelloBlink_b/Exp03_LunchBox_HelloBlink_b.c
    :linenos:
    :language: c
