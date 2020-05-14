#include "shell.h"
#include "utils.h"
#include "prompt.h"
#include "parse.h"

int main (void) {
    // GET ESSENTIALS
    init();

    // REST OF THE CODE
    size_t in_buffer_size = 0;
    ssize_t in_size = 0;
    char* in_line = NULL;

    while (true) {
        // LOOP PROCESSING
        free(in_line);
        in_size = 0;
        in_buffer_size = 0;
        prompt();
        in_size = getline(&in_line, &in_buffer_size, stdin);

        if(in_size <= 0){
            free(in_line);
            break;
        }

        // OTHER THINGS
        if (in_line[in_size-1] == '\n') {
            in_line[in_size - 1] = '\0';
        }
        parse(in_line);
    }

    // CLEANUP
    goodbye();
}