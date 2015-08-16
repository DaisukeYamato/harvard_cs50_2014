/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// blank tile index
int blank_ind[2];

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
  int num = d*d - 1;
  // initialize
  for (int i=0; i<d; i++){
    for (int j=0; j<d; j++){
      board[i][j] = num;
      num--;
    }
  }
  blank_ind[0] = d-1;
  blank_ind[1] = d-1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
  for (int row=0; row < d; row++){
    for (int col=0; col < d; col++){
      if (board[row][col]!=0){
	printf("%2d", board[row][col]);
      }
      else{
	printf(" _");
      }
    }
    printf("\n");
  }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
  // check tile is valid number
  if (tile >= d*d || tile <= 0){
    return false;
  }
  
  // search move tile
  int row=0, col=0;
  for (int i=0; i<d; i++){
    for (int j=0; j<d; j++){
      if (tile==board[i][j]){
	row = i; col = j;
	break;
      }
    }
  }
  // check if the tile can be moved or not
  int row_diff = abs(row - blank_ind[0]);
  int col_diff = abs(col - blank_ind[1]);
  printf("row_diff: %d, col_diff: %d", row_diff, col_diff);
  if ( (row_diff==1 && col_diff==0) 
       || (row_diff==0 && col_diff==1)){
    board[blank_ind[0]][blank_ind[1]] = tile;
    board[row][col] = 0;
    blank_ind[0] = row;
    blank_ind[1] = col;
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
  int num = 1;
  for (int row=0; row<d; row++){
    for (int col=0; col<d; col++){
      if (num < d*d && board[row][col] != num){
	return false;
      }
      num++;
    }
  }
  return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
