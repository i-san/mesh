
#include "MeshClientData.h"
#include <list>


using namespace std;

int MeshClientData::number=0;

MeshClientData::MeshClientData(cSimpleModule *_module,bool isServer) {
	this->module = _module;	
	for (int i=0;i<pieceCount;i++) {
		lPiece.push_back((isServer?RECIEVED:ABSENT));
	}
	quantTime = 0;
	this->isServer = isServer;
	
	if (isServer) {
		userName = "server";
	}else{
		char strBuf[40]; sprintf(strBuf,"%s%i",module->parentModule()->name(),module->parentModule()->index());
		userName = string(strBuf);
	}

	WATCH(lPiece);
}


MeshClientData::~MeshClientData() {
	for(list<MeshUserClientData*>::iterator it=lPartner.begin();it != lPartner.end();it++) {
		delete *it;
	}
}

//******************************* partner control ****************************************

bool MeshClientData::isPartner(const string &userName) {
	for(list<MeshUserClientData*>::iterator it=lPartner.begin();it != lPartner.end();it++) {
		if ((*it)->userName == userName) return true;		
	}
	return false;
}

void MeshClientData::addPartner(const MeshUser &meshUser, bool isActive, unsigned long quantTime, const ListPieceStatusType &lPartnerPiece) {	
	lPartner.push_back(new MeshUserClientData(meshUser.userName,meshUser.ipAddress,(isActive?ACTIVE:INACTIVE),quantTime,lPartnerPiece));
}

void MeshClientData::updatePartner(const string &userName, UserStatusType userStatus,unsigned long quantTime, const ListPieceStatusType &lNewPiece) {
	for(list<MeshUserClientData*>::iterator it=lPartner.begin();it != lPartner.end();it++) {
		if ((*it)->userName == userName) { 
			(*it)->status = userStatus;
			(*it)->lPiece = lNewPiece;
			(*it)->quantTime = quantTime;
		}
	}
}

/*void MeshClientData::removePartner(const string &userName) {
	for(list<MeshUserClientData*>::iterator it=lPartner.begin();it != lPartner.end();it++) {
		if ( (*it)->userName == userName) lPartner.remove(*it);
	}
}*/

//**************************** pieces control ***********************************************

void MeshClientData::updatePartnerPiece(const string &userName, unsigned long pieceNum) {
	//if ((pieceNum>quantTime+pieceCount)||(pieceNum<quantTime)) return;
	for(list<MeshUserClientData*>::iterator it=lPartner.begin();it != lPartner.end();it++) {
//cout << "pieceNum=" << pieceNum << " quantTime="  << quantTime << " p->quantTime="  << (*it)->quantTime << " size=" << lPiece.size() << endl;
		if (((*it)->userName == userName)&&( pieceNum >= (*it)->quantTime )) {

			int size = ( pieceNum - (*it)->quantTime - pieceCount + 1);
			if (size > 0) { 
				for (int i=0;i<size-1;i++) {
					(*it)->lPiece.pop_front();
					(*it)->lPiece.push_back(ABSENT);					
				}
				(*it)->lPiece.pop_front();
				(*it)->lPiece.push_back(RECIEVED);
				(*it)->quantTime+=size;
			}else{
				list<PieceStatusType>::iterator itPiece = (*it)->lPiece.begin();			
				advance(itPiece,pieceNum-(*it)->quantTime);
	
				(*it)->lPiece.erase(itPiece);
				
				itPiece = (*it)->lPiece.begin();			
				advance(itPiece,pieceNum-(*it)->quantTime);
				(*it)->lPiece.insert(itPiece,RECIEVED);
	
				(*it)->status = ACTIVE;
			}	
		}
	}
}

