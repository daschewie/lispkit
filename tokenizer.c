/*
  A Lispkit implementation.

  Copyright (c) 2011  A. Carl Douglas

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/* vim: softtabstop=2 shiftwidth=2 expandtab  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "lispkit.h"

#define MAX_TOKENS  16384

char *token_stack[MAX_TOKENS];
int cur_tok = 0;

struct Token token;

void scanner(void) {
  token.token = token_stack[cur_tok++];
  if (cur_tok >= MAX_TOKENS) {
    printf("scanner is out of tokens %d/%d)\n", cur_tok, MAX_TOKENS);
    exit(-1);
  }
}

void tokenize (FILE *fp) {
  char line[255];
  int i = 0;

  cur_tok = 0;
  for (i = 0; i < MAX_TOKENS; i++) {
    token_stack[i] = NULL;
  }
  i = 0;
  token.line = 0;
  while( fgets(line, 255, fp) ) {
    const char sep[] = " \n\r\t";
    char *tok;
    token.word = 1;
    token.line++;
    for (tok=strtok(line, sep); tok; tok=strtok(NULL,sep)) {
      token_stack[i++] = strdup(tok);
      token.word++;
      if (i >= MAX_TOKENS) {
        printf("out of token space %d/%d\n", i, MAX_TOKENS);
        exit(-1);
      }
    }
  }

  if (ferror(fp) != 0) {
    printf("error: %s\n", strerror(errno));
  }
}

void tokenizer_free(void) {
  int i = 0;
  for (i = 0; i < MAX_TOKENS; i++) {
    if (token_stack[i] != NULL) {
      free(token_stack[i]);
    }
    token_stack[i] = NULL;
  }
}


