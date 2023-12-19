#include "MainWindow.h"

bool MainWindow::OnCreated()
{
    if (!Window::OnCreated())
    {
        return false;
    }
    this->Text(L"Shaders");

    if (!this->view.Create(*this))
    {
        return false;
    }
    this->view.Show();

    return true;
}

void MainWindow::OnSize()
{
    Window::OnSize();
    this->view.MoveTo(0, 0);
    this->view.Resize(this->ClientW(), this->ClientH());
}