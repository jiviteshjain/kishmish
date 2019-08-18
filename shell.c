#include "shell.h"
#include "utils.c"

int main (void) {
    // GET ESSENTIALS
    home_dir = getcwd(NULL, 0);
    user_name = getlogin(); // Statically declared. Don't free() this
    host_name = (char*)malloc(sizeof(char) * (HOST_NAME_MAX + 1));
    gethostname(host_name, (HOST_NAME_MAX + 1));

    // REST OF THE CODE
    char buf[100];
    while (true) {
        printf("%s", get_prompt());
        scanf("%s", buf);
    }

    // FREE POINTERS
    free(home_dir);
    free(host_name);
    return 0;
}