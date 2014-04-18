#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "../Classes/GameScene.h"
#include "basefun.h"
#include <string>

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	std::string title = "������";
    eglView->setViewName(GBKToUTF(title));
    eglView->setFrameSize(640,960); //������Ļ�Ĵ�С
	eglView->setFrameZoomFactor(0.6f);
    return CCApplication::sharedApplication()->run();
}
