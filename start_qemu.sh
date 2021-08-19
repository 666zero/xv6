#!/bin/bash
sudo make clean
sudo make
if [$0 == "gdb"]
then
   sudo make qemu-gdb	
else
   sudo make qemu
fi
