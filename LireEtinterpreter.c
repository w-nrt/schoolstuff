#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// constantes pr les tableaux

#define MAX_CONDITIONS 10
#define MAX_INVENTORY 25
#define MAX_RULES 25

// typedefs
typedef char string[1024];

typedef struct etape    // là où on va stocker les conditions
{

    string actions[MAX_CONDITIONS];
    int nbActions;

    string preconds[MAX_CONDITIONS];
    int nbPreconds;

    string adds[MAX_CONDITIONS];
    int nbAdd;

    string delete[MAX_CONDITIONS];
    int nbDelete;

    bool complete;  // permettra de savoir si on est déjà passé par une condition
} Etape;

// variables globales
int m;
string Start[MAX_CONDITIONS], Finish[MAX_CONDITIONS], Inventory[MAX_INVENTORY]; // Stockage du start, du finish et de l'inventaire
Etape rules[MAX_RULES];
bool peutExecuter;
char c[100];
int nEtapes = -1;
int m;

void printinventaire(void) // Fonction qui affiche notre inventaire pour mieux visualiser.
    printf("Dans l'inventaire on a: ");
    for (int i = 0; i < MAX_INVENTORY; i++)
    {
        if (Inventory[i][0] != '\0')
        { // si la case n'est pas vide
            printf("[%s] ", Inventory[i]);
        }
    }
    printf("\n");
}

bool TrouveString(string t[], int n, string targ)
{
    for (int i = 0; i < n; i++)
    {
        if (t[i][0] != '\0' && strcmp(t[i], targ) == 0)
        {
            return true;
        }
    }
    return false;
}

int searchRemoveString(string source[], string target, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(source[i], target) == 0)
        {
            source[i][0] = '\0'; // Supprimer en rendant la chaine vide
            return 0;
        }
    }
    printf("! ! ! Pas trouvé [%s]! Erreur ! ! !\n", target);
    return -1;
}

void afficherRules(int nbRules) // Fonction pour afficher les règles, cette fonction sert simplement de visualisation
{
    printf("\n=== AFFICHAGE DES REGLES ===\n");
    for (int i = 0; i < nbRules; i++)
    {
        printf("--- REGLE %d ---\n", i + 1);
        printf("  Actions: ");
        for (int j = 0; j < rules[i].nbActions; j++)
            printf("[%s] ", rules[i].actions[j]);
        printf("\n  Preconds: ");
        for (int j = 0; j < rules[i].nbPreconds; j++)
            printf("[%s] ", rules[i].preconds[j]);
        printf("\n  Adds: ");
        for (int j = 0; j < rules[i].nbAdd; j++)
            printf("[%s] ", rules[i].adds[j]);
        printf("\n  Deletes: ");
        for (int j = 0; j < rules[i].nbDelete; j++)
            printf("[%s] ", rules[i].delete[j]);
        printf("\n");
    }
}

int parseLine(char source[], string cible[])
{
    int i = 0, n = 0;

    while (source[i] != '\0' && source[i] != ':')
        i++;
    if (source[i] == ':')
        i++; // Avancer après ':'

    int j = i;
    while (source[i] != '\n' && source[i] != '\0')
    {
        if (source[i] == ',')
        {
            memcpy(cible[n], &source[j], i - j);
            cible[n][i - j] = '\0';
            n++;
            j = i + 1;
        }
        i++;
    }
    return n;
}

bool Execution(int i) // Fonction qui sert à appliquer chaque rule et verifier si elle est applicable ou pas
{
    if (rules[i].complete)
        return true;    // Si on a réussi à compléter l'étape

    peutExecuter = true;
    for (int p = 0; p < rules[i].nbPreconds; p++)
    {
        if (!TrouveString(Inventory, MAX_INVENTORY, rules[i].preconds[p]))
        {
            peutExecuter = false;   // Si dans notre inventaire il nous manque des préconditions de l'étape, alors cette étape ne peut pas être éxécutée.
        }
    }

    if (peutExecuter)               // Si notre inventaire possède tout les preconds de l'étape actuelle
    {
        printf("\n---- EXECUTION DE LA RULE : %d ----\n", i);

        for (int j = 0; j < rules[i].nbDelete; j++)
        {
            searchRemoveString(Inventory, rules[i].delete[j], MAX_INVENTORY);   // On supprime de notre inventaire les "delete" de l'étape.
        }

        for (int k = 0; k < rules[i].nbAdd; k++)
        {
            m = 0;
            while (m < MAX_INVENTORY && Inventory[m][0] != '\0')
                m++; // Trouver une case vide
            if (m < MAX_INVENTORY)
            {
                strcpy(Inventory[m], rules[i].adds[k]);     // On ajoute à notre inventaire les adds.
            }
            else
            {
                printf("Attention! Erreur! Tableau deborde \n");
            }
        }

        rules[i].complete = true;
        printinventaire();
        return true;
    }
    return false; // Impossible d'exécuter pour l'instant
}

