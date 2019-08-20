#include "shell.h"
#include "utils.h"
#include "pwd.h"
#include "cd.h"
#include "ls.h"

int main (void) {
    // GET ESSENTIALS
    home_dir = getcwd(NULL, 0);
    user_name = getlogin(); // Statically declared. Don't free() this
    host_name = (char*)malloc(sizeof(char) * (HOST_NAME_MAX + 1));
    gethostname(host_name, (HOST_NAME_MAX + 1));


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
        ls("/////home/jivitesh/Documents/IIIT/ECell/head", true, false);
    }

    // FREE POINTERS
    free(home_dir);
    free(host_name);
    return 0;
}