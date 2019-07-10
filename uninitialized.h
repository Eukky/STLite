#ifndef _STLITE_UNINITIALIZED_H_
#define _STLITE_UNINITIALIZED_H_

#include "iterator.h"
#include "type_traits.h"

namespace STLite{

    //uninitialized_copy函数
    template <class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last, 
    ForwardIterator result, _true_type){
        memcpy(result, first, (last - first) * sizeof(*first));
        return result + (last - first);
    }

    template <class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last, 
    ForwardIterator result, _false_type){
        int i = 0;
        for(; first != last; ++first, ++i){
            construct((result + i), *first);
        }
        return (result + i);
    }

    template <class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
        //先对迭代器类型进行萃取，判断出型别之后再用type_traits进行判断是否为POD类型
        typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type is_POD_type;
        return _uninitialized_copy_aux(first, last, result, is_POD_type());
    }

    //uninitialized_fill函数
    template <class ForwardIterator, class T>
    void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
    const T& value, _true_type){
        fill(first, last, value);
    }

    template <class ForwardIterator, class T>
    void uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
    const T& value, _false_type){
        for(; first != last; ++first){
            contruct(first, value);
        }
    }

    template <class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
        typedef typename _type_traits<T>::is_POD_type is_POD_type;
        uninitialized_fill_aux(first, last, is_POD_type());
    }

    //uninitialized_fill_n函数
    template <class ForwardIterator, class size, class T>
    ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size n, const T& value, _true_type){
        return fill_n(first, n, value);
    }

    template <class ForwardIterator, class size, class T>
    ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size n, const T& value, _false_type){
        int i = 0;
        for(; i != n; ++i){
            contruct((T*)(first + i), x)
        }
        return (first + i);
    }

    template <class ForwardIterator, class size, class T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, size n, const T& value){
        typedef typename _type_traits<T>::is_POD_type is_POD_type;
        return uninitialized_fill_n_aux(first, n, value, is_POD_type());
    }
}

#endif