#include "dataproc.h"

int countChars(char* str, int strLen)
{
   int n = 0;

   while(n < strLen)
   {
    if (*(str+n) == '\0' || *(str+n) == '\n') break;
    n++;
   }

    return n;
}

int appendString(char* str1, int str1Len, char* str2)
{
    int numElementsStr1 = countChars(str1, str1Len);
    int numElementsStr2 = countChars(str2, str1Len);

    if(str1Len < numElementsStr1 + numElementsStr2 + 1) return 0;

    for(int n = 0; n < numElementsStr2; n++)
    {
        *(str1 + numElementsStr1 + n) = *(str2 + n);
    }

    *(str1+numElementsStr1+numElementsStr2) = '\0';

    return 1;
}

void convCharToPTBR(unsigned char* ch, int flagAcentos)
{
    if((*ch >= '0' && *ch <= '9') || (*ch >= 'a' && *ch <= 'z') || (*ch >= 'A' && *ch <= 'Z'));
    else if(*ch == 130 && flagAcentos) *ch = 'é';
    else if(*ch == 131 && flagAcentos) *ch = 'â';
    else if(*ch == 198 && flagAcentos) *ch = 'ã';
    else if(*ch == 135 && flagAcentos) *ch = 'ç';
    else if(*ch == 136 && flagAcentos) *ch = 'ê';
    else if(*ch == 160 && flagAcentos) *ch = 'á';
    else if(*ch == 161 && flagAcentos) *ch = 'í';
    else if(*ch == 162 && flagAcentos) *ch = 'ó';
    else if(*ch == 147 && flagAcentos) *ch = 'ô';
    else if(*ch == 228 && flagAcentos) *ch = 'õ';
    else if(*ch == 163 && flagAcentos) *ch = 'ú';
    else *ch = '!';
}

int isCharOnString(unsigned char ch, unsigned char* str, int strLen)
{
    int n = 0;

    while(n < strLen)
    {
        if(*(str+n) == ch)
        {
            return 1;
            break;
        }
        n++;
    }

    return 0;
}

int writeNewCharToString(unsigned char ch, unsigned char* str, int strLen)
{
    if(isCharOnString(ch, str, strLen)) return 0;

    int n = 0;
    while(n < strLen)
    {
        if(*(str+n) == '\0')
        {
            *(str+n) = ch;
            *(str+n+1) = '\0';
            break;
        }
        n++;
    }

    return 1;
}

int clearString(unsigned char* str, int strLen)
{
    for(int n = 0; n < strLen; n++)
    {
        *(str+n) = '\0';
    }

    return 1;
}

int getRandomIntInRange(int lowerLimit, int upperLimit)
{
 return (rand() % (upperLimit - lowerLimit + 1)) + lowerLimit;
}

int procCmd(twordData* wordData, unsigned char cmd, unsigned char* usedChars, int maxUsedCharsLen,
            int* flagAjudaVogais, int* flagAjudaPrimeira, int* flagAjudaAleatorio,
            int* pulos)
{
    int n = 0;
    int m = 0;

    if(cmd == '1' && *flagAjudaVogais == 1)
    {
        *flagAjudaVogais = 0;
        writeNewCharToString('A', usedChars, maxUsedCharsLen);
        writeNewCharToString('E', usedChars, maxUsedCharsLen);
        writeNewCharToString('I', usedChars, maxUsedCharsLen);
        writeNewCharToString('O', usedChars, maxUsedCharsLen);
        writeNewCharToString('U', usedChars, maxUsedCharsLen);
    }
    else if(cmd == '2' && *flagAjudaPrimeira == 1)
    {
        *flagAjudaPrimeira = 0;
         writeNewCharToString(wordData->word[0], usedChars, maxUsedCharsLen);
         writeNewCharToString(wordData->word[wordData->wordLength - 1], usedChars, maxUsedCharsLen);
    }
    else if(cmd == '3' && *flagAjudaAleatorio == 1)
    {
        *flagAjudaAleatorio = 0;
        while(n < 3 && m < 10)
        {
            if(writeNewCharToString(wordData->word[getRandomIntInRange(0, wordData->wordLength - 1)], usedChars, maxUsedCharsLen))
                n++;
            else
                m++;
        }
    }
    else if(cmd == '4' && *pulos > 0)
    {
        *pulos -= 1;
        playState = PS_LOAD;
    }
    else if(cmd == '5')
    {
        playState = PS_RETURN;
        gameState = GS_CHOOSE;
    }
    else if(cmd == '6')
    {
        playState = PS_RETURN;
        gameState = GS_CLOSE;
    }
    else
    {
        printf("Comando não válido!\n");
        system("pause");
    }

    return 1;
}

