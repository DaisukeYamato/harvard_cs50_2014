#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h> /*isupper, islower, isalpha functions*/

int main(int argc, string argv[]){
  // check whether correct inputs
  if (argc != 2) {
    printf("Usage: ./vigenere keyword \n");
    return 1;
  }
  for (int i=0,n=strlen(argv[1]); i<n; i++){
    if (!isalpha(argv[1][i])){
      printf("keyword must be letters.\n");
      return 1;
    }
  }
  string keyword = argv[1];
  int key_index = 0;
  int len_key   = strlen(keyword);
  
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
      int key_letter = keyword[key_index];
      int k = key_letter - 'a';
      if (isupper(key_letter)){
	k = key_letter - 'A';
      }

      printf("%c", offset + ((letter - offset + k) % 26) );
      key_index++;
      key_index %= len_key;
    }
    
  }
  printf("\n");
  return 0;
}