void MeshClientData::updateCurrentPiece(unsigned long pieceNum) {
cout << "cli"<<module->parentModule()->index()<<": pieceNum=" << pieceNum << " quantTime=" << quantTime << " " << ((pieceNum>quantTime+pieceCount)||(pieceNum<quantTime)) << endl;
	if ((pieceNum>quantTime+pieceCount)||(pieceNum<quantTime)) return;
	list<PieceStatusType>::iterator itPiece = lPiece.begin();
	advance(itPiece,pieceNum-quantTime);
	lPiece.erase(itPiece);
	itPiece = lPiece.begin();			
	advance(itPiece,pieceNum-quantTime);
	lPiece.insert(itPiece,RECIEVED);
}

ostream& operator<<( ostream &os, const MeshClientData &clientData ) {		
	os << clientData.quantTime ;
	for(list<MeshUserClientData*>::const_iterator it=clientData.lPartner.begin();it != clientData.lPartner.end();it++) {		
		os << **it;
	}
	
}

//******************************** Generation message ******************************

void MeshClientData::addControlInfo(cMessage *msg,const IPAddress &ipAddress) const {
	IPControlInfo *controlInfo = new IPControlInfo();
	controlInfo->setDestAddr(ipAddress);	
	controlInfo->setProtocol(17);
	msg->setControlInfo(controlInfo);
}

cMessage* MeshClientData::genInitMessage() const {	
    	InitMeshRequestMessage *msg = new InitMeshRequestMessage("init_mesh_req");
    	msg->setByteLength(getMessageLength(msg));
	msg->setSrcIpAddress(IPAddressResolver().addressOf(module->parentModule()).get4());
	msg->setSrcName(userName.c_str());
	addControlInfo(msg,IPAddressResolver().resolve("server").get4());
	return msg;
}

cMessage* MeshClientData::genConnectMessage(const IPAddress &ipAddress) const {
	ConnectMeshRequestMessage *msg = new ConnectMeshRequestMessage("conn_mesh_req");
	msg->setByteLength(getMessageLength(msg));
	msg->setSrcIpAddress(IPAddressResolver().addressOf(module->parentModule()).get4());	
	msg->setSrcName(userName.c_str());
	msg->setLPiece(lPiece);
	msg->setQuantTime(quantTime);
	addControlInfo(msg,ipAddress);
	return msg;
}

cMessage* MeshClientData::genConnectResponseMessage(int errorCode,const IPAddress &ipAddress) const {
	ConnectMeshResponseMessage *msg = new ConnectMeshResponseMessage("conn_mesh_resp");
	msg->setByteLength(getMessageLength(msg));	
	msg->setSrcName(userName.c_str());
	msg->setErrorCode(errorCode);
	msg->setLPiece(lPiece);
	msg->setQuantTime(quantTime);
	addControlInfo(msg,ipAddress);
	return msg;
}

cMessage* MeshClientData::genPieceInfoMessage(int numPiece,const IPAddress &ipAddress) const {
	PieceMeshInfoMessage *msg = new PieceMeshInfoMessage("piece_info");
	msg->setByteLength(getMessageLength(msg));
	msg->setNumPiece(numPiece);
	msg->setSrcName(userName.c_str());
	addControlInfo(msg,ipAddress);
	return msg;
}

cMessage* MeshClientData::genPieceRequestMessage(int numPiece, const IPAddress &ipAddress) const {
	PieceMeshRequestMessage *msg = new PieceMeshRequestMessage("piece_req");
	msg->setByteLength(getMessageLength(msg));
	msg->setNumPiece(numPiece);
	msg->setSrcName(userName.c_str());
	addControlInfo(msg,ipAddress);
	return msg;
}

cMessage* MeshClientData::genPiece(int numPiece, const IPAddress &ipAddress) const {
	Piece *msg = new Piece("PiecE");
	msg->setByteLength(getMessageLength(msg));
	msg->setNumPiece(numPiece);
	msg->setSrcName(userName.c_str());
	addControlInfo(msg,ipAddress);
	return msg;
}

//************************************ process message *************************************


