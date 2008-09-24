

const int maxPartners = 3;
const long pieceCount = 40;  // 10 sec
const double pieceSimTimeLength = 0.25; // время веидопотока в секундах, которое передается одним писом

const int pieceStartDelayCount=pieceCount*0.9; // кол-во первых писов которые не запрашиваются в начале симуляции

const int videoSpeed = (500*1024)/8; // speed of video stream 500 kbit/s
const int pieceSize  = videoSpeed * pieceSimTimeLength;