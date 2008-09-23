
#ifndef __IPMESHCLIENTAPP_H__
#define __IPMESHCLIENTAPP_H__

#include <omnetpp.h>
#include "MeshClientData.h"


/**
 * IP traffic generator application. See NED for more info.
 */
class IPMeshClientApp : public cSimpleModule  {
  
    	MeshClientData *clientData;	

 	~IPMeshClientApp();
protected:
    virtual void initialize();  
    virtual void handleMessage(cMessage *msg);
};

#endif
