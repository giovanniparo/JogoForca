#include "graph.h"

int procCursor(int* cursorPos, int numOptions)
{
    char input;

    printf("\n");
    printf("[w(subir) | s(descer) | e(selecionar)]: ");
    scanf("%c", &input);
    fflush(stdin);

    switch(input)
    {
        case 'w':
            (*cursorPos) -= 1;
            break;
        case 's':
            (*cursorPos) += 1;
            break;
        case 'e':
            return 1;
            break;
        default:
            printf("\nDigite uma ação válida!\n");
            system("pause");
            break;
    }

    if(*cursorPos < 0) *cursorPos = numOptions - 1;
    *cursorPos %= numOptions;

    return 0;
}

void printWordData(twordData wordData)
{
    printf("\n");
    printf("!%s!\n", wordData.word);
    printf("Tamanho: %i\n", wordData.wordLength);
    printf("Acentuada? %s\n", (wordData.flagAcentuada == 1) ? "sim" : "não");
    printf("Número letras repetidas: %i\n", wordData.numLetrasRepetidas);
    printf("Dificuldade: %i\n", wordData.dificuldade);
    printf("\n");
}

void printScoreData(tscoreData scoreData)
{
    printf("<%.4s>| ",  scoreData.name);
    if(scoreData.score < 10) printf("000");
    else if(scoreData.score < 100) printf("00");
    else if(scoreData.score < 1000) printf("0");
    printf("%i", scoreData.score);
    printf(" | %s (dif: %i)\n", scoreData.lastWord.word, scoreData.lastWord.dificuldade);
}

void sortScores(tscoreData* casualScoreDataArray, tscoreData* arcadeScoreDataArray)
{
    char defaultNameCasual[] = "CSLL";
    char defaultNameArcade[] = "ARCD";

    unsigned char defaultWordCasual[] = "CASUAL";
    unsigned char defaultWordArcade[] = "ARCADE";

    FILE* file;
    file = fopen("data/scores.bin", "rb+");
    tscoreData tempScoreData;
    tscoreData processingScoreData;
    tscoreData testingScoreData;
    int processingPos = 0;
    int testingPos = 0;
    int counterCasual = 0;
    int counterArcade = 0;

    twordData casualDefaultWordData = populateWordData(defaultWordCasual, sizeof(defaultWordCasual)/sizeof(defaultWordCasual[0]));
    twordData arcadeDefaultWordData = populateWordData(defaultWordArcade, sizeof(defaultWordArcade)/sizeof(defaultWordArcade[0]));

    if(file == NULL)
    {
        fclose(file);

        file = fopen("data/scores.bin", "wb+");
        if(file == NULL)
        {
            printf("Erro ao tentar criar arquivo de scores\n");
        }

        for(int n = 0; n < MAX_PRINTED_SCORES; n++)
        {
            tempScoreData = populateScoreData(defaultNameCasual, MODE_CASUAL, n, casualDefaultWordData);
            fwrite(&tempScoreData, sizeof(tscoreData), 1, file);
            tempScoreData = populateScoreData(defaultNameArcade, MODE_ARCADE, 10 * n, arcadeDefaultWordData);
            fwrite(&tempScoreData, sizeof(tscoreData), 1, file);
        }
    }

    while(processingPos < 2 * MAX_PRINTED_SCORES)
    {
        testingPos = processingPos + 1;

        do
        {
            fseek(file, testingPos*sizeof(tscoreData), SEEK_SET);
            if(fread(&testingScoreData, sizeof(tscoreData), 1, file) != 1) break;
            fseek(file, processingPos*sizeof(tscoreData), SEEK_SET);
            fread(&processingScoreData, sizeof(tscoreData), 1, file);

            if(testingScoreData.score > processingScoreData.score)
            {
                fseek(file, processingPos*sizeof(tscoreData), SEEK_SET);
                fwrite(&testingScoreData, sizeof(tscoreData), 1, file);
                fseek(file, testingPos*sizeof(tscoreData), SEEK_SET);
                fwrite(&processingScoreData, sizeof(tscoreData), 1, file);
            }

        testingPos++;
        } while(1);

        fseek(file, processingPos*sizeof(tscoreData), SEEK_SET);
        fread(&processingScoreData, sizeof(tscoreData), 1, file);

        if(processingScoreData.mode == MODE_CASUAL)
        {
            casualScoreDataArray[counterCasual] = processingScoreData;
            counterCasual++;
        }
        else if(processingScoreData.mode == MODE_ARCADE)
        {
            arcadeScoreDataArray[counterArcade] = processingScoreData;
            counterArcade++;
        }

        processingPos++;
    }

    fclose(file);
}

