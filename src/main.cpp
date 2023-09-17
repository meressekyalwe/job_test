#define OPENCV_IO_ENABLE_OPENEXR 1

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>


int main()
{
    std::string image_source_Path = RESOURCE_DIR "png/sample001.png";

    cv::Mat image_source = cv::imread(image_source_Path);

    if (image_source.empty()) 
    {
        std::cerr << "Impossible de charger l'image." << std::endl;
        return -1;
    }

    std::vector<cv::Mat> mip_level;
    mip_level.push_back(image_source.clone());

    while (image_source.cols > 2 && image_source.rows > 2) 
    {
        cv::resize(image_source, image_source, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);       
        mip_level.push_back(image_source.clone());
    }

    for (size_t i = 0; i < mip_level.size(); ++i) 
    {
        std::string image_output_Path = OUTPUT_DIR "mip_level_" + std::to_string(i) + ".png";
        cv::imwrite(image_output_Path, mip_level[i]);
    }

   return 0;
}