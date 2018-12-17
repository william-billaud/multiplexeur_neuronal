#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Data.h"
#include "neurones.h"
#include "menu.h"


#define entrant 20
#define cache 10
#define sortant 4
#define vitesseApprentissage 0.10

int main(int argc, char *argv[]) {
    double erreur_quadra;
    double max_erreur_quadra;
    int count = 0;
    int valeur_lue;
    double *tab_attendu;
    bool fini = false;
    s_donnees data;
    neurones *tab;
    tab_attendu = calloc(sortant, sizeof(*tab_attendu));
    for (int i = 0; i < sortant; i++) {
        tab_attendu[i] = 0.10;
    }
    tab = creeTabNeurone(entrant, cache, sortant);
    while (fini == false) {
        valeur_lue = menu();
        switch (valeur_lue) {
            case 0 :
                fini = true;
                break;
            case 1 :
                if (argc != 2) {
                    printf("Nombre d'argument invalide");
                } else {

                    printf("nom fichier : %s \n", argv[1]);
                    data = readData(argv[1]);
                    if (NULL == data.donnees) {
                        fini = 0;
                        perror("erreur de lecture du fichier");
                        break;
                    }
                    printf("nb ligne %d, nb colonne %d \n", data.nb_lignes, data.nb_colonnes);
                    for (int i = 0; i < data.nb_lignes; i++) {
                        for (int j = 0; j < data.nb_colonnes; j++) {
                            printf("%lf;", data.donnees[i][j]);
                        }
                        printf("\n");
                    }
                }

                printf("Ce programme cherche à étudier les reseaux neuroneaux!!! \n");

                initTabNeurone(tab, entrant, cache, sortant, sigmoide);


                break;
            case 2 :
                count = 0;

                do {
                    max_erreur_quadra = 0;
                    for (int k = 0; k < data.nb_lignes; k++) {
                        initInput(tab, data.donnees[k], entrant);
                        tab_attendu[(int) round(data.donnees[k][entrant]) - 1] = 1;
                        propage(tab, entrant, cache, sortant);
                        printf(" personne : %f\n", data.donnees[k][entrant]);
                        printf("sortie : \n \t %.8f\t %.8f\t %.8f\t %.8f\t \n", tab[entrant + cache].derniereSortie,
                               tab[entrant + cache + 1].derniereSortie, tab[entrant + cache + 2].derniereSortie,
                               tab[entrant + cache + 3].derniereSortie);
                        printf("Attendu : \n \t %.8f\t %.8f\t \t %.8f\t %.8f\t \n", tab_attendu[0], tab_attendu[1],
                               tab_attendu[2], tab_attendu[3]);

                        erreur_quadra = calculErreurQuadratiqueMoy(tab, tab_attendu, entrant + cache, sortant);
                        if (erreur_quadra > 0.05) {
                            printf("changement des poids! \n");
                            ajustePoids(tab, tab_attendu, entrant, cache, sortant, vitesseApprentissage);
                        }
                        tab_attendu[(int) round(data.donnees[k][entrant]) - 1] = 0.10;
                        //tab_attendu[(int) round(data.donnees[k][entrant])-1] = 0.00;
                        max_erreur_quadra = max_erreur_quadra < erreur_quadra ? erreur_quadra : max_erreur_quadra;
                        printf("erreur : %f \n", erreur_quadra);
                    }
                    count++;
                    //verifier erreur quadratique sur tous
                } while ((count < 25) && max_erreur_quadra > 0.05);


                break;
            case 3 :
                break;
            case 4 :
                break;
            case 5:
                printf("Sauvegarde de la configuration!!");
                ecritCoefCsv("coef.csv", tab, entrant + cache + sortant, entrant);
                break;
            default:
                break;
        }
    }
    //liberation des données utilisé
    if (tab != NULL) {
        libereLien(tab, entrant + cache + sortant);
        free(tab);
    }
    libereDelivre(data);

    exit(0);
}