void printFiller(char fill, int menuWidth)
{
    for(int colPos = 0; colPos < menuWidth; colPos++)
    {
        printf("%c", fill);
    }
    printf("\n");
}

void printLine(char* string, int menuWidth, int posStr, int selected)
{
    char tempString[BOARD_WIDTH] = "";

    posStr  += 3;

        for(int colPos = 0; colPos < menuWidth; colPos++)
        {
            if(colPos == 0 || colPos == menuWidth- 1) tempString[colPos] = '|';
            else if((colPos == posStr - 2 || colPos == posStr - 1) && selected) tempString[colPos] = '>';
            else if(colPos >= posStr && *(string) != '\0')
                {
                    tempString[colPos] = *(string);
                    string++;
                }
            else tempString[colPos] = ' ';
        }

    printf("%s\n", tempString);
}

void printMenu(char* menuName, int cursorPos, char* option1, char* option2, char* option3, char* option4, char* option5, char* option6)
{
    int menuWidth = BOARD_WIDTH / 2;
    int menuHeight = 12;

    int namePos = (menuWidth - countChars(menuName, menuWidth - 2))/2 - 2;
    for(int linePos = 0; linePos < menuHeight; linePos++)
    {
        if(linePos == 0 || linePos == 2 || linePos == menuHeight - 1) printFiller('=', menuWidth);
        else if(linePos == 1) printLine(menuName, menuWidth, namePos, 0);
        else if(linePos == 3 || linePos == menuHeight - 2) printLine("", menuWidth, 0, 0);
        else if(linePos == 4) (cursorPos == 0) ? printLine(option1, menuWidth, 0, 1) : printLine(option1, menuWidth, 0, 0);
        else if(linePos == 5) (cursorPos == 1) ? printLine(option2, menuWidth, 0, 1) : printLine(option2, menuWidth, 0, 0);
        else if(linePos == 6) (cursorPos == 2) ? printLine(option3, menuWidth, 0, 1) : printLine(option3, menuWidth, 0, 0);
        else if(linePos == 7) (cursorPos == 3) ? printLine(option4, menuWidth, 0, 1) : printLine(option4, menuWidth, 0, 0);
        else if(linePos == 8) (cursorPos == 4) ? printLine(option5, menuWidth, 0, 1) : printLine(option5, menuWidth, 0, 0);
        else if(linePos == 9) (cursorPos == 5) ? printLine(option6, menuWidth, 0, 1) : printLine(option6, menuWidth, 0, 0);
        else printLine("", menuWidth, 0, 0);
    }
}

void printMainMenu(int cursorPos)
{
    system("cls");
    char menuName[] = "!JOGO DA FORCA!";
    char option1[] = "Jogar";
    char option2[] = "Opções";
    char option3[] = "Regras";
    char option4[] = "Score";
    char option5[] = "Fechar";
    char option6[] = "";

    printMenu(menuName, cursorPos, option1, option2, option3, option4, option5, option6);

    printf("==========================================\n");
    printf("|               Luan Ciol                |\n");
    printf("|             Giovanni Paro              |\n");
    printf("|Jogos Digitais / Projeto Prog02 / 2020-1|\n");
    printf("==========================================\n");
}

void printOptionsMenu(int cursorPos, int dificuldade, int flagAjudas, int flagAcentos, int language)
{
    system("cls");
    char dificuldadeStr[5] = "";
    char menuName[] = "!OPÇÕES!";
    char option1[BOARD_WIDTH-2] = "Dificuldade:";
    char option2[BOARD_WIDTH-2] = "Habilitar Ajudas?";
    char option3[BOARD_WIDTH-2] = "Habilitar Acentos?";
    char option4[BOARD_WIDTH-2] = "Vocabulário:";
    char option5[] = "Voltar";
    char option6[] = "";

    itoa(dificuldade, dificuldadeStr, 10);
    appendString(option1, sizeof(option1)/sizeof(option1[0]), dificuldadeStr);
    appendString(option2, sizeof(option2)/sizeof(option2[0]), (flagAjudas) ? " sim" : " não");
    appendString(option3, sizeof(option3)/sizeof(option3[0]), (flagAcentos) ? " sim" : " não");
    appendString(option4, sizeof(option4)/sizeof(option4[0]), (language == LAN_PTBR) ? " Português" : " Inglês");

    printMenu(menuName, cursorPos, option1, option2, option3, option4, option5, option6);
}

