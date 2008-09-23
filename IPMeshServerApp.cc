#include <omnetpp.h>
#include "IPMeshServerApp.h"

#include <iostream>
#include "mdebug.h"

using namespace std;

Define_Module(IPMeshServerApp);

IPMeshServerApp::~IPMeshServerApp() {
	delete clientData;
	delete serverData;
}

void IPMeshServerApp::initialize() {
	serverData = new MeshServerData(this);
	clientData = new MeshClientData(this,true);
	scheduleAt(0.000, new cMessage("timerServerStart"));

	WATCH(*clientData);
}

void IPMeshServerApp::handleMessage(cMessage *msg) {	
	if (msg->isSelfMessage()) {
		serverData->processSelfMessage(msg);
		clientData->processSelfMessage(msg);		
	}else {
		serverData->processMessage(msg);
		clientData->processMessage(msg);
	}
	delete msg;
}



