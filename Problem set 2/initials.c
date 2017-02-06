/**
* George Roberts 2017 for CS50 Harvard
*
* When given a person's name, this program will print the initials. 
* Works by assuming the first letter is a capital, then taking the letters after all whitespaces. 
* 
* Usage: User is prompted for input inside the program
*/

#include <stdio.h>
#include <string.h>

// Converts a single letter to upper case using ascii conversion
int lowerCaseToUpperCase(char a);

// Return what the ASCII character is, as if the letter were capitalised
int letterToCapASCII(char name);

int main(void){
    int maxNameLength=100;
    char rawName[maxNameLength];
    printf("Please enter a name: ");
    fgets (rawName, maxNameLength, stdin);
    int nameLength=strlen(rawName);
    printf("The initials are: ");
    printf("%c",(char)letterToCapASCII(rawName[0])); 
    
    for(int i=0; i<nameLength;i++){ 
        int ASCIICapitalLetter=letterToCapASCII(rawName[i]);
        if(ASCIICapitalLetter==32) // If whitespace
                printf("%c",(char)letterToCapASCII(rawName[i+1])); 
    }
    printf("\n");
}

int lowerCaseToUpperCase(char a)
{
    int b=(int)a;       
    if(b<=122 && b>=97) // Lowercase
        b-=32;//subtract 32 from the ASCII number to reach uppercase
    a=(char)b;
    
    return a;
}

int letterToCapASCII(char letter)   
{
    char letterCap=lowerCaseToUpperCase(letter); 
    int ASCIICapitalLetter=(int)letterCap;
    
    return ASCIICapitalLetter;
}
