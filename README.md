ArduinoTools
============

Arduino "Paralel" Working Libraries

Those libs are made to work together, the basic for both libs are: config, begin and process.

On config, you inform which function the lib will run when an event whappens.

On begin, the lib will init any envolved hardware.

The process you will put on "loop" arduino function, to the lib run its processing to check 
if there is any new event. If any, it will run your function.
