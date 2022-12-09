#include "stdio.h"
#include "GPS_TOOLS.h"
#include "string.h"
#include <stdbool.h>

int main(int argc, char** argv) {

    //trame NMEA GGA valide: heure 06:40:36.289, lat= 48.608958°, long = 7.682288°, altitude = 200.2 m
    const char frame0[] = "$GPGGA,064036.289,4836.5375,N,00740.9373,E,1,04,3.2,200.2,M,,,,0000*0E\r\n";
    //trame NMEA RMC valide: heure 05:37:40.00, lat= 25.060532°, long = 121.600165°, vitesse = 2.69 knot
    const char frame1[] = "$GPRMC,053740.000,A,2503.6319,N,12136.0099,E,2.69,79.65,100106,,,A*53\r\n";
    //trame NMEA GGA non valide: heure 06:40:36.289, lat= 48.608958°, long = 7.682288°, altitude = 300.2 m
    const char frame2[] = "$PPGGA,064036.289,4836.5375,N,00740.9373,E,1,04,3.2,300.2,M,,,,0000*0E\r\n";

    const char *findChar(const char *str, const char c, int pos) {
        /*Cette fonction permet de rechercher la première occurrence du caractère « c » dans la chaîne de
        caractères « str » à partir du « pos»ième caractères . Si le caractère « c » est trouvé, la fonction
        retourne le pointeur vers le caractère dans la chaîne « str ». Sinon la fonction renvoie la valeur
        NULL.*/
        char test;
        for (int index = pos; i < strlen(str); index++) {
            test = str[index];
            if (test == c) {
                return (&str[index]);
            };
        };
        return NULL;
    }

    const char *findStr(const char *str, const char *s, int pos) {
        //Le but de trouver la position de premiere lettre du mot puis analyser si il s'agit de celui-ci (avec le warning) sinon on augmente la position de +1 (pas possiblement autrement)
        const char *poslettre;
        int warning = 1;
        while (warning != 0) {
            poslettre = findChar(str, s[0], pos);
            warning = strlen(s);
            if (poslettre == NULL) {
                return NULL;
            }
            for (int index = 0; index < strlen(s); index++) {
                if (*(poslettre + index) == s[index]) {
                    warning--;
                }
            }
            pos++;
        }
        return poslettre;
    }


    int isValidHeader(const char *frame) {
        /*Cette fonction recherche dans la chaîne de caractère « frame » un entête valide de trame NMEA. Si un entête d’un trame GGA est trouvé, elle retourne la valeur XGGA_FRAME. Si l’entête trouvé est de type RMC, elle retourne la valeur XRMC_FRAME, … Si aucun entête valide est trouvé, elle retourne la valeur UNKNOWN_FRAME.*/
        if (findStr(frame, "GPA", 3) != NULL) {
            return (XGGA_FRAME);
        } else if (findStr(frame, "RMC", 3) != NULL) {
            return (XRMC_FRAME);
        } else {
            return (UNKNOWN_FRAME);
        }
    }
    int getFieldsCount(const char *frame) {
        /*Cette fonction compte le nombre de champs que contient la trame NMEA. Les champs d’une trame NMEA sont séparés par une « , » et sont compris entre le « $ » de début de trame et le « * » indiquant le check sum. L’entête de trame est donc le premier champ.*/
        int compt = 1, i;
        char pos = 1;
        while (findChar(frame, '*', pos) != NULL) {
            if (findChar(frame, ',', pos) != NULL) {
                compt++;
                pos = findChar(frame, ',', pos) - &frame[0] + 2;
            } else {
                break;
            }
        }
        return (compt);
    }
//printf("il y a %d field\n",getFieldsCount(frame0));
    char *gotoField(const char* frame, int n){

        /*Cette fonction renvoie le pointeur vers le premier caractère du « n »ieme champs de la trame contenu dans frame. Si le « n »ieme champ n’est pas trouvé, la fonction retour NULL.*/
        int COUNT =0;
        int trame = getFieldsCount(frame);
        if (n > trame){
            return NULL;
        }
        for (int index = 0; frame[index] == '*'; index++){
            if (frame[index] == ',' && frame[index+1] != ','){
                COUNT += 1;
            }

            if (COUNT == n-1){
                return &frame[index+1];
                break;
            }
        }
    }

    char hexToChar(unsigned char value) {
        /* Cette fonction renvoie le caractère ASCII majuscule correspondant à la valeur hexadécimale de
  « value ». « value » doit avoir une valeur entre 0 et 15 sinon le résultat n’est pas garanti.
          Exemples :
  pour value =1 la fonction retourne ‘1’
  pour value =12 la fonction retourne ‘C’*/

        char r;
        if between(value, '0', '15')
        {
            {
                r = printf("%x", value);
            }
            //au début je voulais faire un case, sachant que je voulais mettre un feault comme quoi de 0 à 9 on mettait le même résulat
            /*switch( value ){
                case '10' :
                    r = 'A';
                case '11' :
                    r = 'B';
                case '12' :
                    r = 'C';
                case '13' :
                    r = 'D';
                case '14' :
                    r = 'E';
                case '15' :
                    r = 'F';
                default :
                    r = value;
        */
            else{
                r = NULL;
            }
            return r;
        }
    }

    int computeCheckSum(const char *frame) {
        /*L'objectif de cette fonction est de calculer la valeur numérique du checksum de la trame. Le
        checksum d’un trame NMEA est égale à un XOR (opérateur ^ en C) entre tous les caractères de la
        trame qui sont compris entre le ‘$’ de début de trame et le ‘*’ de délimitation de checksum.
                Exemple :
        la trame $GPGGA,064036.289,4836.5375,N,00740.9373,E,1,04,3.2,200.2,M,,,,0000*0E
        a un checksum égale à 15.*/
  int checksum = 0;

  // Booléen pour indiquer si nous sommes arrivés au '*' de délimitation de checksum
  bool found_delimiter = false;

  // Parcourir la trame caractère par caractère
  while (*frame) {

    if (found_delimiter) {
      break;
    }
    // Si nous trouvons le '$', initialiser le checksum à 0 et continuer
    if (*frame == '$') {
      checksum = 0;
    }
    // Si nous trouvons le '*', indiquer que nous avons trouvé le délimiteur et continuer
    else if (*frame == '*') {
      found_delimiter = true;
    }
    // Si nous trouvons un autre caractère, mettre à jour le checksum en utilisant un XOR
    else {
      checksum ^= *frame;
    }
    frame++;
  }

  return checksum;
}


    }
    return 0;
    }
