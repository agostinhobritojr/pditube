#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::Mat image, image1, image2, image3;
  cv::Mat str;

  if (argc != 2) {
    std::cout << "morfologia entrada saida\n";
  }
  image = cv::imread(argv[1], -1);

  if (!image.data) {
    std::cout << "imagem nao carregou corretamente\n";
    return (-1);
  }

  str = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

  cv::imshow("original", image);
  // bitwise_not(image,image);

  // erosao
  cv::erode(image, image2, str);
  imshow("erosao", image2);

  // dilatacao
  cv::dilate(image, image2, str);
  imshow("dilatacao", image2);

  // abertura
  cv::erode(image, image2, str);
  cv::dilate(image2, image3, str);
  cv::imshow("abertura", image3);

  // fechamento
  cv::dilate(image, image2, str);
  cv::erode(image2, image3, str);
  cv::imshow("fechamento", image3);

  // abertura -> fechamento
  //  cv::subtract(image, image3, interm);
  cv::Mat image4, image5;
  cv::erode(image, image2, str);
  cv::dilate(image2, image3, str);
  cv::dilate(image3, image4, str);
  cv::erode(image4, image5, str);

  cv::imshow("abertura->fechamento", image5);

  while (1) {
    int key;
    key = cv::waitKey(30);
    if (key == 27) break;
  }
  return 0;
}
