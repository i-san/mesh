
#include "mdebug.h"
#include "MeshMessages_m.h"

using namespace std;


long getMessageLength(cMessage *m_msg) {
	if (dynamic_cast<InitMeshRequestMessage*>(m_msg)!=NULL) {
		InitMeshRequestMessage *msg =  (InitMeshRequestMessage*)m_msg;
		return string(msg->getSrcName()).length() + MSG_IPADDRESS_LENGTH;
	}else if (dynamic_cast<InitMeshResponseMessage*>(m_msg)!=NULL) {
		InitMeshResponseMessage *msg =  (InitMeshResponseMessage*)m_msg;
		int payMessage = sizeof(int);
		for (int i=0,size=msg->getVPartners().size();i<size;i++) {
			payMessage+=msg->getVPartners()[i]->length();
		}
		return payMessage;
	}else if (dynamic_cast<ConnectMeshRequestMessage*>(m_msg)!=NULL) {		
		ConnectMeshRequestMessage *msg =  (ConnectMeshRequestMessage*)m_msg;
		return string(msg->getSrcName()).length() + MSG_IPADDRESS_LENGTH;
	}else if (dynamic_cast<ConnectMeshResponseMessage*>(m_msg)!=NULL) {
		return sizeof(int);
	}else if (dynamic_cast<PieceMeshInfoMessage*>(m_msg)!=NULL) {
		PieceMeshInfoMessage *msg = (PieceMeshInfoMessage*)m_msg;
		return sizeof(unsigned long) + string(msg->getSrcName()).length();
	}else if (dynamic_cast<PieceMeshRequestMessage*>(m_msg)!=NULL) {
		PieceMeshRequestMessage *msg= (PieceMeshRequestMessage*)m_msg;
		return sizeof(unsigned long) + string(msg->getSrcName()).length();
	}else if (dynamic_cast<Piece*>(m_msg)!=NULL) {
		return pieceSize;
	}
	return 0;
}

std::ostream& operator<<( std::ostream &os, const VectorPartnersType &vector ) {	
	os << endl << "vector = " << endl;
	for (int i=0,size=vector.size();i<size;i++) {
		os << vector[i]->userName << " " << vector[i]->ipAddress << endl;		
	}
	os << endl;
}

std::ostream& operator<<( std::ostream &os, const VectorPartnersTypeClient &vector ) {
	os << "test";
}

std::ostream& operator<<( std::ostream &os, const ListPieceStatusType &listik ) {
	int i = 0;
	for(list<PieceStatusType>::const_iterator it=listik.begin();it!=listik.end();it++)  {
		os << (*it);
		if (++i%10==0) os << " " ;
	}
	os << endl;
}