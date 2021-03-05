#include "main.h"

void getSaveName(char* name)
{
    do
    {
        printf("Digite as suas iniciais:");
        fgets(name, MAX_NAME_LENGTH, stdin);
        fflush(stdin);
    } while(countChars(name, MAX_NAME_LENGTH) == 0);
}

int updateFaltas(twordData currentWordData, int* faltas, unsigned char input)
{
    if((input >= '0' && input <= '9') || input == '!') return 0;

    for(int n = 0; n < currentWordData.wordLength; n++)
    {
       if(currentWordData.word[n] == input) return 0;
    }

    *faltas += 1;
    return 1;
}

int updateScore(twordData currentWordData, int faltas, int maxFaltas, int* score)
{
    int expFactor = 1;

    for(int n = 0; n < 3; n++)
    {
        expFactor *= (maxFaltas - faltas);
    }

    *score += (currentWordData.wordLength / 2) + currentWordData.dificuldade + expFactor;

    return 1;
}

int checkIfWin(twordData currentWordData, unsigned char* usedChars, int maxUsedCharsLen)
{
    int m = 0;

    for(int n = 0; n < currentWordData.wordLength; n++)
    {
        m = 0;
        while(m < maxUsedCharsLen)
       {
       if(currentWordData.word[n] == *(usedChars + m)) break;
       m++;
       }
       if(m == maxUsedCharsLen) return 0;
    }

    return 1;
}

void procPlay(int mode, int dificuldadeModoCasual, int flagAjudas, int flagAcentos)
{
    int cursorPos = 0;
    int flagAjudaVogais = (flagAjudas) ? 1 : 0;
    int flagAjudaPrimeira  = (flagAjudas) ? 1 : 0;
    int flagAjudaAleatorio  = (flagAjudas) ? 1 : 0;
    int pulos = 3;
    int maxFaltas = 4;
    int faltas = 0;
    int score = 0;
    int dificuldade = 3;

    if(mode == MODE_CASUAL) dificuldade = dificuldadeModoCasual;
    else if(mode == MODE_ARCADE) dificuldade = MIN_DIF;

    twordData currentWordData;
    twordData savedWordData;

    unsigned char input;
    unsigned char usedChars[MAX_USED_CHARS] = "";
    char name[MAX_NAME_LENGTH] = "";
    playState = PS_LOAD;

    do
    {
        switch(playState)
        {
            case PS_LOAD:
                faltas = 0;
                clearString(usedChars, MAX_USED_CHARS);
                currentWordData = getValidRandomWordData(dificuldade, flagAcentos);
                printWordData(currentWordData);
                system("pause");
                playState = PS_CONTINUE;
                break;
            case PS_CONTINUE:
                printBoard(currentWordData, mode, score, maxFaltas - faltas, pulos, dificuldade, usedChars, MAX_USED_CHARS);
                getCharInput(&currentWordData, &input, usedChars, MAX_USED_CHARS,
                                &flagAjudaVogais, &flagAjudaAleatorio, &flagAjudaPrimeira,
                                &pulos, flagAcentos);

                if(checkIfWin(currentWordData, usedChars, MAX_USED_CHARS))
                {
                    updateScore(currentWordData, faltas, maxFaltas, &score);
                    if(mode == MODE_CASUAL) playState = PS_WIN_CASUAL;
                    else if(mode == MODE_ARCADE && dificuldade == MAX_DIF) playState = PS_END_ARCADE;
                    else if(mode == MODE_ARCADE)
                    {
                        savedWordData = currentWordData;
                        dificuldade++;
                        playState = PS_WIN_ARCADE;
                    }
                }

                updateFaltas(currentWordData, &faltas, input);

                if(faltas == maxFaltas && MODE_CASUAL) playState = PS_LOSE;
                else if(faltas == maxFaltas && MODE_ARCADE && dificuldade == MIN_DIF) playState = PS_LOSE;
                else if(faltas == maxFaltas && MODE_ARCADE && dificuldade != MIN_DIF) playState = PS_END_ARCADE;
                break;
            case PS_WIN_CASUAL:
                printWinCasualScreen(currentWordData, cursorPos, score);
                if(procCursor(&cursorPos, 4))
                {
                    switch(cursorPos)
                    {
                        case 0:
                            system("cls");
                            getSaveName(name);
                            saveScore(name, mode, score, currentWordData);
                            playState = PS_RETURN;
                            gameState = GS_SCORES;
                            cursorPos = 0;
                            break;
                        case 1:
                            playState = PS_RETURN;
                            gameState = GS_CHOOSE;
                            cursorPos = 0;
                            break;
                        case 2:
                            playState = PS_RETURN;
                            gameState = GS_MAIN;
                            cursorPos = 0;
                            break;
                        case 3:
                            playState = PS_RETURN;
                            gameState = GS_CLOSE;
                            cursorPos = 0;
                            break;
                        default:
                            printf("Opção Inválida!\n");
                            cursorPos = 0;
                            break;
                    }
                }
                break;
            case PS_LOSE:
                printLoseScreen(currentWordData, cursorPos);
                if(procCursor(&cursorPos, 3))
                {
                    switch(cursorPos)
                    {
                        case 0:
                            playState = PS_RETURN;
                            gameState = GS_CHOOSE;
                            cursorPos = 0;
                            break;
                        case 1:
                            playState = PS_RETURN;
                            gameState = GS_MAIN;
                            cursorPos = 0;
                            break;
                        case 2:
                            playState = PS_RETURN;
                            gameState = GS_CLOSE;
                            cursorPos = 0;
                            break;
                        default:
                            printf("Opção Inválida!\n");
                            cursorPos = 0;
                            break;
                    }
                }
                break;
            case PS_WIN_ARCADE:
                printWinArcadeScreen(currentWordData, cursorPos, score);
                if(procCursor(&cursorPos, 3))
                {
                    switch(cursorPos)
                    {
                        case 0:
                            playState = PS_LOAD;
                            cursorPos = 0;
                            break;
                        case 1:
                            playState = PS_RETURN;
                            gameState = GS_MAIN;
                            cursorPos = 0;
                            break;
                        case 2:
                            playState = PS_RETURN;
                            gameState = GS_CLOSE;
                            cursorPos = 0;
                            break;
                        default:
                            printf("Opção Inválida!\n");
                            cursorPos = 0;
                            break;
                    }
                }
                break;
            case PS_END_ARCADE:
                printEndScreen(currentWordData, cursorPos, score);
                if(procCursor(&cursorPos, 4))
                {
                    switch(cursorPos)
                    {
                        case 0:
                            system("cls");
                            getSaveName(name);
                            saveScore(name, mode, score, savedWordData);
                            playState = PS_RETURN;
                            gameState = GS_SCORES;
                            cursorPos = 0;
                            break;
                        case 1:
                            playState = PS_RETURN;
                            gameState = GS_CHOOSE;
                            cursorPos = 0;
                            break;
                        case 2:
                            playState = PS_RETURN;
                            gameState = GS_MAIN;
                            cursorPos = 0;
                            break;
                        case 3:
                            playState = PS_RETURN;
                            gameState = GS_CLOSE;
                            cursorPos = 0;
                            break;
                        default:
                            printf("Opção Inválida!\n");
                            cursorPos = 0;
                            break;
                    }
                }
                break;
            default:
                printf("Jogo em estado não definido! Fechando...\n.");
                playState = PS_RETURN;
                gameState = GS_CLOSE;
                break;
        }

    } while(playState != PS_RETURN);
}

