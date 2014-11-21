#ifndef DEF_MSGHANDLER // Si la constante n'a pas été définie le fichier n'a jamais été inclus
#define DEF_MSGHANDLER// On définit la constante pour que la prochaine fois le fichier ne soit plus inclus

#include "ArNetworking.h"
//#include "Aria.h"
#include "A_ArNetServer.h"
//class SRMA;
class Tablette;

class MsgHandler
{
public:
  // constructor
  MsgHandler(A_ArNetServer *, Tablette *);
    // Destructor, its just empty
  ~MsgHandler(void) {}
 
   // This callback is called when a new message arrives
  void handleMsgReceived(char **, int, ArSocket *);
 
protected:
  A_ArNetServer *myArNetServer;
  Tablette *tablette;
  // the functor callbacks
  ArFunctor3C<MsgHandler, char **, int, ArSocket *> myHandleMsgReceivedCB;
};
#endif
