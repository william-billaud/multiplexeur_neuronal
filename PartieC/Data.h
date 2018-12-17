//
// Created by Thomas on 22/10/2018.
//

#ifndef RNA_READDATA_H
#define RNA_READDATA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "neurones.h"

typedef struct s_donnees
{
    double** donnees;
    int nb_lignes;
    int nb_colonnes;
}s_donnees;

const char* getField(char *line, int num);
int getNbLines(FILE* fichier);
s_donnees readData(char* path);
void libereDelivre(s_donnees donnees);
void ecritCoefCsv(const char* nom_fichier,neurones *tab_neurones,size_t nb_neurones,size_t entrant);
#endif //RNA_READDATA_H
