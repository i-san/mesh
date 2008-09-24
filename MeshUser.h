#ifndef __MESHUSER_H__
#define __MESHUSER_H__


#include <IPAddress.h>
#include <string>
#include <vector>
#include <list>
#include "constants.h"


enum UserStatusType { INACTIVE, ACTIVE };
enum PieceStatusType { ABSENT, REQUESTED, RECIEVED };

typedef std::list<PieceStatusType> ListPieceStatusType;

struct MeshUser {
	std::string userName;
	IPAddress  ipAddress;	

	MeshUser() {}	
	MeshUser( const std::string &_userName, const IPAddress &_ipAddress ) 
		: userName( _userName ), ipAddress( _ipAddress ) { }	
	int length();
};

struct MeshUserServerData : public MeshUser {
	
	MeshUserServerData( const std::string &userName, const IPAddress &ipAddress );		
};

struct MeshUserClientData : public MeshUser {
	
	unsigned long quantTime;
	UserStatusType status;
	unsigned long	pieceRequested;	
	unsigned long	pieceRecieved;

	std::list<PieceStatusType>	lPiece;

	MeshUserClientData(const std::string &userName, const IPAddress &ipAddress, UserStatusType status, unsigned long quantTime, const ListPieceStatusType &lPiece);

	friend std::ostream& operator<<(std::ostream& os, const MeshUserClientData &clientData);
};

typedef std::vector<MeshUser*> VectorPartnersType;
typedef std::vector<MeshUserClientData*> VectorPartnersTypeClient;

#endif
