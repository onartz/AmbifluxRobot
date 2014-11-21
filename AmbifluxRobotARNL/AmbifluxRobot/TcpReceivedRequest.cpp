#include "TcpReceivedRequest.h"

TCPReceivedRequest::TCPReceivedRequest():socket(0), frame(){
}

TCPReceivedRequest::TCPReceivedRequest(ArSocket *socket, Frame frame):socket(socket), frame(frame){
}


