/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    /* linear search
   for(int i=0;i<n;i++)
       if(values[i]==value)
       return true;
    return false;
    */
    int first=0;
    int last=n-1;
    
    while(n>0){
        int middle=(last-first)/2+first;
        if(values[middle]==value)
            return true;
        else if(values[middle]>value)
            last=middle-1;
        else if(values[middle]<value)
            first=middle+1;
        n=last-first+1;
        
    }
    return false;
}

void swap(int* a, int* b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

/**
 * Sorts array of n values.
 */
 
void sort(int values[], int n) //implemented all three n^2 algorithms, just to check
{
/*    //Bubble sort. Takes first two values, switches them if first is greater than second. Does this for the whole array.
    //Repeats until it runs through the whole thing and nothing changes
    int j;
    do{
        j=0; // parameter defining if anything is swapped. If not, exit.
        for(int i=0; i<n-1;i++){
            if(values[i]>values[i+1]){ //sort from smallest to highest
                swap(values[i],values[i+1]);
                j=1;
            }
        }
    } while(j==1);
*/

/*    //Selection sort
for(int i=0; i<n-1;i++){
    int minIndex=i;
    for(int j=i+1;j<n;j++){
        if(values[j]<values[minIndex])
            minIndex=j;
    }
    if(minIndex!=i)
        swap(&values[i],&values[minIndex]);
}
*/  
    
    //Insertion sort
    
    for(int i=1;i<n;i++){
        int j=i-1;
        int thisValue=values[i];
        while(j>=0 && values[j]>thisValue){
            values[j+1]=values[j]; //shift the larger value to the right.
            j=j-1;
        }
        values[j+1]=thisValue;
    }
    return;
}