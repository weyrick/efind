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

#include "parse_expr.h"

#define FALSE 0
#define TRUE  1

struct option longopts[] = {
    {"version", FALSE, 0, 0},
    {"command-only", TRUE, 0, 'c'},
    {0, 0, 0, 0}
};

int commandOnly = TRUE;

void version() {
    printf("efind v.9\n");
}

void usage(int retval) {

    version();
    printf("Usage:\n");
    printf("efind [path] [expression]\n");
    printf("\t-c\toutput find command only (do not run find)\n");
    exit(retval);

}

runFind(char *argVec[])
{

    char *findPath = "/usr/bin/find";

    execve(findPath, argVec, NULL);
    perror("execve failed");

}

void parseExpression(char *exp) {

    parse_expr(exp);

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

    //char **argVec = parseCommandline();
    //runFind(argVec);

    parseExpression(argv[1]);

    return 0;

}
