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

%include {

	#include <stdio.h>
	#include <stdlib.h>
        #include <assert.h>
        #include <string.h>
        #include "list.h"
	#include "scanner.h"
        #include "efind_parser.h"
        #include "parse_expr.h"

}

%token_prefix TOKEN_
%type WS {int}
%token_type {scanner_token*}
%default_type {scanner_token*}
%syntax_error {
    printf("syntax error at: %s\n", TOKEN->data);
    exit(1);
}
%extra_argument {list* argList}

%left AND.
%left OR.
%right NOT.

%type goal {int}
%type expr {list*}
goal ::= expr(E). { list_push_list(argList, E); }

expr(RET) ::= expr(A) OR expr(B). {
    RET = list_create();
    list_push_list(RET, A);
    list_push_str(RET, strdup("-o"));
    list_push_list(RET, B);
}

expr(RET) ::= expr(A) AND expr(B). {
    RET = list_create();
    list_push_list(RET, A);
    list_push_str(RET, strdup("-a"));
    list_push_list(RET, B);
}

expr(RET) ::= NOT expr(A). {
    RET = list_create();
    list_push_str(RET, strdup("!"));
    list_push_list(RET, A);
}

expr(RET) ::= OWNEDBY WORD(B). {
    RET = list_create();
    list_push_str(RET, strdup("-user"));
    list_push_str(RET, B->data);
}

expr(RET) ::= GROUPEDBY WORD(B). {
    RET = list_create();
    list_push_str(RET, strdup("-group"));
    list_push_str(RET, B->data);
}

expr(RET) ::= NAMED WORD(B). {
    RET = list_create();
    if (B->opt == 0)
        list_push_str(RET, strdup("-name"));
    else if (B->opt == 1)
        list_push_str(RET, strdup("-iname"));
    else if (B->opt == 2)
        list_push_str(RET, strdup("-wholename"));
    else if (B->opt == 3)
        list_push_str(RET, strdup("-iwholename"));
    list_push_str(RET, B->data);
}

expr(RET) ::= SIZE INT(N) SIZEQUAL(B). {
    RET = list_create();
    list_push_str(RET, strdup("-size"));
    int numSize = strlen(N->data);
    char *tmp = malloc(numSize+2);
    memcpy(tmp, N->data, numSize);
    tmp[numSize] = B->opt;
    tmp[numSize+1] = 0;
    list_push_str(RET, tmp);
    free(N->data);
}