void printRules(int cursorPos)
{
    system("cls");
    char menuName[] = "!REGRAS!";
    int menuWidth = 60;

    int namePos = (menuWidth - countChars(menuName, menuWidth - 2))/2 - 2;

    printFiller('=', menuWidth);
    printLine(menuName, menuWidth, namePos, 0);
    printFiller('=', menuWidth);
    printf("Tente descobrir a palavra chutando uma letra por vez!\n");
    printf("No seu turno tente uma letra ou use uma ajuda.\n");
    printf("Se a palavra não contiver a letra você receberá uma falta.\n");
    printf("Você tem direito a 4 faltas por palavra.\n\n");
    printf("Modos de jogo:\n");
    printf("CASUAL: Uma palavra aleatória.\n");
    printf("ARCADE: Palavras com dificuldade crescente até perder.\n\n");
    printf("Palavras maiores / acentuadas valem mais pontos.\n");
    printf("Você receberá mais pontos se errar menos vezes.\n\n");
    printf("Divirta-se e boa sorte.\n");
    printf(">>Voltar\n");
    printFiller('=', menuWidth);

}

void printScores(int cursorPos)
{
    system("cls");
    char* menuName = "!HIGH SCORES!";
    char* casualName = "!CASUAL!";
    char* arcadeName = "!ARCADE!";
    tscoreData casualScoreDataArray[MAX_PRINTED_SCORES];
    tscoreData arcadeScoreDataArray[MAX_PRINTED_SCORES];
    sortScores(casualScoreDataArray, arcadeScoreDataArray);

    int menuWidth = BOARD_WIDTH - 1;

    int namePos = (menuWidth - countChars(menuName, menuWidth - 2))/2 - 2;
    int casualNamePos = (menuWidth - countChars(casualName, menuWidth - 2))/2 - 2;
    int arcadeNamePos = (menuWidth - countChars(arcadeName, menuWidth - 2))/2 - 2;

    printFiller('=', menuWidth);
    printLine(menuName, menuWidth, namePos, 0);
    printFiller('=', menuWidth);
    printLine(casualName, menuWidth, casualNamePos, 0);
    printFiller('=', menuWidth);
    printf("\n");
    for(int n = 0; n < MAX_PRINTED_SCORES; n++)
    {
        printf("|@");
        if(n+1 < 10) printf("00");
        else printf("0");
        printf("%i|", n + 1);
        printScoreData(casualScoreDataArray[n]);
    }
    printf("\n");

    printFiller('=', menuWidth);
    printLine(arcadeName, menuWidth, arcadeNamePos, 0);
    printFiller('=', menuWidth);

    printf("\n");
    for(int n = 0; n < MAX_PRINTED_SCORES; n++)
    {
        printf("|@");
        if(n+1 < 10) printf("00");
        else printf("0");
        printf("%i|", n + 1);
        printScoreData(arcadeScoreDataArray[n]);
    }
    printf("\n");
    printFiller('=', menuWidth);

    printf(">> Voltar\n");
}

void printChooseScreen(int cursorPos)
{
    system("cls");
    char menuName[] = "Escolha o tipo de jogo:";
    char option1[] = "CASUAL";
    char option2[] = "ARCADE";
    char option3[] = "Voltar";
    char option4[] = "";
    char option5[] = "";
    char option6[] = "";

    printMenu(menuName, cursorPos, option1, option2, option3, option4, option5, option6);
    printf("=======================================================================\n");
    printf("|CASUAL: Tente acertar uma palavra aleatória da dificuldade escolhida.|\n");
    printf("|ARCADE: Tente acertar palavras com dificuldade crescente até perder. |\n");
    printf("=======================================================================\n");
}

void printWinCasualScreen(twordData currentWordData, int cursorPos, int score)
{
    system("cls");
    char menuName[] = "!VOCÊ ACERTOU!";
    char option1[] = "Salvar Score";
    char option2[] = "Jogar novamente";
    char option3[] = "Menu principal";
    char option4[] = "Fechar";
    char option5[] = "";
    char option6[] = "";

    printMenu(menuName, cursorPos, option1, option2, option3, option4, option5, option6);

    printf("\n==================================================\n");
    printf("A palavra era:\n");
    printWordData(currentWordData);
    printf("\nSCORE: %i\n", score);
    printf("==================================================\n");
}

