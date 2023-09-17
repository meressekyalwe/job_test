#define OPENCV_IO_ENABLE_OPENEXR 1

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <opencv2/opencv.hpp>

struct Resolution 
{
    uint16_t width;
    uint16_t height;
    uint16_t depth;
};

void WriteHeader(std::ofstream& output_file, const cv::Mat& image, int mip_id) 
{
    if (!output_file.is_open())
    {
        return;
    }

    const char* magic_number = "MAGICNUM1234";

    time_t modification_time = time(nullptr);

    uint32_t imageWidth = image.cols;

    uint32_t image_height = image.rows;

    uint32_t image_depth = 1;

    Resolution pageResolution = {64, 64, 1};

    uint32_t total_pages = mip_id; 

    uint32_t bytes_per_page = 65536;

    uint32_t image_layers = 1;

    uint8_t mip_idCount = mip_id; 

    uint8_t mipTailStartLevel = 0;

    uint32_t mip_tail_data_offset = 0; 

    uint32_t firstPageIndices[16]; 

    int sourceDataFormat = CV_8UC4; 

    output_file.write(magic_number, 12);
    output_file.write(reinterpret_cast<char*>(&modification_time), sizeof(time_t));
    output_file.write(reinterpret_cast<char*>(&imageWidth), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&image_height), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&image_depth), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&pageResolution), sizeof(Resolution));
    output_file.write(reinterpret_cast<char*>(&total_pages), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&bytes_per_page), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&image_layers), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&mip_idCount), sizeof(uint8_t));
    output_file.write(reinterpret_cast<char*>(&mipTailStartLevel), sizeof(uint8_t));
    output_file.write(reinterpret_cast<char*>(&mip_tail_data_offset), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&firstPageIndices), sizeof(uint32_t) * 16);
    output_file.write(reinterpret_cast<char*>(&sourceDataFormat), sizeof(int));
    output_file.close();
}


int main()
{
    std::string image_path = RESOURCE_DIR "png/sample001.png";
    cv::Mat image = cv::imread(image_path);

    int depth = image.depth(); 
    int numChannels = image.channels(); 
    int width = image.cols; 
    int height = image.rows;

    if (image.empty()) 
    {
        std::cerr << "Impossible de charger l'image." << std::endl;
        return -1;
    }


    std::vector<cv::Mat> mip_id; 
    cv::Mat current_image = image.clone();
    while (current_image.cols >= 2 && current_image.rows >= 2) 
    {  
        mip_id.push_back(current_image.clone());
        cv::resize(current_image, current_image, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR); 
                  
    }


    const int page_size = 64 * 1024; // 64 Ko
    std::vector<std::vector<cv::Mat>> mip_pages;
    for (size_t i = 0; i < mip_id.size(); ++i) 
    {
        std::vector<cv::Mat> pages;
        int total_rows = (mip_id[i].rows + 7) / 8;

        int rowStart = 0;
        while (rowStart < total_rows) 
        {    
            int rowEnd = rowStart + std::min(rowStart + 8, total_rows);
            cv::Mat page = mip_id[i].rowRange(rowStart, rowEnd);
            pages.push_back(page);
            rowStart = rowEnd;
        }

        mip_pages.push_back(pages);
    }


    for (size_t i = 0; i < mip_pages.size(); ++i) 
    {
        std::string output_path = OUTPUT_DIR "mip_#" + std::to_string(i) + ".mip";
        std::ofstream output_file(output_path, std::ios::binary);

        for (size_t j = 0; j < mip_pages[i].size(); ++j) 
        {
            int num_rows = mip_pages[i][j].rows;
            int num_cols = mip_pages[i][j].cols * numChannels;
            int data_size = num_rows * num_cols * depth / 8; 
/*
            if (data_size < page_size) 
            {
                cv::Mat zeros = cv::Mat::zeros(page_size - data_size, 1, mip_pages[i][j].type());
                mip_pages[i][j].push_back(zeros);

            }
*/

            const cv::Mat& page = mip_pages[i][j];
            output_file.write(reinterpret_cast<const char*>(page.data), page.total() * page.elemSize());
        }

        WriteHeader(output_file, mip_id[i], i);

    }

    return 0;
}