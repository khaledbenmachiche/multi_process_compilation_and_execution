# MultiProc-Compile-Exec (TP1 OPERATING SYSTEMS 1)

This C program demonstrates the creation of three processes (P1,P2,P3) to compile, link, and execute a C program provided as an argument.

## Usage

### Compilation

To compile the C program, use the following command:

```bash
gcc main.c -o main
```

### Execution

After compilation, execute the program by running:

```bash
./main prog.c
```

Replace prog.c with the desired C file name you want to compile, link, and execute.

### Process Description

* P1: Compiles the provided C program (prog.c) and generates an object file named prog.o.
* P2: Links the object file to create an executable named prog.
* P3: Executes the prog program.
The parent process waits for the termination of P3 and captures the termination conditions, considering both normal and abnormal termination cases.

### Requirements

* C Compiler (e.g., GCC)
* Linux/Unix Environment

### Repository Contents

main.c: The main program managing the creation of processes for compilation and execution.

prog.c : example <programme.c> file to test with.


