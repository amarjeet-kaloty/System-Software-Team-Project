#include <stdlib.h>
#include <stdio.h>
#include "LA.h"
#include "Parser.h"
#include "VM.h"

int main(int argc, char *argv[])
{
    int i;
    int lFlag=0;
    int pFlag=0;
    int tFlag=0;

    for (i=0; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") ==0)
            lFlag=1;

        if (strcmp(argv[i], "-a") ==0)
            pFlag=1;

        if (strcmp(argv[i], "-v") ==0)
            tFlag=1;
    }
    LA(lFlag);
    Parser(pFlag);
    VM(tFlag);
    return 0;
}
