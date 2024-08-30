#pragma once

#include "GLCommon.h"

class GLCamera
{
public:
    GLCamera() : perspective(true), rotate(0.f), vfov(45.f), focal(0.f), orthHeight(1.f)
    {
        this->clip = { 0.f, 0.f };
        this->lookAt = { 0.f, 0.f, 0.f };
        this->position = { 0.f, 0.f, 0.f };
    }
    virtual ~GLCamera() {}

    bool IsPerspective() const
    {
        return this->perspective;
    }
    void IsPerspective(bool isPerspective)
    {
        this->perspective = isPerspective;
    }

    bool OrthogonalHeight(float height)
    {
        if (height <= 0.f)
        {
            return false;
        }

        this->orthHeight = height;
        return true;
    }
    bool VerticalFov(float vfov)
    {
        if (vfov <= 0.f)
        {
            return false;
        }

        this->vfov = vfov;
        this->focal = 0.f;
        return true;
    }
    bool Focal(float focal)
    {
        if (focal <= 0.f)
        {
            return false;
        }

        this->focal = focal;
        this->vfov = 0.f;
        return true;
    }

    void Position(float x, float y, float z)
    {
        this->position = { x, y, z };
    }
    void Position(const Vector<3>& position)
    {
        this->position = position;
    }
    void LookAt(float x, float y, float z)
    {
        this->lookAt = { x, y, z };
    }
    void LookAt(const Vector<3>& lookAt)
    {
        this->lookAt = lookAt;
    }
    void Clip(float _near, float _far)
    {
        this->clip = { _near, _far };
    }
    void Clip(const Vector<2>& clip)
    {
        this->clip = clip;
    }
    void Rotate(float rotate)
    {
        this->rotate = rotate;
    }

    const Vector<3>& Position() const
    {
        return this->position;
    }
    const Vector<3>& LookAt() const
    {
        return this->lookAt;
    }
    const Vector<2>& Clip() const
    {
        return this->clip;
    }
    float Rotate() const
    {
        return this->rotate;
    }

    void SetProject(int width, int height)
    {
        glViewport(0, 0, width, height);

        if (this->perspective)
        {
            float vfov;

            if (this->focal > 0.f)
            {
                vfov = ToDegree(atanf(height * .5f / this->focal)) * 2.f;
            }
            else
            {
                vfov = this->vfov;
            }

            gluPerspective(vfov, (double)width / (double)height, this->clip[0], this->clip[1]);
        }
        else
        {
            auto l = (this->position - this->lookAt).Length();
            auto orthWidth = width * orthHeight / height;
            glOrtho(-.5f * orthHeight, .5f * orthHeight, -.5f * orthHeight, .5f * orthHeight, this->clip[0] - l, this->clip[1] - l);
        }
    }
    void SetLookAt()
    {
        Vector<3> v;
        v = this->position - this->lookAt;

        if (this->perspective)
        {
            Quaternion<> q;

            if (0.f != v[0] || 0.f != v[2])
            {
                q = Quaternion<>::From2Vectors(Vector<3>::ZAxis, { v[0], 0.f, v[2] });
                q = Quaternion<>::From2Vectors({ v[0], 0.f, v[2] }, v) * q;
            }
            else
            {
                q = Quaternion<>::From2Vectors(Vector<3>::ZAxis, v);
            }

            q = Quaternion<>::FromAxisAngle(v, this->rotate);

            Vector<3> yAxis;
            yAxis = q.Rotate(Vector<3>::YAxis);

            gluLookAt(this->position[0], this->position[1], this->position[2], this->lookAt[0], this->lookAt[1], this->lookAt[2], yAxis[0], yAxis[1], yAxis[2]);
        }
        else
        {
            if (0.f != v[0] || 0.f != v[2])
            {
                float a = ToDegree(acosf(CosOfVectors<float>(v, { v[0], 0.f, v[2] }))) * (v[1] > 0.f ? 1.f : -1.f);
                glRotatef(a, 1.f, 0.f, 0.f);

                a = ToDegree(acosf(CosOfVectors<float>({ v[0], 0.f, v[2] }, Vector<3>::ZAxis))) * (v[0] > 0.f ? -1.f : 1.f);
                glRotatef(a, 0.f, 1.f, 0.f);
            }
            else
            {
                float a = 90.f * (v[1] > 0.f ? 1.f : -1.f);
                glRotatef(a, 1.f, 0.f, 0.f);
            }

            glRotatef(this->rotate, v[0], v[1], v[2]);
            glTranslatef(-this->lookAt[0], -this->lookAt[1], -this->lookAt[2]);
        }
    }

protected:
    Vector<2> clip;
    Vector<3> lookAt;
    Vector<3> position;

    float rotate;
    float vfov;
    float focal;
    float orthHeight;
    bool  perspective;
};