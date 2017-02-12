/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 */
 
#define _XOPEN_SOURCE 500
#define _CRT_SECURE_NO_DEPRECATE

#include "cs50.h"
#include <stdio.h>
#include <stdlib.h>
#include "Windows.h"

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
		int tile;
        printf("Tile to move: ");
		scanf("%d", &tile);
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            Sleep(1000);
        }

        // Sleep thread for animation's sake
        Sleep(300);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
	system("CLS");
    //printf("\033[2J");
    //printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    Sleep(1000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int currentNo=d*d-1;
    for(int i=0;i<d;i++){
        for(int j=0;j<d;j++){
            board[i][j]=currentNo;
            currentNo=currentNo-1;
        }
    }
    if(d%2==0){
        int temp=board[d-1][d-3];
        board[d-1][d-3]=board[d-1][d-2];
        board[d-1][d-2]=temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i=0;i<d;i++){
        for(int j=0;j<d;j++){
            if(board[i][j]>0)
                printf("|%2d",board[i][j]);
            else if(board[i][j]==0)
                printf("| __");
        }
        printf("|\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    if(tile>d*d-1 || tile<1){
        return false;
    }
    int row;
    int column;
    int rowBlank;
    int columnBlank;
    
    for(int i=0;i<d;i++){
        for(int j=0;j<d;j++){
            if(tile==board[i][j]){
                row=i;
                column=j;
            }
            if(0==board[i][j]){
                rowBlank=i;
                columnBlank=j;
            }
        }
    }
    
    if((rowBlank==row &&columnBlank==column+1)||(rowBlank==row &&columnBlank==column-1) ||
    (rowBlank==row+1 &&columnBlank==column)||(rowBlank==row-1 &&columnBlank==column)){
        board[rowBlank][columnBlank]=tile;
        board[row][column]=0;
        return true;
    }
    
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int n = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (++n != (d * d) && board[i][j] != n)
            {
                return false;
            }
        }
    }

    return true;
}