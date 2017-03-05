/**
 * Implements a dictionary's functionality.
 */

#include "dictionary.h"

//this is a linked list. The first is a value: word, the next is a pointer to the next list. 
typedef struct node{
    char word[LENGTH+1];
    struct node *next;
} node;

//the actual hashtable
 node *hashTable[HASHTABLESIZE]; 
 
 int noWords=0;  //Define as a global variable

// Ref: https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
int hashValue(const char *word)
{
    unsigned int hash = 0;
    int n=strlen(word);
    for (int i=0; i<n; i++) {
        hash = (hash << 2) ^ word[i];
    }
    return hash % HASHTABLESIZE;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int wordSize=strlen(word);
    char* lowerCaseWord=malloc(sizeof(char)*wordSize+1);
    for(int i = 0; i<wordSize; i++){
        lowerCaseWord[i] = tolower(word[i]);
    }
    lowerCaseWord[wordSize]='\0';
    
    node* wordToCheckList=hashTable[hashValue(lowerCaseWord)];
    while(wordToCheckList!=NULL){
        if(_stricmp(wordToCheckList->word,word)==0){
            free(lowerCaseWord);
            return true;
        }
        else
            wordToCheckList=wordToCheckList->next;
    }
    free(lowerCaseWord);
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary,"r");
    if(dict==NULL){
        printf("Error opening dictionary");
        return false;
    }
    
    int currentHashPosition=0;
    char dictWord[LENGTH+1];
    // use code from speller to collect a word
    while(fscanf(dict,"%s",dictWord)!=EOF)
    {
        noWords+=1;
        //put the word into the first value of a node. 
        node *tempNode = malloc(sizeof(node));
        if(tempNode==NULL){
            unload();
            return false;
        }
        strcpy(tempNode->word,dictWord);
        
        currentHashPosition=hashValue(dictWord); //calculate the hash value (which bucket) based on the above function
        
        if(hashTable[currentHashPosition]==NULL){
            tempNode->next=NULL;
            hashTable[currentHashPosition]=tempNode;
        }
        else{
            tempNode->next=hashTable[currentHashPosition];
            hashTable[currentHashPosition]=tempNode;
        }
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return noWords;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for(int i=1;i<HASHTABLESIZE;i++){
        node *cursor=hashTable[i];
        while(cursor!=NULL){
            node *temp=cursor;
            cursor=cursor->next;
            free(temp);
        }
    }
    return true;
}
