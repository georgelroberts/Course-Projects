/**
* George Roberts 2017 for CS50 Harvard
*
* Implements a vigenere cipher (https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher)
* 
* Usage: ./caesar KEY will prompt a user for a message to encrypt with KEY
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Convert ASCII character to lowercase
char upperToLower(char a);

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("Try again, using the correct input\n");
        return 1;
    }
    else{ //check if ANY character is alphetical
        int keyLen=strlen(argv[1]);
        for(int l=0;l<keyLen;l++){
            if(!isalpha(argv[1][l])){
                printf("Try again, using the correct input\n");
                return 1;
            }
        }
    }
    
    int maxMsgLength=256;
    char msg[maxMsgLength];
    printf("Please enter a message to be encrypted: ");
    fgets (msg, maxMsgLength, stdin);
    int msgLen=strlen(msg);
    
    char* key=argv[1];
    int keyLen=strlen(key); 
    int keyRotation[keyLen]; 
    int keyPosition=0;
    int rotator=0;
    int lowerKey[keyLen]; 
    
    // Convert key into a rotation about the alphabet
    for(int m=0;m<keyLen;m++){
        lowerKey[m]=upperToLower(key[m]);
        keyRotation[m]=(int)lowerKey[m]-97;
    }
    
    for(int i=0;i<msgLen;i++)
    {
        keyPosition=rotator%keyLen; 
        int AsciiLetter=(int)msg[i];
        if(AsciiLetter >=65 && AsciiLetter <=90) //if capital
        {
            printf("%c",(char)((AsciiLetter-65+keyRotation[keyPosition])%26+65));
            rotator+=1;
        }
        else if(AsciiLetter >=97 && AsciiLetter <=122) //if lowercase
        {
            printf("%c",(char)((AsciiLetter-97+keyRotation[keyPosition])%26+97));
            rotator+=1;
        }
        else
        {
            printf("%c",msg[i]); //don't add 1 to k if it isn't an alphabetic letter
        }
    }
    return 0;
}
char upperToLower(char a){
    int b=(int)a;
    if(b<=90 && b>=65) // Uppercase
        b+=32;
    a=(char)b;
    
    return a;
}