void MeshClientData::processSelfMessage(cMessage *msg) {
	if (string(msg->name())=="timerLPieceUpdate") {
		updateLPiece();
		if (!isServer) requestPieces();
			
		module->scheduleAt(module->simTime()+0.05, new cMessage("timerLPieceUpdate"));
	}else if (string(msg->name())=="timerClientStart") {
		updateLPiece();
		module->send(genInitMessage(),"to_ip");
		module->scheduleAt(module->simTime()+0.05, new cMessage("timerLPieceUpdate"));
	}		
}

void MeshClientData::processMessage(cMessage *msg) {
	if (dynamic_cast<InitMeshResponseMessage*>(msg)!=NULL) {
		InitMeshResponseMessage *msgResponse = (InitMeshResponseMessage*)msg;
		if (msgResponse->getErrorCode()==0) {
			vector<MeshUser*> vPartners = msgResponse->getVPartners();
			for (int i=0,size=vPartners.size();i<size;i++)	{
				addPartner(*vPartners[i],INACTIVE);
				module->send(genConnectMessage(vPartners[i]->ipAddress),"to_ip");
				delete vPartners[i];				
			}						
		}else {
			ev << "------Error----errorCode=" << msgResponse->getErrorCode() << endl;
		}
	}else if (dynamic_cast<ConnectMeshRequestMessage*>(msg)!=NULL) {
		ConnectMeshRequestMessage *msgRequest = (ConnectMeshRequestMessage*)msg;
				
		if (!isPartner(msgRequest->getSrcName())) {
			addPartner(MeshUser(msgRequest->getSrcName(),msgRequest->getSrcIpAddress()),true,msgRequest->getQuantTime(),msgRequest->getLPiece());
		} else{
			updatePartner(msgRequest->getSrcName(),ACTIVE,msgRequest->getQuantTime(),msgRequest->getLPiece());
		}
		module->send(genConnectResponseMessage(0,msgRequest->getSrcIpAddress()),"to_ip");
	}else if (dynamic_cast<ConnectMeshResponseMessage*>(msg)!=NULL) {
		ConnectMeshResponseMessage *msgResponse = (ConnectMeshResponseMessage*)msg;
		if (msgResponse->getErrorCode() == 0) {
			updatePartner(msgResponse->getSrcName(),ACTIVE,msgResponse->getQuantTime(),msgResponse->getLPiece());
		}else {
			ev << "------Error----errorCode=" << msgResponse->getErrorCode() << endl;
		}
	}else if (dynamic_cast<PieceMeshInfoMessage*>(msg)!=NULL) {
		PieceMeshInfoMessage *msgInfo = (PieceMeshInfoMessage*)msg;
		updatePartnerPiece(msgInfo->getSrcName(),msgInfo->getNumPiece());
	}else if (dynamic_cast<PieceMeshRequestMessage*>(msg)!=NULL) {
		PieceMeshRequestMessage *msgRequest = (PieceMeshRequestMessage*)msg;
		unsigned long numPiece = msgRequest->getNumPiece();

		list<PieceStatusType>::iterator itPiece = lPiece.begin();
		advance(itPiece,numPiece-quantTime);
		if ( (*itPiece) != RECIEVED ) {
			ev << "---------------ERRROR!---piece is unvailable, numPiece=" << numPiece << " req from " << msgRequest->getSrcName() << endl;
		}

		bool isPartner = false;
		IPAddress ipAddress;
		list<MeshUserClientData*>::iterator itPartner = lPartner.begin();
		for	(;itPartner!=lPartner.end();itPartner++) {
//cout << "servers partner: " << (*itPartner)->userName << endl;
			if ( ( (*itPartner)->userName == msgRequest->getSrcName() )&&((*itPartner)->status == ACTIVE) ) { 
				ipAddress = (*itPartner)->ipAddress;
				isPartner = true;
				break;
			}
		}
		if (isPartner) {
			module->send(genPiece(numPiece,ipAddress),"to_ip") ;
		}else {	
			ev << "---------------ERRROR!---partner is uvailable, numPiece=" << numPiece << " req from " <<msgRequest->getSrcName() << endl;
		}
		
	}else if (dynamic_cast<Piece*>(msg)!=NULL) {
		Piece* msgPiece = (Piece*)msg;				
		unsigned long numPiece = msgPiece->getNumPiece();
		updateCurrentPiece(numPiece);

		string partnerName = string(msgPiece->getSrcName());
		for (list<MeshUserClientData*>::iterator it=lPartner.begin();it!=lPartner.end();it++) {
			if ( (*it)->userName == partnerName ) {
				(*it)->pieceRecieved++;				
			}
			if ( (*it)->status == ACTIVE ) {				
cout << userName << ": send info for piece " << numPiece << endl;
				module->send(genPieceInfoMessage(numPiece, (*it)->ipAddress ),"to_ip") ;
			}
		}
	}
}