void printLoseScreen(twordData currentWordData, int cursorPos)
{
    system("cls");
    char menuName[] = "!FIM DE JOGO!";
    char option1[] = "Jogar novamente";
    char option2[] = "Menu principal";
    char option3[] = "Fechar";
    char option4[] = "";
    char option5[] = "";
    char option6[] = "";

    printMenu(menuName, cursorPos, option1, option2, option3, option4, option5, option6);

    printf("\n==================================================\n");
    printf("A palavra era:\n");
    printWordData(currentWordData);
    printf("==================================================\n");
}

void printWinArcadeScreen(twordData currentWordData, int cursorPos, int score)
{
    system("cls");
    char menuName[] = "!VOCÊ ACERTOU!";
    char option1[] = "Próxima dificuldade";
    char option2[] = "Menu principal";
    char option3[] = "Fechar";
    char option4[] = "";
    char option5[] = "";
    char option6[] = "";

    printMenu(menuName, cursorPos, option1, option2, option3, option4, option5, option6);

    printf("\n==================================================\n");
    printf("A palavra era:\n");
    printWordData(currentWordData);
    printf("\nSCORE: %i\n", score);
    printf("==================================================\n");
}

void printEndScreen(twordData currentWordData, int cursorPos, int score)
{
    system("cls");
    char menuName[] = "!FIM DE JOGO!";
    char option1[] = "Salvar Score";
    char option2[] = "Jogar Novamente";
    char option3[] = "Menu principal";
    char option4[] = "Fechar";
    char option5[] = "";
    char option6[] = "";

    printMenu(menuName, cursorPos, option1, option2, option3, option4, option5, option6);

    printf("\n==================================================\n");
    printf("A palavra era:\n");
    printWordData(currentWordData);
    printf("\nSCORE TOTAL: %i\n", score);
    printf("==================================================\n");
}

void addSignedStringToBoard(unsigned char board[BOARD_HEIGHT][BOARD_WIDTH], char* string, int stringLen, int linePos, int colPos)
{
    for(int m = colPos; m < colPos + stringLen && m < BOARD_WIDTH; m++)
    {
        board[linePos][m] = (unsigned char)*string;
        string++;
    }
}

void addUnsignedStringToBoard(unsigned char board[BOARD_HEIGHT][BOARD_WIDTH], unsigned char* string, int stringLen, int linePos, int colPos)
{
    for(int m = colPos; m < colPos + stringLen && m < BOARD_WIDTH; m++)
    {
        board[linePos][m] = *string;
        string++;
    }
}

void addWordToBoard(unsigned char board[BOARD_HEIGHT][BOARD_WIDTH], unsigned char* word, int wordLen, unsigned char* usedChars, int maxUsedCharsLen, int linePos, int colPos)
{
    int i = 0;
    for(int m = colPos - 1; m < colPos + (wordLen * 2) + 1 && m < BOARD_WIDTH; m++)
    {
        if((m - colPos - 1) % 2 == 0)
            board[linePos][m] = '|';
        else
        {
            i = 0;
            while(i < maxUsedCharsLen)
            {
            if(*(usedChars + i) == *word)
            {
                board[linePos][m] = *word;
                break;
            }
            else board[linePos][m] = '_';
            i++;
            }
            word++;
        }
    }
}

void addUsedCharsToBoard(unsigned char board[BOARD_HEIGHT][BOARD_WIDTH], unsigned char* usedChars, int currentUsedCharsLen, int linePos, int colPos)
{
   if(currentUsedCharsLen > MAX_SHOW_USED_CHARS)
   {
       usedChars+=(currentUsedCharsLen - MAX_SHOW_USED_CHARS);
   }

   for(int m = colPos - 1; m < colPos + currentUsedCharsLen * 2 + 1 && m <= colPos - 1 + 2 * MAX_SHOW_USED_CHARS && m < BOARD_WIDTH; m++)
   {
        if((m - colPos - 1) % 2 == 0)
            board[linePos][m] = 187;
        else
        {
            board[linePos][m] = *usedChars;
            usedChars++;
        }
   }
}

