#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void printDouble(unsigned long long l) {
    double d;
    memcpy(&d, &l, 8);
    unsigned long long ExpBits = l & (1ULL << 63) - (1ULL << 52);
    int ExpRaw = ExpBits >> 52;
    int Exp = ExpRaw - 1023;
    unsigned long long Mantissa = l & ((1ULL << 52) - 1);
    int isSub = ExpRaw == 0;
    int isNorm = ExpRaw != 0 && ExpRaw != 2047;
    char *state = "";
    if (isSub)
        state = "SubNormal";
    else if (isNorm)
        state = "   Normal";

    fprintf(stdout, "S: %u E: %4d(%4d) %s\n", (unsigned int)(l >> 63), ExpRaw, Exp, state);
    fprintf(stdout, "S: 0x%016llx\nE: 0x%016llx\nM: 0x%016llx\n", l & (1ULL<<63), ExpBits, Mantissa);
    fprintf(stdout, "%f(0x%llx)\n", d, l);
}

int main() {
    printf("Type in a number, prefixed with its type:\n");
    printf("    r: raw double         d: pretty-printed double\n");
    printf("    z: integer(gets cast) b: binary\n");
    char *line;
    while (line = readline("raw data> ")) {
        char *text = line+1;
        switch (*line) {
          case 'r': {
              long long l = strtoll(text, NULL, 0);
              printDouble(l);
              break;
          }
          case 'd': {
              double d;
              sscanf(text, "%lf", &d);
              unsigned long long l;
              memcpy(&l, &d, 8);
              printDouble(l);
              break;
          }
          case 'z': {
              long long l = strtoll(text, NULL, 0);
              double d = l;
              memcpy(&l, &d, 8);
              printDouble(l);
              break;
          }
          case 'b': {
              // Why did I do this again?
              long l = 0;
              while (*text != '\n' && *text != '\0') {
                  switch(*text++) {
                    case '0':
                      l <<= 1;
                      continue;
                    case '1':
                      l = (l << 1) | 1;
                      continue;
                    default:
                      break;
                  }
                  // parse error?
                  break;
              }
              printDouble(l);
          }
        }
        free(line);
    }
}
