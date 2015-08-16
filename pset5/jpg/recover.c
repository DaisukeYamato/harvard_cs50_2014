/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char* argv[]) {
  
  char* infile = "./card.raw";
  FILE* inptr = fopen(infile, "r");
  FILE* outptr;
  char outfile[10];
  if (inptr == NULL){
    printf("Could not open %s.\n", infile);
    return 1;
  }
  
  BYTE bf[512];
  int file_num = -1;
  bool change = false;
  
  while (fread(&bf, sizeof(BYTE), 512, inptr) != 0){
    if ( (bf[0]==0xff && bf[1]==0xd8 && bf[2]==0xff && bf[3]==0xe0) 
	 || (bf[0]==0xff && bf[1]==0xd8 && bf[2]==0xff && bf[3]==0xe1)){
      change = true;
      file_num++;
    }
    else {
      change = false;
    }
    
    // file open
    if ( change ){
      if (file_num > 0) { fclose(outptr); }
      sprintf(outfile, "%03d.jpg", file_num);
      outptr = fopen(outfile, "w");
      if (outptr==NULL){
	fclose(inptr);
	fprintf(stderr, "Could not create %s.\n", outfile);
	return 2;
      }
    }
    
    // writing body
    if ( file_num >= 0 ) {
      fwrite(&bf, sizeof(BYTE), 512, outptr);
    }
  }
  
  fclose(inptr);
  fclose(outptr);
  
  // OK. finish.
  return 0;
}
