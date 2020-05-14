#include "shell.h"
#include "utils.h"
#include "ls.h"

void handle_ls(int argc, char** argv) {
    // checks for 'a' and 'l' flags, and whether a directory to list has been specified
    // or not, otherwise defaults to home directory


    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc == 0) {
        ls(".", false, false);
        return;
    }

    bool a_flag = false;
    bool l_flag = false;

    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            int j = 1;
            while (argv[i][j] != '\0') {
                if (argv[i][j] == 'l')
                    l_flag = true;
                if (argv[i][j] == 'a')
                    a_flag = true;
                j++;
            }
        }
    }

    bool dir_found = false;

    for (int i = 0; i < argc; i++) {
        if (argv[i][0] != '-') {
            dir_found = true;
            ls(argv[i], a_flag, l_flag);
        }
    }
    if (!dir_found) {
        ls(".", a_flag, l_flag);
    }
}

int filter (const struct dirent* s) {

    if (strlen(s->d_name) > 0 && s->d_name[0] == '.') {
        return 0;
    }
    return 1;
}

ls_data get_ls_data (const char* path) {
    // path has to be path to that file, not just file name
    
    struct stat s;
    ls_data f;

    int ret_val = stat(path, &s);
    if (ret_val < 0) {
        // This error has to be printed here, because perror
        perror("Could not list file details");
        f.valid = false;
        return f;
    }
    
    // Donot assign a pointer to something inside s, as it will not be accessible after function returns. Copying variables is fine
    f.valid = true;
    f.user = getpwuid(s.st_uid)->pw_name; // TODO: perror
    f.group = getgrgid(s.st_gid)->gr_name; // TODO: perror
    f.size = s.st_size;
    f.blocks = s.st_blocks;
    f.num_h_link = s.st_nlink;

    // To get last modified times
    f.time = (char*)malloc(sizeof(char) * 26);
    ctime_r(&s.st_mtime, f.time);
    f.time = f.time + 4;
    f.time[16] = '\0';
    
    // To get permission string
    f.perms = (char*)malloc(sizeof(char) * 11);
    for (int i = 0; i < 10; i++) {
        f.perms[i] = '-';
    }
    f.perms[10] = '\0';

    if (S_ISLNK(s.st_mode))
        f.perms[0] = 'l';
    else if (S_ISDIR(s.st_mode))
        f.perms[0] = 'd';

    if (s.st_mode & S_IRUSR)
        f.perms[1] = 'r';

    if (s.st_mode & S_IWUSR)
        f.perms[2] = 'w';

    if (s.st_mode & S_IXUSR)
        f.perms[3] = 'x';

    if (s.st_mode & S_IRGRP)
        f.perms[4] = 'r';

    if (s.st_mode & S_IWGRP)
        f.perms[5] = 'w';

    if (s.st_mode & S_IRGRP)
        f.perms[6] = 'x';

    if (s.st_mode & S_IROTH)
        f.perms[7] = 'r';

    if (s.st_mode & S_IWOTH)
        f.perms[8] = 'w';

    if (s.st_mode & S_IXOTH)
        f.perms[9] = 'x';

    return f;
}

void print_ls_data (const ls_data f) {

    if (f.perms[0] == 'd') {
        printf("%s\t%d\t%s\t%s\t%lu\t%s\t" ANSI_CYAN_BOLD "%s\n" ANSI_DEFAULT,
            f.perms,
            f.num_h_link,
            f.user,
            f.group,
            f.size,
            f.time,
            f.name
        );
    } else if (f.perms[3] == 'x') {
        printf("%s\t%d\t%s\t%s\t%lu\t%s\t" ANSI_GREEN_BOLD "%s\n" ANSI_DEFAULT,
            f.perms,
            f.num_h_link,
            f.user,
            f.group,
            f.size,
            f.time,
            f.name
        );
    } else {
        printf("%s\t%d\t%s\t%s\t%lu\t%s\t%s\n",
            f.perms,
            f.num_h_link,
            f.user,
            f.group,
            f.size,
            f.time,
            f.name
        );
    }
}

bool ls(char* path, bool flag_a, bool flag_l) {
    // This is reqd because on empty strings, ls and cd behave differently.
    // ~ is handled by shell
    // . and .. are handled by the syscalls themselves 

    // ERROR HANDLING ON PATH
    struct stat path_stat;
    if (stat(path, &path_stat) < 0) {
        perror("Could not list directory contents");
        return false;
    }

    if (!S_ISDIR(path_stat.st_mode)) {
        fprintf(stderr, "Could not list directory contents: Not a directory.\n");
        return false;
    }

    // GET DIRECTORY CONTENTS
    struct dirent** files;
    int num_files;
    
    if (flag_a) {
        num_files = scandir(path, &files, NULL, alphasort);
    } else {
        num_files = scandir(path, &files, filter, alphasort);
    }

    if (num_files < 0) {
        perror("Could not list directory contents");
        free(files); // TODO: free the pointers in the array 'files' as well?
        return false;
    }

    // PRINT FILENAMES IF NOT -l
    if (!flag_l) {
        printf(ANSI_GREEN_BOLD "DIR: %s\n" ANSI_DEFAULT, path);
        for (int i = 0; i < num_files; i++) {
            printf("%s\n", files[i]->d_name);
            free(files[i]);
        }
        free(files);
        return true;
    }

    // WORRY ABOUT -l
    blkcnt_t blocks = 0;
    size_t path_len = strlen(path);
    printf(ANSI_GREEN_BOLD "\nDIR: %s\n" ANSI_DEFAULT, path);
    for (int i = 0; i < num_files; i++) {
                char* file = files[i]->d_name;
        size_t file_len = strlen(file);
        
        char* file_path = (char*)malloc(sizeof(char) * (file_len + path_len + 2));
        strcpy(file_path, path);
        strcat(file_path, "/");  // ls parser ensures no leading "/"
        strcat(file_path, file);
        
        ls_data f = get_ls_data(file_path);
        if (!f.valid) {
            free(file_path);
            free(files[i]);
            
            // printing error done by get_ls_data()
            continue; // just one file failed
        }
        blocks = blocks + f.blocks;
        f.name = files[i]->d_name;
        print_ls_data(f);
        free(file_path);
        free(files[i]);
    }
    printf(ANSI_GREEN "total: %ld\n" ANSI_DEFAULT, blocks / 2);
    free(files);
    return true;
}