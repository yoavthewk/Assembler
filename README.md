# Assembler Project - Final Project

This project is an Assembler written in the C programming language that supports 15 different commands, macros, labels, and different types of data (e.g. strings).

The commands it supports are as following:
mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts, stop

## The Computer
The computer in this project consists of a processor (a CPU), registers, and RAM (Random Access Memory). Part of the memory is used as a stack. The computer only works with integers, both positive and negative. There is no support for real numbers. Arithmetically, it works using the 2's completement method, and it supports characters that are represented in ascii code.

### The CPU (and registers)
The CPU has 16 registers, all labled r + their index number (e.g. r0, r1.. r15). Each of these registers can contain up to 20 bits. The bits in each of those are labeled in the same way, the bit in the 0 index is the LSB (Least Significant Bit) while the bit in the 19th index is the MSB (Most Significant Bit). Furthermore, the CPU has a registered labeled PSW (Program Status Word), which holds several flags that describe the status of the CPU in every single moment.

### The Memory
The memory has 8192 cells, all labled with a number from 0 to 8192. Each cell has 20 bits, and is called a word. The bits are labeled in the same way as they are in the registers.
