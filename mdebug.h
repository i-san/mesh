#ifndef __MDEBUG_H__
#define __MDEBUG_H__


#include "MeshUser.h"
#include <iostream>

const int MSG_IPADDRESS_LENGTH = 15;

long getMessageLength(cMessage *m_msg);

std::ostream& operator<<( std::ostream &os, const VectorPartnersType &vector );
std::ostream& operator<<( std::ostream &os, const VectorPartnersTypeClient &vector );
std::ostream& operator<<( std::ostream &os, const ListPieceStatusType &list );

#endif

