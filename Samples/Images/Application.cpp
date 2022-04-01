#include "Application.h"
#include <gdiplus.h>

using namespace Gdiplus;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    ULONG_PTR token;
    GdiplusStartupInput input;
    GdiplusStartupOutput output;
    GdiplusStartup(&token, &input, &output);

    auto ret = Application(hInstance).Run(nCmdShow);

    GdiplusShutdown(token);

    return ret;
}

Application::Application(HINSTANCE instance) : MainWindow(instance)
{
}

bool Application::OnCreated()
{
    if (!MainWindow::OnCreated())
    {
        return false;
    }

    if (!this->view.Create(this))
    {
        return false;
    }
    this->view.Show();

    return true;
}

void Application::OnSize()
{
    this->view.Resize(this->ClientWidth(), this->ClientHeight());
    MainWindow::OnSize();
}