int main(void) // Fonction principale qui lis le .txt qu'on lui envoie et tente de le résoudre à l'aide des autres fonctions.
{
    FILE *Fichier = fopen("Hello.txt", "r");
    if (!Fichier)
    {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    int nLigne = 0;

    while (fgets(c, 100, Fichier) != NULL)
    {
        if (c[0] == '*')
            continue; // Ignorer les astérisques

        if (nEtapes == -1)                  // On commence à une étape qui existe pas, c'est-à-dire le Start et le Finish qui ne sont pas des étapes.
        {
            if (nLigne == 0)
                parseLine(c, Start);        // Récupère le Start
            else if (nLigne == 1)
                parseLine(c, Finish);       // Récupère le Finish
            else
            {
                nEtapes = 0;
                nLigne = 0;
                continue;
            }
        }

        if (nEtapes != -1)              // On entre dans les étapes, après le Start et le Finish donc, une étape se trouve entre deux lignes d'astérisques et contient "action", "preconds", "add" et "delete".
        {
            if (strncmp(c, "action:", 7) == 0)
            {
                rules[nEtapes].nbActions = parseLine(c, rules[nEtapes].actions);    // Récupération des "action" de l'étape actuelle.
            }
            else if (strncmp(c, "preconds:", 9) == 0)
            {
                rules[nEtapes].nbPreconds = parseLine(c, rules[nEtapes].preconds);  // Récupération des "preconds" de l'étape actuelle.
            }
            else if (strncmp(c, "add:", 4) == 0)
            {
                rules[nEtapes].nbAdd = parseLine(c, rules[nEtapes].adds);           // Récupération des "add" de l'étape actuelle.
            }
            else if (strncmp(c, "delete:", 7) == 0)
            {
                rules[nEtapes].nbDelete = parseLine(c, rules[nEtapes].delete);      // Récupération des "delete" de l'étape actuelle.
                rules[nEtapes].complete = false;                                    // Au départ, une action n'est jamais compléte.
                nEtapes++;
            }
        }
        nLigne++;
    }
    fclose(Fichier);

    // Initialisation de l'inventaire
    printf("=== INIT INVENTAIRE ===\n");
    for (int i = 0; i < MAX_CONDITIONS; i++)
    {
        if (Start[i][0] != '\0')
        {
            strcpy(Inventory[i], Start[i]);     // Au départ, notre inventaire est constitué du Start, il faut qu'à la fin notre inventaire soit égal au Finish.
        }
    }

    afficherRules(nEtapes);
    printinventaire();

  // Boucle d'exécution
    bool objectifAtteint = false;

    while (!objectifAtteint)    // Tant que les preconds ne sont pas égales au finish, on répète cette partie de code
    {
        bool actionRealisee = false; // Sert à éviter que le programme cherche pour toujours
        bool chercheObj = true;

        for (int i = 0; i < nEtapes; i++)
        {
            if (!rules[i].complete) // Si on a pas complété les rules
            {
                if (Execution(i))   // Si l'éxecution de cette étape s'est bien déroulée.
                {
                    actionRealisee = true; 
                    
   
                    objectifAtteint = true; // true par défaut, set a false si non vrai
                    m = 0;
                    
                    while (m < MAX_CONDITIONS && Finish[m][0] != '\0' && chercheObj)
                    {
                        // Si on ne trouve pas une des conditions finales dans l'inventaire
                        if (!TrouveString(Inventory, MAX_INVENTORY, Finish[m]))
                        {
                            objectifAtteint = false; 
                            chercheObj = false; // On arrête la boucle de suite
                        }
                        m++;
                    }

                    // Si on a toutes les conditions de Finish, on arrête tout immédiatement, sa permet d'éviter de revenir sur une étape, pouvant causer une boucle infinie alors qu'on a rempli le Finish.
                    if (objectifAtteint)
                    {
                        printf("\n-- EXECUTION COMPLETE! SUCCES!\n");
                        return 0; // on sort directement du programme, pas besoin de faire d'autre chose
                    }
                }
            }
        }

        // c'est-à-dire qu'on est coincé, on ne peut appliquer plus de règles. dans ce cas on évite une boucle infinie en sortant
        if (!actionRealisee)
        {
            printf("\n! ! ! BLOCAGE : AUCUNE ACTION POSSIBLE. ECHEC ! ! !\n");
            objectifAtteint = true; // sort de la boucle
        }
    }

    return 0;
}
