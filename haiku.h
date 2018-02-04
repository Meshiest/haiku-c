#ifndef HAIKU_H
#define HAIKU_H

typedef struct wordItem WordItem;

// Individual words, also tracks syllables
typedef struct wordItem {
  WordItem* next;
  char* word;
  int syllables;
} WordItem;

// Linked list for storing words
typedef struct {
  WordItem* first;
  WordItem* last;
  WordItem** arr;
  int size;
} WordList;

// Functions have docs in the c file
void wordlist_addWord(WordList*, char*);
void wordlist_toArray(WordList*);
void tokenizeWord(WordItem*);
void randWord(char*, WordList*);
void haikuLine(char*, WordList**, int);

#endif // HAIKU_H
