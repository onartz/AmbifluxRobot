#ifndef DEF_FRAME_H // Si la constante n'a pas �t� d�finie le fichier n'a jamais �t� inclus
#define DEF_FRAME_H // On d�finit la constante pour que la prochaine fois le fichier ne soit plus inclus
#include <vector>
#include <string>
//Messages envpyes
struct Frame
	{
	public : 
		Frame();
		Frame(char**, int);
		std::vector<std::string> msg;
		//char* msg[32];
		//int nbArgs;
	};
#endif