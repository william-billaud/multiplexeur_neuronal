//
// Created by Thomas on 22/10/2018.
//

#include "menu.h"

int menu (){
    int cod;
    printf("\n\t***************************************************\n");
    printf("\t Ok Google\n");
    printf("\t***************************************************\n");
    printf("\t0 : Fin \n");
    printf("\t1 : Chargez les données depuis un fichier \n");
    printf("\t2 : Apprendre depuis des donnees \n");
    printf("\t3 : Charger une configuration \n");
    printf("\t4 : Trouver à qui appartient une emprunte \n");
    printf("\t5 : Sauvegarder une configuration \n");
    printf("\t***************************************************\n");
    printf("\tVotre choix : ");
    scanf("%d",&cod);
    getchar();
    printf("\n");
    return(cod);
}