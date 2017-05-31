//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Edited by Aman Vora
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include <iostream>
#include "Image.h"
#include <time.h>

using namespace std;

// Constructor and Desctructors
MyImage::MyImage()
{
	Data = NULL;
	Width = -1;
	Height = -1;
	imgType = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data!=NULL )
		delete Data;
}

// Constructor to initialize all data values
MyImage::MyImage(int w, int h, int c, unsigned char val) {
	Height = h;
	Width = w;
	imgType = c;
	Data = new unsigned char[Width*Height*c];
	for (int i = 0; i < (Height*Width*c); i++) {
		Data[i] = val;
	}
}

// Copy constructor
MyImage::MyImage(MyImage& otherImage)
{
	Height = otherImage.Height;
	Width = otherImage.Width;
	imgType = otherImage.imgType;
	//if (Data != NULL)
	//	delete[] Data;

	Data = new unsigned char[Width*Height*imgType];
	//strcpy(otherImage->ImagePath, ImagePath);
	strcpy(ImagePath, (char*)otherImage.ImagePath);

	for (int i = 0; i < (Height*Width*imgType); i++)
	{
		Data[i] = otherImage.Data[i];
	}

}



// = operator overload
MyImage & MyImage::operator= (MyImage &otherImage)
{
	Height = otherImage.Height;
	Width = otherImage.Width;
	imgType = otherImage.imgType;

	//if (Data != NULL)
	//	delete[] Data;
	Data = new unsigned char[Width*Height*imgType];
	//strcpy((char *)otherImage.ImagePath, ImagePath);
	strcpy(ImagePath, (char *)otherImage.ImagePath);
	for (int i = 0; i < (Height*Width*imgType); i++)
	{
		Data[i] = otherImage.Data[i];
	}

	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage(int c)
{
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	FILE* IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}
	int i;
	unsigned char *Rbuf = new unsigned char[Height*Width];
	unsigned char *Gbuf = new unsigned char[Height*Width];
	unsigned char *Bbuf = new unsigned char[Height*Width];

	if (c == GRAY) {
		// Create and populate RGB buffers
		for (i = 0; i < Width*Height; i++)
		{
			Rbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i++)
		{
			Gbuf[i] = Rbuf[i];
		}
		for (i = 0; i < Width*Height; i++)
		{
			Bbuf[i] = Rbuf[i];
		}

	}
	else if (c == RGB) {
		// Create and populate RGB buffers
		for (i = 0; i < Width*Height; i++)
		{
			Rbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i++)
		{
			Gbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i++)
		{
			Bbuf[i] = fgetc(IN_FILE);
		}
	}
	for (i = 0; i < Height*Width; i++)
	{
		Data[3 * i] = Bbuf[i];
		Data[3 * i + 1] = Gbuf[i];
		Data[3 * i + 2] = Rbuf[i];
	}

	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;

	fclose(IN_FILE);
	return true;

}


// Return the particular channel of an image as a separate MyImage object
MyImage* MyImage::getChannel(int c) {
	MyImage* channel = new MyImage;
	channel->Height = Height;
	channel->Width = Width;
	channel->imgType = GRAY;
	channel->Data = new unsigned char[Width*Height];

	for (int i = 0; i < Height*Width; i++) {
		channel->Data[i] = Data[3 * i + c];
	}

	return channel;
}

void MyImage::combineChannels(const MyImage* bChannel, const MyImage* gChannel, const MyImage* rChannel) {
	Height = bChannel->Height;
	Width = bChannel->Width;
	Data = new unsigned char[Width*Height*RGB];

	for (int i = 0; i < Height*Width; i++) {
		Data[3 * i + B] = bChannel->Data[i];
		Data[3 * i + G] = gChannel->Data[i];
		Data[3 * i + R] = rChannel->Data[i];
	}
}

