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

#include "stdio.h"
#include "GPS_TOOLS.h"
#include <string.h>
#include <stdbool.h>
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
}
GPS_DATA;


    // QUESTION 3
    const char *findChar(const char *str, const char c, int pos) {
        /*Cette fonction permet de rechercher la première occurrence du caractère « c » dans la chaîne de
        caractères « str » à partir du « pos»ième caractères . Si le caractère « c » est trouvé, la fonction
        retourne le pointeur vers le caractère dans la chaîne « str ». Sinon la fonction renvoie la valeur
        NULL.*/
        char test;							//Déclaration d'une variable du type charactère
        int i;								//Déclaration d'une variable du type entier
        for (int index = pos; i < strlen(str); index++) {		//Début d'une boucle 'pour' avec un index à remplir qu'on incrémente a chaque fois
            test = str[index];						//Test récupère la valeur de str[index]
            if (test == c) {						//Début d'une boucle 'si' ou l'on vérifi que test soit égale à C
                return (&str[index]);					//Retour de l'adresse de la variable str[index]
            };								//Fin de la boucle 'si'
        };								//Fin de la boucle 'pour'
        return NULL;							//Retour null
    }
    
    // QUESTION 4
    const char *findStr(const char *str, const char *s, int pos) {
        //Le but de trouver la position de premiere lettre du mot puis analyser si il s'agit de celui-ci (avec le warning) sinon on augmente la position de +1 (pas possiblement autrement)
        const char *poslettre;						//Déclaration d'un pointeur poslettre
        int warning = 1;						//Déclaration d'un entier égal à 1
        while (warning != 0) {						//Début d'une boucle 'tant que' warning est pas égal à 0 on fait notre boucle
            poslettre = findChar(str, s[0], pos);			//poslettre pointe l'adresse de findChar
            warning = strlen(s);					//warning prend la valeur de strlen(s)
            if (poslettre == NULL) {					//Début d'une boucle 'si' qui vérifie si poslettre est égal à null
                return NULL;						//On retourne null
            }								//Fin de la boucle 'si'
            for (int index = 0; index < strlen(s); index++) {		//Début d'une boucle 'pour' dans la qu'elle on déclare un entier index qui est égal à zéro qu'on incrémente à chaque fois qu'on 									utilise la boucle
                if (*(poslettre + index) == s[index]) {			//Début de la boucle 'si' dans la qu'elle on additionne poslettre et index qu'on compare avec s[index]
                    warning--;						
                }							//Fin de la boucle 'si'
            }								//Fin de la boucle 'pour'
            pos++;							//On incrémente pos
        }								//Fin de la boucle 'tant que'
        return poslettre;						//Retour poslettre
    }

    // QUESTION 5
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
    
    // QUESTION 6
    int getFieldsCount(const char *frame){
        /*Cette fonction compte le nombre de champs que contient la trame NMEA. Les champs d’une trame NMEA sont séparés par une « , » et sont compris entre le « $ » de début de trame et le « * » indiquant le check sum. L’entête de trame est donc le premier champ.*/
        int compt=1,i;
        char pos=1;
        while (findChar(frame,'*',pos)!=NULL)
        {
            if (findChar(frame,',',pos)!=NULL)
            {
                compt++;
                pos=findChar(frame,',',pos)-&frame[0]+2;
            }
            else
            {
                break;
            }
        }
        return(compt);
    }
    
    // QUESTION 7
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

    // QUESTION 8
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
            return r; // on retourne r
        }
    }

    // QUESTION 9
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
    
    // QUESTION 10
    int isValidGpsData( const char *str_signal)
	{
		
		//test si l'entete est correcte
		int entete;
		entete = isValidHeader(const char *str_signal);
		return entete if entete == UNKNOWN_FRAME;
		
		
		
		// test la longeur maximale de 82 caractères
		if (strlen ( str_signal) > 82){
			return UNKNOWN_FRAME;
			}
		
		
		// test le nombre de champs 
		int nb_champs;
		nb_champs = getFieldsCount (const char *str_signal);
		if ( nb_champs < 11 || nb_champs > 15){
			return UNKNOWN_FRAME;
		}
		
		// test la checksum
		int sum_bin;
		sum_bin = computeCheckSum(const char *str_signal);
		
		// lit la sum dans le message , les deux caractères suivant *
		
		char * ptr = findChar(const char *str_signal,'*',0);
		return if ptr == NULL;		
		
		char sum_char[3];
		sum_char[0] = *ptr;
		sum_char[1] = *ptr+1
		sum_char[2] = 0;// pour terminer la chaine de caracteres 
		
		if ( sum_char[0] != hexToChar(sum_bin/16) || sum_char[1] != hexToChar(sum_bin%16) ){
			return UNKNOWN_FRAME;
		}
		
	
		return entete;
	
	}
	// QUESTION 11 (pas fini)
		char *getField( char *str) {
		char field[64];
		int ii=0;
		while ( *(str+ii) != ','){
			field[ii]= *(str+ii);	
			++ii;
		}
		*(str+ii)=0;
		
		return field;
	}
	
	
	
	int  extractGpsData(const char *frame , gps_data *data)
	{
		char buffer[64];
		char nombre_char[64];
		
		int entete = isValidGpsData( const char *frame);
		return entete if entete == UNKNOWN_FRAME;
		
		data.frame = frame;
		data.is_valid = entete;
	// récupération de l'heure dans buffer;
		strcpy( buffer, getField( gotoField( frame , 2) ));
		data.year = 0;
		data.month = 0;
		data.day = 0;
		//traitement des heures
		nombre_char[0] = buffer[0];
		nombre_char[1] = buffer[1];
		nombre_char[2] = 0;
		data.hours =  atoi(nombre_char);
		nombre_char[0] = buffer[2];
		nombre_char[1] = buffer[3];
		nombre_char[2] = 0;
		data.minutes =  atoi(nombre_char);
		data.secondes =  atof(buffer[4]);
		
	// récupération de la latitude  dans buffer;
		strcpy( buffer, getField( gotoField( frame , 3) ));
	// degres/minute => minute on divise par 60
	// récupération de la direction Si N alors *(1) Si S alors *(-1)
		strcpy( buffer, getField( gotoField( frame , 4) ));		
				
		
		return entete;
	}

	// récupération de la longitude  dans buffer;
		strcpy( buffer, getField( gotoField( frame , 5) ));
	// degres/minute => minute on divise par 60
	// récupération de la direction Si E alors *(1) Si O alors *(-1)	
		strcpy( buffer, getField( gotoField( frame , 6) ));
			
			
		
		return entete;
	}
	
	// récupération de l'altitude dans buffer;
		strcpy( buffer, getField( gotoField( frame , 10) ));
			
			
		
		return entete;
	}



#endif //doit etre la derniere ligne du fichier
