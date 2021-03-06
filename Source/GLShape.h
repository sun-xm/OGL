#pragma once
#include "GLObject.h"
#include "GLBuffer.h"
#include "GLScene.h"
#include "GLTexture.h"
#include "GLMaterial.h"
#include <vector>

class GLShape : public GLObject
{
public:
    GLShape();
    virtual ~GLShape();

    bool Elements(const Element* elements, size_t count);
    bool Vertices(const Vertex* vertices, size_t count);
    bool Normals(const Normal* normals, size_t count);
    bool TexCoords(const Coordinate* coords, size_t count);

    GLenum Mode();
    void Mode(GLenum);

    GLTexture&  Texture();
    GLMaterial& Material();

    virtual void Render(const GLScene& scene);
    virtual void Release();

    bool HasChild() const;
    bool HasChild(const GLShape* child) const;
    void AddChild(GLShape* child);
    void RemoveChild(const GLShape* child);

protected:
    virtual size_t Apply(const GLScene&);
    virtual size_t ApplyVertices();
    virtual size_t ApplyNormals();
    virtual size_t ApplyTexCoords();
    virtual void   ApplyTexture();

    virtual void Revoke();
    virtual void RevokeVertices();
    virtual void RevokeNormals();
    virtual void RevokeTexCoords();
    virtual void RevokeTexture();

    GLenum mode;
    GLElmBuf ebo;
    GLBuffer vbo, nbo, tbo;
    GLTexture  texture;
    GLMaterial material;

    GLShape* parent;
    std::vector<GLShape*> children;
};
