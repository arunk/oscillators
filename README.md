# Experiments on the STM32F407G-DISC1 Board

This repo is a collection of various experiments on the STM32F4 Discovery Board.

# History
This project started with compiling the blink led project [here](http://www.wolinlabs.com/blog/linux.stm32.discovery.gcc.html) and from there to experiments with various oscillators.

## Generating saw wave on STM32F407G-Discovery1
This is code from [here](http://www.mind-dump.net/configuring-the-stm32f4-discovery-for-audio). But with Makefiles to help build the project.

# Common Instructions

These instructions apply to each of the examples.

Follow the instructions [here](http://www.wolinlabs.com/blog/linux.stm32.discovery.gcc.html) on setting up your Linux enironment for development on STM32F4xx boards.

Modify your Makefile to change STLINK and STM_COMMON to point them to the places you have installed and compiled these tools and libraries.

Run

  $ make

to make the xxx.bin file.

  $ make burn

to burn the file to your STM32 Discovery board.

# Debugging

To debug code, first run st-util, then run

  $ arm-none-eabi-gdb xxxx.elf

Then from the gdb console

  (gdb) target remote localhost:4242

You can now step through your code and examine what happens.
