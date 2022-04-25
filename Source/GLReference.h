#pragma once

#include <functional>

template<typename T>
class GLReference
{
public:
    GLReference() : ref(nullptr) {}

    virtual bool Create()
    {
        this->Release();

        if (this->create && this->create())
        {
            this->ref = new int;
            (*this->ref) = 1;
            return true;
        }

        return false;
    }

    virtual void Release()
    {
        if (this->release)
        {
            this->release();
        }

        if (this->ref)
        {
            (*this->ref)--;

            if (!(*this->ref))
            {
                this->destroy();
                delete this->ref;
                this->ref = nullptr;
            }
        }
    }

    GLReference& operator=(const GLReference& other)
    {
        this->Release();

        if (other.ref)
        {
            this->ref = other.ref;
            this->obj = other.obj;
            (*this->ref)++;
        }

        return *this;
    }

protected:
    std::function<bool()> create;
    std::function<void()> release;
    std::function<void()> destroy;

    int* ref;
    T    obj;
};