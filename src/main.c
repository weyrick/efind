/*

Copyright (c) 2012, Shannon Weyrick <weyrick@mozek.us>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "list.h"
#include "parse_expr.h"

#define FALSE 0
#define TRUE  1

struct option longopts[] = {
    {"version", FALSE, 0, 0},
    {"command-only", TRUE, 0, 'c'},
    {"verbose", FALSE, 0, 'v'},
    {0, 0, 0, 0}
};

int commandOnly = FALSE;
int verbose = FALSE;

void version() {
    printf("efind v.9\n");
}

void usage(int retval) {

    version();
    printf("Usage:\n");
    printf("efind [path] [expression]\n");
    printf("\t-c\toutput find command only (do not run find)\n");
    printf("\t-v\tverbose output\n");
    exit(retval);

}

void runFind(char *path, list *argList)
{

    list *finalArgs = list_create();
    list_push_str(finalArgs, strdup(path));
    list_push_list(finalArgs, argList);

    char **argVec = list_to_array(finalArgs);
    if (argVec == NULL)
        return;

    if (verbose || commandOnly) {
        if (verbose) {
            char *s = argVec[0];
            int i=0;
            while (s) {
                printf("%i: %s\n", i++, s);
                s = argVec[i];
            }
        }
        printf("find %s\n", list_to_str(finalArgs));
    }

    if (commandOnly) {
        exit(0);
    }

    if (execvp("find", argVec) == -1)
        perror("exec failed");
    exit(-1);

}

int main(int argc, char *argv[]) {

    if (argc < 3)
        usage(0);

    int opt, idx;
    int optionsError = FALSE;
    while ((opt = getopt_long(argc, argv, "cv", longopts, &idx)) !=
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
            case 'c':
                commandOnly = TRUE;
                break;
            case 'v':
                verbose = TRUE;
                break;
        }
    }

    // check for options errors
    if (optionsError || argc <= optind+1)
        usage(1);

    list *argVec = parse_expr(argv[optind+1], verbose);
    if (argVec == NULL) {
        printf("null expression\n");
        exit(1);
    }
    runFind(argv[optind], argVec);

    return 0;

}
