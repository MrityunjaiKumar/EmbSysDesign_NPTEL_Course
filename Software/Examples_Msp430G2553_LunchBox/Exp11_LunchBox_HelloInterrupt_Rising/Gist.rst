Experiment 10 - LunchBox Hello Interrupt on Rising Edge
===================================

Description
^^^^^^^^^^^^
This example code provides the functions and register settings for toggling the state of onboard LED with an external switch having interrupt on rising edge.

Hardware needed
^^^^^^^^^^^^^^^
Push Button Switch, Resistor (1 kohm), Breadboard, Connecting wires.

Connections
^^^^^^^^^^^
One end of switch is connected to Ground and another end is connected to pin 1.4 of Lunchbox. This pin is also connected to Vcc (3.3 V) through a pull-up resistor.

Output
^^^^^^

Code
^^^^

.. literalinclude:: ../../../../Software/EmbSysDesign/Software/Examples_Msp430G2553_LunchBox/Exp10_LunchBox_HelloInterrupt_Rising/Exp10_LunchBox_HelloInterrupt_Rising.c
    :linenos:
    :language: c