int main()
{
    setlocale(LC_CTYPE, "Portuguese");
    srand(time(0));

    gameState = GS_MAIN;
    lanState = LAN_PTBR;
    int dificuldadeCasual = MIN_DIF;
    int flagAjudas = 1;
    int flagAcentos = 0;
    int cursorPos = 0;

    createWordDataFiles(lanState);

    do
    {
        switch(gameState)
        {
            case GS_MAIN:
                printMainMenu(cursorPos);
                if(procCursor(&cursorPos, 5))
                {
                    switch(cursorPos)
                    {
                        case 0:
                            gameState = GS_CHOOSE;
                            cursorPos = 0;
                            break;
                        case 1:
                            gameState = GS_OPTIONS;
                            cursorPos = 0;
                            break;
                        case 2:
                            gameState = GS_RULES;
                            cursorPos = 0;
                            break;
                        case 3:
                            gameState = GS_SCORES;
                            cursorPos = 0;
                            break;
                        case 4:
                            gameState = GS_CLOSE;
                            cursorPos = 0;
                            break;
                        default:
                            printf("Opção Inválida!\n");
                            cursorPos = 0;
                            break;
                    }
                }
                break;
            case GS_OPTIONS:
                printOptionsMenu(cursorPos, dificuldadeCasual, flagAjudas, flagAcentos, lanState);
                if(procCursor(&cursorPos, 5))
                {
                    switch(cursorPos)
                    {
                        case 0:
                            dificuldadeCasual++;
                            dificuldadeCasual %= MAX_DIF + 1;
                            if(dificuldadeCasual < MIN_DIF) dificuldadeCasual = MIN_DIF;
                            cursorPos = 0;
                            break;
                        case 1:
                            if (flagAjudas) flagAjudas = 0;
                            else if (!flagAjudas) flagAjudas = 1;
                            cursorPos = 0;
                            break;
                        case 2:
                            if (flagAcentos) flagAcentos = 0;
                            else if (!flagAcentos) flagAcentos = 1;
                            cursorPos = 0;
                            break;
                        case 3:
                            lanState++;
                            lanState %= 2;
                            cursorPos = 0;
                            break;
                        case 4:
                            createWordDataFiles(lanState);
                            gameState = GS_MAIN;
                            cursorPos = 0;
                            break;
                        default:
                            printf("Opção Inválida!\n");
                            cursorPos = 0;
                            break;
                    }
                }
                break;
            case GS_RULES:
                printRules(cursorPos);
                if(procCursor(&cursorPos, 1))
                {
                    gameState = GS_MAIN;
                    cursorPos = 0;
                }
                break;
            case GS_CHOOSE:
                printChooseScreen(cursorPos);
                if(procCursor(&cursorPos, 3))
                {
                    switch(cursorPos)
                    {
                        case 0:
                            gameState = GS_PLAY_CASUAL;
                            cursorPos = 0;
                            break;
                        case 1:
                            gameState = GS_PLAY_ARCADE;
                            cursorPos = 0;
                            break;
                        case 2:
                            gameState = GS_MAIN;
                            cursorPos = 0;
                            break;
                        default:
                            printf("Opção Inválida!\n");
                            cursorPos = 0;
                            break;
                    }

                }
                break;
            case GS_SCORES:
                printScores(cursorPos);
                if(procCursor(&cursorPos, 1))
                {
                    gameState = GS_MAIN;
                    cursorPos = 0;
                }
                break;
            case GS_PLAY_CASUAL:
                procPlay(MODE_CASUAL, dificuldadeCasual, flagAjudas, flagAcentos);
                cursorPos = 0;
                break;
            case GS_PLAY_ARCADE:
                procPlay(MODE_ARCADE, dificuldadeCasual, flagAjudas, flagAcentos);
                cursorPos = 0;
                break;
            default:
                printf("Jogo em estado não definido! Fechando...\n.");
                return -1;
                break;
        }

    } while(gameState != GS_CLOSE);

    system("cls");
    printf("Fechando...\n");
    return 0;
}
