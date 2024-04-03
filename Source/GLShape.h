#pragma once
#include "GLObject.h"
#include "GLBuffer.h"
#include "GLScene.h"
#include "GLTexture.h"
#include "GLMaterial.h"
#include <algorithm>
#include <memory>
#include <vector>

class GLShape : public GLObject
{
public:
    GLShape() : mode(GL_TRIANGLES), parent(nullptr)
    {
        this->Position = { 0.f, 0.f, 0.f };
        this->Rotation = { 0.f, 0.f, 0.f, 0.f };
        this->Scaling  = { 1.f, 1.f, 1.f };
    }
    virtual ~GLShape() {}

    bool Elements(const Element* elements, size_t count)
    {
        return this->ebo.Data(elements, count * sizeof(*elements), GL_STATIC_DRAW);
    }
    bool Vertices(const Vertex* vertices, size_t count)
    {
        return this->vbo.Data(vertices, count * sizeof(*vertices), GL_STATIC_DRAW);
    }
    bool Normals(const Normal* normals, size_t count)
    {
        return this->nbo.Data(normals, count * sizeof(*normals), GL_STATIC_DRAW);
    }
    bool Colors(const Color* colors, size_t count)
    {
        return this->cbo.Data(colors, count * sizeof(*colors), GL_STATIC_DRAW);
    }
    bool TexCoords(const Coordinate* coords, size_t count)
    {
        return this->tbo.Data(coords, count * sizeof(*coords), GL_STATIC_DRAW);
    }

    GLenum Mode() const
    {
        return this->mode;
    }
    void Mode(GLenum mode)
    {
        switch (mode)
        {
            case GL_TRIANGLES:
            case GL_TRIANGLE_STRIP:
            case GL_TRIANGLE_FAN:
                this->mode = mode;
            default:
                break;
        }
    }

    GLTexture&  Texture()
    {
        return this->texture;
    }
    GLMaterial& Material()
    {
        return this->material;
    }

    virtual void Render(const GLScene& scene)
    {
        glPushMatrix();

        glTranslatef(this->Position[0], this->Position[1], this->Position[2]);
        glRotatef(this->Rotation[3], this->Rotation[0], this->Rotation[1], this->Rotation[2]);
        glScalef(this->Scaling[0], this->Scaling[1], this->Scaling[2]);

        auto vc = this->Apply(scene);
        if (vc)
        {
            auto ec = this->ebo.Size() / sizeof(GLuint);
            if (ec)
            {
                glDrawElements(this->mode, (GLsizei)ec, GL_UNSIGNED_INT, 0);
            }
            else
            {
                glDrawArrays(this->mode, 0, (GLsizei)vc);
            }

            this->Revoke();
        }

        for (auto child : this->children)
        {
            child->Render(scene);
        }

        glPopMatrix();
    }
    virtual void Release()
    {
        for (auto& child : this->children)
        {
            child->Release();
        }
        this->children.clear();

        this->texture.Release();
        this->nbo.Release();
        this->tbo.Release();
        this->vbo.Release();
        this->cbo.Release();
        this->ebo.Release();
    }

    bool HasChild() const
    {
        return !this->children.empty();
    }
    bool HasChild(const GLShape* child) const
    {
        for (auto& c : this->children)
        {
            if (c.get() == child)
            {
                return true;
            }
        }

        return false;
    }
    void AddChild(const std::shared_ptr<GLShape>& child)
    {
        if (child)
        {
            child->parent = this;
            this->children.push_back(child);
        }
    }
    void RemoveChild(const GLShape* child)
    {
        this->children.erase(std::find_if(this->children.begin(), this->children.end(), [child](const std::shared_ptr<GLShape>& p){ return p.get() == child; }));
    }
    void ClearChildren()
    {
        this->children.clear();
    }

    const std::vector<std::shared_ptr<GLShape>>& Children() const
    {
        return this->children;
    }

protected:
    virtual size_t Apply(const GLScene&)
    {
        auto vc = this->ApplyVertices();
        if (vc)
        {
            this->ApplyNormals();
            this->ApplyColors();
            this->ApplyTexCoords();
            this->ApplyTexture();
            this->material.Apply();
        }
        return vc;
    }
    virtual size_t ApplyVertices()
    {
        if (this->vbo && this->vbo.Size())
        {
            this->vbo.Bind();
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glEnableClientState(GL_VERTEX_ARRAY);
            return this->vbo.Size() / sizeof(Vertex);
        }

        return 0;
    }
    virtual size_t ApplyNormals()
    {
        if (this->nbo && this->nbo.Size())
        {
            this->nbo.Bind();
            glNormalPointer(GL_FLOAT, 0, 0);
            glEnableClientState(GL_NORMAL_ARRAY);
            return this->nbo.Size() / sizeof(Normal);
        }

        return 0;
    }
    virtual size_t ApplyColors()
    {
        if (this->cbo && this->cbo.Size())
        {
            this->cbo.Bind();
            glColorPointer(4, GL_FLOAT, 0, 0);
            glEnableClientState(GL_COLOR_ARRAY);
            return this->cbo.Size() / sizeof(Color);
        }

        return 0;
    }
    virtual size_t ApplyTexCoords()
    {
        if (this->tbo && this->tbo.Size())
        {
            this->tbo.Bind();
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            return this->tbo.Size() / sizeof(Coordinate);
        }

        return 0;
    }
    virtual void   ApplyTexture()
    {
        if (this->texture)
        {
            this->texture.Apply();
        }
        else if (this->parent)
        {
            this->parent->ApplyTexture();
        }
    }

    virtual void Revoke()
    {
        this->material.Revoke();
        this->RevokeTexture();
        this->RevokeTexCoords();
        this->RevokeNormals();
        this->RevokeVertices();
    }
    virtual void RevokeVertices()
    {
        if (this->vbo)
        {
            this->vbo.Bind();
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }
    virtual void RevokeNormals()
    {
        if (this->nbo)
        {
            this->nbo.Bind();
            glDisableClientState(GL_NORMAL_ARRAY);
        }
    }
    virtual void RevokeColors()
    {
        if (this->cbo)
        {
            this->cbo.Bind();
            glDisableClientState(GL_COLOR_ARRAY);
        }
    }
    virtual void RevokeTexCoords()
    {
        if (this->tbo)
        {
            this->tbo.Bind();
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    virtual void RevokeTexture()
    {
        if (this->texture)
        {
            this->texture.Revoke();
        }
        else if (this->parent)
        {
            this->parent->RevokeTexture();
        }
    }

    GLenum mode;
    GLElmBuf ebo;
    GLBuffer vbo, nbo, tbo, cbo;
    GLTexture  texture;
    GLMaterial material;

    GLShape* parent;
    std::vector<std::shared_ptr<GLShape>> children;
};