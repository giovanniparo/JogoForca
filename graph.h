#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "dataproc.h"
#include "mydefs.h"

void printWordData(twordData wordData);
void printScoreData(tscoreData scoreData);
void sortScores(tscoreData* casualScoreDataArray, tscoreData* arcadeScoreDataArray);
void printBoard(twordData wordData, int mode, int score, int faltas, int trocas, int dificuldade, unsigned char* usedChars, int usedCharsLen);
void printRules(int cursorPos);
void printMainMenu(int cursorPos);
void printOptionsMenu(int cursorPos, int dificuldade, int flagAjudas, int flagAcentos, int language);
void printChooseScreen(int cursorPos);
void printScores(int cursorPos);
void printWinCasualScreen(twordData currentWordData, int cursorPos, int score);
void printLoseScreen(twordData currentWordData, int cursorPos);
void printWinArcadeScreen(twordData currentWordData, int cursorPos, int score);
void printEndScreen(twordData currentWordData, int cursorPos, int score);
int procCursor(int* cursorPos, int numOptions);

#endif // GRAPH_H_INCLUDED
