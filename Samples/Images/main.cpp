#include <Application.h>
#include <gdiplus.h>
#include "MainWindow.h"

using namespace Gdiplus;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    ULONG_PTR token;
    GdiplusStartupInput input;
    GdiplusStartupOutput output;
    GdiplusStartup(&token, &input, &output);

    auto ret = Application(hInstance).Run(MainWindow(), nCmdShow);

    GdiplusShutdown(token);

    return ret;
}