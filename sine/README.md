# Generating saw wave on STM32F407G-Discovery1
This is code from [here](http://www.mind-dump.net/configuring-the-stm32f4-discovery-for-audio). But with Makefiles to help build the project.

# Instructions

Follow the instructions [here](http://www.wolinlabs.com/blog/linux.stm32.discovery.gcc.html) on setting up your Linux enironment for development on STM32F4xx boards.

Modify your Makefile to change STLINK and STM_COMMON to point them to the places you have installed and compiled these tools and libraries.

Run

  $ make

to make the generator.bin file.

  $ make burn

to burn the file to your STM32 Discovery board.
