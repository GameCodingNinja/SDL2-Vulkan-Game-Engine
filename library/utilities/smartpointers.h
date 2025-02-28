//*****************************************************************************
/*!
 *  \file   smartpointers.h
 *
 *  \brief  Smart pointer template classes
 *
 *****************************************************************************/

#pragma once

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// SDL lib dependencies
#include <SDL3/SDL.h>

namespace NSmart
{
    //*****************************************************************************
    /*!
     *  \brief  Smart pointer for file handlers
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    class scoped_filehandle_ptr : boost::noncopyable
    {
    public:

        scoped_filehandle_ptr(T * p = nullptr) : ptr(p)
        {}

        ~scoped_filehandle_ptr()
        { freePtr(); }

        void reset(T * p = nullptr)
        {
            freePtr();
            ptr = p;
        }

        T* release()
        {
            T *t = ptr;
            ptr = nullptr;
            return t;
        }

        bool isNull()
        { return (ptr == nullptr); }

        T * get()
        { return ptr; }

        T * operator->()
        { return ptr; }

        T & operator*()
        { return *ptr; }

    private:

        void freePtr()
        {
            if( !isNull() )
            {
                fclose( ptr );
                ptr = nullptr;
            }
        }

    private:

        T * ptr;
    };
    
    
    //*****************************************************************************
    /*!
     *  \brief  Smart pointer for file handlers
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    class scoped_SDL_filehandle_ptr : boost::noncopyable
    {
    public:

        scoped_SDL_filehandle_ptr(T * p = nullptr) : ptr(p)
        {}

        ~scoped_SDL_filehandle_ptr()
        { freePtr(); }

        void reset(T * p = nullptr)
        {
            freePtr();
            ptr = p;
        }

        T* release()
        {
            T *t = ptr;
            ptr = nullptr;
            return t;
        }

        bool isNull()
        { return (ptr == nullptr); }

        T * get()
        { return ptr; }

        T * operator->()
        { return ptr; }

        T & operator*()
        { return *ptr; }

    private:

        void freePtr()
        {
            if( !isNull() )
            {
                SDL_CloseIO( ptr );
                ptr = nullptr;
            }
        }

    private:

        T * ptr;
    };


    //*****************************************************************************
    /*!
     *  \brief  Smart pointer for file handlers
     *
     *  \param  T *& ptr - A reference to a pointer of type T
     *
     *****************************************************************************/
    template <class T>
    class scoped_com_ptr : boost::noncopyable
    {
    public:

        scoped_com_ptr(T * p = nullptr) : ptr(p)
        {}

        ~scoped_com_ptr()
        { freePtr(); }

        void reset(T * p = nullptr)
        {
            freePtr();
            ptr = p;
        }

        T* release()
        {
            T *t = ptr;
            ptr = nullptr;
            return t;
        }

        bool isNull()
        { return (ptr == nullptr); }

        T * get()
        { return ptr; }

        T * operator->()
        { return ptr; }

    private:

        void freePtr()
        {
            if( !isNull() )
            {
                ptr->Release();
                ptr = nullptr;
            }
        }

    private:

        T * ptr;
    };
}
