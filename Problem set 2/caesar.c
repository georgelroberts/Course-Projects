/**
* George Roberts 2017 for CS50 Harvard
*
* Implements a caesar (shift) cipher (https://en.wikipedia.org/wiki/Caesar_cipher)
* 
* Usage: ./caesar INT will prompt a user for a message to encrypt with INT
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    if(argc!=2 || atoi(argv[1])<=0 || !isdigit(*argv[1]))
    {
        printf("Correct useage is ./caesar INT\n");
        return 1;
    }
    
    //ensure the number shifts around the alphabet size
    int rotation=atoi(argv[1])%26; 
    
    int maxMsgLength=256;
    char msg[maxMsgLength];
    printf("Please enter a message to be encrypted: ");
    fgets (msg, maxMsgLength, stdin);
    int msgLen=strlen(msg);
    
    for(int i=0;i<msgLen;i++)
    {
        int AsciiLetter=(int)msg[i];
        //if a space, don't rotate
        if(AsciiLetter==32) 
            printf(" ");
        //if a capital letter
        else if(AsciiLetter >=65 && AsciiLetter <=90) 
            printf("%c",(char)((AsciiLetter-65+rotation)%26+65));
        //if not a capital letter
        else if(AsciiLetter >=97 && AsciiLetter <=122) 
            printf("%c",(char)((AsciiLetter-97+rotation)%26+97));
        //if not a letter at all
        else
            printf("%c",msg[i]);
    }
    return 0;
}

