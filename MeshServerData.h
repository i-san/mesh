
#ifndef __MESHSERVERDATA_H__
#define __MESHSERVERDATA_H__

#include "MeshUser.h"
#include <vector>
#include "MeshMessages_m.h"

class MeshServerData {
	std::vector<MeshUserServerData*> vUser;		

	cSimpleModule *module;

public:
	//---------------------
	MeshServerData( cSimpleModule *_module) 
			: module(_module) {} // size of partners list for one user
	~MeshServerData();

	void processSelfMessage(cMessage *msg);
	void processMessage(cMessage *msg);

private:
	void processInitMessage(InitMeshRequestMessage* msgRequest);
	cMessage* genInitResponseMessage();

	void addControlInfo(cMessage *msg,const IPAddress &ipAddress);

	bool initNewUser(const std::string &userName,const IPAddress &ipAddress);	
	std::vector<MeshUser*>* getPartnersList(const std::string &userName);

};

#endif
