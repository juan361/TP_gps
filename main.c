#include "stdio.h"
#include "GPS_TOOLS.h"
#include "string.h"


int main(int argc, char** argv)
{
    
    //trame NMEA GGA valide: heure 06:40:36.289, lat= 48.608958°, long = 7.682288°, altitude = 200.2 m
    const char frame0[]="$GPGGA,064036.289,4836.5375,N,00740.9373,E,1,04,3.2,200.2,M,,,,0000*0E\r\n";
    //trame NMEA RMC valide: heure 05:37:40.00, lat= 25.060532°, long = 121.600165°, vitesse = 2.69 knot
    const char frame1[]="$GPRMC,053740.000,A,2503.6319,N,12136.0099,E,2.69,79.65,100106,,,A*53\r\n";
    //trame NMEA GGA non valide: heure 06:40:36.289, lat= 48.608958°, long = 7.682288°, altitude = 300.2 m
    const char frame2[]="$PPGGA,064036.289,4836.5375,N,00740.9373,E,1,04,3.2,300.2,M,,,,0000*0E\r\n";

    const char *findChar(const char *str, const char c, int pos){
        /*Cette fonction permet de rechercher la première occurrence du caractère « c » dans la chaîne de
        caractères « str » à partir du « pos»ième caractères . Si le caractère « c » est trouvé, la fonction
        retourne le pointeur vers le caractère dans la chaîne « str ». Sinon la fonction renvoie la valeur
        NULL.*/
        char test;
        for (int i=pos; i<strlen(str); i++){
            test = str[i];
            if (test == c){
                return (&str[i]);
            };
        };
        return NULL;
    }

    const char *findStr(const char *str, const char *s, int pos){
        //Le but de trouver la position de premiere lettre du mot puis analyser si il s'agit de celui-ci (avec le warning) sinon on augmente la position de +1 (pas possiblement autrement)
        const char *poslettre;
        int warning=1;
        while(warning!=0){
            poslettre = findChar(str,s[0],pos);
            warning = strlen(s);
            if (poslettre == NULL){
                return NULL;
            }
            for(int i=0; i<strlen(s);i++){
                if(*(poslettre+i) == s[i]){
                    warning--;
                }
            }
            pos++;
        }
        return poslettre;
    }


    int isValidHeader(const char *frame){
	/*Cette fonction recherche dans la chaîne de caractère « frame » un entête valide de trame NMEA. Si un entête d’un trame GGA est trouvé, elle retourne la valeur XGGA_FRAME. Si l’entête trouvé est de type RMC, elle retourne la valeur XRMC_FRAME, … Si aucun entête valide est trouvé, elle retourne la valeur UNKNOWN_FRAME.*/
	if (findStr(frame,"GPA",3)!=NULL){
		return(XGGA_FRAME);}
	else if (findStr(frame,"RMC",3)!=NULL){
		return(XRMC_FRAME);}
	else {
		return(UNKNOWN_FRAME);}
	}
int getFieldsCount(const char *frame){
	/*Cette fonction compte le nombre de champs que contient la trame NMEA. Les champs d’une trame NMEA sont séparés par une « , » et sont compris entre le « $ » de début de trame et le « * » indiquant le check sum. L’entête de trame est donc le premier champ.*/
	int compt=1,i;
	char pos=1;
	while (findChar(frame,'*',pos)!=NULL){
		if (findChar(frame,',',pos)!=NULL){
			compt++;
			pos=findChar(frame,',',pos)-&frame[0]+2;
			}
		else{
			break;
			}
		}
	return(compt);
	}
//printf("il y a %d field\n",getFieldsCount(frame0));
char *gotoField(const char* frame, int n){
    /*Cette fonction renvoie le pointeur vers le premier caractère du « n »ieme champs de la trame contenu dans frame. Si le « n »ieme champ n’est pas trouvé, la fonction retour NULL.*/
}






    return 0;
}

