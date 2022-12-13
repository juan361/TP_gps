#include "GPS_TOOLS.h"
#include "stdio.h"
#include "stdlib.h"


void PrintGpsData(const gps_data *data) {
    //réalisé par Amara Ranya et El Belkasmi Meriem
    printf("################### AFFICHAGE DE L'ENSEMBLE DES INFOS CONTENUS DANS LA STRUCTURE DATA");
    printf("frame : %s", data->frame);
    printf("type de frame : %d", data->frame_type); //permet l'affichage du type de "FRAME"
    printf("validite de frame : %d", data->is_valid); //permet dedonner une idee sur la validite de frame
    printf("year : %d", data->year); //affichage de lannee
    printf("month : %d", data->month);
    printf("day : %d", data->day);
    printf("hours : %d", data->hours);
    printf("minutes : %d", data->minutes);
    printf("seconde : %ld", data->seconde);
    printf("latitude : %ld",
           data->latitude); // en degres, latitude nord devra etre positive alors que celle de sud devra etre negative
    printf("longitude : %ld",
           data->longitude); // en degres, longitude est devra etre positive alors que celle de ouest devra etre negative
    printf("altitude : %ld", data->altitude);
}

