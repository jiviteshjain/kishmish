#include "shell.h"
#include "utils.h"
#include "ls.h"

int filter (const struct dirent* s) {
    // Removes .. and .
    if (strcmp(s->d_name, "..") == 0) {
        return 0;
    } else if (strcmp(s->d_name, ".") == 0) {
        return 0;
    } else {
        return 1;
    }
}

ls_data get_ls_data (const char* path) {
    // path has to be path to that file, not just file name
    
    struct stat s;
    ls_data f;

    int ret_val = stat(path, &s);
    if (ret_val < 0) {
        // This error has to be printed here, because perror
        perror("Could not list directory contents");
        f.valid = false;
        return f;
    }
    
    // Donot assign a pointer to something inside s, as it will not be accessible after function returns. Copying variables is fine
    f.valid = true;
    f.user = getpwuid(s.st_uid)->pw_name; // TODO: perror
    f.group = getgrgid(s.st_gid)->gr_name; // TODO: perror
    f.size = s.st_size;
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
    printf(ANSI_GREEN "%s\n" ANSI_DEFAULT, f.name);
    printf("PERMISSIONS: %s\tSIZE: %lu\n", f.perms, f.size);
    printf("LAST MODIFIED ON: %s\tHARD LINKS: %d\n", f.time, f.num_h_link);
    printf("USER: %s\tGROUP: %s\n", f.user, f.group);
    printf("_____ _____ _____ _____ _____ _____ _____ _____ _____\n\n");

    // printf("%s\t%d\t%s\t%s\t%lu\t%s\t%s\n",
    //        f.perms,
    //        f.num_h_link,
    //        f.user,
    //        f.group,
    //        f.size,
    //        f.time,
    //        f.name
    //     );
}

bool ls(char* path, bool flag_a, bool flag_l) {
    // This is reqd because on empty strings, ls and cd behave differently.
    // ~ is handled by shell
    // . and .. are handled by the syscalls themselves 
    if (path[0] == '\0') {
        strcpy(path, ".");
    }

    // ERROR HANDLING ON PATH
    struct stat path_stat;
    if (stat(path, &path_stat) < 0) {
        perror("Could not list directory contents");
        return false;
    }

    if (!S_ISDIR(path_stat.st_mode)) {
        printf("Could not list directory contents: Not a directory.\n");
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
        for (int i = 0; i < num_files; i++) {
            printf("%s\n", files[i]->d_name);
            free(files[i]);
        }
        free(files);
        return true;
    }

    // WORRY ABOUT -l
    size_t path_len = strlen(path);

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
            free(files);
            // printing error done by get_ls_data()
            return false;
        }
        f.name = files[i]->d_name;
        print_ls_data(f);
        free(file_path);
        free(files[i]);
    }
    free(files);
    return true;
}