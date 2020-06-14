Experiment 18 - LunchBox Hello Hardware PWM (8Bit)
==================================================

Description
^^^^^^^^^^^^
This example code provides function and register settings for using Hardware PWM on LunchBox. In this example code, internal 16 bit Timer is used for generating PWM signal of fixed duty cycle. Pin 1.6 of LunchBox is used for getting PWM output.

Hardware needed
^^^^^^^^^^^^^^^
Breadboard, Connecting wires, LED, Resistor (1 kohm). 

Connections
^^^^^^^^^^^
Pin 1.6 of Lunchbox is connected to anode of LED through a resistor of 1 kohm and cathode to Ground.

Output
^^^^^^

Code
^^^^

.. literalinclude:: ../../../../Software/EmbSysDesign/Software/Examples_Msp430G2553_LunchBox/Exp19_LunchBox_HelloHardwarePWM_8Bit/Exp19_LunchBox_HelloHardwarePWM_8Bit.c
    :linenos:
    :language: c
