/**
 * intuitive find frontend
 * Shannon Weyrick <weyrick@mozek.us>
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FALSE 0
#define TRUE  1

struct option longopts[] = {
    {"version", FALSE, 0, 0},
    {"command-only", TRUE, 0, 'c'},
    {0, 0, 0, 0}
};

int commandOnly = TRUE;

void version() {
    printf("strfind v.9\n");
}

void usage(int retval) {

    version();
    printf("Usage:\n");
    printf("strfind [path] [expression]\n");
    printf("\t-c\toutput find command only (do not run find)\n");
    exit(retval);

}

runFind(char *argVec[])
{

    char *findPath = "/usr/bin/find";

    execve(findPath, argVec, NULL);
    perror("execve failed");

}

char **parseCommandline() {

    char **argVec = malloc(sizeof(char*)*2);
    argVec[0] = "/etc/";
    argVec[1] = NULL;
    return argVec;

}

int main(int argc, char *argv[]) {

    if (argc < 2)
        usage(0);

    int opt, idx;
    int optionsError = FALSE;
    while ((opt = getopt_long(argc, argv, "c", longopts, &idx)) !=
           -1) {
        switch (opt) {
            case 0:
                // long option tied to a flag variable
                if (strcmp(longopts[idx].name,"version") == 0) {
                    version();
                    exit(0);
                }
                break;
            case '?':
                optionsError = TRUE;
                break;
            case 'm':
                commandOnly = TRUE;
                break;
        }
    }

    // check for options errors
    if (optionsError)
        usage(1);

    char **argVec = parseCommandline();
    runFind(argVec);

    return 0;

}
