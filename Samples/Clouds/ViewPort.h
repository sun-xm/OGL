#include "GLWindow.h"
#include "GLProgram.h"
#include "GLCloud.h"
#include "GLScene.h"

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
    bool LoadCloud();

private:
    POINT   cursor;
    GLScene scene;
    GLCloud cloud;

    GLProgram center;
    GLProgram shift;
};