int getCharInput(twordData* wordData, unsigned char* input, unsigned char* usedChars, int maxUsedCharsLen,
                 int* flagAjudaVogais, int* flagAjudaPrimeira, int* flagAjudaAleatorio,
                 int* pulos, int flagAcentos)
{
    unsigned char tempInput;

    if(*flagAjudaVogais) printf("1 - Revelar vogais\n");
    if(*flagAjudaPrimeira) printf("2 - Revelar primeira e ultima letra\n");
    if(*flagAjudaAleatorio) printf("3 - Revelar três letras aleatórias\n");
    if(*pulos > 0) printf("4 - Pular (%i)\n", *pulos);
    printf("5 - Voltar\n");
    printf("6 - Fechar\n");

    printf("Tente uma LETRA ou USE uma das ajudas:\n");
    printf(">>");

    tempInput = fgetc(stdin);
    fflush(stdin);

    convCharToPTBR(&tempInput, flagAcentos);
    tempInput = toupper(tempInput);

    if(tempInput >= '0' && tempInput <= '9') //Se for um comando (0-9) processar o comando
    {
        procCmd(wordData, tempInput, usedChars, maxUsedCharsLen,
            flagAjudaVogais, flagAjudaPrimeira, flagAjudaAleatorio,
            pulos);
    }
    else if(tempInput != '!' && writeNewCharToString(tempInput, usedChars, maxUsedCharsLen)) //Se não for um comando adicionar ao vetor de caracteres usados
    {
        *input = tempInput;
        return 1;
    }

    *input = '!'; //Input inválido
    return 0;
}

unsigned char processSpecialChar(unsigned char* ch)
{
    if(*ch == 195 && *(ch+1) == 161) return 'á';
    else if(*ch == 195 && *(ch+1) == 162) return 'â';
    else if(*ch == 195 && *(ch+1) == 163) return 'ã';
    else if(*ch == 195 && *(ch+1) == 167) return 'ç';
    else if(*ch == 195 && *(ch+1) == 169) return 'é';
    else if(*ch == 195 && *(ch+1) == 170) return 'ê';
    else if(*ch == 195 && *(ch+1) == 173) return 'í';
    else if(*ch == 195 && *(ch+1) == 179) return 'ó';
    else if(*ch == 195 && *(ch+1) == 180) return 'ô';
    else if(*ch == 195 && *(ch+1) == 181) return 'õ';
    else if(*ch == 195 && *(ch+1) == 186) return 'ú';
    else return '!';
}

twordData populateWordData(unsigned char* str, int strLen)
{
    twordData tempWordData;
    unsigned char letrasRepetidas[MAX_WORD_LENGTH] = "";
    int n = 0;

    clearString(tempWordData.word, sizeof(tempWordData.word)/sizeof(tempWordData.word[0]));
    tempWordData.wordLength = 0;
    tempWordData.flagBadWord = 0;
    tempWordData.flagAcentuada = 0;
    tempWordData.dificuldade = 0;
    tempWordData.numLetrasRepetidas = 0;

    while(n < MAX_WORD_LENGTH - 1)
    {
        if((*(str+n) >= 0 && *(str+n) <= 64) || (*(str+n) >= 91 && *(str+n) <= 96) || (*(str+n) >= 123 && *(str+n) <= 127))
            tempWordData.flagBadWord = 1;
        if(*(str+n) > 127)
            tempWordData.flagAcentuada = 1;

        for(int m = n - 1; m >= 0; m--)
        {
            if(*(str+m) == *(str+n) &&
               writeNewCharToString(*(str+n), letrasRepetidas, sizeof(letrasRepetidas)/sizeof(letrasRepetidas[0])))
            {
                tempWordData.numLetrasRepetidas += 1;
                break;
            }
        }

        tempWordData.word[n] = toupper(*(str+n));
        n++;
        if(n == MAX_WORD_LENGTH - 1) tempWordData.flagBadWord = 1;
        if(*(str+n) == '\0' || *(str+n) == '/') break;
    }

    tempWordData.word[n] = '\0';
    tempWordData.wordLength = n;

    tempWordData.dificuldade = tempWordData.wordLength - tempWordData.numLetrasRepetidas + (2 * tempWordData.flagAcentuada);

    if (tempWordData.dificuldade > MAX_DIF) tempWordData.dificuldade = MAX_DIF;

    return tempWordData;
}

