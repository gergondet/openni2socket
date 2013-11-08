#include "openni2socket.h"

inline void rgb24_to_rgba(unsigned char * rgb24_buffer, unsigned int nb_pixels, unsigned char * rgba_buffer)
{
    for(unsigned int i = nb_pixels; i > 0; --i)
    {
        memcpy(&(rgba_buffer[4*(i-1)]), &(rgb24_buffer[3*(i-1)]), 3);
    }
}

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
        while(!new_data_ && !stopped_)
        {
            usleep(1e5);
        }
        if(stopped_) { break; }
        boost::mutex::scoped_lock lock(work_mutex);
        rgb24_to_rgba((unsigned char*)(img_->raw_data), img_->pixels, (unsigned char *)(img_->raw_data));
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
        /* Encoding is CV_8UC3, for now let's act as if it is CV_8UC4 */
        std::memcpy(img_->raw_data, fr.m_image.data, 3*img_->pixels);
        new_data_ = true;
    }
}
