//
// Created by Thomas on 22/10/2018.
//
#include "Data.h"

const char* getField(char *line, int num){
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n")) {
        if (!--num)
            return tok;
    }
    return NULL;
}

int getNbLines(FILE* fichier){
    fseek(fichier,SEEK_SET,0);
    int nb_lignes = 0;
    int c;
    while((c=fgetc(fichier)) != EOF) {
        if(c=='\n')
            nb_lignes++;
    }
    return nb_lignes;
}

s_donnees readData(char* path){
    s_donnees _data;
    FILE* stream = fopen(path, "r");
    if(stream == NULL){
        _data.donnees =NULL;
        _data.nb_lignes=0;
        _data.nb_colonnes=0;
        return  _data;
    }
    char line[1024];
    _data.nb_lignes = getNbLines(stream);
    _data.nb_colonnes = 21;
    double* voice_print = malloc(21*sizeof(double));
    double** data = malloc(_data.nb_lignes*sizeof(voice_print));
    int l = 0;
    fseek(stream,SEEK_SET,0);
    while (fgets(line, 1024, stream)){
        for(int i =1; i<_data.nb_colonnes+1;i++){
            char* tmp = strdup(line);
            const char* d = getField(tmp, i);
            //printf("%s;", d);
            sscanf(d, "%lf", &voice_print[i-1]);
            //printf("%lf;",voice_print[i-1]);
            free(tmp);
        }
        //printf("\n\n" );
        data[l] = voice_print;
        voice_print=  malloc(21*sizeof(double));
        l++;
    }
    free(voice_print);
    _data.donnees = data;
    return _data;
}

void libereDelivre(s_donnees donnees){
    for (int i = 0; i < donnees.nb_lignes; ++i)
        free(donnees.donnees[i]);
    free(donnees.donnees);
}

/***
 * Ecrit le coeficient des poids entre les neurones dans un fichier csv.
 * Chaque neurones est écrit sur une ligne,avec son numéro de neurone en entête.
 * Les liaisons inextistante sont représenté par un 0
 * la ligne correspondent au neurone d'entrée.
 * Seul ses liaisons "sortante" sont enregistrée
 * la premiére colonne correspond au premier neurone de la couche caché
 * @param nom_fichier
 * @param tab_neurones
 * @param nb_neurones
 * @param entrant
 */
void ecritCoefCsv(const char* nom_fichier,neurones *tab_neurones,size_t nb_neurones,size_t entrant)
{
    FILE *fp;
    double **tab_coef=calloc(nb_neurones, sizeof(*tab_coef));
    lien * buf_lien;
    int i;
    for( i=0;i<nb_neurones;i++)
    {
        tab_coef[i]=calloc(nb_neurones, sizeof(**tab_coef));
    }
    for(i=0;i<nb_neurones;i++)
    {
        buf_lien=tab_neurones[i].lien_out;
        while(buf_lien != NULL)
        {
            tab_coef[i][buf_lien->id_dest]=buf_lien->poids;
            buf_lien=buf_lien->next_lien_out;
        }
    }
    fp=fopen(nom_fichier,"w");
    for(i=0;i<nb_neurones;i++)
    {
        fprintf(fp,"\n%d",i+1);
        {
            for (int j = (int) entrant; j < nb_neurones; ++j) {
                fprintf(fp,",%f ",tab_coef[i][j]);
            }
        }
    }
    //liberation de la memoire
    for( i=0;i<nb_neurones;i++)
    {
        free(tab_coef[i]);
    }
    free(tab_coef);
    fclose(fp);
}