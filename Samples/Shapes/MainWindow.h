#pragma once

#include "ViewPort.h"
#include "Panel.h"

class MainWindow : public Window
{
protected:
    bool OnCreated() override;
    bool OnCommand() override;
    void OnSize() override;


private:
    ViewPort vport;
    Panel    panel;
};