#ifndef _STLITE_ALLOCATOR_H_
#define _STLITE_ALLOCATOR_H_

#include "alloc.h"
#include "construct.h"

namespace STLite{
    
    //空间配置器
    //将alloc类再次进行封装
    template<class T>
    class allocator{
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   different_type;
    public:
        static T *allocator();
        static T *allocator(size_t n);
        static void deallcator(T *ptr);
        static void deallcator(T *ptr, size_t n);
        
        static void construct(T *ptr);
        static void construct(T *ptr, const T& value);
        static void destroy(T *ptr);
        static void destroy(T *first, T *last);
    };

    template<class T>
    T *allocator<T>::allocator(){
        //将返回的指针的类型转换为T类型
        return static_cast<T *>(alloc::allocator(sizeof(T)));
    }

    //给n个T单位分配内存
    template<class T>
    T *allocator<T>::allocator(size_t n){
        if(n == 0){
            return 0;
        }
        return static_cast<T *>(alloc::allocator(sizeof(T) * n));
    }

    template<class T>
    void allocator<T>::deallcator(T *ptr){
        alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
    }

    template<class T>
    void allocator<T>::deallcator(T *ptr, size_t n){
        alloc::deallocate(static_cast<void *>(ptr), sizeof(T) * n);
    }

    template<class T>
    void allocator<T>::construct(T *ptr){
        new(ptr)T();
    }

    template<class T>
    void allocator<T>::construct(T *ptr, const T& value){
        new(ptr)T(value);
    }

    template<class T>
    void allocator::destroy(T *ptr){
        ptr -> ~T();
    }

    template<class T>
    void allocator::destroy(T *first, T *last){
        for(; first != last; ++first){
            first -> ~T();
        }
    }
}

#endif