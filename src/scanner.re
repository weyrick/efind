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

	while(SCANNER_RETCODE_IMPOSSIBLE == r) {
	
		/*!re2c
		re2c:indent:top = 2;
		re2c:yyfill:enable = 0;
		
		INTEGER = [0-9]+;
		INTEGER {
		
			char *num;
			int n;
			num = strndup(q,YYCURSOR - q);
			n = atoi(num);

			DEBUG(printf("scanner num: '%s', YYCURSOR: '%s' YYLIMIT: '%s' q: '%s'\n",num,YYCURSOR,YYLIMIT,q));
			free(num);
			q = YYCURSOR;
			// DEBUG(printf("integer, cursor: %s, number len: %ld, number: %d\n",YYCURSOR,YYCURSOR-s->start,n));
			token->data.n = n;
                        token->tokType = TOKEN_INTEGER;
			
			return 0;
		}

		"+" {
                        token->tokType = TOKEN_ADD;
			return 0;
		}

		"-" {
                        token->tokType = TOKEN_SUB;
			return 0;
		}

		"*" {
                        token->tokType = TOKEN_MUL;
			return 0;
		}

		"/" {
                        token->tokType = TOKEN_DIV;
			return 0;
		}
		
		"\000" { r = SCANNER_RETCODE_EOF; break; }

		[^] { r = SCANNER_RETCODE_ERR; break; }
		*/
	}
	return r;
}
