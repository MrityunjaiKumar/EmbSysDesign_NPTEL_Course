Experiment 6 - LunchBox Hello Switch Pull Up
===================================

Description
^^^^^^^^^^^^
This example code provides the functions and register settings for toggling the state of LED on LunchBox with an external switch connected with a Pull Up resistor.

Hardware needed
^^^^^^^^^^^^^^^
Push Button Switch, Resistor (1 kohm), Breadboard, Connecting Wires.

Connections
^^^^^^^^^^^
One end of Switch is connected to Ground and another end is at pin 1.4 of LunchBox. This pin is also connected to Vcc (3.3 V) through a Pull Up resistor.

Output
^^^^^^

Code
^^^^

.. literalinclude:: ../../../../Software/EmbSysDesign/Software/Examples_Msp430G2553_LunchBox/Exp06_LunchBox_HelloSwitch_PullUp/Exp06_LunchBox_HelloSwitch_PullUp.c
    :linenos:
    :language: c
