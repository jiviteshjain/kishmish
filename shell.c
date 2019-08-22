#include "shell.h"
#include "utils.h"
#include "prompt.h"
#include "parse.h"
#include "pwd.h"
#include "cd.h"
#include "ls.h"
#include "echo.h"
#include "pinfo.h"
#include "external.h"
#include "history.h"
#include "nightswatch.h"

int main (void) {
    // GET ESSENTIALS
    home_dir = getcwd(NULL, 0);
    user_name = getlogin(); // Statically declared. Don't free() this
    host_name = (char*)malloc(sizeof(char) * (HOST_NAME_MAX + 1));
    gethostname(host_name, (HOST_NAME_MAX + 1));
    init_history();

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
        // nightswatch(1, 2);
    }

    // FREE POINTERS
    free(home_dir);
    free(host_name);
    return 0;
}