#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Uretim.h"

#include "Taktik.h"
#include "Koloni.h"
#include "Oyun.h"

int powOf10(int num)
{
    int res = 1;
    int i;
    for (i = 2; i <= num; i++)
    {
        res *= 10;
    }
    return res;
}



int strToNum(char *str, int num)
{
    int i, res = 0;
    for(i = num-1; i >= 0; i--)
    {
        res += (int) (str[i] - '0') * powOf10(num - i);
    }
    return res;
}


int* readInputBuffer(char* str, int* stringSize, int* numberOfColonies)
{
    fgets(str, 100, stdin);
    *stringSize = strlen(str);

    *numberOfColonies = 0;
    for (int i = 0; i < *stringSize; i++)
    {
        if (str[i] == ' ')
        {
            (*numberOfColonies)++;
        }
    }
    (*numberOfColonies)++;
    printf("%d\n", strToNum(str, 4));
    int* numdatasarr = (int*)malloc(sizeof(int) * (*numberOfColonies));
    int numlen = 0, koloni_index = 0;

    for (int i = 0; i < *stringSize; i++)
    {
        numlen++;
        if (str[i] == ' ')
        {
            numdatasarr[koloni_index] = strToNum(str + i - numlen + 1, numlen - 1);
            printf("str: %c\n", *(str + i - numlen + 1));
            printf("index: %d\n", i);
            koloni_index++;
            numlen = 0;
        }
        if(i == (*stringSize)-1)
        {
            numdatasarr[koloni_index] = strToNum(str + i - numlen + 1, numlen - 1);
        }

    }

    return numdatasarr;
}


int randomInterval(int interval1, int interval2)
{
    return interval1 > interval2 ? (rand()%interval1 + interval2) : (rand()%interval2 + interval1);
}


int main()
{
    char str[100];
    int stringSize = 0;
    int numberOfColonies = 0;
    int *numdatasarr = readInputBuffer(str, &stringSize, &numberOfColonies);
    Colony* colonies = (Colony*)malloc(sizeof(Colony) * numberOfColonies);
    printf("%d", numdatasarr[numberOfColonies - 1]);
    for (int i = 0; i < numberOfColonies; ++i)
        createColonyTest(&colonies[i], numdatasarr);
    int deathcolonienumber = 0;
    int turnnumber = 1;
    srand(time(NULL));
    while (deathcolonienumber < numberOfColonies - 1)
    {
        printf("----------------------------------------------------------------------------\n");
        printf("Number of Turn: %d\n", turnnumber);
        printf("Colony    Population   Food Stock     Wins       Losses\n");
        turnnumber++;
        for (int i = 0; i < numberOfColonies; i++) {
            if (!colonies[i]->isDead && (colonies[i]->population <= 0 || colonies[i]->foodStock <= 0))
            {
                deathcolonienumber++;
                colonies[i]->isDead = 1;
            }
            if (!colonies[i]->isDead) {
                printf("%4c %8d %13d %13d %13d\n", colonies[i]->symbol, colonies[i]->population, colonies[i]->foodStock, colonies[i]->wins, colonies[i]->losses);
            }
            else
            {
                printf("%4c      --             --            --            --\n", colonies[i]->symbol);
            }
        }
        printf("----------------------------------------------------------------------------\n");
        int Battleresults[2];
        for (int i = 0; i < numberOfColonies - 1; i++)
        {
            for (int j = i + 1; j < numberOfColonies; j++)
            {
                if (colonies[i]->isDead == 0 && colonies[j]->isDead == 0)
                {
                    Battleresults[0] = Savas();
                    Battleresults[1] = Savas();

                    if (Battleresults[0] < Battleresults[1])
                    {
                        colonies[i]->population -= colonies[i]->population * ((int)((Battleresults[1] - Battleresults[0]) / 1000.));
                        colonies[j]->foodStock += (int)((colonies[i]->foodStock / 100.) * 10);
                        colonies[i]->foodStock -= (int)((colonies[j]->foodStock / 100.) * 10);
                        colonies[i]->losses = colonies[i]->losses + 1;
                        colonies[j]->wins = colonies[j]->wins + 1;
                    } else if (Battleresults[0] > Battleresults[1])
                    {
                        colonies[j]->population -= ((int)((Battleresults[0] - Battleresults[1]) / 1000.)) * colonies[j]->population;
                        colonies[i]->foodStock += (int)(10 * (colonies[j]->foodStock / 100.));
                        colonies[j]->foodStock -= (int)(10 * (colonies[i]->foodStock / 100.));
                        colonies[i]->wins = colonies[i]->wins + 1;
                        colonies[j]->losses = colonies[j]->losses + 1;
                    } else if (Battleresults[0] == Battleresults[1])
                    {
                        if (colonies[i]->population == colonies[j]->population)
                        {
                            if (randomInterval(1, 2) == 1)
                            {
                                colonies[i]->foodStock += (int) (10 * (colonies[j]->foodStock / 100.));
                                colonies[j]->foodStock -= (int) (10 * (colonies[i]->foodStock / 100.));
                                colonies[i]->wins = colonies[i]->wins + 1;
                                colonies[j]->losses = colonies[j]->losses + 1;
                            } else
                            {
                                colonies[j]->foodStock += (int) ((colonies[i]->foodStock / 100.) * 10);
                                colonies[i]->foodStock -= (int) ((colonies[i]->foodStock / 100.) * 10);
                                colonies[i]->losses = colonies[i]->losses + 1;
                                colonies[j]->wins = colonies[j]->wins + 1;
                            }
                        } else if (colonies[i]->population > colonies[j]->population)
                        {
                            colonies[i]->foodStock += (int) (10 * (colonies[j]->foodStock / 100.));
                            colonies[j]->foodStock -= (int) (10 * (colonies[i]->foodStock / 100.));
                            colonies[i]->wins = colonies[i]->wins + 1;
                            colonies[j]->losses = colonies[j]->losses + 1;
                        } else
                        {
                            colonies[j]->foodStock += (int) ((colonies[i]->foodStock / 100.) * 10);
                            colonies[i]->foodStock -= (int) ((colonies[j]->foodStock / 100.) * 10);
                            colonies[i]->losses = colonies[i]->losses + 1;
                            colonies[j]->wins = colonies[j]->wins + 1;
                        }

                    }
                }
            }
        }

        for (int i = 0; i < numberOfColonies; i++)
        {
            colonies[i]->population = (int)(colonies[i]->population*1.2);
            colonies[i]->foodStock = colonies[i]->foodStock - colonies[i]->population * 2;
        }


    }

    free(colonies);
    return 0;
}
