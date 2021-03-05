#ifndef DATAPROC_H_INCLUDED
#define DATAPROC_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>
#include "mydefs.h"

int countChars(char* str, int strLen);
int appendString(char* str1, int str1Len, char* str2);
twordData populateWordData(unsigned char* str, int strLen);
tscoreData populateScoreData(char* name, int mode, int score, twordData lastWord);
int saveScore(char* name, int mode, int score, twordData lastWord);
int createWordDataFiles(int language);
int getCharInput(twordData* currentWordData, unsigned char* input, unsigned char* usedChars, int maxUsedCharsLen,
                 int* flagAjudaVogais, int* flagAjudaPrimeira, int* flagAjudaAleatorio,
                 int* pulos, int flagAcentos);
twordData getValidRandomWordData(int dificuldade, int flagAcentos);
int clearString(unsigned char* str, int strLen);

#endif // DATAPROC_H_INCLUDED
