#ifndef _H_OPENNI2SOCKET_H_
#define _H_OPENNI2SOCKET_H_

#include <stream2socket/stream2socketprocess.h>

#include <boost/asio.hpp>

//FIXME Temporary incldue for frame structure
#include "frame.h"

class OpenNI2Socket : public boost::noncopyable
{
public:
    /* Constructor, port argument is where we will listen for image requests (UDP) */
    OpenNI2Socket(short port);

    ~OpenNI2Socket();

    void Start();

    void EncodeAndSendFrame(const frame & fr); 
private:
    boost::thread * th_;
    void ThreadFunction();

    boost::asio::io_service io_service_;
    boost::thread * io_service_th_;

    bool started_;
    bool stopped_;
    bool new_data_;
    visionsystem::Stream2SocketProcess process_;
    vision::Image<uint32_t, vision::RGB> * img_;
    boost::mutex work_mutex;
};

#endif
