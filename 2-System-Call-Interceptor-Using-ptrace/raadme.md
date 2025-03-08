# System Call Interceptor

A tool for intercepting and logging system calls using ptrace, similar to strace.

## Description

This project implements a system call interceptor that:
- Uses the `ptrace` syscall to monitor process execution
- Logs system calls made by a target process
- Provides optional functionality to restrict specific system behaviors

## Learning Objectives

### System Fundamentals
- Understanding system calls in Linux environments
- Process-level virtualization and isolation concepts

### Core Technical Concepts
- System call implementation and interception
- Process management
- Signal handling
- Using the `ptrace` system call

## Features
- System call monitoring and logging
- Process tracking
- Optional behavior restriction capabilities

## Technical Requirements
- C programming language
- Linux environment
- `ptrace` syscall support

## Educational Value
This project provides hands-on experience with:
- Low-level system programming
- Process monitoring and control
- System call manipulation