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
// la structure au dessus sert à stocker chaque étape du indiquée dans le .txt, elle contient les actions, les préconditions, les adds et les deletes de chaque étape

string Start[10], Finish[10], Inventory[10];
char ole[100];
Etape rules[10];
int searchRemoveString(string source[], string target, int n){
        for (int i = 0; i < n; i++)
        {
            if (strcmp(source[i], target) == 0)
            {
                // supprimer la chaine en la remplaçant par une chaine vide
                source[i][0] = '\0';
                return 0;
            }
        }
        printf("pas trouvé %s! Erreur!\n", target);
        return -1;
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
    }


    // On fait l'inventaire!

    for (int i = 0; i < 10; i++)
    {
        if (Start[i][0] != '\0') // qui n'est pas vide
        {
            strcpy(Inventory[i], Start[i]); // est copié dans l'inventaire
        }
    }

    // maintenant on regarde chaque delete et on l'enlève de l'inventaire, et chaque add et on l'ajoute à l'inventaire, pour chaque règle

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (rules[i].delete[j][0] != '\0') // si le delete n'est pas vide
            {
                printf("On supprime [%s] de l'inventaire...\n", rules[i].delete[j]);
                searchRemoveString(Inventory, rules[i].delete[j], 10); // supprimer le delete de l'inventaire
            }
        }
        for (int j = 0; j < 10; j++)
        {
            if (Inventory[j][0] == '\0') // trouver une place vide dans l'inventaire
            {
                for (int k = 0; k < rules[i].nbAdd; k++) //loop les adds de rules[i]
                {
                    if (rules[i].adds[k][0] != '\0') // si l'add n'est pas vide
                    {
                        printf("On ajoute [%s] a l'inventaire...\n", rules[i].adds[k]);
                        strcpy(Inventory[j + k], rules[i].adds[k]); // ajouter les add à l'inventaire
                    }
                }
            }
        }
    }

    fclose(Fichier);
}
