#ifndef _DEF__
#define _DEF__

#include "assert.h"
#define DECLARE_SINGLETON(T)    \
private:    \
    static T* _instance_; \
public:     \
    static void InitSingleTon() { assert(!_instance_); _instance_ = new T(); } \
    static T* Instance()    \
    {   \
        assert(_instance_ != nullptr);  \
        return _instance_;  \
    }

#define IMPLEMENT_SINGLETON(T)  \
    T* T::_instance_ = nullptr;

#endif