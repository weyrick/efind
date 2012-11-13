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
#include <stdlib.h>
#include <string.h>
#define _IN_SCANNER
#include "scanner.h"
#if 0
#define DEBUG(stmt) stmt
#else
#define DEBUG(stmt)
#endif

int scan(scanner_state *s, scanner_token *token) {

	int r=SCANNER_RETCODE_IMPOSSIBLE;
    char *q=s->start; //keep initial start

#define YYCTYPE char
#define YYCURSOR (s->start)
#define YYLIMIT (s->end)
#define YYMARKER (s->ptr)
#define YYDEBUG(s, c) printf("state: %d char: %c\n", s, c)

	while(SCANNER_RETCODE_IMPOSSIBLE == r) {
	
		/*!re2c
		re2c:indent:top = 2;
		re2c:yyfill:enable = 0;

                SPACE = [\r\n\t ]+;
                WORD  = [-a-zA-Z0-9_*?./]+;
                INT   = [0-9]+;
                ANY   = [^];

                'or' {
                    token->tokType = TOKEN_OR;
                    return 0;
                }

                'and' {
                    token->tokType = TOKEN_AND;
                    return 0;
                }

                'not' {
                    token->tokType = TOKEN_NOT;
                    return 0;
                }

                'size' {
                    token->tokType = TOKEN_SIZE;
                    return 0;
                }

                'uid' {
                    token->tokType = TOKEN_UID;
                    return 0;
                }

                'gid' {
                    token->tokType = TOKEN_GID;
                    return 0;
                }

                'byte''s'* {
                    token->tokType = TOKEN_SIZEQUAL;
                    token->opt = 'c';
                    return 0;
                }

                'kilobyte''s'*|'kb' {
                    token->tokType = TOKEN_SIZEQUAL;
                    token->opt = 'k';
                    return 0;
                }

                'megabyte''s'*|'mb' {
                    token->tokType = TOKEN_SIZEQUAL;
                    token->opt = 'M';
                    return 0;
                }

                'gigabyte''s'*|'gb' {
                    token->tokType = TOKEN_SIZEQUAL;
                    token->opt = 'G';
                    return 0;
                }

                'owned by'|'owner' {
                    token->tokType = TOKEN_OWNEDBY;
                    return 0;
                }

                'grouped by'|'group' {
                    token->tokType = TOKEN_GROUPEDBY;
                    return 0;
                }

                'name''d'* {
                    token->tokType = TOKEN_NAMED;
                    token->opt = 0; // name
                    return 0;
                }

                'iname''d'* {
                    token->tokType = TOKEN_NAMED;
                    token->opt = 1; // iname
                    return 0;
                }

                'wholename''d'* {
                    token->tokType = TOKEN_NAMED;
                    token->opt = 2; // wholename
                    return 0;
                }

                'iwholename''d'* {
                    token->tokType = TOKEN_NAMED;
                    token->opt = 3; // iwholename
                    return 0;
                }

                'directory' {
                    token->tokType = TOKEN_FTYPE;
                    token->opt = 0; // directory
                    return 0;
                }

                'pipe' {
                    token->tokType = TOKEN_FTYPE;
                    token->opt = 1; // pipe
                    return 0;
                }

                'file' {
                    token->tokType = TOKEN_FTYPE;
                    token->opt = 2; // file
                    return 0;
                }

                'link' {
                    token->tokType = TOKEN_FTYPE;
                    token->opt = 3; // link
                    return 0;
                }

                'socket' {
                    token->tokType = TOKEN_FTYPE;
                    token->opt = 4; // socket
                    return 0;
                }

                'executable' {
                    token->tokType = TOKEN_UNARY;
                    token->opt = 0; // executable
                    return 0;
                }

                'empty' {
                    token->tokType = TOKEN_UNARY;
                    token->opt = 1; // empty
                    return 0;
                }

                'no'SPACE*'group' {
                    token->tokType = TOKEN_UNARY;
                    token->opt = 2; // nogroup
                    return 0;
                }

                'no'SPACE*'user' {
                    token->tokType = TOKEN_UNARY;
                    token->opt = 3; // nouser
                    return 0;
                }

                'readable' {
                    token->tokType = TOKEN_UNARY;
                    token->opt = 4; // readable
                    return 0;
                }

                'writable' {
                    token->tokType = TOKEN_UNARY;
                    token->opt = 5; // writable
                    return 0;
                }

                'with'|'having'|'has'|'are'|'is'|'a'|'any' {
                    token->tokType = TOKEN_CONNECTOR;
                    return 0;
                }

                SPACE {
                    token->tokType = TOKEN_WS;
                    return 0;
                }

                INT {
                    token->tokType = TOKEN_INT;
                    token->data = strndup(q, YYCURSOR - q);
                    return 0;
                }

                WORD {
                    token->tokType = TOKEN_WORD;
                    token->data = strndup(q, YYCURSOR - q);
                    return 0;
                }

                "\000" { r = SCANNER_RETCODE_EOF; break; }

                ANY { r = SCANNER_RETCODE_ERR; break; }

		*/

	}
	return r;
}
