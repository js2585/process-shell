#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

char* trim(char* s);
void parse_line(char* line, char** args);