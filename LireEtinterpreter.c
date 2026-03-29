#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef char string[1024];

typedef struct etape
{
    string actions[10];
    int nbActions;

    string preconds[10];
    int nbPreconds;

    string adds[10];
    int nbAdd;

    string delete[10];
    int nbDelete;

} Etape;

string Start[10], Finish[10], has[60] = {""};
int currentHas;
char ole[100];
Etape rules[10];

bool matchTableau(string toMatch)
{
    // bool toReturn;

    for (int i = 0; i < currentHas; i++){
        printf("%s regarde %s", toMatch, has[i]);
    
        if (strcmp(toMatch, has[i]) != 0)
        {
            return false;
        }
    }
    return true;
}

void afficherRules(int nbRules) // donner le nombre max de rules
{
    printf("=== AFFICHAGE DES REGLES (RULES) ===\n");
    for (int i = 0; i < nbRules; i++)
    {
        printf("\n--- REGLE %d ---\n", i + 1);

        printf("  Actions (%d) : ", rules[i].nbActions);
        for (int j = 0; j < rules[i].nbActions; j++)
        {
            printf("[%s] ", rules[i].actions[j]);
        }

        printf("\n  Preconditions (%d) : ", rules[i].nbPreconds);
        for (int j = 0; j < rules[i].nbPreconds; j++)
        {
            printf("[%s] ", rules[i].preconds[j]);
        }

        printf("\n  Adds (%d) : ", rules[i].nbAdd);
        for (int j = 0; j < rules[i].nbAdd; j++)
        {
            printf("[%s] ", rules[i].adds[j]);
        }

        printf("\n  Deletes (%d) : ", rules[i].nbDelete);
        for (int j = 0; j < rules[i].nbDelete; j++)
        {
            printf("[%s] ", rules[i].delete[j]);
        }
        printf("\n");
    }
}
int parseLine(char source[], string cible[])
{
    int i = 0, n = 0; // i: l'indice de source[], n: l'indice de cible[]

    while (source[i] != ':')
        i++;   // avancer jusqu'à ':'
    i++;       // avancer au debut du premier mot
    int j = i; // marquer le debut du premier mot avec j
    while (source[i] != '\n')
    {

        if (source[i] == ',')
        {
            // extraire les i-j caracteres a partir de j
            int x = i;
            string temp;

            while (source[x] != ',' && source[x] != ':')
            {
                temp[x] = source[x];
                x = x - 1;
            }
            // printf("%s", temp);

            memcpy(&cible[n], &source[j], i - j);
            n++;
            j = i + 1; // marquer le debut de la chaine suivante avec j
        }
        i++;
    }
    return n;
}

int main(void)
{
    FILE *Fichier = fopen("Hello.txt", "r");
    char c[100]; // stocker le contenu de la ligne, 100 est le maxium de caracteres
    int nLigne = 0, nEtapes = -1;
    bool notEmpty = true;
    fgets(c, 100, Fichier); // début
    while (notEmpty)
    {

        if (nEtapes == -1)
        {
            if (nLigne == 0)
            {
                parseLine(c, Start);
            }
            else if (nLigne == 1)
            {
                parseLine(c, Finish);
            }
            else
            {
                nEtapes = 0;
                nLigne = 0;
            }
        }
        if (nEtapes != -1)
        {
            if (nLigne == 0)
            {

                rules[nEtapes].nbActions = parseLine(c, rules[nEtapes].actions);
                // printf("Pour ca on a: %s\n", rules[nEtapes].actions[0]);
            }

            if (nLigne == 1)
            {
                rules[nEtapes].nbPreconds = parseLine(c, rules[nEtapes].preconds);
            }

            if (nLigne == 2)
            {
                rules[nEtapes].nbAdd = parseLine(c, rules[nEtapes].adds);
            }

            if (nLigne == 3)
            {
                rules[nEtapes].nbDelete = parseLine(c, rules[nEtapes].delete);
                nLigne = -1;
                nEtapes++;
            }
        }

        nLigne++;
        strcpy(ole, c);
        fgets(c, 100, Fichier);

        if (c[0] == '*')
        {
            fgets(c, 100, Fichier);
        }
        if (c == NULL || strcmp(ole, c) == 0)
        {
            notEmpty = false;
        }
        // printf("Etape: %d ;La premiere et deuxieme lettre de la ligne %d sont %c%c\n", nEtapes, nLigne, c[0], c[1]);
    }

    // afficherRules(6);
    currentHas = sizeof(Start);
    printf("%d\n", currentHas);
    for (int i = 0; i < currentHas-1; i++)
    {
        //printf("on veut assigner %s à %s\n", Start[i], has[i]);
       // strcpy(has[i], Start[i]);
    }

    printf("%d", nEtapes);
    for (int i = 0; i < nEtapes - 1; i++)
    {
        printf("i:%d\n", i);
        for (int n = 0; n < rules[i].nbPreconds; n++)
        {
            printf("n:%d\n", n);
            if (matchTableau(rules[i].preconds[n]))
            {
                printf("Incoherence!");
                return 0;
            }
        }
    }

    fclose(Fichier);
}
