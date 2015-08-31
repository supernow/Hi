#ifndef __NETEQ__H__
#define __NETEQ__H__

//#include "msfilter.h"
//#include "msticker.h"

class webrtc_rtpPacket;
class webrtc_neteqClass;
class NETEQ_Decoder;
class webrtc_neteq
{
public:
	webrtc_neteq();
	webrtc_neteq(short _frameLen);
	~webrtc_neteq();
	void process(unsigned char* in, unsigned char* out);
	
private:
	short frameLen;
	webrtc_rtpPacket *rtp;
	webrtc_neteqClass *neteq;
	NETEQ_Decoder *dec;
};


#endif
