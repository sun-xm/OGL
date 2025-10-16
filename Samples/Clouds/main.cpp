#include "MainWindow.h"
#include <Application.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(MainWindow(), nCmdShow);
}