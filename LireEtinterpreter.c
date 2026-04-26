/* Ce code sert à lire un fichier du type Hello.txt (qu'on peut trouver sur le GitHub https://github.com/w-nrt/schoolstuff) et d'essayer d'executer toute les instructions.
La façon dont il marche est la suivante: 
--Interpreter chaque instruction: et les stocker dans un tableau "d'étapes" qui est un enregistrement composé d'actions, de preconditions, d'ajouts et de suppression
--On fait l'inventaire des choses qu'on a déjà et de ce dont on a besoin à la fin
--On execute chaque étape, si une étape échoue on passe à la suivante puis on recommence
--Chaque fois, on vérifie l'inventaire pour trouver nos pre-réquis stockés dans "Final", si on les trouve tous l'execution est un succés, sinon au bout d'un moment on arrête la boucle d'éxecution
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// constantes pr les tableaux

#define MAX_CONDITIONS 10
#define MAX_INVENTORY 25
#define MAX_RULES 25

// typedefs
typedef char string[1024];

typedef struct etape // là où on va stocker les conditions
{

    string actions[MAX_CONDITIONS];
    int nbActions;

    string preconds[MAX_CONDITIONS];
    int nbPreconds;

    string adds[MAX_CONDITIONS];
    int nbAdd;

    string delete[MAX_CONDITIONS];
    int nbDelete;

    bool complete; // permettra de savoir si on est déjà passé par une condition
} Etape;

// variables globales
int m;
string Start[MAX_CONDITIONS], Finish[MAX_CONDITIONS], Inventory[MAX_INVENTORY]; // Stockage du start, du finish et de l'inventaire
Etape rules[MAX_RULES];
bool peutExecuter;
char c[200]; // On augmente la capacité pour les lignes plus longues 100-->200  
int nEtapes = -1;
int m;

void printinventaire(void)
{ // Fonction qui affiche notre inventaire pour mieux visualiser.
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
    printf("\nAffichage des etapes: \n");
    for (int i = 0; i < nbRules; i++)
    {
        printf("--- ETAPE NUMERO %d ---\n", i + 1);
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
        return true; // Si on a réussi à compléter l'étape

    peutExecuter = true;
    for (int p = 0; p < rules[i].nbPreconds; p++)
    {
        if (!TrouveString(Inventory, MAX_INVENTORY, rules[i].preconds[p]))
        {
            peutExecuter = false; // Si dans notre inventaire il nous manque des préconditions de l'étape, alors cette étape ne peut pas être éxécutée.
        }
    }

    if (peutExecuter) // Si notre inventaire possède tout les preconds de l'étape actuelle
    {
        printf("\n-- EXECUTION DE L'ETAPE : %d --\n", i);

        for (int j = 0; j < rules[i].nbDelete; j++)
        {
            searchRemoveString(Inventory, rules[i].delete[j], MAX_INVENTORY); // On supprime de notre inventaire les "delete" de l'étape.
        }

        for (int k = 0; k < rules[i].nbAdd; k++)
        {
            m = 0;
            while (m < MAX_INVENTORY && Inventory[m][0] != '\0')
                m++; // Trouver une case vide
            if (m < MAX_INVENTORY)
            {
                strcpy(Inventory[m], rules[i].adds[k]); // On ajoute à notre inventaire les adds.
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

    while (fgets(c, 200, Fichier) != NULL) // On passe de 100 à 200 ici aussi
    {
        if (c[0] == '*')
            continue; // Ignorer les astérisques

        if (nEtapes == -1) // On commence à une étape qui existe pas, c'est-à-dire le Start et le Finish qui ne sont pas des étapes.
        {
            if (nLigne == 0)
                parseLine(c, Start); // Récupère le Start
            else if (nLigne == 1)
            {
                parseLine(c, Finish); // Récupère le Finish
            }
            else
            {
                nEtapes = 0;
                nLigne = 0;
                continue;
            }
        }

        if (nEtapes != -1) // On entre dans les étapes, après le Start et le Finish donc, une étape se trouve entre deux lignes d'astérisques et contient "action", "preconds", "add" et "delete".
        {
            if (strncmp(c, "action:", 7) == 0)
            {
                rules[nEtapes].nbActions = parseLine(c, rules[nEtapes].actions); // Récupération des "action" de l'étape actuelle.
            }
            else if (strncmp(c, "preconds:", 9) == 0)
            {
                rules[nEtapes].nbPreconds = parseLine(c, rules[nEtapes].preconds); // Récupération des "preconds" de l'étape actuelle.
            }
            else if (strncmp(c, "add:", 4) == 0)
            {
                rules[nEtapes].nbAdd = parseLine(c, rules[nEtapes].adds); // Récupération des "add" de l'étape actuelle.
            }
            else if (strncmp(c, "delete:", 7) == 0)
            {
                rules[nEtapes].nbDelete = parseLine(c, rules[nEtapes].delete); // Récupération des "delete" de l'étape actuelle.
                rules[nEtapes].complete = false;                               // Au départ, une action n'est jamais compléte.
                nEtapes++;
            }
        }
        nLigne++;
    }
    fclose(Fichier);

    // Initialisation de l'inventaire
    printf("Initialisation Inventaire :\n");
    for (int i = 0; i < MAX_CONDITIONS; i++)
    {
        if (Start[i][0] != '\0')
        {
            strcpy(Inventory[i], Start[i]); // Au départ, notre inventaire est constitué du Start, il faut qu'à la fin notre inventaire soit égal au Finish.
        }
    }

    afficherRules(nEtapes);
    printinventaire();

    // Boucle d'exécution
    bool objectifAtteint = false;
    bool blocage = false; 

    // boucle continue jusqu'à qu'on arrive à executer les instructions ou que celles-ci "bloquent"
    while (!objectifAtteint && !blocage)
    {
        bool actionRealisee = false; // Sert à éviter que le programme cherche pour toujours

        // For multicondition pour qu'on puisse l'arrêter si besoin
        for (int i = 0; i < nEtapes*2 && !objectifAtteint; i++)
        {
            if (!rules[i].complete) // Si on a pas complété les rules
            {
                if (Execution(i)) // Si l'éxecution de cette étape s'est bien déroulée.
                {
                    actionRealisee = true;

                    objectifAtteint = true; // true par défaut
                    m = 0;


                    while (m < MAX_CONDITIONS && Finish[m][0] != '\0' && objectifAtteint)
                    {
                        printf("\n JE VERIFIE SI: [%s] EST DANS L'INVENTAIRE\n", Finish[m]);

                        // Si on ne trouve pas une des conditions finales dans l'inventaire
                        if (!TrouveString(Inventory, MAX_INVENTORY, Finish[m]))
                        {
                            objectifAtteint = false; // Stoppe le while au prochain tour
                        }
                        m++;
                    }

                    if (objectifAtteint)
                    {
                        printf("\n- - EXECUTION COMPLETE! SUCCES! - -\n");
                    }
                }
            }
        }

        // Si on est coincé (aucune règle n'a pu être appliquée ce tour-ci)
        if (!actionRealisee && !objectifAtteint)
        {
            printf("\n! ! ! BLOCAGE : AUCUNE ACTION POSSIBLE. ECHEC ! ! !\n");
            blocage = true; // On sort de la boucle while et on passe à la suite
        }
    }

    printf("\n- - BILAN - -\n");
    printinventaire();

    printf("On voulait au minimum: ");
    for (int i = 0; i < MAX_CONDITIONS; i++) //on affiche tout les pre requis pour l'execution
    {
        if (Finish[i][0] != '\0')
        {
            printf("[%s] ", Finish[i]);
        }
    }
    printf("\n\n");

return 0; // Tradition
}
