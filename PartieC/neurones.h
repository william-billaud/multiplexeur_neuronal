//
// Created by william on 15/10/18.
//

#ifndef RNA_NEURONES_H
#define RNA_NEURONES_H

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <error.h>
#include <stdio.h>


struct neurones;
struct lien;
typedef struct neurones {
    int id_neurone; //Id unique du neurone pour l'identifier plus facilement->position dans le tableau
    double biais; //valeur du biais en entrée
    double (*fct_tranfere)(double); //valeur de la fonction de transfère de la sortie
    struct lien *lien_in; //liste chainé des connexions entrante
    struct lien *lien_out; //liste chainé des connexions sortante
    double derniereSortie;
} neurones;

typedef struct lien {
    double poids;
    int id_origine;//id du neurones d'origine
    int id_dest; //id du neurone de destination
    struct lien *next_lien_in; //prochain lien d'entrée pour le neurone de destination
    struct lien *next_lien_out;//prochain lien de sortie pour le neurone d'origine.
} lien;


lien *creeLien(int id_in, int id_out);

neurones *creeTabNeurone(size_t nb_entrant, size_t nb_cache, size_t nb_sortie);

int initTabNeurone(neurones *tab_neurones, size_t nb_entrant, size_t nb_cache, size_t nb_sortie,
                   double (*fct_transfere)(double));

int propage(neurones *tab_neurones, int nb_entree, int nb_cache,int nb_sortie);

int libereLien(neurones *tab_neurones, int nb_neurones);

double calculErreurQuadratiqueMoy(neurones *tab_neurones,const double *tab_prev,int premier_sortie, int nb_sortie);
        double sigmoide(double x);
int initInput(neurones *tab_neurone, double *tab_entree ,int nb_entree);

int ajustePoids(neurones *tab_neurones, const double *tab_prev, size_t entre, size_t cache, size_t sortie, double eta) ;
#endif //RNA_NEURONES_H