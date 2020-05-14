#include "shell.h"
#include "utils.h"
#include "parse.h"
#include "redirection.h"

redirect_meta redirection_begin(const redirect r) {
    // initiate redirection as per requests in r
    // duplicate file handles and return back everything needed to stop

    redirect_meta m;
    m.in_file = -1;
    m.out_file = -1;
    m.stdin_copy = -1;
    m.stdout_copy = -1;

    if (r.input) {

        if (r.input_file[0] == '\0') {
            fprintf(stderr, "Could not redirect input: Parse error near '<'.\n");
            return m;
        }

        int f = open(r.input_file, O_RDONLY);
        if (f <= 0) {
            perror("Could not redirect input");
            return m;
        }

        int temp = dup(STDIN_FILENO);
        if (temp < 0) {
            perror("Could not redirect input");
            close(f);
            return m;
        }

        if (dup2(f, STDIN_FILENO) < 0) {
            perror("Could not redirect input");
            close(temp);
            close(f);
            return m;
        }

        m.in_file = f;
        m.stdin_copy = temp;
    }
    if (r.output) {
        
        if (r.output_file[0] == '\0') {
            fprintf(stderr, "Could not redirect output: Parse error near '>'.\n");
            return m;
        
        }
        
        int f;
        if (r.append) {
            f = open(r.output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        } else {
            f = open(r.output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (f <= 0) {
            perror("Could not redirect output");
            return m;
        }

        int temp = dup(STDOUT_FILENO);
        if (temp < 0) {
            perror("Could not redirect output");
            close(f);
            return m;
        }

        if (dup2(f, STDOUT_FILENO) < 0) {
            perror("Could not redirect output");
            close(temp);
            close(f);
            return m;
        }

        m.out_file = f;
        m.stdout_copy = temp;
    }
    return m;
}

void redirection_end(const redirect r, const redirect_meta m) {
    // duplicate back file handles to normal
    
    if (r.input) {
        if (dup2(m.stdin_copy, STDIN_FILENO) < 0) {
            perror("Fatal I/O Error");
            fprintf(stderr, "Exiting...\n");
            goodbye(); // TODO: LOTS OF UN-FREED MEMORY
        }
        close(m.stdin_copy);
        close(m.in_file);
    }

    if (r.output) {
        if (dup2(m.stdout_copy, STDOUT_FILENO) < 0) {
            perror("Fatal I/O Error");
            fprintf(stderr, "Exiting...\n");
            goodbye();  // TODO: LOTS OF UN-FREED MEMORY
        }
        close(m.stdout_copy);
        close(m.out_file);
    }
}