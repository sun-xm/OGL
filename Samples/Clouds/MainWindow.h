#include "ViewPort.h"

class MainWindow : public Window
{
protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    ViewPort vport;
};