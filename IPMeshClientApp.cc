#include <omnetpp.h>
#include "IPMeshClientApp.h"
#include "mdebug.h"

using namespace std;

Define_Module(IPMeshClientApp);



void IPMeshClientApp::initialize() {
	clientData = new MeshClientData(this);
	scheduleAt((rand()%10)*0.20, new cMessage("timerClientStart"));
	
	WATCH(*clientData);
}

void IPMeshClientApp::handleMessage(cMessage *msg) {
	if (msg->isSelfMessage()) {		
		clientData->processSelfMessage(msg);		
	}else {
		clientData->processMessage(msg);		
	}	
	delete msg;	
}

IPMeshClientApp::~IPMeshClientApp() {
	delete clientData;
}
