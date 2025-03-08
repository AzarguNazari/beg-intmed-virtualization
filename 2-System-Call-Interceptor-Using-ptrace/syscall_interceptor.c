#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>   // For ORIG_EAX
#include <sys/user.h>  // For struct user_regs_struct
#include <errno.h>

/**
 * Trace a child process and intercept its system calls.
 * @param pid The process ID of the child to trace.
 */
void trace_process(pid_t pid) {
    int status;
    struct user_regs_struct regs;

    // Wait for the child process to stop
    waitpid(pid, &status, 0);

    // Continue tracing until the child process exits
    while (WIFSTOPPED(status)) {
        // Get the child's register state
        ptrace(PTRACE_GETREGS, pid, 0, &regs);

        // Log the system call number (stored in ORIG_EAX on x86)
        long syscall_num = regs.orig_eax;
        printf("System call: %ld\n", syscall_num);

        // Allow the system call to execute
        ptrace(PTRACE_SYSCALL, pid, 0, 0);

        // Wait for the child to stop again
        waitpid(pid, &status, 0);

        // Log the system call return value (stored in EAX on x86)
        ptrace(PTRACE_GETREGS, pid, 0, &regs);
        printf("Return value: %ld\n", regs.eax);

        // Continue to the next system call
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, &status, 0);
    }
}

/**
 * Main function to start the system call interceptor.
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return Exit status
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process: execute the target program
        ptrace(PTRACE_TRACEME, 0, 0, 0); // Allow tracing
        execvp(argv[1], &argv[1]);       // Execute the target program
        perror("execvp");               // If execvp fails
        exit(1);
    } else {
        // Parent process: trace the child
        trace_process(pid);
    }

    return 0;
}