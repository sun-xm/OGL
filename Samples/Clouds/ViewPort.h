#include "GLWindow.h"
#include "GLScene.h"
#include "GLCloud.h"

class ViewPort : public GLWindow
{
public:
    ViewPort();

protected:
    LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM) override;
    bool OnContextCreated() override;
    void OnContextDestroy() override;
    void OnPaint() override;

private:
    void LoadCloud();

private:
    POINT   cursor;
    GLScene scene;
    GLCloud cloud;
};