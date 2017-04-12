#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_SELECTOR "-name"
#define TYPE_SELECTOR "-type"
#define PERM_SELECTOR "-perm"
#define PRINT_SELECTOR "-print"
#define DELETE_SELECTOR "-delete"
#define EXEC_SELECTOR "-exec"

char* name;
char fileType = 'a'; // File type (f - file, d - directory , l - link, a - all) - All by default
int permMode = 0;    // File permissions - Ignore by default
int print = 0;       // Print flag - Disabled by default
int delete = 0;      // Delete flag - Disabled by default
int exec = 0;        // Exec flag - Disabled by default
char* command;       // The container for the command string

char * replace(char const * const original, char const * const pattern, char const * const replacement) {
    size_t const replen = strlen(replacement);
    size_t const patlen = strlen(pattern);
    size_t const orilen = strlen(original);

    size_t patcnt = 0;
    const char * oriptr;
    const char * patloc;

    // find how many times the pattern occurs in the original string
    for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen) {
        patcnt++;
    }

    {
        // allocate memory for the new string
        size_t const retlen = orilen + patcnt * (replen - patlen);
        char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

        if (returned != NULL) {
            // copy the original string, 
            // replacing all the instances of the pattern
            char * retptr = returned;
            for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen) {
                size_t const skplen = patloc - oriptr;
                // copy the section until the occurence of the pattern
                strncpy(retptr, oriptr, skplen);
                retptr += skplen;
                // copy the replacement 
                strncpy(retptr, replacement, replen);
                retptr += replen;
            }
            // copy the rest of the string.
            strcpy(retptr, oriptr);
        }
    return returned;
    }
}

void printUsageMessage() {
    printf("\nWrong number of arguments!\n");
    printf("Usage: sfind -name filename [-type fileType] [-perm permMode] [-print] [-delete] [-exec command]\n");
    printf("filename: string - extact name of the file;\n");
    printf("fileType: char - the file type; can be f (normal file), d (directory), l (simbolyc link), (all by default)\n");
    printf("permMode: octal - file access permissions (ignored by default);\n");
    printf("print: shows the name of the files found;\n");
    printf("delete: deletes the files found;\n");
    printf("command: executes the given command (the files found are represented with {});\n\n");
}

int parseCommandLineArgs(int argc, char *argv[]) {
    unsigned int i;
    char * cmd;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], NAME_SELECTOR) == 0) {
            // File name
            name = argv[++i];
        } else if (strcmp(argv[i], TYPE_SELECTOR) == 0) {
            // File type
            if (argv[i+1][0] == 'f' || argv[i+1][0] == 'd' || argv[i+1][0] == 'l')
                fileType = argv[++i][0];
            else 
                printf("Warning: Character %c not recognized for option fileTye. Assuming all by default\n", argv[i+1][0]);
        } else if (strcmp(argv[i], PERM_SELECTOR) == 0) {
            permMode = strtol(argv[++i], 0, 8);
        } else if (strcmp(argv[i], PRINT_SELECTOR) == 0) {
            print = 1;
        } else if (strcmp(argv[i], DELETE_SELECTOR) == 0) {
            delete = 1;
        } else if (strcmp(argv[i], EXEC_SELECTOR) == 0) {
            exec = 1;
            cmd = argv[++i];
        }
    }
    if (strlen(name) == 0) return 0;
    if (exec) command = replace(cmd, "{}", name);
    return 1;
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printUsageMessage();
    } else if (!parseCommandLineArgs(argc, argv)) {
        printf("\nFile name was not found!\n");
        printUsageMessage();
    }

    // Test Print
   printf ("Name: %s\nFileType: %c\nPermMode: %#o\nPrint: %d\nDelete: %d\nExec: %d\nCommand: %s\n", name, fileType, permMode, print, delete, exec, command);
   return 0;
}

