#ifndef __MESHCLIENTDATA_H__
#define __MESHCLIENTDATA_H__

#include "MeshUser.h"
#include "IPControlInfo.h"
#include "IPAddressResolver.h"
#include "MeshMessages_m.h"
#include "IPvXAddress.h"

class MeshClientData {

	static int number;
	std::string userName;

	std::list<MeshUserClientData*> lPartner;			
	cSimpleModule *module;

	std::list<PieceStatusType> lPiece;
	unsigned long	quantTime;

	bool isServer;
public:	
	
	MeshClientData(cSimpleModule *_module,bool isServer = false);
	~MeshClientData();

	void processSelfMessage(cMessage *msg);
	void processMessage(cMessage *msg);

	friend std::ostream& operator<<( std::ostream &os, const MeshClientData &clientData);
private:
	
	cMessage* genInitMessage() const;
	cMessage* genConnectMessage(const IPAddress &ipAddress) const;
	cMessage* genConnectResponseMessage(int errorCode,const IPAddress &ipAddress) const;
	cMessage* genPieceInfoMessage(int numPiece,const IPAddress &ipAddress) const ;
	cMessage* genPieceRequestMessage(int numPiece, const IPAddress &ipAddress) const;
	cMessage* genPiece(int numPiece, const IPAddress &ipAddress) const;

	void addControlInfo(cMessage *msg,const IPAddress &ipAddress) const;	

	bool isPartner(const std::string &userName) ;
	void addPartner(const MeshUser &meshUser, bool isActive, const ListPieceStatusType &lNewPiece = ListPieceStatusType() );
	void updatePartner(const std::string &userName,UserStatusType userStatus,const ListPieceStatusType &lNewPiece = ListPieceStatusType() );
	//void removePartner(const std::string &userName);	
	
	void updatePartnerPiece(const std::string &userName, unsigned long pieceNum);
	void updateCurrentPiece(unsigned long pieceNum);

	void updateLPiece();
	void requestPieces();
};

#endif
