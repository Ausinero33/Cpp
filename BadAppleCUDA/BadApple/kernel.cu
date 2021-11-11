#include "BattleElfEng.h"
#include "Video.h"

#include <opencv2/opencv.hpp>
#include <mmsystem.h>

#include <cuda_runtime.h>
#include "device_launch_parameters.h"

#define PWIDTH 3
#define PHEIGHT 3

__global__ void processPixel(unsigned char* pixelPtr, CHAR_INFO* screen, int width, int height, int cols, float factor, int cn) {
	int i = threadIdx.x;
	int j = blockIdx.x;

	float meanGS = 0, meanR = 0, meanG = 0, meanB = 0;
	for (int k = 0; k < PHEIGHT; k++) {
		for (int h = 0; h < PWIDTH; h++) {
			meanR += (pixelPtr[(((cols * j * PHEIGHT) + (cols * k)) + (i * PWIDTH + h)) * cn + 2] / factor);
			meanG += (pixelPtr[(((cols * j * PHEIGHT) + (cols * k)) + (i * PWIDTH + h)) * cn + 1] / factor);
			meanB += (pixelPtr[(((cols * j * PHEIGHT) + (cols * k)) + (i * PWIDTH + h)) * cn + 0] / factor);
		}
	}

	short sym;
	short bg_col;
	short fg_col;

	float luminance = 0.2987f * meanR + 0.5870f * meanG + 0.1140f * meanB;
	int pixel_bw = (int)(luminance * 13.0f);
	switch (pixel_bw)
	{
	case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

	case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
	case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
	case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

	case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
	case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
	case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
	case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

	case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
	case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
	case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
	case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
	}

	short color = (bg_col | fg_col);

	screen[i + j * width].Char.UnicodeChar = sym;
	screen[i + j * width].Attributes = color;
}

int main() {
	cv::VideoCapture cap(".\\resources\\badapple.mp4");
	short width = (cap.get(cv::CAP_PROP_FRAME_WIDTH) / PWIDTH), height = (cap.get(cv::CAP_PROP_FRAME_HEIGHT) / PHEIGHT);

	PlaySound(TEXT(".\\resources\\badapple.wav"), NULL, SND_FILENAME | SND_ASYNC);

	cv::Mat frame;

	unsigned char* pixelPtr, * dev_pixelPtr;
	int cn;

	float factor = (255.0 * (PHEIGHT * PWIDTH));
	bool flag = true; 

	//WindowCMD wind(width, height, cap.get(cv::CAP_PROP_FPS));
	WindowCMD wind(width, height);

	CHAR_INFO* screen = new CHAR_INFO[width * height];
	CHAR_INFO* dev_screen;

	int a = sizeof(CHAR);

	cudaMalloc((void**)&dev_screen, sizeof(CHAR_INFO) * width * height);

	while (true) {
		wind.frameStart = std::chrono::system_clock::now();

		cap >> frame;
		if (!frame.data)
			break;

		pixelPtr = (unsigned char*)frame.data;
		cn = frame.channels();

		if (flag) {
			cudaMalloc((void**)&dev_pixelPtr, sizeof(unsigned char) * frame.cols * frame.rows * cn);
			flag = false;
		}

		cudaMemcpy(dev_pixelPtr, pixelPtr, sizeof(unsigned char) * frame.cols * frame.rows * cn, cudaMemcpyHostToDevice);

		processPixel << <height, width >> > (dev_pixelPtr, dev_screen, width, height, frame.cols, factor, cn);
		//cudaDeviceSynchronize();

		cudaMemcpy(screen, dev_screen, sizeof(CHAR_INFO) * width * height, cudaMemcpyDeviceToHost);

		wind.setScreen(screen);

		wind.draw();
		wind.time->timeFunction();
	}

	cap.release();
}