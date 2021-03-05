#ifndef MYDEFS_H_INCLUDED
#define MYDEFS_H_INCLUDED

#define BOARD_HEIGHT 10
#define BOARD_WIDTH 62

#define MAX_USED_CHARS 50
#define MAX_SHOW_USED_CHARS 10

#define MIN_DIF 4
#define MAX_DIF 12
#define MAX_FALTAS 4

#define MAX_WORD_LENGTH 20
#define MAX_NAME_LENGTH 5

#define MAX_PRINTED_SCORES 15

#define MAX_NUM_TRIES 20000

typedef struct type_wordData
{
   unsigned char word[MAX_WORD_LENGTH];
   int wordLength;
   int flagBadWord;
   int flagAcentuada;
   int numLetrasRepetidas;
   int dificuldade;

} twordData;

typedef struct type_scoreData
{
   unsigned char name[MAX_NAME_LENGTH];
   int mode;
   int score;
   twordData lastWord;

} tscoreData;

enum possible_game_states {
GS_CLOSE = 0,
GS_MAIN,
GS_OPTIONS,
GS_RULES,
GS_SCORES,
GS_CHOOSE,
GS_PLAY_CASUAL,
GS_PLAY_ARCADE
} gameState;

enum possible_play_states {
PS_RETURN = 0,
PS_WIN_CASUAL,
PS_LOSE,
PS_WIN_ARCADE,
PS_END_ARCADE,
PS_LOAD,
PS_CONTINUE
} playState;

enum possible_game_modes {
MODE_CASUAL = 0,
MODE_ARCADE
} modeState;

enum possible_languages {
LAN_PTBR = 0,
LAN_ENG,
} lanState;

#endif // MYDEFS_H_INCLUDED
