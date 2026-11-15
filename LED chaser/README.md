# LED Chaser Circuit 🚥
## Overview
This project is a hardware-based LED Chaser circuit designed to create a sequential, flashing light effect without the use of a programmable microcontroller. The circuit relies on the integration of standard ICs to trigger 32 LEDs in a predetermined, repetitive chasing cycle. 

This project was developed at J.C. Bose University of Science and Technology, YMCA as part of the B.Tech Electronics (IoT) curriculum, under the supervision of Ms. Dipika.

## How It Works
The core functionality is driven by two main integrated circuits:
*   **NE555 Timer IC:** Configured in astable mode, this IC acts as a clock oscillator. It generates a continuous square wave (pulses), alternating between high and low states.
*   **74HC595 Shift Register:** An 8-bit shift register utilizing a Serial IN Parallel OUT (SIPO) protocol. It receives the clock pulses from the 555 timer and shifts the data serially, driving the output pins parallelly to sequentially light up the LEDs matrix.

## Circuit Design & Schematic
Below is the circuit simulation and schematic, demonstrating the wiring of the 555 Timer, the 74HC595 Shift Register, and the 32 LED matrix:

*<img width="1080" height="602" alt="WhatsApp Image 2026-11-15 at 5 15 32 PM" src="https://github.com/user-attachments/assets/14f3b098-c89a-4e4c-a4a9-2c742bd13685" />
*

## Hardware Implementation
Below is the PCB trace layout design used for etching, followed by the final custom-soldered board showing the physical component placement and copper routing:

**PCB Trace Layout:**

*(<img width="749" height="712" alt="WhatsApp Image 2026-11-15 at 5 15 30 PM" src="https://github.com/user-attachments/assets/a3b28770-6d0e-494f-b059-8701e88bdd98" />
)*

**Final Soldered Board:**

*<img width="899" height="1600" alt="WhatsApp Image 2026-11-15 at 5 14 10 PM" src="https://github.com/user-attachments/assets/dbd9d838-67d9-4a4c-bfe7-fa56db55649e" />
*

## Components Used
*   **ICs:** NE555 Timer, 74HC595 Shift Register
*   **Transistor:** BC547 (NPN bipolar junction transistor for switching)
*   **LEDs:** 32x LEDs
*   **Passives:** 10kΩ Potentiometer, 3x 10kΩ Resistors, 4.7µF 25V Capacitor
*   **Hardware:** Double-sided copper-clad PCB, Female headers, 3.7V Battery

## Applications
LED Chasers are widely deployed in visual signaling, including advertising displays, running light ropes, small disco lighting, knight rider sequences, and home decorations.
