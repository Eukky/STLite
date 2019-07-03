#ifndef _STLITE_CONSTRUCT_H_
#define _STLITE_CONSTRUCT_H_

#include <new>

#include "type_traits.h"

namespace STLite{

    template<class T1, class T2>
    inline void construct(T1 *ptr, const T2 &value){
        //placement new
        //在分配的ptr指向的空间构造对象
        //调用T1::T1(value)
        new(ptr) T1(value);
    }

    template<class T>
    inline void destory(T *ptr){
        ptr -> ~T();
    }

    template<class ForwardIterator>
    inline void destory(ForwardIterator first, ForwardIterator last){
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        _destory(first, last, is_POD_type());
    }

    template<class ForwardIterator>
    inline void _destory(ForwardIterator first, ForwardIterator last, _true_type){}

    template<class ForwardIterator>
    inline void _destory(ForwardIterator first, ForwardIterator last, _false_type){
        for(; first != last; ++first){
            destory(&*first);
        }
    }
}

#endif