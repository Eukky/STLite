#ifndef _STLITE_ALGORITHM_H_
#define _STLITE_ALGORITHM_H_

namespace STLite{
    template <class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator first, Size n, const T& value){
        for(; n > 0; --n, ++first){
            *first = value;
        }
        return first;
    }
}

#endif