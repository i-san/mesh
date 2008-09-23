

#include "MeshUser.h"
#include <iostream>

using namespace std;

int MeshUser::length() {
	return userName.length() + 15;
}

MeshUserServerData::MeshUserServerData( const std::string &userName, const IPAddress &ipAddress ) {
	this->userName = userName;
	this->ipAddress = ipAddress;
}

MeshUserClientData::MeshUserClientData( const std::string &userName, const IPAddress &ipAddress, UserStatusType status) {
	this->userName = userName;
	this->ipAddress = ipAddress;
	this->status = status;
	pieceRequested = 0;
	pieceRecieved = 0;	
}

std::ostream& operator<<(std::ostream& os,const MeshUserClientData &clientData) {
	os << clientData.userName << " "<< clientData.ipAddress << " " << clientData.status << " (" << clientData.pieceRequested << "," << clientData.pieceRecieved << ")" << endl;
	int i=0;
	for (list<PieceStatusType>::const_iterator it=clientData.lPiece.begin();it!=clientData.lPiece.end();it++) {
		os << (*it); 
		if(++i%10==0) os << " ";
	}
	os << endl;
}
