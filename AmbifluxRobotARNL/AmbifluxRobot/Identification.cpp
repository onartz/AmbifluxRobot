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
	string firstName = "";
	string lastName = "";
	string cardId = "";
	string emailAddress = "";

	
	//Recuperer infos de l utilisateur
	string json = DALRest::getEmployeeByCardId(string(bufCardId));
	
	try{
		ptree pt = JSONParser::parse(json);
		firstName = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("firstname").c_str()));
		lastName = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("lastname").c_str()));
		cardId = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("cardID").c_str()));
		emailAddress = string((char*)(pt.get_child("GetEmployeeByCardIdResult").get<std::string>("emailAddress").c_str()));	
		*person = Person(json);
	}
	catch(std::exception const&  ex)
	{
		cardId = string(bufCardId);
		firstName = "";
		lastName = "";
		emailAddress = "";

		printf("JSON Error. %s", ex.what());
	}
	//Person * operateur = new Person(json);
	
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


