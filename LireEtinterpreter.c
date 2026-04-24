#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// constantes pr les tableaux
       
#define MAX_CONDITIONS 10    
#define MAX_INVENTORY 25    
#define MAX_RULES 25        

//typedefs
typedef char string[1024];

typedef struct etape {
    string actions[MAX_CONDITIONS];
    int nbActions;
    string preconds[MAX_CONDITIONS];
    int nbPreconds;
    string adds[MAX_CONDITIONS];
    int nbAdd;
    string delete[MAX_CONDITIONS];
    int nbDelete;
    bool complete;
} Etape;

// variables globales
int m;
string Start[MAX_CONDITIONS], Finish[MAX_CONDITIONS], Inventory[MAX_INVENTORY];
Etape rules[MAX_RULES]; 
bool peutExecuter;
char c[100]; 
int nEtapes = -1;

void printinventaire(void) {
    printf("Dans l'inventaire on a: ");
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (Inventory[i][0] != '\0') { // si la case n'est pas vide
            printf("[%s] ", Inventory[i]);
        }
    }
    printf("\n");
}

bool TrouveString(string t[], int n, string targ) {
    for (int i = 0; i < n; i++) {
        if (t[i][0] != '\0' && strcmp(t[i], targ) == 0) {
            return true;
        }
    }
    return false;
}

int searchRemoveString(string source[], string target, int n) {
    for (int i = 0; i < n; i++) {
        if (strcmp(source[i], target) == 0) {
            source[i][0] = '\0'; // Supprimer en rendant la chaine vide
            return 0;
        }
    }
    printf("! ! ! Pas trouvé [%s]! Erreur ! ! !\n", target);
    return -1;
}

void afficherRules(int nbRules) {
    printf("\n=== AFFICHAGE DES REGLES ===\n");
    for (int i = 0; i < nbRules; i++) {
        printf("--- REGLE %d ---\n", i + 1);
        printf("  Actions: ");
        for (int j = 0; j < rules[i].nbActions; j++) printf("[%s] ", rules[i].actions[j]);
        printf("\n  Preconds: ");
        for (int j = 0; j < rules[i].nbPreconds; j++) printf("[%s] ", rules[i].preconds[j]);
        printf("\n  Adds: ");
        for (int j = 0; j < rules[i].nbAdd; j++) printf("[%s] ", rules[i].adds[j]);
        printf("\n  Deletes: ");
        for (int j = 0; j < rules[i].nbDelete; j++) printf("[%s] ", rules[i].delete[j]);
        printf("\n");
    }
}

int parseLine(char source[], string cible[]) {
    int i = 0, n = 0;

    while (source[i] != '\0' && source[i] != ':') i++;
    if (source[i] == ':') i++; // Avancer après ':'

    int j = i;
    while (source[i] != '\n' && source[i] != '\0') {
        if (source[i] == ',') {
            memcpy(cible[n], &source[j], i - j);
            cible[n][i - j] = '\0'; 
            n++;
            j = i + 1;
        }
        i++;
    }
    return n;
}

bool Execution(int i) {
    if (rules[i].complete) return true; // Déjà exécutée

    peutExecuter = true;
    for (int p = 0; p < rules[i].nbPreconds; p++) {
        if (!TrouveString(Inventory, MAX_INVENTORY, rules[i].preconds[p])) {
            peutExecuter = false;
        }
    }

    if (peutExecuter) {
        printf("\n---- EXECUTION RULE %d ----\n", i);
        
        for (int j = 0; j < rules[i].nbDelete; j++) {
            searchRemoveString(Inventory, rules[i].delete[j], MAX_INVENTORY);
        }
        
        for (int k = 0; k < rules[i].nbAdd; k++) {
            m = 0;
            while (m < MAX_INVENTORY && Inventory[m][0] != '\0') m++; // Trouver une case vide
            if (m < MAX_INVENTORY) {
                strcpy(Inventory[m], rules[i].adds[k]);
            } else {
                printf("Attention! Erreur! Tableau deborde \n");
            }
        }
        
        rules[i].complete = true; 
        printinventaire();
        return true;
    }
    return false; // Impossible d'exécuter pour l'instant
}

int main(void) {
    FILE *Fichier = fopen("Hello.txt", "r");
    if (!Fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    int nLigne = 0;
    

    while (fgets(c, 100, Fichier) != NULL) {
        if (c[0] == '*') continue; // Ignorer les astérisques

        if (nEtapes == -1) {
            if (nLigne == 0) parseLine(c, Start);
            else if (nLigne == 1) parseLine(c, Finish);
            else { nEtapes = 0; nLigne = 0; continue; }
        }
        
        if (nEtapes != -1) {
            if (strncmp(c, "action:", 7) == 0) {
                rules[nEtapes].nbActions = parseLine(c, rules[nEtapes].actions);
            } else if (strncmp(c, "preconds:", 9) == 0) {
                rules[nEtapes].nbPreconds = parseLine(c, rules[nEtapes].preconds);
            } else if (strncmp(c, "add:", 4) == 0) {
                rules[nEtapes].nbAdd = parseLine(c, rules[nEtapes].adds);
            } else if (strncmp(c, "delete:", 7) == 0) {
                rules[nEtapes].nbDelete = parseLine(c, rules[nEtapes].delete);
                rules[nEtapes].complete = false;
                nEtapes++; 
            }
        }
        nLigne++;
    }
    fclose(Fichier);

    // Initialisation de l'inventaire
    printf("=== INIT INVENTAIRE ===\n");
    for (int i = 0; i < MAX_CONDITIONS; i++) {
        if (Start[i][0] != '\0') {
            strcpy(Inventory[i], Start[i]);
        }
    }

    afficherRules(nEtapes); 
    printinventaire();

    // Boucle d'exécution
    bool succesGlobal = false;
    while (!succesGlobal) {
        succesGlobal = true;
        bool actionRealisee = false; // Sécurité anti boucle-infinie

        for (int i = 0; i < nEtapes; i++) {
            if (!rules[i].complete) {
                if (Execution(i)) {
                    actionRealisee = true;
                } else {
                    succesGlobal = false; // Il reste des règles non complétées
                }
            }
        }

        if (!succesGlobal && !actionRealisee) {
            printf("\n! ! ! AUCUNE ACTION POSSIBLE. FIN ! ! !\n");
            break;
        }
    }

    return 0;
}
