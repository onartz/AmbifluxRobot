#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Identification.h"	

void Identification::Identifier(Person *person)
{
	LecteurCarte lecteurCarte;
	ArTime myStateStartTime;
	lecteurCarte.open();
	//On tente une fois
	int r = LecteurCarte::NOCARD;
	r = lecteurCarte.read(20000);
	lecteurCarte.close();
	if(r != LecteurCarte::CARD)
		return;

	char * bufCardId = lecteurCarte.getCardId();
	
	//Recuperer infos de l utilisateur
	string json = DALRest::getEmployeeByCardId(string(bufCardId));
	//Person * operateur = new Person(json);
	*person = Person(json);
	return ;
}

bool Identification:: DetectCard(void)
{
	LecteurCarte lecteurCarte;
	ArTime myStateStartTime;
	lecteurCarte.open();
	//On tente une fois
	int r = LecteurCarte::NOCARD;
	r = lecteurCarte.read(20000);
	lecteurCarte.close();
	return(r == LecteurCarte::CARD);
}


