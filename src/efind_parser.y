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
	#include "scanner.h"
        #include "efind_parser.h"
        #include "parse_expr.h"
}

%token_type {scanner_token*}
%default_type {scanner_token*}
%type expr {scanner_token*}
%type TOKEN_INTEGER {scanner_token*}
%left TOKEN_ADD TOKEN_SUB.
%left TOKEN_MUL TOKEN_DIV.
%syntax_error {printf("syntax error\n");}

in ::= expr(A). {printf("in expr(A):\n");}
expr(A) ::= expr(B) TOKEN_ADD expr(C). { printf("expr(A) expr(B) expr(C)\n"); } //A->data.n = B->data.n + C->data.n;}
expr(A) ::= expr(B) TOKEN_SUB expr(C). { printf("expr(A) expr(B) expr(C)\n"); } //A->data.n = B->data.n - C->data.n;}
expr(A) ::= expr(B) TOKEN_MUL expr(C). { printf("expr(A) expr(B) expr(C)\n"); } //A->data.n = B->data.n * C->data.n;}
expr(A) ::= expr(B) TOKEN_DIV expr(C). {/* TODO: fix division by 0 */
        //A->data.n = B->data.n / C->data.n;
        printf("expr(A) expr(B) expr(C)\n");
        }
expr(A) ::= TOKEN_INTEGER(B). {
printf("expr(A) expr(B)\n");
//A->data.n = B->data.n;
}
