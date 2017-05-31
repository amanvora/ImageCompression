//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Edited by Aman Vora
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define B 0
#define G 1
#define R 2

#define V 0
#define S 1
#define H 2

#define GRAY 1
#define RGB 3

// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	unsigned char*	Data;			// RGB/grayscale data of the image
	int		imgType;				// Either GRAY or RGB
public:
	// Defualt Constructor
	MyImage();

	// Constructor
	MyImage(int, int, int, unsigned char);

	// Copy Constructor
	MyImage(MyImage& otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= ( MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth(const int w) { Width = w; };
	void	setHeight(const int h) { Height = h; };
	void	setType(const int t) { imgType = t; };
	void	setImageData(const char *img) { Data = (unsigned char *)img; }
	void	setImagePath(const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; }
	int		getHeight() { return Height; }
	int		getType() { return imgType; }
	unsigned char*	getImageData() { return Data; };
	MyImage* getChannel(int);
	void	combineChannels(const MyImage*, const MyImage*, const MyImage*);
	char*	getImagePath() { return ImagePath; }

	// Input Output operations
	bool	ReadImage(int);
	bool	WriteImage(int);
	// Modifications
	void	allocData();
	bool	Modify();
	void	kMeansSegment(MyImage*, int);
	MyImage* rgb2hsv();
	MyImage* hsv2rgb();
};

#endif //IMAGE_DISPLAY
