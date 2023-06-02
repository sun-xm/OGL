#pragma once

#include "Viewport.h"

class MainWindow : public Window
{
protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    Viewport view;
};