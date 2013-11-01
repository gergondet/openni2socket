#include "openni2socket.h"

OpenNI2Socket::OpenNI2Socket(short port)
: io_service_(), io_service_th_(0), started_(false), stopped_(false), new_data_(false), process_(io_service_, port), img_(0)
{
}

OpenNI2Socket::~OpenNI2Socket()
{
    stopped_ = true;
    if(th_) { th_->join(); }
    delete th_;
    io_service_.stop();
    if(io_service_th_) { io_service_th_->join(); }
    delete io_service_th_;
    delete img_;
}

void OpenNI2Socket::Start()
{
    th_ = new boost::thread(boost::bind(&OpenNI2Socket::ThreadFunction, this));
}

void OpenNI2Socket::ThreadFunction()
{
    while(!stopped_)
    {
        while(!new_data_)
        {
            usleep(1e5);
        }
        boost::mutex::scoped_lock lock(work_mutex);
        process_.SendImage(*img_);
        new_data_ = false;
    }
}

void OpenNI2Socket::EncodeAndSendFrame(const frame & fr)
{
    boost::mutex::scoped_lock lock(work_mutex, boost::try_to_lock);
    if(lock)
    {
        if(!started_)
        {
            /* Initialize H.264 encoder and I/O service on the first frame */
            process_.Initialize(vision::ImageRef(fr.width(), fr.height()), (float)30);
            img_ = new vision::Image<uint32_t, vision::RGB>(fr.width(), fr.height());
            io_service_th_ = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
            started_ = true;
        }

        /* Convert from the frame structure to libvision structure */
        /* FIXME Probably only works if fr.m_image is 8UC4 type */
        std::memcpy(img_->raw_data, fr.m_image.data, img_->data_size);
        new_data_ = true;
    }
}
