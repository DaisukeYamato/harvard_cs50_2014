#include <cs50.h>
#include <stdio.h>

int main(void){
    /* variables */
    int height;
    
    /* user input for height */
    do {
        printf("Height: ");
        height = GetInt();
    } while (height < 0 || height > 23);
    
    /* print the blocks */
    for (int i=0; i<height; i++) {
        for (int j=0; j<height-(i+1); j++) {
            printf(" ");
        }
        for (int j=0; j<i+2; j++) {
            printf("#");
        }
        printf("\n");
    }
}
