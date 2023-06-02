#include <App.h>
#include <gdiplus.h>
#include "MainWindow.h"

using namespace Gdiplus;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartupOutput output;
    Gdiplus::GdiplusStartup(&token, &input, &output);

    auto ret = App(hInstance).Run(MainWindow(), nCmdShow);

    Gdiplus::GdiplusShutdown(token);

    return ret;
}