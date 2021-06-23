#include <iostream>
#include <iterator>
#include <vector>

#include "opencv2/opencv.hpp"

void detectAndDraw(cv::Mat& img);

cv::CascadeClassifier cascadeFace, cascadeSmile;

cv::String cascadeFaceName = "haarcascade_frontalface_default.xml";
cv::String cascadeSmileName = "haarcascade_smile.xml";

int main(int argc, const char** argv) {
  cv::Mat frame, frameCopy, image;
  cv::VideoCapture cap(2);
  int key;

  if (!cascadeFace.load(cascadeFaceName)) {
    std::cerr << "ERRO: Nao carregou filtro em cascata facefrontal"
              << std::endl;
    return -1;
  }
  if (!cascadeSmile.load(cascadeSmileName)) {
    std::cerr << "ERRO: Nao carregou filtro em cascata smile" << std::endl;
    return -1;
  }

  for (;;) {
    cap >> frame;
    cv::flip(frame, frameCopy, 1);  // inverte a imagem horizontalmente
    detectAndDraw(frameCopy);       // detecta

    key = (char)cv::waitKey(10);
    if (key == 27) break;
  }
  return 0;
}

void detectAndDraw(cv::Mat& img) {
  int i = 0;
  double t = 0;
  std::vector<cv::Rect> faces;

  cascadeFace.detectMultiScale(
      img,    // imagem para deteccao
      faces,  // vetor com os retangulos encontrados
      1.1,    // escala de multiresolucao
      3,      // numero de vizinhos que cada candidato a retangulo
              // devera contemplar. evita multiplas deteccoes parecidas
              // na mesma regiao
      0 | cv::CASCADE_FIND_BIGGEST_OBJECT,
      // 0,
      // parametros (normalmente nao usados)
      cv::Size(30, 30));  // mínimo tamanho para deteccao de um objeto

  for (std::vector<cv::Rect>::const_iterator r = faces.begin();
       r != faces.end(); r++) {
    cv::Mat imgROI;
    std::vector<cv::Rect> nestedObjects;

    // desenha um retangulo onde a face eh detectada
    cv::rectangle(img, cv::Point(r->x, r->y),
                  cv::Point(r->x + r->width, r->y + r->height),
                  CV_RGB(255, 0, 0), 1, 8, 0);

    if (cascadeSmile.empty()) continue;

    // posicao aproximada da boca em relacao a face...
    cv::Rect mouthROI =
        cv::Rect(r->x, r->y + (r->height / 1.5), r->width, r->height / 2.5);

    // imagem, retangulo, cor, espessura, tipo de linha (8 conectada)
    cv::rectangle(img, mouthROI, CV_RGB(255, 255, 0), 1, 8);

    imgROI = img(mouthROI);

    cascadeSmile.detectMultiScale(imgROI, nestedObjects, 1.1, 2,
                                  0 | cv::CASCADE_FIND_BIGGEST_OBJECT,
                                  cv::Size(30, 30));
    // busca os sorrisos e desenha os retangulos onde achou
    for (std::vector<cv::Rect>::const_iterator nr = nestedObjects.begin();
         nr != nestedObjects.end(); nr++) {
      rectangle(img, cv::Point(r->x + nr->x, r->y + (r->height / 1.5) + nr->y),
                cv::Point(r->x + nr->x + nr->width,
                          r->y + (r->height / 1.5) + nr->y + nr->height),
                CV_RGB(255, 0, 255), 1, 8);
    }
  }
  imshow("Face track", img);
}
