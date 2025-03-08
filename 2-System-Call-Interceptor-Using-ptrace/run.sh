#!/bin/bash

gcc -o project syscall_interceptor.c && ./syscall_interceptor ls