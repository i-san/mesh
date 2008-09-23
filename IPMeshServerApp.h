#ifndef __IPMESHSERVERAPP_H__
#define __IPMESHSERVERAPP_H__

#include <omnetpp.h>
#include "MeshServerData.h"
#include "MeshClientData.h"

/**
 * IP traffic generator application. See NED for more info.
 */
class IPMeshServerApp : public cSimpleModule  {
	MeshServerData *serverData;
	MeshClientData *clientData;
	
  public:
	~IPMeshServerApp();
  protected:
    virtual void initialize();  
    virtual void handleMessage(cMessage *msg);
};

#endif