void MeshClientData::updateLPiece() {
	int newQuantTime = module->simTime()/pieceSimTimeLength;	
	if (!isServer) {
		//cout << "client: newQuantTime=" << newQuantTime << " quantTime" << quantTime << endl;
		for (int i=0,count=newQuantTime-quantTime;i<count;i++) {
			lPiece.pop_front();
			lPiece.push_back(ABSENT);		
		}
		/*for (list<MeshUserClientData*>::iterator it=lPartner.begin();it!=lPartner.end();it++) {
			for (int i=0,count=newQuantTime-quantTime;i<count;i++) {
				(*it)->lPiece.pop_front();
				(*it)->lPiece.push_back(ABSENT);
			}	
		}*/
	}else {
		//cout << "server: newQuantTime=" << newQuantTime << " quantTime" << quantTime << endl;
		for (int i=0,count=newQuantTime-quantTime;i<count;i++) {
			for (list<MeshUserClientData*>::iterator it=lPartner.begin();it!=lPartner.end();it++) {
				module->send(genPieceInfoMessage(quantTime+i+pieceCount,(*it)->ipAddress),"to_ip");
			}
		}
	}
	quantTime = newQuantTime;
}


bool compare_partners_piece_recieved(MeshUserClientData* first, MeshUserClientData* second) {
	if (first->pieceRequested == 0) return false;
	if (second->pieceRequested == 0) return true;
	return ( ((double)(first->pieceRecieved)) / (first->pieceRequested) <  ((double)(second->pieceRecieved)) / (second->pieceRequested) );
}

void MeshClientData::requestPieces() {
	int iPiece = ( quantTime>pieceStartDelayCount ? 0 : pieceStartDelayCount-quantTime );
	lPartner.sort(compare_partners_piece_recieved);
	list<PieceStatusType>::iterator itPiece=lPiece.begin();
	advance(itPiece,iPiece);	
	while(iPiece<pieceCount) {	
	if ((*itPiece)==ABSENT) {
//cout << userName << ": search piece num " << iPiece+quantTime << endl;
		for (list<MeshUserClientData*>::iterator itPartner=lPartner.begin();itPartner!=lPartner.end();itPartner++) {
			if ((*itPartner)->status == ACTIVE) {

				if ( (iPiece+quantTime >= (*itPartner)->quantTime )&&( iPiece+quantTime <= (*itPartner)->quantTime+pieceCount) ) {
					list<PieceStatusType>::iterator it = (*itPartner)->lPiece.begin();
					advance(it,iPiece+quantTime-(*itPartner)->quantTime);
					if ((*it)==RECIEVED) {
cout << userName << ": req piece num " << iPiece+quantTime << " from " << (*itPartner)->userName << endl;
						module->send(genPieceRequestMessage(iPiece+quantTime,(*itPartner)->ipAddress),"to_ip");
						
						lPiece.erase(itPiece);
						itPiece = lPiece.begin();
						advance(itPiece,iPiece);
						lPiece.insert(itPiece,REQUESTED);
						itPiece = lPiece.begin();
						advance(itPiece,iPiece);
		
						(*itPartner)->pieceRequested++;
						lPartner.sort(compare_partners_piece_recieved);
						break;
					}
				}
			}
		}	
	}
	iPiece++; itPiece++;
	}
}
