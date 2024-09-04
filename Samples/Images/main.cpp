#include <App.h>
#include <gdiplus.h>
#include "MainWindow.h"

using namespace Gdiplus;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    ULONG_PTR token;
    GdiplusStartupInput input;
    GdiplusStartupOutput output;
    GdiplusStartup(&token, &input, &output);

    auto ret = App(hInstance).Run((Window&)MainWindow(), nCmdShow);

    GdiplusShutdown(token);

    return ret;
}