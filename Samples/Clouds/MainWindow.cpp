#include "MainWindow.h"

bool MainWindow::OnCreated()
{
    if (!this->vport.Create(*this))
    {
        return false;
    }
    this->vport.Show();

    return Window::OnCreated();
}

void MainWindow::OnSize()
{
    Window::OnSize();
    this->vport.Resize(this->ClientW(), this->ClientH());
    this->vport.MoveTo(0, 0);
}