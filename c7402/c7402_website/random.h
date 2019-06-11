#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define MAXSUB	28

//Function Prototypes
void encode (char *, char *);
void decode (char *, char *);
int LocateIndex (char *, char );

char sub[MAXSUB] = "qazwsxedcrfvtgbyhnujm ikolp";
char alphabet[MAXSUB] = "abcdefghijklmnopqrstuvwxyz";