void clearBoard(unsigned char board[BOARD_HEIGHT][BOARD_WIDTH])
{
    for(int linePos = 0; linePos < BOARD_HEIGHT; linePos++)
    {
        for(int colPos = 0; colPos < BOARD_WIDTH; colPos++)
        {
            if(linePos == 0 || linePos == BOARD_HEIGHT - 1) board[linePos][colPos] = '=';
            else if(colPos == 0 || colPos == BOARD_WIDTH - 1) board[linePos][colPos] = '|';
            else board[linePos][colPos] = ' ';
        }
    }
}

int countValidUnsignedStringChars(unsigned char* str, int strLen)
{
    int n = 0;

   while(n < strLen)
   {
    if (*str == '\0' || *str == '\n') break;
    str++;
    n++;
   }

    return n;
}

void printBoard(twordData wordData, int mode, int score, int faltas, int trocas, int dificuldade, unsigned char* usedChars, int maxUsedCharsLen)
{
    system("cls");

    unsigned char board[BOARD_HEIGHT][BOARD_WIDTH];
    unsigned char scoreStr[] = "SCORE PARCIAL:";
    unsigned char faltasStr[] = "FALTAS RESTANTES:";
    unsigned char tamanhoStr[] = "TAMANHO:";
    unsigned char dificuldadeStr[] = "DIFICULDADE:";
    char intScoreStr[10];
    char intFaltasStr[10];
    char intTamanhoStr[10];
    char intDificuldadeStr[10];
    int scoreStrLen = sizeof(scoreStr)/sizeof(scoreStr[0]);
    int faltasStrLen = sizeof(faltasStr)/sizeof(faltasStr[0]);
    int tamanhoStrLen = sizeof(tamanhoStr)/sizeof(tamanhoStr[0]);
    int dificuldadeStrLen = sizeof(dificuldadeStr)/sizeof(dificuldadeStr[0]);

    itoa(score, intScoreStr, 10);
    itoa(faltas, intFaltasStr, 10);
    itoa(wordData.wordLength, intTamanhoStr, 10);
    itoa(wordData.dificuldade, intDificuldadeStr, 10);

    int intScoreStrLen = countChars(intScoreStr,10);
    int intFaltasStrLen = countChars(intFaltasStr,10);
    int intTamanhoStrLen = countChars(intTamanhoStr,10);
    int intDificuldadeStrLen = countChars(intDificuldadeStr,10);

    clearBoard(board);

    if(mode == MODE_ARCADE)
    {
        addUnsignedStringToBoard(board, scoreStr, scoreStrLen, 1, 1);
        addSignedStringToBoard(board, intScoreStr, intScoreStrLen, 1, scoreStrLen);
    }

    addUnsignedStringToBoard(board, dificuldadeStr, dificuldadeStrLen, 1, (BOARD_WIDTH - dificuldadeStrLen - intDificuldadeStrLen) / 2);
    addSignedStringToBoard(board, intDificuldadeStr, intDificuldadeStrLen, 1, (BOARD_WIDTH + dificuldadeStrLen - intDificuldadeStrLen) / 2);

    addUnsignedStringToBoard(board, tamanhoStr, tamanhoStrLen, 1, BOARD_WIDTH - tamanhoStrLen - intTamanhoStrLen - 2);
    addSignedStringToBoard(board, intTamanhoStr, intTamanhoStrLen, 1, BOARD_WIDTH - intTamanhoStrLen - 2);

    addUnsignedStringToBoard(board, faltasStr, faltasStrLen, BOARD_HEIGHT - 2, BOARD_WIDTH - faltasStrLen - intFaltasStrLen - 2);
    addSignedStringToBoard(board, intFaltasStr, intFaltasStrLen, BOARD_HEIGHT - 2, BOARD_WIDTH - intFaltasStrLen - 2);

    addUsedCharsToBoard(board, usedChars, countValidUnsignedStringChars(usedChars,maxUsedCharsLen), BOARD_HEIGHT - 2, 2);

    addWordToBoard(board, wordData.word, wordData.wordLength, usedChars, maxUsedCharsLen, (BOARD_HEIGHT - 2)/ 2, (BOARD_WIDTH - 2) / 2 - ((2*wordData.wordLength + 1) / 2));

    for(int linePos = 0; linePos < BOARD_HEIGHT; linePos++)
    {
        for(int colPos = 0; colPos < BOARD_WIDTH; colPos++)
        {
            printf("%c", board[linePos][colPos]);
        }
        printf("\n");
    }
}
