#include <iostream>
#include <opencv2/opencv.hpp>


int main()
{
    std::string imagePath = RESOURCE_DIR "jpeg/sample001.jpeg";

    cv::Mat image = cv::imread(imagePath);

    if (image.empty()) {
        std::cerr << "Impossible de charger l'image." << std::endl;
        return -1;
    }

    image = cv::imread(imagePath);

    std::cout << "Largeur de l'image : " << image.cols << " pixels" << std::endl;
    std::cout << "Hauteur de l'image : " << image.rows << " pixels" << std::endl;

    cv::waitKey(0);

   return 0;
}
