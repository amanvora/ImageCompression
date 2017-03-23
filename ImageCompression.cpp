//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Used to read in a standard RGB image and display it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Modified by:
// Aman Vora
// For CSCI 576 HW2
//
//*****************************************************************************

#include <time.h>

// Include class files
#include "Image.h"
using namespace std;
#define MAX_LOADSTRING 100

// Global Variables:
MyImage			inImage, segmentedImage;		// image objects
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

#define WIDTH	352
#define HEIGHT	288
int N;											// Number of 2-bit vectors

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	char inImagePath[_MAX_PATH];
	sscanf(lpCmdLine, "%s %d", &inImagePath, &N);
	if (strstr(inImagePath, ".rgb") != NULL) {
		inImage.setType(RGB);

		inImage.setImagePath(inImagePath);
		inImage.setHeight(HEIGHT);
		inImage.setWidth(WIDTH);

		// Ensure input buffer pointer isn't null
		inImage.allocData();
	}
	else if (strstr(inImagePath, ".raw") != NULL) {
		inImage.setType(GRAY);

		inImage.setImagePath(inImagePath);
		inImage.setHeight(HEIGHT);
		inImage.setWidth(WIDTH);

		// Ensure input buffer pointer isn't null
		inImage.allocData();
	}
	else {
		AfxMessageBox("Image has to be a '.rgb' file\nUsage - Image.exe image.rgb w h");
	}

	if (!inImage.ReadImage(inImage.getType())){
		AfxMessageBox("Could not read image\nUsage - Image.exe image.rgb w h");
		//return FALSE;
	}

	// k-means segmentation on R channel. Same replicated to G and B channels
	if (inImage.getType() == GRAY) {
		MyImage* rChannel = inImage.getChannel(R);
		MyImage* segmentedR = new MyImage;
		*segmentedR = *rChannel;
		rChannel->kMeansSegment(segmentedR, N);

		// The segmentation has been done. Now just for the sake of display purposes, replicate the
		// result to all three channels
		MyImage* segmentedG = new MyImage;
		*segmentedG = *segmentedR;
		MyImage* segmentedB = new MyImage;
		*segmentedB = *segmentedR;

		segmentedImage.combineChannels(segmentedB, segmentedG, segmentedR);
	}
	else if (inImage.getType() == RGB) {
		// Image segmentation in each separate channel of RGB image won't give satisfatory results for low N
		//MyImage* rChannel = inImage.getChannel(R);
		//MyImage* segmentedR = new MyImage;
		//*segmentedR = *rChannel;
		//rChannel->kMeansSegment(segmentedR, N);

		//MyImage* gChannel = inImage.getChannel(G);
		//MyImage* segmentedG = new MyImage;
		//*segmentedG = *gChannel;
		//gChannel->kMeansSegment(segmentedG, N);

		//MyImage* bChannel = inImage.getChannel(B);
		//MyImage* segmentedB = new MyImage;
		//*segmentedB = *bChannel;
		//bChannel->kMeansSegment(segmentedB, N);
		//segmentedImage.combineChannels(segmentedB, segmentedG, segmentedR);

		int sumOfPows = log10(N) / log10(2); // since N = 2^(x+y+w)
		int x = ceil(sumOfPows/3.0);		 // Approx value of x
		int y = ceil((sumOfPows-x)/2.0);   // Approx value of y
		int w = sumOfPows - x - y;

		MyImage* hsv = inImage.rgb2hsv();

		MyImage* hChannel = hsv->getChannel(H);
		MyImage* segmentedH = new MyImage;
		*segmentedH = *hChannel;
		hChannel->kMeansSegment(segmentedH, pow(2,w));

		MyImage* sChannel = hsv->getChannel(S);
		MyImage* segmentedS = new MyImage;
		*segmentedS = *sChannel;
		sChannel->kMeansSegment(segmentedS, pow(2,y));

		MyImage* vChannel = hsv->getChannel(V);
		MyImage* segmentedV = new MyImage;
		*segmentedV = *vChannel;
		vChannel->kMeansSegment(segmentedV, pow(2,x));

		MyImage* segmentedHSV = new MyImage;
		*segmentedHSV = *hsv;
		segmentedHSV->combineChannels(segmentedV, segmentedS, segmentedH);
		segmentedImage = *(segmentedHSV->hsv2rgb());
	}
	segmentedImage.setType(RGB);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, (int)0x032F, (int)0x01AF, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// TO DO: part useful to render video frames, may place your own code here in this function
	// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case ID_MODIFY_IMAGE:
			inImage.Modify();
			InvalidateRect(hWnd, &rt, false);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TO DO: Add any drawing code here...
		char text[1000];
		sprintf(text, "Image Width:%d, Height:%d\nOriginal | Compressed", inImage.getWidth(), inImage.getHeight());
		DrawText(hdc, text, strlen(text), &rt, DT_CENTER);
		BITMAPINFO bmi;
		CBitmap bitmap;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
		bmi.bmiHeader.biWidth = inImage.getWidth();
		bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 24;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();
		clock_t startTime, executionTime;
		// Keep track of system time of execution also
		startTime = clock();

		// Display original image
		SetDIBitsToDevice(hdc,
			20, 40, inImage.getWidth(), inImage.getHeight(),
			0, 0, 0, inImage.getHeight(),
			inImage.getImageData(), &bmi, DIB_RGB_COLORS);

		// Display compressed image
		SetDIBitsToDevice(hdc,
			20+WIDTH+20, 40, segmentedImage.getWidth(), segmentedImage.getHeight(),
			0, 0, 0, segmentedImage.getHeight(),
			segmentedImage.getImageData(), &bmi, DIB_RGB_COLORS);

		// Total system time of execution
		executionTime = clock() - startTime;
		double execInMillis = (executionTime * 1000) / CLOCKS_PER_SEC;

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}




// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}