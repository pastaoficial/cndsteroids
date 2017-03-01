# cndSteroids

Plugin template for x64dbg that allow the possibility to set breakpoints when a memory zone has a particular text.

## Installation

Grab the latest [release](https://github.com/pastaCLS/cndsteroids/blob/master/bin/x32/cndsteroids.dp32) and copy to the x32dbg's plugin floder.

## How it works?

The plugin creates the command setString, this command asociate the string with a number, later the number will identify the string in another commands

![alt tag](doc/setString.png?raw=true)

For our test we set the registers as follow:

![alt tag](doc/TestString-esp.png?raw=true)

We have "TestString" in the stack pointer

Now we have the expression function cndsteroids.ismystring() that receives a debugge's memory address, and returns true or false if has the text.

![alt tag](doc/ismystring.png?raw=true)

As you can see, the address has our string, so returns true:

![alt tag](doc/return.png?raw=true)

This feature could be used in a bpcnd, to stop is a especific argument of a syscall, for example:

![alt tag](doc/script.png?raw=true)

and the debugger pauses in

![alt tag](doc/stringinstack.png?raw=true)

You can find unicode or ansi strings depends on the second argument of cndsteroids.ismystring ($ANSI or $UNICODE)

By the way, the strings are dinamicly allocated by the plugin, so if you want free it you have available the command delString that receives the identifier number of the string and deallocate it.
