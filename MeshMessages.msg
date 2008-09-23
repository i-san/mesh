cplusplus {{
	#include "MeshUser.h"
	#include <IPAddress.h>	
	#include <vector>
	#include "mdebug.h"

}};

class noncobject VectorPartnersType;
class noncobject IPAddress;
class noncobject MeshUser;
class noncobject ListPieceStatusType;

message InitMeshRequestMessage {	
	fields:
		string 	  srcName; 
		IPAddress srcIpAddress;
};

message InitMeshResponseMessage {	
	fields:
		int errorCode = 0; // 0 - no error		
		VectorPartnersType vPartners;
};

message ConnectMeshRequestMessage {
	fields:
		string 	  srcName;
		IPAddress srcIpAddress;
		ListPieceStatusType lPiece;
		unsigned long quantTime;
};

message ConnectMeshResponseMessage {
	fields:
		string 	  srcName;
		int errorCode = 0; // 0 - no error, connect accepted
		ListPieceStatusType lPiece;
		unsigned long quantTime;
};

message PieceMeshInfoMessage {
	fields:
		string 	  srcName;
		unsigned long numPiece;
};

message PieceMeshRequestMessage {
	fields:
		string srcName;		
		unsigned long numPiece;
};

message Piece {
	fields:
		string srcName;
		unsigned long numPiece;
};