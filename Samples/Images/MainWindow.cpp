#include "MainWindow.h"

bool MainWindow::OnCreated()
{
    if (!Window::OnCreated())
    {
        return false;
    }

    if (!this->view.Create(*this))
    {
        return false;
    }
    this->view.Show();

    return true;
}

void MainWindow::OnSize()
{
    this->view.Resize(this->ClientW(), this->ClientH());
    Window::OnSize();
}