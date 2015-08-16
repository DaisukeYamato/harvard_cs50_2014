/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

int n_words = 0;

/*
 */
int hash(const char* c){
  // if ( isupper(c[0]) ) return -1;
  if ( c[0] - '\'' == 0) return 26;
  if ( isupper(c[0]) ) {
    return c[0] - 'A';
  }
  if ( c[0] - 'a'>=0 && c[0] - 'a'<= 25) 
    return (c[0] - 'a');
  // not alphabet
  return -2;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
  dict* cursor = my_dict;
  for (int i=0, n=strlen(word); i<=n; i++){
    if (i==n){ return cursor->is_word; }
    else {
      if ( cursor->next[ hash(&word[i]) ] == NULL )
	return false;
      else {
	cursor = cursor -> next[ hash(&word[i]) ];
      }
    }
  }
  return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
  // check file exists
  FILE* inptr = fopen(dictionary,"r");
  if (inptr == NULL){
    printf("file %s cannot be opened.", dictionary);
    return false;
  }
  
  // initialize my_dict
  my_dict = malloc(sizeof(dict));
  my_dict->is_word = false;
  for (int i=0; i<27; i++) {
    my_dict->next[i] = NULL;
  }
  dict* cursor = my_dict;
  char word[LENGTH+1];
  while ( fscanf(inptr, "%s", word) != EOF ) {
    
    // init cursor
    cursor = my_dict;
    //printf("my_dict: %p, cursor: %p", my_dict, cursor);

    for (int i=0, n=strlen(word); i<=n; i++) {
      if (i==n) { 
	cursor->is_word = true; 
      } 
      else {
	//if ( cursor->next[ hash(&word[i]) ]==NULL ){
	int index = hash(&word[i]);
	if ( cursor->next[index]==NULL){
	  //dict* new_ptr = (dict*) malloc(sizeof(dict));
	  cursor->next[index] = malloc(sizeof(dict));
	  //dict* new_ptr = malloc(sizeof(dict));
	  cursor->next[index]->is_word = false;
	  //new_ptr->is_word = false;
	  for (int j=0; j<27; j++) {
	    //new_ptr->next[j] =NULL;
	    cursor->next[index]->next[j] = NULL;
	  }
	  //cursor->next[ hash(&word[i]) ] = new_ptr;
	}
	cursor = cursor->next[ hash(&word[i]) ];
      }
    }
    n_words++;
  }
  
  return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return n_words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
  //int depth = 0;
  //dict_free(my_dict, &depth);
  dict_free(my_dict);
  return true;
}

void dict_free(dict* my_dictionary){
  //if (my_dictionary==NULL) { return true; }
  if (my_dictionary==NULL) { return; }
  //printf("OK 0.\n");
  for (int i=0; i<27; i++){
    if (my_dictionary->next[i] != NULL){
      dict_free(my_dictionary->next[i]);
    }
  }
  
  free(my_dictionary);
}

