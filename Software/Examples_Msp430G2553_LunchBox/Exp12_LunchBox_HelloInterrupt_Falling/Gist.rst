Experiment 12 - LunchBox Hello Interrupt on Falling Edge
========================================================

Description
^^^^^^^^^^^^
This example code provides the functions and register settings for toggling the state of onboard LED with an external switch having interrupt on falling edge.

Course link
^^^^^^^^^^^^
Covered in lecture number - Lecture 26

Hardware needed
^^^^^^^^^^^^^^^
Push Button Switch, Resistor (1 kohm), Capacitor(1uF), Breadboard, Connecting wires.

Schematic image
^^^^^^^^^^^^^^^
https://drive.google.com/file/d/1hfJpNFtMRx7g5U098fhPMUVDBGL48AU1/view?usp=sharing
https://drive.google.com/file/d/1_wK_YnGEVktMPqLMu3tF29diAxjtUcgu/view?usp=sharing

Connections
^^^^^^^^^^^
One end of switch is connected to Ground and another end is connected to pin 1.4 of Lunchbox. This pin is also connected to Vcc (3.3 V) through a pull-up resistor. Capacitor is connected across two ends of switch.

Output
^^^^^^
Onboard LED will toggle on pressing external switch.

Code
^^^^

.. literalinclude:: ../../../../Software/EmbSysDesign/Software/Examples_Msp430G2553_LunchBox/Exp12_LunchBox_HelloInterrupt_Falling/Exp12_LunchBox_HelloInterrupt_Falling.c
    :linenos:
    :language: c
