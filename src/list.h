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

#ifndef LIST_H
#define LIST_H

// simple linked list that holds string data
typedef struct list {
    char *data;
    struct list *next;
} list;

// allocate a new list head
list *list_create(void);

// push data onto the end of the given list
// the list becomes the owner of the data, and frees
// it in list_free
void list_push_str(list *l, char *data);

// push all nodes in r onto the end of l
// l becomes the new owner, r nodes are freed (but not their data, which
// is now owned by l)
void list_push_list(list *l, list *r);

// combine all nodes into a single string
// caller becomes owner of the string
char *list_to_str(list *l);

// convert the data from all nodes into a char *[]
// terminates with a null entry
// data is shared with the array (not copied)
char **list_to_array(list *l);

// free the list nodes and optionall all data it owns
void list_free(list* l, int freeData);

#endif