tscoreData populateScoreData(char* name, int mode, int score, twordData lastWord)
{
    tscoreData tempScoreData;
    int n = 0;

    while(n < MAX_NAME_LENGTH - 1)
    {
        tempScoreData.name[n] = toupper(*name);
        if(tempScoreData.name[n] == '\0' || tempScoreData.name[n] == '\n')
        {
            tempScoreData.name[n] = '\0';
            break;
        }
        name++;
        n++;
    }

    if(n == MAX_NAME_LENGTH - 1) tempScoreData.name[n] = '\0';

    tempScoreData.mode = mode;
    tempScoreData.score = score;
    tempScoreData.lastWord = lastWord;

    return tempScoreData;
}

void getLine(unsigned char* str, int strLen, FILE* file)
{
    int n = 0;

    while(n < strLen)
    {
        *(str+n) = fgetc(file);
        if(*(str+n) == '\n' || *(str+n) == '\0')
        {
            *(str+n) = '\0';
            break;
        }
        n++;
    }
}

int saveScore(char* name, int mode, int score, twordData lastWord)
{
    FILE* file;
    file = fopen("data/scores.bin", "ab+");

    tscoreData currentScoreData = populateScoreData(name, mode, score, lastWord);

    if(file == NULL)
    {
        printf("Erro ao tentar abrir arquivo para salvar score.\n");
        return 0;
    }

    fwrite(&currentScoreData, sizeof(tscoreData), 1, file);

    fclose(file);
    return 1;
}

twordData getValidRandomWordData(int dificuldade, int flagAcentos)
{
    FILE* file;
    unsigned char errorWordData[] = "!!!";
    twordData tempWordData = populateWordData(errorWordData, sizeof(errorWordData)/sizeof(errorWordData[0]));
    int n = 0;

    file = fopen("data/data.bin", "rb");

    if(file != NULL)
    {
        do
        {
           fseek(file, rand()*sizeof(twordData), SEEK_SET);
           fread(&tempWordData, sizeof(twordData), 1, file);
           n++;
        } while((tempWordData.flagAcentuada && !flagAcentos) ||
                (tempWordData.dificuldade != dificuldade) ||
                (n < MAX_NUM_TRIES));

        if(n == MAX_NUM_TRIES)
        {
            printf("Não foi possivel sortear uma palavra aceitável após %i tentativas\n", MAX_NUM_TRIES);
        }
    }
    else printf("Erro ao tentar ler palavra do arquivo\n");

    fclose(file);
    return tempWordData;
}

int createWordDataFiles(int language)
{
    FILE* inFile;
    FILE* outDataFile;

    twordData tempWordData;
    unsigned char tempStr[MAX_WORD_LENGTH] = "";
    char ptbrWordsFilePath[] = "data/pt_BR.txt";
    char engWordsFilePath[] = "data/en_US.txt";

    if(language == LAN_PTBR)
        inFile = fopen(ptbrWordsFilePath, "r");
    else if(language == LAN_ENG)
        inFile = fopen(engWordsFilePath, "r");
    if(inFile == NULL)
    {
        printf("Erro ao tentar abrir o arquivo palavras para leitura.\n");
        return 0;
    }

    outDataFile = fopen("data/data.bin", "wb");

    if(outDataFile == NULL)
    {
        printf("Erro ao tentar abrir arquivo bin para escrita.\n");
        return 0;
    }

    while(!feof(inFile))
    {
        getLine(tempStr, sizeof(tempStr)/sizeof(tempStr[0]), inFile);
        tempWordData = populateWordData(tempStr, sizeof(tempStr)/sizeof(tempStr[0]));
        fwrite(&tempWordData, sizeof(twordData), 1, outDataFile);
    }

    fclose(inFile);
    fclose(outDataFile);
    return 1;
}
