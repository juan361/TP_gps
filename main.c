#include "stdio.h"
#include "GPS_TOOLS.h"
#include <string.h>
#include <stdbool.h>


int main(int argc, char** argv) {

    //trame NMEA GGA valide: heure 06:40:36.289, lat= 48.608958°, long = 7.682288°, altitude = 200.2 m
    const char frame0[] = "$GPGGA,064036.289,4836.5375,N,00740.9373,E,1,04,3.2,200.2,M,,,,0000*0E\r\n";
    //trame NMEA RMC valide: heure 05:37:40.00, lat= 25.060532°, long = 121.600165°, vitesse = 2.69 knot
    const char frame1[] = "$GPRMC,053740.000,A,2503.6319,N,12136.0099,E,2.69,79.65,100106,,,A*53\r\n";
    //trame NMEA GGA non valide: heure 06:40:36.289, lat= 48.608958°, long = 7.682288°, altitude = 300.2 m
    const char frame2[] = "$PPGGA,064036.289,4836.5375,N,00740.9373,E,1,04,3.2,300.2,M,,,,0000*0E\r\n";

    // QUESTION 3
    const char *findChar(const char *str, const char c, int pos) {
        /*Cette fonction permet de rechercher la première occurrence du caractère « c » dans la chaîne de
        caractères « str » à partir du « pos»ième caractères . Si le caractère « c » est trouvé, la fonction
        retourne le pointeur vers le caractère dans la chaîne « str ». Sinon la fonction renvoie la valeur
        NULL.*/
        char test;
        int i;
        for (int index = pos; i < strlen(str); index++) {
            test = str[index];
            if (test == c) {
                return (&str[index]);
            };
        };
        return NULL;
    }
    
    // QUESTION 4
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
            return r;
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

	// QUESTION 12 (pas fini)
		void printGpsData( const gps_data *data){
		
		char  gps_frame_type[64];
		
		printf("\n----------------------\naffichage de la Frame sélectionnée\n----------------------\n");
		switch( data.is_valid ){
		case '1' :
			strcpy(gps_frame_type, "XRMC_FRAME");
			break;
		case '2' :
			strcpy(gps_frame_type, "XGGA_FRAME");
			break;
		case '3' :
			strcpy(gps_frame_type, "XGLL_FRAME");
			break;
		case '4' :
			strcpy(gps_frame_type, "XGNS_FRAME");
			break;
 	
		}
		
		printf( "Frame de type %s  datée  ( H/M/S ) %02d%02d%02.5f \n", gps_frame_type,data.hours,data.minutes,data.secondes);
		
		
	}
	
	

}
return 0;
}
