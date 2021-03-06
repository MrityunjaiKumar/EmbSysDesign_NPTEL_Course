Experiment 23 - LunchBox Hello LFSR 8Bit
========================================

Description
^^^^^^^^^^^^
This example code provides function and register settings for reading analog data. This code converts the analog value provided by potentiometer into a corresponding 10 bit digital value. The converted digital value is then used to change the brightness of an external LED using PWM.

Hardware needed
^^^^^^^^^^^^^^^
Potentiometer (100 k), Resistor (330 ohms), LED, Breadboard, Connecting wires.

Connections
^^^^^^^^^^^
Pin 1.6 of LunchBox is connected to anode of LED through a resistor of 1 kohm and cathode to Ground. Pin 1.0 of LunchBox is connected to Analog pin (middle pin) of potentiometer and the other two ends are connected to Vcc and Ground respectively.

Output
^^^^^^

Code
^^^^

.. literalinclude:: ../../../../Software/EmbSysDesign/Software/Examples_Msp430G2553_LunchBox/Exp24_LunchBox_HelloLFSR_8Bit/Exp24_LunchBox_HelloLFSR_8Bit.c
    :linenos:
    :language: c
