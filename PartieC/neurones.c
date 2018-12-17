//
// Created by william on 15/10/18.
//


#include "neurones.h"

/***
 *
 * @param nb_entrant
 * @param nb_cache
 * @param nb_sortie
 * @return
 */
neurones *creeTabNeurone(size_t nb_entrant, size_t nb_cache, size_t nb_sortie) {
    if (!(nb_entrant > 0 && nb_cache > 0 && nb_sortie > 0)) {
        return NULL;
    }
    return (neurones *) calloc(nb_entrant + nb_cache + nb_sortie, sizeof(neurones));
}

/***
 *
 * @param id_in
 * @param id_out
 * @return
 */
lien *creeLien(int id_in, int id_out) {
    lien *nouveau_lien = calloc(1, sizeof(lien));
    nouveau_lien->id_dest = id_out;
    nouveau_lien->id_origine = id_in;
    nouveau_lien->next_lien_out = NULL;
    nouveau_lien->next_lien_in = NULL;
    return nouveau_lien;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc30-c"
/***
 *
 * @param tab_neurones
 * @param nb_entrant
 * @param nb_cache
 * @param nb_sortie
 * @param fct_transfere
 * @return
 */
int initTabNeurone(neurones *tab_neurones, const size_t nb_entrant, const size_t nb_cache, const size_t nb_sortie,
                   double (*fct_transfere)(double)) {

    lien *dernier_lien = NULL;
    srand((unsigned int) time(NULL));
    //Initialisation de la première couche de neurone (couche des entrée)
    for (int i = 0; i < nb_entrant; ++i) {
        tab_neurones[i].id_neurone = i;
        tab_neurones[i].fct_tranfere = NULL;
        tab_neurones[i].biais = 0;
        tab_neurones[i].derniereSortie = 0;
        tab_neurones[i].lien_out=NULL;
        tab_neurones[i].lien_in=NULL;

    }
    //Initaliasation de la couche du milieu
    for (int i = (int) nb_entrant; i < (nb_entrant + nb_cache); i++) {
        tab_neurones[i].id_neurone = i;
        tab_neurones[i].fct_tranfere = fct_transfere;
        tab_neurones[i].biais = (((float) rand()) / ((float) RAND_MAX) * (1.f)) - 0.5f;
        for (int j = 0; j < nb_entrant; j++) {
            dernier_lien = creeLien(j, i);
            dernier_lien->poids =
                    (((float) rand()) / ((float) RAND_MAX) * (1.f)) - 0.5f; //génère un poids aléatoire en -5
            dernier_lien->next_lien_in = tab_neurones[i].lien_in;
            dernier_lien->next_lien_out = tab_neurones[j].lien_out;
            tab_neurones[i].lien_in = dernier_lien;
            tab_neurones[j].lien_out = dernier_lien;
        }
    }

    //Initalisation de la couche exterieur et du lien avec la couche du milieu
    for (int i = (int) (nb_entrant + nb_cache); i < (nb_entrant + nb_cache + nb_sortie); i++) {
        tab_neurones[i].id_neurone = i;
        tab_neurones[i].fct_tranfere = fct_transfere;
        tab_neurones[i].lien_out = NULL;
        tab_neurones[i].biais =
                (((float) rand()) / ((float) RAND_MAX) * (1.f)) - 0.5;
        for (int j = (int) nb_entrant; j < nb_entrant+nb_cache; j++) {
            dernier_lien = creeLien(j, i);
            dernier_lien->poids =
                    (((float) rand()) / ((float) RAND_MAX) * (1.f)) - 0.5; //génère un poids aléatoire en -5
            dernier_lien->next_lien_in = tab_neurones[i].lien_in;
            dernier_lien->next_lien_out = tab_neurones[j].lien_out;
            tab_neurones[j].lien_out = dernier_lien;
            tab_neurones[i].lien_in = dernier_lien;
        }
    }
    return 0;
}
#pragma clang diagnostic pop

/***
 * fait fonctionner le RNA une fois (en faisant propager vers l'avant);
 * @param tab_neurones
 * @param nb_neurones
 * @return
 */
int propage(neurones *tab_neurones, int nb_entree, int nb_cache, int nb_sortie) {
    lien *lien_actuel;
    double sum;
    double res;
    for (int i = nb_entree; i < (nb_entree + nb_cache + nb_sortie); ++i) {
        sum = tab_neurones[i].biais;
        lien_actuel = tab_neurones[i].lien_in;
        while (lien_actuel != NULL) {
            sum += lien_actuel->poids * tab_neurones[lien_actuel->id_origine].derniereSortie;
            lien_actuel = lien_actuel->next_lien_in;
        };
        res = tab_neurones[i].fct_tranfere(sum);
        tab_neurones[i].derniereSortie = res;
    }
    return 0;
}

/***
 *
 * @param tab_neurones tableau contenant l'ensemble des neurones
 * @param premier_sortie indice du premier neurone corresepondant à la couche de sortie
 * @param nb_sortie nb de neurone sur le couche de sortie
 * @return
 */
double calculErreurQuadratiqueMoy(neurones *tab_neurones, const double *tab_prev, int premier_sortie, int nb_sortie) {

    double sum = 0;
    for (int i = 0; i < nb_sortie; ++i) {
        sum += (tab_neurones[premier_sortie + i].derniereSortie - tab_prev[i]) *
              (tab_neurones[premier_sortie + i].derniereSortie - tab_prev[i]);
    }
    return sum / (2 * nb_sortie);
}

/***
 * Fonction libérant tous les lien alloué entre les neurones.
 *
 * @param tab_neurones tableau contenant les neurones
 * @param nb_neurones nombre de neurones
 * @return
 */
int libereLien(neurones *tab_neurones, int nb_neurones) {
    lien *lien_actuel;
    lien *lien_actuel_tmp;
    for (int i = 0; i < nb_neurones; ++i) {
        lien_actuel = tab_neurones[i].lien_in;
        while (lien_actuel != NULL) {
            lien_actuel_tmp = lien_actuel;
            lien_actuel = lien_actuel_tmp->next_lien_in;
            free(lien_actuel_tmp);
        };
    }
    return 0;
}

double sigmoide(double x) {
    return 1 / (1 + exp(x));
}

/***
 *
 * @param tab_neurone
 * @param tab_entree tableau contenant les valeurs à mettre en entrée. Doit etre de taile nb_entree
 * @param nb_entree
 * @return
 */
int initInput(neurones *tab_neurone, double tab_entree[], int nb_entree) {
    for (int i = 0; i < nb_entree; i++) {
        tab_neurone[i].derniereSortie = tab_entree[i];
    }
    return 0;
}

int ajustePoids(neurones *tab_neurones, const double *tab_prev, size_t entre, size_t cache, size_t sortie, double eta) {

    double sum_delta;
    double * delta=calloc(entre+cache+sortie, sizeof(double));
    double derniere_sortie;
    lien * buf_lien;
    //calcul delta couche sortie
    for (int i=0;i<sortie;i++)
    {
        derniere_sortie=tab_neurones[entre+cache+i].derniereSortie;
        delta[entre+cache+i]=(tab_prev[i]-derniere_sortie)*(derniere_sortie)*(1.f-derniere_sortie);
    }

    //calcul detla autre couche
    for (int j = (int) entre; j < entre + cache ; ++j) {
        derniere_sortie=tab_neurones[j].derniereSortie;
        sum_delta=0;
        buf_lien=tab_neurones[j].lien_out;
        while(buf_lien != NULL)
        {
            sum_delta+= delta[buf_lien->id_dest]*buf_lien->poids;
            buf_lien=buf_lien->next_lien_out;
        }
        delta[j]=derniere_sortie*(1-derniere_sortie)*sum_delta;
    }
    //mise à jour du poids, pour le biais, yi =-1
    for (int k = (int) entre; k < entre + cache + sortie ; ++k) {
        buf_lien=tab_neurones[k].lien_in;
        while(buf_lien != NULL)
        {
            buf_lien->poids=buf_lien->poids+eta*delta[k]*(tab_neurones[buf_lien->id_origine].derniereSortie);
            buf_lien=buf_lien->next_lien_in;
        }
        //Pour le biais Yi =-1;
        tab_neurones[k].biais=tab_neurones[k].biais+eta*(-1.f)*delta[k];

    }
    free(delta);
    return 0;
}
