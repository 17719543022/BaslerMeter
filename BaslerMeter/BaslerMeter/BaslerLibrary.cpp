#include "pch.h"

// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 1;

// Number of video images to be grabbed.
static const uint32_t c_countOfVideoImagesToGrab = 10000;

extern CPen penNull;

extern HBITMAP NearestNeighborResize(HDC hdc, HBITMAP hBmpSrc, int newWidth, int newHeight)
{
    // 获取源位图的信息
    BITMAP bmpSrc;
    GetObject(hBmpSrc, sizeof(BITMAP), &bmpSrc);

    // 创建目标位图
    HBITMAP hBmpDest = CreateCompatibleBitmap(hdc, newWidth, newHeight);

    // 获取源位图和目标位图的数据
    int srcWidth = bmpSrc.bmWidth;
    int srcHeight = bmpSrc.bmHeight;
    int srcBytesPerPixel = bmpSrc.bmBitsPixel / 8;
    int srcBytesPerLine = ((srcWidth * srcBytesPerPixel + 3) / 4) * 4;
    int destBytesPerLine = ((newWidth * srcBytesPerPixel + 3) / 4) * 4;
    std::vector<BYTE> srcData(srcBytesPerLine * srcHeight);
    std::vector<BYTE> destData(destBytesPerLine * newHeight);
    GetBitmapBits(hBmpSrc, srcData.size(), srcData.data());

    // 执行最近邻缩放
    for (int y = 0; y < newHeight; y++)
    {
        for (int x = 0; x < newWidth; x++)
        {
            int srcX = x * srcWidth / newWidth;
            int srcY = y * srcHeight / newHeight;
            for (int i = 0; i < srcBytesPerPixel; i++)
            {
                destData[y * destBytesPerLine + x * srcBytesPerPixel + i] =
                    srcData[srcY * srcBytesPerLine + srcX * srcBytesPerPixel + i];
            }
        }
    }

    // 设置目标位图的数据
    SetBitmapBits(hBmpDest, destData.size(), destData.data());
    return hBmpDest;
}

// 给菜单中的“图像”用
extern int GrubAndSave()
{
    // The exit code of the sample application.
    int exitCode = 0;

    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();

    try
    {
        // Create an instant camera object with the camera device found first.
        CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());

        // Print the model name of the camera.
        cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

        // The parameter MaxNumBuffer can be used to control the count of buffers
        // allocated for grabbing. The default value of this parameter is 10.
        camera.MaxNumBuffer = 5;

        // Start the grabbing of c_countOfImagesToGrab images.
        // The camera device is parameterized with a default configuration which
        // sets up free-running continuous acquisition.
        camera.StartGrabbing(c_countOfImagesToGrab);

        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
        while (camera.IsGrabbing())
        {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
                // 两个函数区别只在这一个地方
                CImagePersistence::Save(ImageFileFormat_Bmp, "GrabbedImage.bmp", ptrGrabResult);
            }
            else
            {
                cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
        }
    }
    catch (const GenericException& e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
            << e.GetDescription() << endl;
        exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    //cerr << endl << "Press enter to exit." << endl;
    //while (cin.get() != '\n');

    // Releases all pylon resources.
    PylonTerminate();

    return exitCode;
}

// 给菜单中的“视频”用
extern int GrubAndShow(HDC hdc, CClientDC& dc)
{
    // The exit code of the sample application.
    int exitCode = 0;

    // Before using any pylon methods, the pylon runtime must be initialized.
    PylonInitialize();

    try
    {
        // Create an instant camera object with the camera device found first.
        CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());

        // Print the model name of the camera.
        cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

        // The parameter MaxNumBuffer can be used to control the count of buffers
        // allocated for grabbing. The default value of this parameter is 10.
        camera.MaxNumBuffer = 5;

        // Start the grabbing of c_countOfImagesToGrab images.
        // The camera device is parameterized with a default configuration which
        // sets up free-running continuous acquisition.
        camera.StartGrabbing(c_countOfVideoImagesToGrab);

        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
        while (camera.IsGrabbing())
        {
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
                // 两个函数区别只在这一个地方，
                // 下面这几行是从PylonSamples.sln中找出来的
                CPylonImage image;
                image.AttachGrabResultBuffer(ptrGrabResult);
                CPylonBitmapImage bitmapImage;
                bitmapImage.CopyImage(image);
                HBITMAP vedioBitmap = bitmapImage;

                // 这些内容是抄的之前OnCapture函数的
                //HBITMAP bBitMapDownScale = NearestNeighborResize(hdc, vedioBitmap, image.GetWidth() / 4, image.GetHeight() / 4);

                CBitmap m_Bitmap;
                m_Bitmap.Attach(vedioBitmap);

                BITMAP bitMap;
                m_Bitmap.GetBitmap(&bitMap);

                CBrush brush;
                brush.CreatePatternBrush(&m_Bitmap);

                //CPen pen(PS_NULL, 1, RGB(0, 0, 0));

                dc.SelectObject(&brush);
                dc.SelectObject(&penNull);

                dc.Rectangle(0, 0, bitMap.bmWidth, bitMap.bmHeight);

                m_Bitmap.Detach();
            }
            else
            {
                cout << "Error: " << std::hex << ptrGrabResult->GetErrorCode() << std::dec << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
        }
    }
    catch (const GenericException& e)
    {
        PylonTerminate();

        // Error handling.
        cerr << "An exception occurred." << endl
            << e.GetDescription() << endl;
        exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    //cerr << endl << "Press enter to exit." << endl;
    //while (cin.get() != '\n');

    // Releases all pylon resources.
    PylonTerminate();

    return exitCode;
}