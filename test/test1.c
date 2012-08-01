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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse_expr.h"

typedef struct {
    char *expr;
    char *expect;
} test;

test testList[] = {
    // expr, expected find commands
    {"owned by weyrick",  "-user weyrick"},
    {"owner weyrick",     "-user weyrick"},
    {"grouped by weyrick", "-group weyrick"},
    {"group weyrick", "-group weyrick"},
    {"owner weyrick or group weyrick", "-user weyrick -o -group weyrick"},
    {"size 10 BYTES", "-size 10c"},
    {"size 10 bytes", "-size 10c"},
    {"size 1 byte", "-size 1c"},
    {"size 10 kilobytes", "-size 10k"},
    {"size 10 kilobyte", "-size 10k"},
    {"size 10 kb", "-size 10k"},
    {"size 10 megabytes", "-size 10M"},
    {"size 10 megabyte", "-size 10M"},
    {"size 10 mb", "-size 10M"},
    {"size 10 gigabytes", "-size 10G"},
    {"size 10 gigabyte", "-size 10G"},
    {"not owned by weyrick", "! -user weyrick"},
    {"not owned by weyrick and not grouped by root", "! -user weyrick -a ! -group root"},
    {"named foo*", "-name foo*"},
    {"named *bar*", "-name *bar*"},
    {"named bar??foo", "-name bar??foo"},
    {"name foo*", "-name foo*"},
    {"inamed bar??foo", "-iname bar??foo"},
    {"iname foo*", "-iname foo*"},
    {"wholenamed bar??foo", "-wholename bar??foo"},
    {"wholename foo/bar*", "-wholename foo/bar*"},
    {"iwholenamed bar??foo", "-iwholename bar??foo"},
    {"iwholename foo*", "-iwholename foo*"},
    {"directory", "-type d"},
    {"file", "-type f"},
    {"pipe", "-type p"},
    {"link", "-type l"},
    {"socket", "-type s"},
    {"directory named foo", "-type d -name foo"},
    {"file named foo", "-type f -name foo"},
    {"executable", "-executable"},
    {"executable file named foo", "-executable -type f -name foo"},
    {"empty", "-empty"},
    {"empty executable file named foo", "-empty -executable -type f -name foo"},
    {"nogroup", "-nogroup"},
    {"no group", "-nogroup"},
    {"nouser", "-nouser"},
    {"no user", "-nouser"},
    {"readable", "-readable"},
    {"writable", "-writable"},
    {"gid 123", "-gid 123"},
    {"uid 123", "-uid 123"},
    //
    {0,0}
};

// return 0 on pass, non-0 on fail
int runTest(char *expr, char *expect) {

    list *argList = parse_expr(expr, 0);
    char *result = list_to_str(argList);
    int cmp = strcmp(result, expect);
    if (cmp)
        printf("FAIL: [%s] != [%s]\n", result, expect);
    else
        printf("OK  : [%s] == [%s]\n", expr, expect);

    list_free(argList, 1);
    free(result);

    return cmp;
}

int main(int argc, char *argv[]) {

    printf("efind test suite\n");

    int i, fail, pass;
    i = pass = fail = 0;
    while (testList[i].expr) {
        if (runTest(testList[i].expr, testList[i].expect))
            fail++;
        else
            pass++;
        i++;
    }

    printf("%i pass, %i fail\n", pass, fail);

    if (fail)
        return 1;
    else
        return 0;

}


