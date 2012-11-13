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
%type CONNECTOR {int}
%token_type {scanner_token*}
%default_type {scanner_token*}
%syntax_error {
    if (TOKEN)
        printf("syntax error at: [%i][%s][%i]\n", TOKEN->tokType, TOKEN->data, TOKEN->opt);
    else
        printf("missing expected token\n");
    exit(1);
}
%extra_argument {list* argList}

%left AND.
%left OR.
%right NOT.

%type goal {int}
%type expr {list*}
%type expr_list {list*}

goal ::= expr_list(E). {
    list_push_list(argList, E);
}

expr_list(RET) ::= expr(A). {
    RET = A;
}

expr_list(RET) ::= expr_list(A) expr(B). {
    list_push_list(A, B);
    RET = A;
}

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

expr(RET) ::= NAMED(A) WORD(B). {
    RET = list_create();
    if (A->opt == 0)
        list_push_str(RET, strdup("-name"));
    else if (A->opt == 1)
        list_push_str(RET, strdup("-iname"));
    else if (A->opt == 2)
        list_push_str(RET, strdup("-wholename"));
    else if (A->opt == 3)
        list_push_str(RET, strdup("-iwholename"));
    list_push_str(RET, B->data);
}

expr(RET) ::= FTYPE(B). {
    RET = list_create();
    list_push_str(RET, strdup("-type"));
    if (B->opt == 0)      // directory
        list_push_str(RET, strdup("d"));
    else if (B->opt == 1) // pipe
        list_push_str(RET, strdup("p"));
    else if (B->opt == 2) // file
        list_push_str(RET, strdup("f"));
    else if (B->opt == 3) // link
        list_push_str(RET, strdup("l"));
    else if (B->opt == 4) // socket
        list_push_str(RET, strdup("s"));
}

expr(RET) ::= UNARY(B). {
    RET = list_create();
    if (B->opt == 0)
        list_push_str(RET, strdup("-executable"));
    else if (B->opt == 1)
        list_push_str(RET, strdup("-empty"));
    else if (B->opt == 2)
        list_push_str(RET, strdup("-nogroup"));
    else if (B->opt == 3)
        list_push_str(RET, strdup("-nouser"));
    else if (B->opt == 4)
        list_push_str(RET, strdup("-readable"));
    else if (B->opt == 5)
        list_push_str(RET, strdup("-writable"));
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

expr(RET) ::= UID INT(N). {
    RET = list_create();
    list_push_str(RET, strdup("-uid"));
    list_push_str(RET, N->data);
}

expr(RET) ::= GID INT(N). {
    RET = list_create();
    list_push_str(RET, strdup("-gid"));
    list_push_str(RET, N->data);
}

