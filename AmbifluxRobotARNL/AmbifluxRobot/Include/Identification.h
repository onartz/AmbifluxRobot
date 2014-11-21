#ifndef DEF_IDENTIFICATION_H // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_IDENTIFICATION_H // On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "Person.h"
//#include "Resource.h"
#include "springprox.h"
#include "AriaUtil.h"
#include "LecteurCarte.h"
#include "DalRest.h"

class Identification
{
public:

	enum Resultat
	{
		RES_UNKNOWN,
		RES_IDENTIFICATION_FAILED,
		RES_IDENTIFICATION_OK,
		RES_TIMEOUT,
		RES_ERROR
	};


	 static void Identifier(Person*);
	 //static resource * Identifier();
	
	
	 //Accesseurs

	 //Resultat getResultat(void);
	// Resource *getOperateur(void);

private:
	

	
};

#endif