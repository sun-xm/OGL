#include "MainWindow.h"

bool MainWindow::OnCreated()
{
    if (!Window::OnCreated())
    {
        return false;
    }

    if (!this->vport.Create(*this))
    {
        return false;
    }
    this->vport.Show();

    if (!this->panel.Create(*this))
    {
        return false;
    }
    this->panel.Show();

    return true;
}

bool MainWindow::OnCommand()
{
    this->vport.CreateShape(this->command);
    return true;
}

void MainWindow::OnSize()
{
    this->vport.MoveTo(0, 0);
    this->vport.Resize(this->ClientW(), this->ClientH() - this->panel.Height());

    this->panel.MoveTo(0, this->vport.Height());
    this->panel.Resize(this->ClientW(), this->panel.Height());
}