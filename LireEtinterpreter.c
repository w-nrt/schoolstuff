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



int m; // utilisé plus tard pour trouver un endroit ou ranger dans le tableau
string Start[10], Finish[10], Inventory[10];
char ole[100];
Etape rules[10];
bool peutExecuter;
char c[100]; // stocker le contenu de la ligne, 100 est le maxium de caracteres
int nLigne = 0, nEtapes = -1;
bool Succes;
bool notEmpty = true;

void printinventaire(void){
printf("Dans l'inventaire on a: ");
for (int i = 0; i<10; i++){
printf("[%s], ", Inventory[i]);
}
printf("\n\n");
}

bool TrouveString(string t[], int n, string targ){
for (int i = 0; i<n; i++){

	if (t[i][0] != '\0'){

		if (strcmp(t[i], targ) == 0){
			printf("%s est dans l'inventaire\n", targ);
			return true;
			
		}
	}

}
printf("%s pas dans l'inventaire\n", targ);
return false; // on a jamais trouvé le string et on sort de la boucle
}

void ViderEtape (int x){
		rules[x].nbActions = -1;
		rules[x].nbPreconds = -1;
		rules[x].nbAdd = -1;
		rules[x].nbDelete = -1;
}
	
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
        printf("! ! ! Pas trouvé %s! Erreur! ! ! !\n", target);
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

bool Execution(int i){

	if (rules[i].nbAdd == -1) return true; // si l'etape a deja été executée
    printf("\n----LOOP NUMERO %d ----\n\n", i);
    	peutExecuter = true;
    	for (int p=0; p < rules[i].nbPreconds; p++){
    		if (!TrouveString(Inventory, 10, rules[i].preconds[p])) peutExecuter = false;  // si au moins 1 precond est invalide alors on ne peut pas continuer 
    	}
    	if (peutExecuter){
        for (int j = 0; j < rules[i].nbDelete; j++) // j = delete 
        {
                printf("On supprime [%s] de l'inventaire...\n", rules[i].delete[j]);
                searchRemoveString(Inventory, rules[i].delete[j], 10); // supprimer le delete de l'inventaire
        }
        	for (int k=0; k<rules[i].nbAdd; k++){ // tout les adds de x a nbadd
			printf("On veut ajouter [%s] a l'inventaire\n", rules[i].adds[k]);
			m=0;
			while (m!=11 && Inventory[m][0] != '\0') m++; //on navigue jusqua un endroit vide dans le tableau
			if (m==11) printf("Attention! Erreur! Tableau deborde \n");
			strcpy(Inventory[m], rules[i].adds[k]);
        	}
        	ViderEtape(i);
        	printinventaire();
        	}else{
        	printf("! ! ! On ne peut pas executer l'action pour la rule suivante: %d \n Les preconditions ne sont pas remplies. ! ! !\n", i);
        	return false;
        	}
	return true;


}

int main(void)
{
FILE *Fichier = fopen("Hello.txt", "r");
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


    // on fait l'inventaire

    for (int i = 0; i < 10; i++)
    {
        if (Start[i][0] != '\0') // qui n'est pas vide
        {
            strcpy(Inventory[i], Start[i]); // est copié dans l'inventaire
            printf("J'ajoute: [%s] a l'inv\n", Start[i]);
        }
        printf("\n");
    }

afficherRules(6);
printinventaire();
    // maintenant on regarde chaque delete et on l'enlève de l'inventaire, et chaque add et on l'ajoute à l'inventaire, pour chaque règle

    while (!Succes){
   	 Succes = true;
   	 
    	for (int i = 0; i < nEtapes; i++){ // i = nombre etape
    	if (!(Execution(i))) Succes = false;
   	 }
   	 printinventaire();
    }


    fclose(Fichier);
}
