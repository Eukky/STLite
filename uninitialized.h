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
    
}

#endif