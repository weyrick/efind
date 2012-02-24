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

#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define COMBINE_BUF_SIZE 512

list *list_create(void) {
    list *head = (list*)malloc(sizeof(list));
    head->data = 0;
    head->next = 0;
    return head;
}

void list_push(list *l, char *data) {
    // find the end
    list *end = l;
    while (end) {
        if (end->next)
            end = end->next;
        else
            break;
    }
    // special case: if l is empty head,
    // we use it rather than creating a new one
    list *node;
    if (end == l && !end->data) {
        node = l;
    }
    else {
        node = malloc(sizeof(list));
        end->next = node;
    }
    // note we *do not* copy, we take ownership!
    node->data = data;
}

char *list_to_string(list *l) {
    char *buf = malloc(COMBINE_BUF_SIZE);
    char *p = buf;
    int len = 0, total = 0;
    memset(buf, 0, COMBINE_BUF_SIZE);
    while (l) {
        assert(l->data);
        len = strlen(l->data)+1; // +1 for space, +1 for null
        if (total+len >= COMBINE_BUF_SIZE)
            break;
        strncpy(p, l->data, len-1);
        p[len-1] = ' ';
        total += len;
        p += len;
        l = l->next;
    }
    // kill the final space
    buf[total-1] = 0;
    return buf;
}

char **list_to_array(list *l) {
    int size = 0, i = 0;
    while (l) {
        size++;
        l = l->next;
    }
    if (!size)
        return NULL;
    char **result = malloc(size+1);
    while (l) {
        assert(l->data);
        result[i++] = l->data;
        l = l->next;
    }
    result[i] = NULL;
    return result;
}

void list_free(list* l) {
    list *n = l;
    list *t = 0;
    while (n) {
        t = n->next;
        if (n->data)
            free(n->data);
        free(n);
        n = t;
    }
}

