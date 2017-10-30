# Zvuk9

USB MIDI controller with 9 force and position sensing pads used to play melodic instruments or drums / samples. Pads are sending:

- note
- note velocity
- note aftertouch
- 2 control changes through x and y coordinates (touch position) or pitch bend
 
Note, aftertouch, x and y sending can be disabled each independently. X and Y have configurable min and max values, curve and MIDI controller that is being sent. All nine pads can use the same X,Y cc/max/min/curve values (useful for melodic instruments) or could be set uniquely (useful for playing drums, samples). Pads are set up to play one of the predetermined scales. Alternatively, they can be set to play custom scale or chords through user defined presets. Controller is capable of sending MIDI messages via USB or via MIDI Out port.

# Compiling

In order to compile Zvuk9, Atmel Studio 7 is needed. Debug and Release configurations are available. Debug configuration uses LUFA virtual Serial mode to print various messages during Zvuk9 usage, while Release configuration compiles with USB MIDI support.

# Licencing

Zvuk9 uses various modules which are licenced under different licenses. 

- LUFA is licenced under modified MIT licence.
- MIDI module is licenced under MIT.
- U8G2 is licenced under Two-clause BSD licence.
- AVR-Core, AVR-Versioning, AVR-DB and strings modules are available under MIT licence.

This project is available under GNU GPLv3 licence. For commerical uses commercial licence is available on demand.