Experiment 16 - LunchBox Hello Timer
===================================

Description
^^^^^^^^^^^^
This example code provides function and register settings for generating Timer interrupt at 1 second. Using these settings, an eight bit binary up counter implementation has been represented on eight LEDs.

Hardware needed
^^^^^^^^^^^^^^^
Eight LEDs, Eight Resistors, Breadboard, Connecting Wires.

Connections
^^^^^^^^^^^
Port 1 pins of Lunchbox are connected to Eight LEDs via resistors in series. Pin 1.0 will represent LSB and Pin 1.7 will represent MSB of that Binary up counter. 

Output
^^^^^^

Code
^^^^

.. literalinclude:: ../../../../Software/EmbSysDesign/Software/Examples_Msp430G2553_LunchBox/Exp16_LunchBox_HelloTimer/Exp16_LunchBox_HelloTimer.c
    :linenos:
    :language: c
