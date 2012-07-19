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

// this is the main parse function which takes a buffer and scans and parses
// it using the re2c generated lexer and the lemon generated parser

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "parse_expr.h"
#include "scanner.h"
#include "efind_parser.h"

#define TOKEN_STACK 50

void *ParseAlloc(void *(*mallocProc)(size_t));
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
);
void Parse(
  void *yyp,                  /* The parser */
  int yymajor,                /* The major token code number */
  scanner_token *yyminor,      /* The value for the token */
  list *argList
);

list* parse_expr(char *expr, int verbose) {

    scanner_state state;

    int stat;

    list *argList = list_create();

    void *pParser = (void*)ParseAlloc(malloc);

    state.start = expr;

    if (verbose)
        printf("parse_expr: [%s]\n", expr);

    void* tokenStack = malloc(sizeof(scanner_token) * TOKEN_STACK);
    memset(tokenStack, 0, sizeof(scanner_token) * TOKEN_STACK);
    scanner_token *token = (scanner_token*)tokenStack;

    while (0 <= (stat = scan(&state, token))) {
        state.end = state.start;
        if (token->tokType == TOKEN_WS)
            continue;
        if (verbose)
            printf("token: [%i][%s][%i][%x]\n", token->tokType, token->data, token->opt, token);
        Parse(pParser, token->tokType, token, argList);
        token += sizeof(scanner_token) % (sizeof(scanner_token) * TOKEN_STACK);
        token->opt = 0;
        token->data = 0;
    }

    Parse(pParser,0,0,argList);
    ParseFree(pParser, free);
    free(tokenStack);
    return argList;

}

