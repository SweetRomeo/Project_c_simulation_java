#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include "Oyun.h"
#include "Taktik.h"
#include "Koloni.h"
#include "Uretim.h"

#define randomize() srand(time(NULL))
#define SIZE 100

int powOf10(int num)
{
    int res = 1;
    int i;
    for (i = 2; i <= num; i++)
        res *= 10;
    return res;
}

int strToNum(char *str, int num) {
    int i, res = 0;
    for(i = num-1; i >= 0; i--)
        res += (int)(str[i] - '0')*powOf10(num - i);
    return res;
}


int* readInputBuffer(char* str, char ch, int* stringSize, int* spaceCount, int* lastSpaceFlag)
{
    fgets(str, 100, stdin);
    *stringSize = strlen(str);
    (*stringSize)++;

    *spaceCount = 0;
    for (int i = 0; i < *stringSize; i++) {
        if (str[i] == ' ') {
            (*spaceCount)++;
        }
    }
    (*spaceCount)++;
    printf("%d\n", strToNum(str, 4));
    int* numdatasarr = (int*)malloc(sizeof(int) * (*spaceCount));
    int numlen = 0, koloni_index = 0;

    for (int i = 0; i < *stringSize; i++) {
        numlen++;
        if (str[i] == ' ') {
            numdatasarr[koloni_index] = strToNum(str + i - numlen + 1, numlen - 1);
            printf("str: %c\n", *(str + i - numlen + 1));
            printf("index: %d\n", i);
            //printf("numdatasarr : %d ",numdatasarr[koloni_index] );
            koloni_index++;
            numlen = 0;
        }
        if(i == (*stringSize)-1)
            numdatasarr[koloni_index] = strToNum(str + i - numlen + 1, numlen - 1);

    }

    return numdatasarr;
}


int main()
{
    char str[SIZE];
    char ch = 0;
    int stringSize = 0;
    int spaceCount = 0, lastSpaceFlag = 0;
    int *numdatasarr = readInputBuffer(str, ch, &stringSize, &spaceCount, &lastSpaceFlag );
    printf("%d",*numdatasarr);
    str[stringSize] = ' ';
    str[stringSize + 1] = '\0';
    printf("\n");
    printf("Input string: %s\n", str);
    printf("Number of spaces: %d\n", spaceCount);
    int numberOfColonies = ++spaceCount;
    Colony* colonies = (Colony*)malloc(sizeof(Colony) * numberOfColonies);
    for (int i = 0; i < numberOfColonies; ++i)
        createColonyTest(&colonies[i], numdatasarr);
    int deathCount = 0;
    int turnCount = 1;
    while (1) {
        printf("----------------------------------------------------------------------------\n");
        printf("Turn Count: %d\n", turnCount);
        printf("Colony    Population   Food Stock     Wins       Losses\n");
        turnCount++;
        for (int i = 0; i < spaceCount; i++) {
            if (!colonies[i]->isDead && (colonies[i]->population <= 0 || colonies[i]->foodStock <= 0)) {
                deathCount++;
                colonies[i]->isDead = 1;
                printf("%d died.\n", i);
            }
            if (!colonies[i]->isDead)
                printf("%4c %8d %13d %13d %13d\n", colonies[i]->symbol, colonies[i]->population, colonies[i]->foodStock, colonies[i]->wins, colonies[i]->losses);
            else
                printf("%4c      --         --         --        --\n", colonies[i]->symbol);
        }
        printf("----------------------------------------------------------------------------\n");

        for (int i = 0; i < spaceCount - 1; i++) {
            for (int j = i + 1; j < spaceCount; j++) {
                if (!(colonies[i]->isDead || colonies[j]->isDead)) {
                    int tempBattle1 = Savas(); // Colony i
                    int tempBattle2 = Savas(); // Colony j
                    int winner;
                    int foodTransferTemp;

                    if (tempBattle1 == tempBattle2) {
                        if (colonies[i]->population == colonies[j]->population)
                            winner = rand() % 2 + 1;
                        else if (colonies[i]->population > colonies[j]->population)
                            winner = 1;
                        else
                            winner = 2;
                    } else if (tempBattle1 > tempBattle2) {
                        winner = 1;
                    } else {
                        winner = 2;
                    }

                    if (winner == 1) {
                        colonies[i]->wins++;
                        colonies[j]->losses++;
                        colonies[j]->population -= colonies[j]->population * ((int)((double)(tempBattle1 - tempBattle2) / 1000));
                        foodTransferTemp = (int)(((double)colonies[j]->foodStock / 100) * 10);
                        colonies[i]->foodStock += foodTransferTemp;
                        colonies[j]->foodStock -= foodTransferTemp;
                    } else {
                        colonies[i]->losses++;
                        colonies[j]->wins++;
                        colonies[i]->population -= colonies[i]->population * ((int)((double)(tempBattle2 - tempBattle1) / 1000));
                        foodTransferTemp = (int)(((double)colonies[i]->foodStock / 100) * 10);
                        colonies[j]->foodStock += foodTransferTemp;
                        colonies[i]->foodStock -= foodTransferTemp;
                    }
                }
            }
        }

        for (int i = 0; i < spaceCount; i++) {
            colonies[i]->population += (int)(((double)colonies[i]->population / 100) * 20);
            colonies[i]->foodStock -= colonies[i]->population * 2;
        }

        if (deathCount >= spaceCount - 1)
            break;


    }

    free(colonies);
    return 0;
}
