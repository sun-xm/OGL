#pragma once

#include "GLCommon.h"
#include "GLLight.h"
#include "GLCamera.h"
#include <GL/glew.h>
#include <stdexcept>

class GLScene
{
public:
    GLScene() : enableDepth(true)
    {
        memset(this->lights, 0, sizeof(this->lights));
    }
    virtual ~GLScene()
    {
        for (auto i = 0; i < COUNTOF(this->lights); i++)
        {
            delete this->lights[i];
        }
    }

    GLCamera& Camera()
    {
        return this->camera;
    }

    void DepthTest(bool enable)
    {
        this->enableDepth = enable;
    }

    GLLight& EnableLight(int index)
    {
        if (index < 0 || index > GL_LIGHT7)
        {
            throw std::out_of_range("Light index can be only between 0 and 7");
        }

        if (!this->lights[index])
        {
            this->lights[index] = new GLLight(index);
        }

        return *this->lights[index];
    }
    void DisableLight(int index)
    {
        if (index < 0 || index > GL_LIGHT7)
        {
            throw std::out_of_range("Light index can be only between 0 and 7");
        }

        delete this->lights[index];
        this->lights[index] = nullptr;
    }

    const Matrix<4>& WorldView() const
    {
        return this->matrix;
    }

    virtual void Begin(int width, int height)
    {
        this->width  = width;
        this->height = height;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        this->camera.SetProject(width, height);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        this->camera.SetLookAt();
        glGetFloatv(GL_MODELVIEW_MATRIX, this->matrix);
        this->matrix = this->matrix.Transpose();

        if (this->enableDepth)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
        }

        bool lighting = false;
        for (auto i = 0; i < COUNTOF(this->lights); i++)
        {
            if (this->lights[i])
            {
                this->lights[i]->Enable();
                lighting = true;
            }
        }

        if (lighting)
        {
            glEnable(GL_LIGHTING);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    }
    virtual void End()
    {
        for (auto i = 0; i < COUNTOF(this->lights); i++)
        {
            if (this->lights[i])
            {
                this->lights[i]->Disable();
            }
        }

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        glFlush();
    }

    int Width() const
    {
        return this->width;
    }
    int Height() const
    {
        return this->height;
    }

protected:
    bool enableDepth;

    GLCamera camera;
    GLLight* lights[GL_LIGHT7 - GL_LIGHT0];

    Matrix<4> matrix;

    int width, height;
};