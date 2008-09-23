

#include <IPAddress.h>
#include <iostream>
#include "IPControlInfo.h"
#include "IPAddressResolver.h"


#include "MeshServerData.h"

using namespace std;

bool MeshServerData::initNewUser(const string &userName,const IPAddress &ipAddress) {	
	for (int i=0,size=vUser.size();i<size;i++)
		if (vUser[i]->userName==userName) {
			return true;
		}
	vUser.push_back(new MeshUserServerData(userName,ipAddress));
	return true;
}

MeshServerData::~MeshServerData() {	
	for (int i=0,size=vUser.size();i<size;i++)
		delete vUser[i];
}

vector<MeshUser*>* MeshServerData::getPartnersList(const std::string &userName) {
	vector<MeshUser*> *vPartners = new vector<MeshUser*>;
	int nUser = vUser.size();
	if (maxPartners>=nUser) {				
		vPartners->reserve(nUser);
		for (int i=0;i<nUser;i++) {
			vPartners->push_back( new MeshUser(vUser[i]->userName,vUser[i]->ipAddress) );
		}
	}else{	
		vPartners->reserve(maxPartners);			
		
		for (int i=0;i<maxPartners;i++) {
			int iUser; 
			bool isIUser;
			do {
 				isIUser = false;
				iUser = rand()%nUser;
				for (int j=0;j<i;j++) {					
					if ((*vPartners)[j]->userName == vUser[iUser]->userName)
						isIUser = true;
				}
			}while(isIUser);
			vPartners->push_back( new MeshUser(vUser[i]->userName,vUser[i]->ipAddress) );
		}		
	}
	return vPartners;
}


void MeshServerData::addControlInfo(cMessage *msg,const IPAddress &ipAddress) {
	IPControlInfo *controlInfo = new IPControlInfo();
	controlInfo->setDestAddr(ipAddress);	
	controlInfo->setProtocol(17);
	msg->setControlInfo(controlInfo);
}

void MeshServerData::processInitMessage(InitMeshRequestMessage* msgRequest) {	
	vector<MeshUser*> *vPartners = getPartnersList(msgRequest->getSrcName());
			
	InitMeshResponseMessage *msgResponse = new InitMeshResponseMessage("init_mesh_resp");
	if ( initNewUser(msgRequest->getSrcName(),msgRequest->getSrcIpAddress())) {
		msgResponse->setVPartners(*vPartners);
		delete vPartners;
		msgResponse->setByteLength(getMessageLength(msgResponse));
		addControlInfo(msgResponse,msgRequest->getSrcIpAddress());
	}else {		
		msgResponse->setErrorCode(1);	
	}
	module->send(msgResponse,"to_ip");
}

void MeshServerData::processSelfMessage(cMessage *msg) {
	if (string(msg->name())=="timerServerStart") {
		initNewUser("server",IPAddressResolver().resolve("server").get4());		
		module->scheduleAt(module->simTime()+pieceSimTimeLength, new cMessage("timerLPieceUpdate"));
	}
}

void MeshServerData::processMessage(cMessage *msg) {	
	if (dynamic_cast<InitMeshRequestMessage*>(msg)!=NULL) {
		processInitMessage((InitMeshRequestMessage*)msg);			
	}	
}
