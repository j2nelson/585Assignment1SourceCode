#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

void tintImageBlue(Mat& image);
void grayscaleImage(Mat& image);
void flipHorizontalImage(Mat& image);
void flipVerticalImage(Mat& image);
void blurImage(Mat& image);
void average(int row, int col, Mat& image);


int main()
{
	Mat originalImage, alteredImage; 
	int key;
	int loopNum = 0;
	int blur = 0;
	bool wait = true;
	
	std::cout << "Press the right and left arrows to change the altered image." << std::endl;
	std::cout << "Press the up arrow to increase blur and the down arrow to decrease blur." << std::endl;
	std::cout << "Press q to quit." << std::endl;

	originalImage = imread("jamie.jpg", IMREAD_COLOR); // Read the file
	if (originalImage.empty()) // Check for invalid input
		{
			cout << "Could not open or find the image" << std::endl;
			return 0;
	}

	do {	
		alteredImage = imread("jamie.jpg", IMREAD_COLOR);
		if (originalImage.empty() || alteredImage.empty()) // Check for invalid input
		{
			cout << "Could not open or find the image" << std::endl;
			return 0;
		}
		switch (loopNum) {
			case 1:
				tintImageBlue(alteredImage);
				break;
			case 2:
				grayscaleImage(alteredImage);
				break;
			case 3:
				flipHorizontalImage(alteredImage);
				break;
			case 4:
				flipVerticalImage(alteredImage);
				break;
		}

		for (int b = 0; b < blur; b++) {
			blurImage(alteredImage);
		}

		imshow("Original Picture", originalImage); 
		imshow("Altered Picture", alteredImage);                // Show our image inside it.

		do {
			key = waitKeyEx(); // Wait for a keystroke in the window
			wait = false;
			if (key == 2555904) {
				if (loopNum < 4) {
					loopNum++;
				} else {
					loopNum = 0;
				}
			} else if (key == 2424832) {
				if (loopNum == 0) {
					loopNum = 4;
				} else {
					loopNum--;
				}
			} else if (key == 2490368) {
				blur++;
			} else if (key == 2621440 && blur != 0) {
				blur--;
			} else if (key == 113) {
				std::cout << "Exiting...";
			} else {
				wait = true;
			}
		} while (wait);
	} while (key != 113);
	imwrite("alteredImage.jpg", alteredImage);
	destroyAllWindows();
	return 0;
}

void tintImageBlue(Mat& image)
{
	if (image.empty())
	{
		cout << "Error: image is unallocated" << endl;
		return;
	}
	int channels = image.channels();
	for (int row = 0; row < image.rows; row++)
	{
		//the .ptr method of the Mat class returns a pointer to the specifed row.
		//the type of the returned pointer is given inside the angle brackets
		//
		// The reason you might prefer to use this notation is because
		// Mat objects can be used to store other types of data too, not just unsigned char's.
		// However, the data member variable is always of unsigned char.
		unsigned char* rowPtr = image.ptr<unsigned char>(row);
		for (int col = 0; col < image.cols; col++)
		{
			// Once you have a pointer to the beginning of the row, you still need to calculate
			// the index of the element of the row that you want to access
			int index = col * channels + 0;	//0 is the blue channel index
			rowPtr[index] = 255;
		}
	}
}

void grayscaleImage(Mat& image)
{
	if (image.empty())
	{
		cout << "Error: image is unallocated" << endl;
		return;
	}
	int channels = image.channels();
	for (int row = 0; row < image.rows; row++)
	{
		unsigned char* rowPtr = image.ptr<unsigned char>(row);
		for (int col = 0; col < image.cols; col++)
		{
			int indexBlue = col * channels + 0;	//0 is the blue channel index
			int indexGreen = col * channels + 1;	//1 is the green channel index
			int indexRed = col * channels + 2;	//2 is the red channel index
			int value = 0.07 * rowPtr[indexBlue] + 0.72 * rowPtr[indexGreen] + 0.21 * rowPtr[indexRed];
			rowPtr[indexBlue] = value;
			rowPtr[indexGreen] = value;
			rowPtr[indexRed] = value;
		}
	}
}

void flipHorizontalImage(Mat& image)
{
	if (image.empty())
	{
		cout << "Error: image is unallocated" << endl;
		return;
	}
	int channels = image.channels();
	for (int col = 0; col <= (image.cols / 2); col++)
	{
		for (int row = 0; row < image.rows; row++)
		{
			unsigned char* rowPtr = image.ptr<unsigned char>(row);
			for (int i = 0; i < 3; i++) {
				int index = col * channels + i;
				int indexOtherHalf = (image.cols - col) * channels + i;
				int temp = rowPtr[index];
				rowPtr[index] = rowPtr[indexOtherHalf];
				rowPtr[indexOtherHalf] = temp;
			}

		}
	}
}

void flipVerticalImage(Mat& image)
{
	if (image.empty())
	{
		cout << "Error: image is unallocated" << endl;
		return;
	}
	int channels = image.channels();
	for (int row = 0; row <= (image.rows / 2); row++)
	{
		unsigned char* rowPtr = image.ptr<unsigned char>(row);
		unsigned char* rowHalfPtr = image.ptr<unsigned char>((image.rows - row));
		for (int col = 0; col < image.cols; col++)
		{
			int indexBlue = col * channels + 0;	//0 is the blue channel index
			int indexGreen = col * channels + 1;	//1 is the green channel index
			int indexRed = col * channels + 2;	//2 is the red channel index
			int temp = rowPtr[indexBlue];
			rowPtr[indexBlue] = rowHalfPtr[indexBlue];
			rowHalfPtr[indexBlue] = temp;

			temp = rowPtr[indexGreen];
			rowPtr[indexGreen] = rowHalfPtr[indexGreen];
			rowHalfPtr[indexGreen] = temp;

			temp = rowPtr[indexRed];
			rowPtr[indexRed] = rowHalfPtr[indexRed];
			rowHalfPtr[indexRed] = temp;
		}
	}
}

void blurImage(Mat& image)
{
	if (image.empty())
	{
		cout << "Error: image is unallocated" << endl;
		return;
	}
	for (int row = 0; row < image.rows; row++)
	{
		for (int col = 0; col < image.cols; col++)
		{
			average(row, col, image);
		}
	}
}

void average(int row, int col, Mat& image) {
	int average[3] = {0, 0, 0};
	int total = 0;
	int channels = image.channels();
	
	for(int x = -1; x <= 1; x++) {
		if ((row + x) < 0 || (row + x) > image.rows) {
			continue;
		}
		for (int y = -1; y <= 1; y++) {
			if ((col + y) < 0 || (col + y) > image.cols) {
				continue;
			}
			unsigned char* rowPtr = image.ptr<unsigned char>((row + x));
			for (int i = 0; i < 3; i++) {
				int index = (col + y) * channels + i;
				average[i] = average[i] + rowPtr[index];
			}
			total++;
		}
	}
	unsigned char* originalRowPtr = image.ptr<unsigned char>(row);
	for (int i = 0; i < 3; i++) {
		average[i] = average[i] / total;
		int originalIndex = col * channels + i;
		originalRowPtr[originalIndex] = average[i];
	}
}