// MyImage functions defined here
bool MyImage::WriteImage(int c)
{
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	FILE* OUT_FILE = fopen(ImagePath, "wb");
	if (OUT_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}
	// Create and populate RGB buffers
	int i;
	unsigned char *Rbuf = new unsigned char[Height*Width];
	unsigned char *Gbuf = new unsigned char[Height*Width];
	unsigned char *Bbuf = new unsigned char[Height*Width];

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3 * i];
		Gbuf[i] = Data[3 * i + 1];
		Rbuf[i] = Data[3 * i + 2];
	}

	if (c == GRAY) {
		// Write data to file
		for (i = 0; i < Width*Height; i++)
		{
			fputc(Rbuf[i], OUT_FILE);
		}
	}
	else if(c == RGB){
		// Write data to file
		for (i = 0; i < Width*Height; i++)
		{
			fputc(Rbuf[i], OUT_FILE);
		}
		for (i = 0; i < Width*Height; i++)
		{
			fputc(Gbuf[i], OUT_FILE);
		}
		for (i = 0; i < Width*Height; i++)
		{
			fputc(Bbuf[i], OUT_FILE);
		}
	}
	// Clean up and return
	delete[] Rbuf;
	delete[] Gbuf;
	delete[] Bbuf;
	fclose(OUT_FILE);
	return true;
}


// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{
	// sample operation
	for (int i = 0; i < Width*Height; i++)
	{
		Data[3 * i] = 0;
		Data[3 * i + 1] = 0;
		Data[3 * i + 2] = 0;
	}

	return false;
}

// Allocate memory for the data buffer
void MyImage::allocData() {
	// This is hard coded to be 3 because I was unable to display
	// grayscale images using the MFC function SetDIBitsToDevice
	Data = new unsigned char[Width*Height*3];
}

int findClosest(int i, int j, pair<double, double>* centers, int k) {
	int index = 0;
	float minDist = pow((i - centers[0].first), 2) + pow((j - centers[0].second), 2);
	for (int c = 1; c < k; c++) {
		float currDist = pow((i - centers[c].first), 2) + pow((j - centers[c].second), 2);
		if (currDist < minDist) {
			minDist = currDist;
			index = c;
		}
	}
	return index;
}

