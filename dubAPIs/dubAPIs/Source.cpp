//OpenCV Includes
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

//Realsense RSSDK Inclues
#include <pxcsession.h>
#include <pxcsensemanager.h>
#include <pxccapturemanager.h>
#include "util_render.h"

//Cinder Includes, default namespace ---cinder::	to use gl... cinder::gl...
#include <cinder/gl/texture.h>
#include <cinder/gl/gl.h>
#include <cinder/app/AppNative.h>
#include "CinderOpenCV.h"
#include <cinder/ImageIo.h>
#include <cinder/Capture.h>
#include "Client.h"
//OpenCV Namespace
using namespace cv;
using namespace ci;
using namespace ci::app;
//#include <GLFW/glfw3.h>
//#include <thread>
#include <vector>


#define DEFAULT_PACKET_SIZE 5000

struct dubCam
{
	PXCCapture::DeviceInfo one;
	PXCCapture::DeviceInfo two;
};

struct pxcDust
{
	PXCImage * pxcOne;
	PXCImage * pxcTwo;
};

struct myMats
{
	cv::Mat one;
	cv::Mat two;
};

struct myConversion
{
	IplImage *image;
	PXCImage::ImageData data;
	PXCImage::ImageData info;
	unsigned char * rgbData;
	cv::Mat cvImage;
	int height, width, rotAngle;
	cv::Mat rotMat;

};

CvSize gab_size;
cv::Mat myHomoMat;

dubCam getDeviceInfo(PXCSenseManager * mySenseMan);
void CaptureDeviceStream(PXCSenseManager * sm, PXCImage * color, pxcDust * combo, PXCCapture::Sample * mySample);
int configStreams(PXCSenseManager * senseMan,PXCCapture::DeviceInfo dev1, pxcCHAR * file);
void pxcToMat(pxcDust * imgsToConvert, myMats * convertedImages, myConversion * one);
int renderImages(myMats * imgs, SOCKET * mySock);
void calcHomo(myMats * testImgs);

//UtilRender *renderColor = new UtilRender(L"COLOR STREAM");
//UtilRender *renderColor2 = new UtilRender(L"COLOR STREAM");

void main()
{

	char c;

	//Create our SDK Session for Module Management
	PXCSenseManager *sm = PXCSenseManager::CreateInstance();
	//PXCSenseManager *sm2 = PXCSenseManager::CreateInstance();

	SOCKET mySocket;
	mySocket = setupClient();
	dubCam currentDevices;
	currentDevices = getDeviceInfo(sm);

	pxcCHAR * firstCam = L"C:/Users/Group34/Desktop/cam1.pcsdk";
	//pxcCHAR * secCam = L"C:/Users/Group34/Desktop/cam2.pcsdk";

	myMats cvImages;
	PXCImage* color = 0;
	pxcDust myPXCs = {};

	myConversion camOne = {}, camTwo = {};
	//Universal conversion info///
	gab_size.height = 480;
	gab_size.width = 640;

	camOne.image = cvCreateImage(gab_size, 8, 3);
	camTwo.image = cvCreateImage(gab_size, 8, 3);

	if (configStreams(sm,currentDevices.one,firstCam) != 0) wprintf_s(L"Error initializing\n");

	myMats convertedImages = {};
	PXCCapture::Sample * samp = 0;
	for (;;)
	{
		printf("Entering for loop\n");
		CaptureDeviceStream(sm,color,&myPXCs, samp);
		printf("Entering for loop 2\n");
		
		pxcToMat(&myPXCs, &convertedImages, &camOne);
		if (renderImages(&convertedImages,&mySocket) != 0) break;
		printf("We never even left the first iteration..\n");
	}


	//Keep our session alive as long as we are accessing modules within it
	sm->Release();
	closeClient(&mySocket);
	printf_s("Just making sure the programs working\n");
	scanf_s("%c", &c);
}



