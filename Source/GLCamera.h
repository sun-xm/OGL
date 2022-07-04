#pragma once

#include "GLCommon.h"

class GLCamera
{
public:
    GLCamera();

    bool IsPerspective() const;
    void IsPerspective(bool isPerspective);
    bool OrthogonalHeight(float height);
    bool VerticalFov(float vfov);
    bool Focal(float focal);
    void Position(float x, float y, float z);
    void Position(const Vector<3>& position);
    void LookAt(float x, float y, float z);
    void LookAt(const Vector<3>& lookAt);
    void Rotate(float rotate);
    void Clip(float near, float far);
    void Clip(const Vector<2>& clip);

    const Vector<3>& Position() const;
    const Vector<3>& LookAt() const;
    const Vector<2>& Clip() const;

    float Rotate() const;

    void SetProject(int width, int height);
    void SetLookAt();

private:
    Vector<2> clip;
    Vector<3> lookAt;
    Vector<3> position;

    float rotate;
    float vfov;
    float focal;
    float orthHeight;
    bool  perspective;
};