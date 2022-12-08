#ifndef GPS_TOOLS_H
#define GPS_TOOLS_H

/**
* \name       GPS_TOOLS
* \brief      Ensemble de fonction permettant l'extraction de coordonees et 
*             de calcul gps. Ce fichier se fonde sur la transmission de type
*             NMEA0183 XGGA ou XRMC:
*                (https://fr.wikipedia.org/wiki/NMEA_0183)
*                (https://gpsd.gitlab.io/gpsd/NMEA.html)
* \author     ???
* \version    V1.0
* \date       2022/11/21
**/        


#include "math.h"

//declarez vos fonctions ici

typedef enum
{
    UNKNOWN_FRAME = 0,
    XRMC_FRAME,
    XGGA_FRAME,
    XGLL_FRAME,
    XGNS_FRAME
} GPS_FRAME_TYPE;

typedef struct
{
    char *frame;
    int frame_type;
    int is_valid;
    int year, month, day, hours, minutes;
    double seconds;
    double latitude;
    double longitude;
    double altitude;
} GPS_DATA;


#endif //doit etre la derniere ligne du fichier
