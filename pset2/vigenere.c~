#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h> /*isupper, islower, isalpha functions*/

int main(int argc, string argv[]){
  // check whether correct inputs
  if (argc != 2) {
    printf("Usage: ./caesar number \n");
    return 1;
  }
  int k = atoi(argv[1]);
  
  //string p = "Be sure to drink your Ovaltine!\n";
  string p = GetString();
  
  // encrypted text
  for (int i=0, n=strlen(p); i<n; i++){ 
    if (!isalpha(p[i])){
      printf("%c", p[i]);
    }
    else{
      int offset='A';
      if (islower(p[i])){
	offset='a';
      }
      int letter = p[i];
      
      printf("%c", offset + ((letter - offset + k) % 26) );
    }
  }
  printf("\n");
  return 0;
}
