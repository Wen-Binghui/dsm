#pragma once

#include <string>
#include <vector>

#include "opencv2/core.hpp"

#include "FullSystem/DSMLib.h"

namespace dsm {
class DSM_EXPORTS_DLL TUMReader {
   public:
    TUMReader(const std::string& imageFolder, const std::string& timestampFile,
              bool reverse);
    ~TUMReader();

    // grab image files
    bool open();

    // resets sequence
    void reset();

    // check if successfully opened
    bool isOpened() const;

    // reads next image
    bool read(cv::Mat& img, double& timestamp);
    bool read_depth(cv::Mat& img);

    // sequence frames per second
    double fps() const;

   private:
    bool readImageNames();

   private:
    // input data
    const std::string imagePath;
    const std::string timestampPath;
    const int inc;

    // dataset data
    std::vector<std::string> files;
    std::vector<std::string> depth_files;
    std::vector<double> timestamps;
    double fps_;

    // image counter
    int id;
};
}  // namespace dsm