#include "BattleElfEng.h"
#include "Video.h"

#include <opencv2/opencv.hpp>
#include <mmsystem.h>

#define PWIDTH 3
#define PHEIGHT 3

int main() {
	cv::VideoCapture cap(".\\resources\\badapple.mp4");
	short width = (cap.get(cv::CAP_PROP_FRAME_WIDTH) / PWIDTH), height = (cap.get(cv::CAP_PROP_FRAME_HEIGHT) / PHEIGHT);

	PlaySound(TEXT(".\\resources\\badapple.wav"), NULL, SND_FILENAME | SND_ASYNC);

	cv::Mat frame;

	unsigned char* pixelPtr;
	int cn;

	float factor = (255.0 * (PHEIGHT * PWIDTH));

	WindowCMD wind(width, height, cap.get(cv::CAP_PROP_FPS));
	//WindowCMD wind(width, height);

	while (true) {
		wind.frameStart = std::chrono::system_clock::now();
		wind.time->timeFunction();

		cap >> frame;
		if (!frame.data)
			break;

		pixelPtr = (unsigned char*)frame.data;
		cn = frame.channels();

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				float meanGS = 0, meanR = 0, meanG = 0, meanB = 0;
				for (int k = 0; k < PHEIGHT; k++) {
					for (int h = 0; h < PWIDTH; h++) {
						meanR += (pixelPtr[(((frame.cols * j * PHEIGHT) + (frame.cols * k)) + (i * PWIDTH + h)) * 3 + 2] / factor);
						meanG += (pixelPtr[(((frame.cols * j * PHEIGHT) + (frame.cols * k)) + (i * PWIDTH + h)) * 3 + 1] / factor);
						meanB += (pixelPtr[(((frame.cols * j * PHEIGHT) + (frame.cols * k)) + (i * PWIDTH + h)) * 3 + 0] / factor);
					}
				}

				wchar_t sym;
				short bg_col;
				short fg_col;

				ClassifyPixel_Grey(meanR, meanG, meanB, sym, fg_col, bg_col);

				wind.setChar(i, j, sym, bg_col | fg_col);
				
			}
		}

		wind.draw();
	}

	cap.release();
}