dubCam getDeviceInfo(PXCSenseManager * mySenseMan)
{
	//Session Module - Create and Query instances of I/O and algorithm implmentations
	PXCCapture::DeviceInfo camOne, camTwo;
	struct dubCam myCameras = {};
	PXCSession::ImplVersion v = mySenseMan->QuerySession()->QueryVersion();

	//Create an ImplDesc to capture our video module implementation
	PXCSession::ImplDesc desc1 = {};
	desc1.group = PXCSession::IMPL_GROUP_SENSOR;
	desc1.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;

	for (int m = 0;; m++)
	{
		PXCSession::ImplDesc desc2 = {};
		if (mySenseMan->QuerySession()->QueryImpl(&desc1, m, &desc2) < PXC_STATUS_NO_ERROR) break;
		wprintf_s(L"Module[%d]: %s\n", m, desc2.friendlyName);

		PXCCapture *captureOne = 0;

		pxcStatus sts = mySenseMan->QuerySession()->CreateImpl<PXCCapture>(&desc2, &captureOne);
		//Create CaptureManager Module.
		if (sts < PXC_STATUS_NO_ERROR) continue;
		PXCCapture::DeviceInfo dinfo;

		for (int d = 0;; d++)
		{
			sts = captureOne->QueryDeviceInfo(d, &dinfo);
			if (d == 0) camOne = dinfo;

			//else if (d == 3) camTwo = dinfo;

			if (sts < PXC_STATUS_NO_ERROR) break;
			wprintf_s(L"	Device[%d]: %s\n", d, dinfo.name);
		}
		captureOne->Release();
	}

	myCameras.one = camOne;
	myCameras.two = camOne;
	return myCameras;//camOne, camTwo;

}

void CaptureDeviceStream(PXCSenseManager * senseMan, PXCImage * color, pxcDust * combo, PXCCapture::Sample * mySample)
{
	printf("Entered capture\n");
	//PXCImage::Rotation rotatedImage;
	if (senseMan->AcquireFrame(false) < PXC_STATUS_NO_ERROR)throw(0);
	mySample = senseMan->QuerySample();
	color = mySample->color;
	printf("Got frame.\n");
	/*
	if (sm2->AcquireFrame(false) < PXC_STATUS_NO_ERROR)throw(0);
	mySample = sm2->QuerySample();
	color2 = mySample->color;
	*/
	combo->pxcOne = color;
	combo->pxcTwo = 0;
	printf("Assigned frame\n");
	senseMan->ReleaseFrame();
	printf("Released Frame..\n");
}

void pxcToMat(pxcDust * imgsToConvert, myMats * convertedImages, myConversion * one)
{


	printf("Converting image\n");
	imgsToConvert->pxcOne->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB24, &one->data);
	
	one->rgbData = one->data.planes[0];
	for (int y = 0; y < 480; y++)
	{
		for (int x = 0; x < 640; x++)
		{
			for (int k = 0; k < 3; k++)
			{
				one->image->imageData[y * 640 * 3 + x * 3 + k] = one->rgbData[y * 640 * 3 + x * 3 + k];
			}
		}
	}
	printf("Got data in MAT\n");
	imgsToConvert->pxcOne->ReleaseAccess(&one->data);
	one->cvImage = cv::cvarrToMat(one->image);
	one->height = one->cvImage.rows / 2.;
	one->width = one->cvImage.cols / 2.;
	one->rotAngle = 180;
	one->rotMat = getRotationMatrix2D(Point(one->width, one->height), (one->rotAngle), 1);
	cv::warpAffine(one->cvImage, one->cvImage, one->rotMat, one->cvImage.size());

	convertedImages->one = one->cvImage;
	convertedImages->two = one->cvImage;

}

int configStreams(PXCSenseManager * senseMan, PXCCapture::DeviceInfo dev1, pxcCHAR * file)
{
	pxcDust testPxcs = {};
	myMats testMats = {};
	PXCCaptureManager * cm = senseMan->QueryCaptureManager();
	
	senseMan->EnableStream(PXCCapture::STREAM_TYPE_COLOR,640, 480, 0);
	
	pxcBool record = false;

	cm->FilterByDeviceInfo(&dev1);
	
	senseMan->Init();
	
	/*
	if (senseMan->AcquireFrame(false) < PXC_STATUS_NO_ERROR)throw(0);
	PXCCapture::Sample* sample = senseMan->QuerySample();
	testPxcs.pxcOne = sample->color;

	if (sm2->AcquireFrame(false) < PXC_STATUS_NO_ERROR)throw(0);
	PXCCapture::Sample *sample2 = sm2->QuerySample();
	testPxcs.pxcTwo = sample2->color;

	myConversion one, two;

	one.image = cvCreateImage(gab_size, 8, 3);
	two.image = cvCreateImage(gab_size, 8, 3);

	pxcToMat(&testPxcs, &testMats, &one, &two);

	//calcHomo(&testMats);
	wprintf_s(L"Calculated homography without errors\n");
	senseMan->ReleaseFrame();
	sm2->ReleaseFrame();*/

	//Initialize our sockets
	return 0;

}

