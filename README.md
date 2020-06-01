# Simple Shell

A simple implementation of Shell. 

Except that all commands are executed asynchronously.

## Author

Ilan Kleiman

## Compiling & Running

Compile:

`$ make clean all`

Run:

`$ ./smash [-d] [filename]`

## Debugging

To see some basic debugging information, simply supply the `-d` flag upon execution of the `smash` binary.

`$ ./smash -d`

To see even more verbose debugging, compile the program via make with debug:

`$ make clean debug`

Then run the debug-binary the same way,

`$ ./smash -d`

## Interactive & Non-Interactive Modes

There are two different modes, one mode is interactive - meaning upon execution it spawns a process that appears just like a shell would. The non-interactive mode is run by supplying the optional filename parameter to the smash executable.

### Interactive

To run interactive mode, simply DO NOT supply a filename argument to the `smash` binary.

### Non-Interactive

To run non-interactive mode, supply an executable file as a parameter to the `smash` binary.

`$ ./smash SomeExecutableFile`

Note: You may need to change permissions of the file before being able to run it.

`$ chmod +x SomeExecutableFile`

#### Executable file for non-interactive mode

To execute multiple commands, you must separate each command by a new line in the executable file.

```shell

echo hello
echo $PATH
```

Sample executable file.

## Environment Variables

All the environment variables are accessible via the `echo` command and also other commands too.
Prior to starting `smash`, you can set more environment variables using `$ export SOMEKEY=SOMEVALUE`.

The `$PATH` variable is parsed and upon execution of commands, `smash` looks in each of the path's given by the `$PATH` variable.
