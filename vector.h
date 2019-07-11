#ifndef _STLITE_VECTOR_H_
#define _STLITE_VECTOR_H_

#include "allocator.h"
#include "uninitialized.h"

namespace STLite{
    template <class T, class Alloc = allocator<T> >
    class vector{
    public:
        typedef T           value_type;
        typedef value_type* pointer;
        typedef value_type* iterator;
        typedef value_type& reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

    protected:
        typedef Alloc data_allocator;
        iterator start;
        iterator finish;
        iterator end_of_storage;

        void insert_aux(iterator position, const T& x);
        void deallocate(){
            if(start){
                data_allocator::deallocate(start, end_of_storage - start);
            }
        }
        void fill_initialize(size_type n, const T& value){
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }
        iterator allocate_and_fill(size_type n, const T& x){
            iterator result = data_allocator::allocate(n);
            uninitialized_fill_n(result, n, x);
            return result;
        }

    public:
        vector() : start(0), finish(0), end_of_storage(0) {}
        vector(size_type n, const T& value) {fill_initialize(n, value);}
    };
}

#endif