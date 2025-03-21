#ifndef _CameraServerMediaSubsession_h
#define _CameraServerMediaSubsession_h

#include "OnDemandServerMediaSubsession.hh"
#include "CameraFramedSource.hh"

class CameraServerMediaSubsession : public OnDemandServerMediaSubsession {
public:
    static CameraServerMediaSubsession* createNew(UsageEnvironment& env) {
        return new CameraServerMediaSubsession(env);
    }

protected:
    char const* _sdpLines;
    int fWidth;
    int fHeight;
    
    CameraServerMediaSubsession(UsageEnvironment& env)
        : OnDemandServerMediaSubsession(env, false), fWidth(640), fHeight(480) {
            // 生成SDP描述
            char const* fmt = "m=video 0 RTP/AVP 96\r\n"
                              "c=IN IP4 0.0.0.0\r\n"
                              "a=rtpmap:96 H264/90000\r\n"
                              "a=fmtp:96 packetization-mode=1;profile-level-id=42001F;"
                              "sprop-parameter-sets=Z0IAH5WoFAFuQA==,aM48gA==\r\n"
                              "a=control:%s\r\n"
                              "a=x-dimensions:%d,%d\r\n";
            
            unsigned sdpSize = strlen(fmt) + 20 + 20; // 为参数预留空间
            char* sdpLines = new char[sdpSize];
            sprintf(sdpLines, fmt, trackId(), fWidth, fHeight);
            this->_sdpLines = strdup(sdpLines);
            delete[] sdpLines;
        }

    virtual char const* sdpLines() {
        return this->_sdpLines;
    }

    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) {
        printf("[%s] createNewStreamSource\n", envir().getResultMsg());
        estBitrate = 500; // 估计的比特率
        return (FramedSource*)CameraFramedSource::createNew(envir());
    }

    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) {
        printf("[%s] createNewRTPSink\n", envir().getResultMsg());
        H264VideoRTPSink* rtpSink = H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
        // rtpSink->setVideoFormat("H264", fWidth, fHeight);
        return rtpSink;
    }
};

#endif // _CameraServerMediaSubsession_h
