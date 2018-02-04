//!/usr/bin/gcc %1 -o haiku
#include <assert.h>
#include "haiku.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SYLLABLES 7

int main() {

  FILE *dictHandler = fopen("./dict.txt","r");
  assert(dictHandler);

  // Get the length of the dictionary
  fseek(dictHandler, 0L, SEEK_END);
  long dictLenBytes = ftell(dictHandler);
  fseek(dictHandler, 0L, SEEK_SET);

  // Create base wordlist for storing all the words
  WordList* wordList = calloc(1, sizeof(WordList));
  wordList->size = 0;
  char line[256];
  // Read the contents of the dictionary
  while(fgets(line, dictLenBytes, dictHandler)) {
    // remove the newline from the end of the word
    char* word = (char*)calloc(strlen(line)-1, sizeof(char));
    strcpy(word, line);
    word[strlen(word)-1]='\0';

    // add the word to our dictionary
    wordlist_addWord(wordList, word);
  }
  fclose(dictHandler);

  // create syllable lists
  WordList** sylLists = (WordList**)calloc(MAX_SYLLABLES, sizeof(WordList));

  // populate empty syllable lists
  for(int i = 0; i < MAX_SYLLABLES; i++) {
    sylLists[i] = (WordList*)calloc(1, sizeof(WordList));
    sylLists[i]->size = 0;
  }

  // add words to syllable lists
  for(WordItem* item = wordList->first; item != NULL; item=item->next) {
    // count the syllables in each word, remove asterisks
    tokenizeWord(item);

    // place word in specified list
    if(item->syllables-1 < MAX_SYLLABLES)
      wordlist_addWord(sylLists[item->syllables-1], item->word);
  }

  // output word counts for each syllable list
  for(int i = 0; i < MAX_SYLLABLES; i++) {
    // convert list to an array for easier randomization
    sylLists[i]->arr = (WordItem**)calloc(sylLists[i]->size, sizeof(WordItem*));
    wordlist_toArray(sylLists[i]);
  }


  // set up random
  srand(time(NULL));

  // empty buffer to store the haiku
  char buff[1024];
  strcpy(buff, "");
  haikuLine(buff, sylLists, 5);
  strcat(buff, "\n");
  haikuLine(buff, sylLists, 7);
  strcat(buff, "\n");
  haikuLine(buff, sylLists, 5);
  strcat(buff, "\n");
  printf("%s",buff);
  return 0;
}

// adds a word to a word list
void wordlist_addWord(WordList* wordList, char* word) {
  // create a new word item
  WordItem* wordItem = (WordItem*)calloc(1, sizeof(WordItem));
  wordItem->word = word;
  // add the word to the beginning and end
  // if there are no words
  if(wordList->first == NULL)
    wordList->first = wordItem;
  if(wordList->last == NULL)
    wordList->last = wordItem;
  else { // otherwise, append it to the end
    wordList->last->next = wordItem;
    wordList->last = wordItem;
  }
  wordList->size ++;
}

// fills the array attribute of the wordList structure
void wordlist_toArray(WordList* wordList) {
  int i = 0;
  WordItem* item = wordList->first;
  while(item != NULL) {
    wordList->arr[i++] = item;
    item = item->next;
  }
}

// tokenizes a single word. Removes asterisks and adds a syllable count relative to the number of asterisks
void tokenizeWord(WordItem* item) {
  int syls = 0;
  char *readPtr = item->word, *writePtr = item->word;
  // remove all asterisks from the string
  while(*readPtr) {
    syls += (*readPtr == '*');
    *writePtr = *readPtr++;
    writePtr += (*writePtr != '*');
  }
  // end the string where the new end should be
  *writePtr = '\0';

  // syllables count = number of asterisks + 1
  syls ++;
  item->syllables = syls;
}

// gets a single word from the array
void randWord(char* buff, WordList* sylList) {
  int random = rand() % sylList->size;
  strcpy(buff, sylList->arr[random]->word);
}

// creates a line of random words given a number of syllables
void haikuLine(char* buff, WordList** sylLists, int sylNum) {
  while(sylNum > 0) {
    int syl = rand() % (sylNum > MAX_SYLLABLES ? MAX_SYLLABLES : sylNum);
    char word[32];
    randWord(word, sylLists[syl]);
    strcat(buff, word);
    strcat(buff, " ");
    sylNum -= syl + 1;
  }
}
