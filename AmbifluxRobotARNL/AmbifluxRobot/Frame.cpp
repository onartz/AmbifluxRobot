#include "Frame.h"

Frame::Frame(){}

Frame::Frame(char** msg, int nbArgs):msg( msg, msg + nbArgs )
{}