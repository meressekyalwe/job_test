#define OPENCV_IO_ENABLE_OPENEXR 1

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <opencv2/opencv.hpp>

void WriteHeader(std::ofstream& output_file, const cv::Mat& image_source, int mip_levels) 
{
    if (!output_file.is_open())
    {
        return;
    }

    const char* magic_number = "MAGICNUM1234";

    time_t modification_time = time(nullptr);

    uint32_t imageWidth = image_source.cols;

    uint32_t image_height = image_source.rows;

    uint32_t image_depth = 1;

    struct Resolution 
    {
        uint16_t width;
        uint16_t height;
        uint16_t depth;
    };

    Resolution pageResolution = {64, 64, 1};

    uint32_t total_pages = mip_levels; 

    uint32_t bytes_per_page = 65536;

    uint32_t image_layers = 1;

    uint8_t mip_levelsCount = mip_levels; 

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
    output_file.write(reinterpret_cast<char*>(&mip_levelsCount), sizeof(uint8_t));
    output_file.write(reinterpret_cast<char*>(&mipTailStartLevel), sizeof(uint8_t));
    output_file.write(reinterpret_cast<char*>(&mip_tail_data_offset), sizeof(uint32_t));
    output_file.write(reinterpret_cast<char*>(&firstPageIndices), sizeof(uint32_t) * 16);
    output_file.write(reinterpret_cast<char*>(&sourceDataFormat), sizeof(int));
    output_file.close();
}


int main()
{
    std::string image_source_Path = RESOURCE_DIR "png/sample001.png";
    cv::Mat image_source = cv::imread(image_source_Path);

    int depth = image_source.depth(); 
    int numChannels = image_source.channels(); 
    int width = image_source.cols; 
    int height = image_source.rows;

    if (image_source.empty()) 
    {
        std::cerr << "Impossible de charger l'image." << std::endl;
        return -1;
    }


    std::vector<cv::Mat> mip_levels; 
    cv::Mat current_image = image_source.clone();
    while (current_image.cols >= 2 && current_image.rows >= 2) 
    {
        mip_levels.push_back(current_image.clone());
        cv::resize(current_image, current_image, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);           
    }


    const int page_size = 64 * 1024; // 64 Ko
    std::vector<std::vector<cv::Mat>> mip_pages;
    for (size_t i = 0; i < mip_levels.size(); ++i) 
    {
        std::vector<cv::Mat> pages;
        int numRows = (mip_levels[i].rows + 7) / 8;

        for (int j = 0; j < numRows; ++j) 
        {
            int rowStart = i * 8;
            int rowEnd = std::min(rowStart + 8, mip_levels[j].rows);
            cv::Mat page = mip_levels[j].rowRange(rowStart, rowEnd);
            pages.push_back(page);
        }

        mip_pages.push_back(pages);
    }


    for (size_t i = 0; i < mip_pages.size(); ++i) 
    {
        for (size_t j = 0; j < mip_pages[i].size(); ++j) 
        {
            int num_rows = mip_pages[i][j].rows;
            int num_cols = mip_pages[i][j].cols * numChannels;
            int data_size = num_rows * num_cols * depth / 8; 

            if (data_size < page_size) 
            {
                cv::Mat zeros = cv::Mat::zeros(page_size - data_size, 1, mip_pages[i][j].type());
                mip_pages[i][j].push_back(zeros);

            }

            std::string output_path = OUTPUT_DIR "mip_page_" + std::to_string(i) + std::to_string(j) + ".mip";
            std::ofstream output_file(output_path, std::ios::binary);
            WriteHeader(output_file, mip_pages[i][j], i);
        }
    }

    return 0;
}