// Performs image segmentation using KNN on the grayscale img
void MyImage::kMeansSegment(MyImage* comp, int k) {
	// ------------------------------ Form a 256*256 mapping buffer/table ------------------------------ //
	MyImage* mappingTable = new MyImage(256, 256, 1, 0); // 256 by 256 array with 1 channel all values initialized to zero

	unsigned char* tableData = mappingTable->Data;
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j += 2) {
			int imgIndex = j + i*Width;
			unsigned char pixel1 = Data[imgIndex];
			unsigned char pixel2 = Data[imgIndex + 1];

			tableData[(int)pixel1 + 256 * ((int)pixel2)] += 1;
		}
	}
	// ------------------------------------------------------------------------------------------------- //

	// -------------------------------------- Make k-clusters ------------------------------------------ //

	// Determine number of rows and cols of k-clusters' initialization
	//int sumOfPows = log10(k) / log10(2); // since k = 2^(x+y), determine sumOfPows = (x+y)
	//int powOfRows = sumOfPows / 2;		 // Approx value of x
	//int powOfCols = sumOfPows - powOfRows;// Approx value of y

	//int clustRows = pow(2, powOfRows);
	//int clustCols = pow(2, powOfCols);

	// Initialize random seed
	srand(time(NULL));

	// Declare k cluster centers
	pair<double, double>* clusterCenters = new pair<double, double>[k];

	pair<double, double>* clusterAvg = new pair<double, double>[k];

	// Declare an array of size k representing the size of each cluster
	int* clusterSize = new int[k];
	int* prevSize = new int[k];

	// Initialize previous size as zero
	for (int i = 0; i < k; i++)
		prevSize[i] = 0;

	// Initialize cluster centers randomly
	for (int i = 0; i < k; i++) {
		clusterCenters[i].first = rand() % 256;
		clusterCenters[i].second = rand() % 256;
	}
	//for (int i = 1; i <= clustRows; i++) {
	//	for (int j = 1; j <= clustCols; j++) {
	//		clusterCenters[j + clustRows*i].first = i*256.0/(clustRows + 1);
	//		clusterCenters[j + clustRows*i].second = j*256.0/(clustCols + 1);
	//	}
	//}

	// ------------------------------------------------------------------------------------------------- //

	// ------------------------------------ Start convergence ------------------------------------------ //
	// 256 by 256 clusterIndex with 1 channel, all values initialized to zero
	MyImage* clusterIndex = new MyImage(256, 256, 1, 0);
	bool converged = false;
	int iterations = 0;
	while (converged == false) {
		iterations++;
		converged = true;
		// Initialize each cluster as empty
		for (int i = 0; i < k; i++)
			clusterSize[i] = 0;

		// Initialize each cluster's average
		for (int i = 0; i < k; i++) {
			clusterAvg[i].first = 0;
			clusterAvg[i].second = 0;
		}

		for (int i = 0; i < 256; i++) {
			for (int j = 0; j < 256; j++) {

				// Check if cluster point(s) is present or not 
				if (tableData[j + i * 256] > 0) {
					// Find the nearest cluster center's index to (i,j)
					int nearestIndex = findClosest(i, j, clusterCenters, k);

					// Save the index
					clusterIndex->Data[j + i * 256] = (unsigned char)nearestIndex;

					// Update size of the cluster with this index
					clusterSize[nearestIndex] += (int)tableData[j + i * 256];

					// Update the average of cluster
					clusterAvg[nearestIndex].first += i*(int)tableData[j + i * 256];
					clusterAvg[nearestIndex].second += j*(int)tableData[j + i * 256];
				}
			}
		}

		// Check if size of each cluster has changed or not, i.e., whether there is convergence or not
		for (int i = 0; i < k; i++) {
			if (clusterSize[i] != prevSize[i]) {
				converged = false;
				break;
			}
		}

		// Compute each cluster's average
		for (int i = 0; i < k; i++) {
			if (clusterSize[i] > 0) {
				clusterAvg[i].first /= clusterSize[i];
				clusterAvg[i].second /= clusterSize[i];
			}
			else {
				// If cluster was empty, re-initialize to some random 2D point
				clusterAvg[i].first = rand() % 256;
				clusterAvg[i].second = rand() % 256;
			}
		}

		// Update new cluster centers
		for (int i = 0; i < k; i++) {
			clusterCenters[i].first = clusterAvg[i].first;
			clusterCenters[i].second = clusterAvg[i].second;
		}

		// Initialize previous size as zero
		for (int i = 0; i < k; i++)
			prevSize[i] = clusterSize[i];

	}
	// ------------------------------------------------------------------------------------------------- //

	// ---------------------------- Segment image using cluster centers -------------------------------- //
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j += 2) {
			int imgIndex = j + i*Width;
			unsigned char pixel1 = Data[imgIndex];
			unsigned char pixel2 = Data[imgIndex + 1];
			int cIndex = clusterIndex->Data[(int)pixel1 + 256 * ((int)pixel2)];
			comp->Data[imgIndex] = (unsigned char)clusterCenters[cIndex].first;
			comp->Data[imgIndex + 1] = (unsigned char)clusterCenters[cIndex].second;
		}
	}
	// ------------------------------------------------------------------------------------------------- //
	delete[] clusterCenters;
	delete[] clusterAvg;
}


