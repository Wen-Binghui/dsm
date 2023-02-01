/**
 * This file is part of DSM.
 *
 * Copyright (C) 2019 CEIT (Universidad de Navarra) and Universidad de Zaragoza
 * Developed by Jon Zubizarreta,
 * for more information see <https://github.com/jzubizarreta/dsm>.
 * If you use this code, please cite the respective publications as
 * listed on the above website.
 *
 * DSM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DSM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DSM. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TUMReader.h"

#include <iostream>
#include <fstream>

#include "opencv2/highgui.hpp"

namespace dsm {
TUMReader::TUMReader(const std::string& imageFolder,
                     const std::string& timestampFile, bool reverse)
    : imagePath(imageFolder),
      timestampPath(timestampFile),
      id(0),
      inc(reverse ? -1 : 1) {}

TUMReader::~TUMReader() {}

bool TUMReader::open() {
    bool readOK = this->readImageNames();

    if (readOK) {
        // sequence length in seconds
        double diff = this->timestamps.back() - this->timestamps.front();

        // fps
        this->fps_ = this->timestamps.size() / diff;

        // reset
        this->reset();

        std::cout << "EurocMav sequence found!" << std::endl;

        return true;
    }

    return false;
}

void TUMReader::reset() {
    if (this->inc > 0)
        this->id = 0;
    else
        this->id = (int)this->files.size() - 1;
}

bool TUMReader::isOpened() const { return (this->files.size() > 0); }

bool TUMReader::read(cv::Mat& img, double& timestamp) {
    // MARK read imgs
    if (this->id < this->files.size() && this->id >= 0) {
        // std::cout << this->files[this->id] << std::endl;
        img = cv::imread(this->files[this->id], cv::IMREAD_UNCHANGED);
        timestamp = this->timestamps[this->id];

        this->id += this->inc;

        return true;
    }

    return false;
}

bool TUMReader::read_depth(cv::Mat& depth_img) {
    // MARK read imgs
    if (this->id < this->depth_files.size() && this->id >= 0) {
        // std::cout << this->files[this->id] << std::endl;
        depth_img =
            cv::imread(this->depth_files[this->id], cv::IMREAD_ANYDEPTH);

        this->id += this->inc;

        return true;
    }

    return false;
}

double TUMReader::fps() const { return this->fps_; }

bool TUMReader::readImageNames() {
    // clear all data
    this->timestamps.clear();
    this->files.clear();

    // read timestamps and images with names equal to timestamps
    std::ifstream infile;
    infile.open(this->timestampPath);
    while (!infile.eof() && infile.good()) {
        std::string line, line_rgb, line_time_, line_depth, rgb_file_name,
            depth_file_name;
        std::getline(infile, line);

        if (!line.empty()) {
            std::size_t pos = line.find(";");
            line_rgb = line.substr(0, pos);
            line_depth = line.substr(pos + 1, line.size() - 1);
            std::size_t pos_rgb_space = line_rgb.find(" ");
            std::size_t pos_depth_space = line_depth.find(" ");
            line_time_ = line_rgb.substr(0, pos_rgb_space);
            rgb_file_name = line_rgb.substr(pos_rgb_space + 1, pos);
            depth_file_name =
                line_depth.substr(pos_depth_space + 1, line_depth.size() - 1);

            this->files.push_back(imagePath + "/" + rgb_file_name);
            this->depth_files.push_back(imagePath + "/" + depth_file_name);

            // std::cout << depth_file_name << std::endl;

            this->timestamps.push_back(std::atof(line_time_.c_str()) /
                                       1e9);  // transform to seconds
        }
    }

    if (this->timestamps.size() > 0 &&
        this->timestamps.size() == this->files.size()) {
        return true;
    } else {
        this->timestamps.clear();
        this->files.clear();
        std::cout << "this->timestamps.size() == 0" << std::endl;
    }

    return false;
}
}  // namespace dsm