void calcHomo(myMats * testImgs)
{

	std::vector<KeyPoint> objKeys;
	std::vector<KeyPoint>  sceneKeys;
	cv::Mat greyOne, greyTwo;

	Ptr<xfeatures2d::SURF> surf = xfeatures2d::SURF::create(100);

	//cv::Mat desc_obj, desc_scene;
	cv::cvtColor(testImgs->one, greyOne, CV_RGB2GRAY);
	cv::cvtColor(testImgs->two, greyTwo, CV_RGB2GRAY);

	cv::Mat descObj, sceneObj;

	surf->detectAndCompute(greyOne, Mat(), objKeys, descObj);
	surf->detectAndCompute(greyTwo, Mat(), sceneKeys, sceneObj);

	FlannBasedMatcher matcher;
	std::vector<DMatch> matches;
	matcher.match(descObj, sceneObj, matches);

	double maxDist = 0, minDist = 100;

	std::vector<DMatch> good_matches;

	for (int i = 0; i < descObj.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < minDist) minDist = dist;
		if (dist > maxDist) maxDist = dist;
	}

	for (int i = 0; i < descObj.rows; i++)
	{
		if (matches[i].distance < (3 * minDist))
		{
			good_matches.push_back(matches[i]);
		}
	}
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		obj.push_back(objKeys[good_matches[i].queryIdx].pt);
		scene.push_back(sceneKeys[good_matches[i].trainIdx].pt);
	}

	myHomoMat = findHomography(obj, scene, CV_RANSAC);
	
}

int renderImages(myMats * imgs,SOCKET * mySock)
{

	cv::Mat result;

	Size sz1 = imgs->one.size();
	Mat im3(sz1.height, sz1.width, CV_8UC3);
	imgs->two.copyTo(im3);

	int byteChecker = 0;
	int size, temp;
	char * rdyUp;

	rdyUp = (char *)malloc(sizeof(char) * 6);
	if (imgs->two.data != NULL)
	{

		temp = 0;
		size = (int) imgs->two.total() * imgs->two.elemSize();
		printf("Size of transfer = %d\n", size);
		/*byteChecker = sendClient(mySock, (unsigned char *)"HEADS-UP", 8);
		if (byteChecker != 0)
		{
			printf("CLIENT: Issue sending heads up.");
		}*/

		/*recvClient(mySock, (unsigned char*)rdyUp, 5);
		if (strcmp(rdyUp, "Ready") == 0)
		{
			printf("Recieved ready! |%s|\n", rdyUp);
			memset(rdyUp, '\0', 6);
		*/	
			while (size >= (temp + DEFAULT_PACKET_SIZE))
			{
				byteChecker = sendClient(mySock, imgs->two.data + temp, DEFAULT_PACKET_SIZE);
				if (byteChecker != 0)
				{
					printf("Error with sendClient. \n");
				}
				temp += DEFAULT_PACKET_SIZE;
			}
						
			int bytesLeft = size - temp;
			if (bytesLeft != 0)
			{

				byteChecker = sendClient(mySock, (unsigned char *)imgs->two.data + (temp-1), bytesLeft);
			}
			if (byteChecker != 0)
			{
				printf("Something went wrong with the last packet. \n");
			}
		}
		cv::Mat AfterImg = cv::Mat(cv::Size(640, 480), CV_8UC3, imgs->two.data);

		imshow("ClientSide", AfterImg);
		if (cvWaitKey(10) >= 0)
		{
			throw(0);
		}
	//}
	return 0;
}