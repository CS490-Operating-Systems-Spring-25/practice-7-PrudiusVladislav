#!/bin/bash

as -o task.o task.S
ld -o task task.o
./task
echo -e $"\nReturn code {$?}"