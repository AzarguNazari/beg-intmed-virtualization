#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define stack and memory sizes
#define STACK_SIZE 256
#define MEMORY_SIZE 1024

// Virtual Machine structure
typedef struct {
    int stack[STACK_SIZE];       // Stack for storing values
    int memory[MEMORY_SIZE];    // Memory for storing data (not used in this example)
    int stack_pointer;          // Points to the top of the stack
    int program_counter;       // Points to the current instruction in the code
    uint8_t *code;              // Bytecode program to execute
    int code_size;              // Size of the bytecode program
} VM;

// Instruction set for the VM
typedef enum {
    OP_PUSH,    // Push a value onto the stack
    OP_POP,     // Pop a value from the stack
    OP_ADD,     // Add the top two values on the stack
    OP_SUB,     // Subtract the top value from the second-top value on the stack
    OP_JUMP,    // Jump to a new location in the code
    OP_HALT     // Stop execution
} OpCode;

/**
 * Initialize the virtual machine.
 * @param vm Pointer to the VM structure
 * @param code Pointer to the bytecode program
 * @param code_size Size of the bytecode program
 */
void vm_init(VM *vm, uint8_t *code, int code_size) {
    vm->stack_pointer = -1;     // Initialize stack pointer to -1 (empty stack)
    vm->program_counter = 0;   // Start execution at the first instruction
    vm->code = code;           // Set the bytecode program
    vm->code_size = code_size; // Set the size of the bytecode program

    // Initialize stack and memory to 0
    for (int i = 0; i < STACK_SIZE; i++) vm->stack[i] = 0;
    for (int i = 0; i < MEMORY_SIZE; i++) vm->memory[i] = 0;
}

/**
 * Push a value onto the stack.
 * @param vm Pointer to the VM structure
 * @param value Value to push onto the stack
 */
void vm_push(VM *vm, int value) {
    if (vm->stack_pointer >= STACK_SIZE - 1) {
        printf("Stack overflow!\n");
        exit(1);
    }
    vm->stack[++vm->stack_pointer] = value; // Increment stack pointer and store value
}

/**
 * Pop a value from the stack.
 * @param vm Pointer to the VM structure
 * @return The value popped from the stack
 */
int vm_pop(VM *vm) {
    if (vm->stack_pointer < 0) {
        printf("Stack underflow!\n");
        exit(1);
    }
    return vm->stack[vm->stack_pointer--]; // Decrement stack pointer and return value
}

/**
 * Execute the bytecode program.
 * @param vm Pointer to the VM structure
 */
void vm_execute(VM *vm) {
    while (vm->program_counter < vm->code_size) {
        uint8_t instruction = vm->code[vm->program_counter++]; // Fetch the next instruction

        switch (instruction) {
            case OP_PUSH: {
                // Push the next value in the code onto the stack
                int value = vm->code[vm->program_counter++];
                vm_push(vm, value);
                break;
            }
            case OP_POP: {
                // Pop a value from the stack
                vm_pop(vm);
                break;
            }
            case OP_ADD: {
                // Add the top two values on the stack
                int a = vm_pop(vm);
                int b = vm_pop(vm);
                vm_push(vm, a + b);
                break;
            }
            case OP_SUB: {
                // Subtract the top value from the second-top value on the stack
                int a = vm_pop(vm);
                int b = vm_pop(vm);
                vm_push(vm, b - a);
                break;
            }
            case OP_JUMP: {
                // Jump to a new location in the code
                int offset = vm->code[vm->program_counter++];
                vm->program_counter += offset;
                break;
            }
            case OP_HALT: {
                // Stop execution
                return;
            }
            default: {
                // Handle unknown instructions
                printf("Unknown instruction: %d\n", instruction);
                exit(1);
            }
        }
    }
}

/**
 * Main function to demonstrate the stack-based VM.
 */
int main() {
    // Example bytecode program: PUSH 5, PUSH 10, ADD, PUSH 20, SUB, HALT
    uint8_t code[] = {
        OP_PUSH, 5,    // Push 5 onto the stack
        OP_PUSH, 10,   // Push 10 onto the stack
        OP_ADD,        // Add the top two values (5 + 10 = 15)
        OP_PUSH, 20,   // Push 20 onto the stack
        OP_SUB,        // Subtract the top value from the second-top value (15 - 20 = -5)
        OP_HALT        // Stop execution
    };

    // Initialize and execute the VM
    VM vm;
    vm_init(&vm, code, sizeof(code));
    vm_execute(&vm);

    // Print the result (top of the stack)
    printf("Result: %d\n", vm_pop(&vm)); // Should print -5
    return 0;
}