// Converts RGB to HSV 
// Used this site: https://www.cs.rit.edu/~ncs/color/t_convert.html
MyImage* MyImage::rgb2hsv() {
	MyImage* hsv = new MyImage;
	*hsv = *this;
	MyImage* rChannel = this->getChannel(R);
	MyImage* gChannel = this->getChannel(G);
	MyImage* bChannel = this->getChannel(B);

	// Declare h,s and v
	MyImage* hChannel = new MyImage;
	*hChannel = *rChannel;
	MyImage* sChannel = new MyImage;
	*sChannel = *rChannel;
	MyImage* vChannel = new MyImage;
	*vChannel = *rChannel;

	for (int i = 0; i < Height*Width; i++) {
		unsigned char minColor, maxColor, delta;
		minColor = min(rChannel->Data[i], gChannel->Data[i]);
		minColor = min(minColor, bChannel->Data[i]);

		maxColor = max(rChannel->Data[i], gChannel->Data[i]);
		maxColor = max(maxColor, bChannel->Data[i]);

		vChannel->Data[i] = maxColor;
		delta = maxColor - minColor; // Definitely in the range [0, 255]

		float hVal;
		if (rChannel->Data[i] == maxColor)
			hVal = ((float)gChannel->Data[i] - bChannel->Data[i]) / delta;
		else if (gChannel->Data[i] == maxColor)
			hVal = 2 + ((float)bChannel->Data[i] - rChannel->Data[i]) / delta;
		else
			hVal = 4 + ((float)rChannel->Data[i] - gChannel->Data[i]) / delta;

		hVal *= 60;
		if (hVal < 0)
			hVal += 360;

		hChannel->Data[i] = (unsigned char)(hVal * 255.0 / 360);

		if (maxColor != 0) {
			sChannel->Data[i] = ((unsigned char)((255.0*delta) / maxColor));
		}
		else {
			sChannel->Data[i] = 0;
			hChannel->Data[i] = 0;
		}
	}
	hsv->combineChannels(vChannel, sChannel, hChannel);
	return hsv;
}


// Converts HSV to RGB
// Used this site: https://www.cs.rit.edu/~ncs/color/t_convert.html
MyImage* MyImage::hsv2rgb() {
	MyImage* rgb = new MyImage;
	*rgb = *this;
	MyImage* hChannel = this->getChannel(H);
	MyImage* sChannel = this->getChannel(S);
	MyImage* vChannel = this->getChannel(V);

	// Declare r, g and b
	MyImage* rChannel = new MyImage;
	*rChannel = *hChannel;
	MyImage* gChannel = new MyImage;
	*gChannel = *hChannel;
	MyImage* bChannel = new MyImage;
	*bChannel = *hChannel;

	for (int i = 0; i < Height*Width; i++) {
		float hVal = (hChannel->Data[i] * 360.0 / 255) / 60;
		float f = hVal - int(hVal);
		unsigned char p = (unsigned char)(vChannel->Data[i] * (1 - sChannel->Data[i] / 255.0));
		unsigned char q = (unsigned char)(vChannel->Data[i] * (1 - sChannel->Data[i] * f / 255));
		unsigned char t = (unsigned char)(vChannel->Data[i] * (1 - sChannel->Data[i] * (1 - f) / 255));

		switch ((int)hVal)
		{
		case 0:
			rChannel->Data[i] = vChannel->Data[i];
			gChannel->Data[i] = t;
			bChannel->Data[i] = p;
			break;

		case 1:
			rChannel->Data[i] = q;
			gChannel->Data[i] = vChannel->Data[i];
			bChannel->Data[i] = p;
			break;

		case 2:
			rChannel->Data[i] = p;
			gChannel->Data[i] = vChannel->Data[i];
			bChannel->Data[i] = t;
			break;

		case 3:
			rChannel->Data[i] = p;
			gChannel->Data[i] = q;
			bChannel->Data[i] = vChannel->Data[i];
			break;

		case 4:
			rChannel->Data[i] = t;
			gChannel->Data[i] = p;
			bChannel->Data[i] = vChannel->Data[i];
			break;

		default:
			rChannel->Data[i] = vChannel->Data[i];
			gChannel->Data[i] = p;
			bChannel->Data[i] = q;
			break;
		}

		if (sChannel->Data[i] == 0) {
			rChannel->Data[i] = vChannel->Data[i];
			gChannel->Data[i] = vChannel->Data[i];
			bChannel->Data[i] = vChannel->Data[i];
		}
	}
	rgb->combineChannels(bChannel, gChannel, rChannel);
	return rgb;
}
