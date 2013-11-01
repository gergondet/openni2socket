#ifndef _H_FRAME_H_
#define _H_FRAME_H_

#include <opencv2/opencv.hpp>

struct frameinfo
{
    unsigned int foo;
};

struct frame
{
    cv::Mat m_image;
    cv::Mat m_depthmap;

    frameinfo m_imageinfo;
    frameinfo m_depthmapinfo;

    frame clone() const
    {
        frame out;
        out.m_image = m_image.clone();
        out.m_depthmap = m_depthmap.clone();
        out.m_imageinfo = m_imageinfo;
        out.m_depthmapinfo = m_depthmapinfo;
        return out;
    }

    size_t width() const {return m_image.cols;}
    size_t height() const {return m_image.rows;}
    size_t size() const {return m_image.cols*m_image.rows;}

    void release()
    {
        m_image.release();
        m_depthmap.release();
    }

    void save(const std::string image_ext,const std::string depth_ext);
    void save(const std::string outputdir,const std::string imname,const
std::string depthname,int count);
    void load(const std::string image_ext,const std::string depth_ext);
    void load(const std::string input,const std::string imname,const
std::string depthname,int count);
};

#endif
