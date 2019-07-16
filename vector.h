#ifndef _STLITE_VECTOR_H_
#define _STLITE_VECTOR_H_

#include "allocator.h"
#include "uninitialized.h"

namespace STLite{

    template <class T, class Alloc = allocator<T>>
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

        //容器配置相关  
        void insert_aux(iterator position, const T& value);
        void insert_aux(iterator position, iterator first, iterator last);
        void deallocate();
        void fill_initialize(size_type n, const T& value);
        iterator allocate_and_fill(size_type n, const T& value);
        void allocate_and_copy(iterator first, iterator last);
        void destroy_and_deallocate_all();

    public:
        //容器本身相关函数
        vector() : start(0), finish(0), end_of_storage(0) {}
        vector(size_type n, const T& value);
        ~vector();

        //迭代器相关函数
        iterator begin();
        iterator end();

        //容量相关函数
        size_type size();
        size_type capacity();
        bool empty();
        void resize(size_type new_size, const T& value);

        //访问元素相关
        reference front();
        reference back();

        //操作符重置
        reference operator[](const difference_type i);
        vector& operator = (const vector& v);
        vector& operator = (vector&& v);
        bool operator == (const vector& v);
        bool operator != (const vector& v);
        
        //修改容器相关操作
        void push_back(const T& value);
        void pop_back();
        iterator insert(iterator position, const T& value);
        iterator insert(iterator position, iterator first, iterator last;)
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
    };



    template <class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const T& value){
        //在position位置中插入value
        if(finish != end_of_storage){
            construct(finish, *(finish - 1));
            ++finish;
            T value_copy = value;
            std::copy_backward(position, finish - 2, finish - 1);
            *position = value_copy;
        }else{
            const size_type old_size = size();
            //配置一段新的空间，如果原大小为0，则新空间为1，如果原大小不为0，则新空间为原大小的两倍                
            //前半段用来存储原数据，后半段用来存放新数据
            const size_type len = old_size != 0 ? 2 * old_size : 1; 

            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;

            try{
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, value);
                ++new_finish;
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch(...){
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }
                
            destroy(begin(), end());
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, iterator first, iterator last){
        difference_type left = end_of_storage - finish;
        difference_type need = last - first;
        if(left >= need){
            if(finish - position >= need){
                uninitialized_copy(finish - need, finish, finish);
                std::copy_backward(position, finish - need, finish);
                std::copy(first, last, position);
            }else{
                iterator tmp = uninitialized_copy(first + (finish - position), last, finish);
                uninitialized_copy(position, finish, tmp);
                std::copy(first, first + (finish - position), position);
            }
            finish += need;
        }else{
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2 * old_size : 1;

            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;

            try{
                new_finish = uninitialized_copy(start, position, new_start);
                new_finish = uninitialized_copy(first, last, new_finish);
                new_finish = uninitialized_copy(position, last, new_finish);
            }
            catch(...){
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }

            destroy(begin(), end());
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::deallocate(){
        if(start){
            data_allocator::deallocate(start, end_of_storage - start);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::fill_initialize(size_type n, const T& value){
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::allocate_and_fill(size_type n, const T& value){
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, value);
        return result;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::allocate_and_copy(iterator first, iterator last){
        start = data_allocator::allocate(last - first);
        finish = uninitialized_copy(first, last, start);
        end_of_storage = finish;
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::destroy_and_deallocate_all(){
        if(capacity() != 0){
            data_allocator::destory(start, finish);
            data_allocator::deallocate(start, capacity());
        }
    }

    template <class T, class Alloc>
    vector<T, Alloc>::vector(size_type n, const T& value){
        fill_initialize(n, value);
    }

    template <class T, class Alloc>
    vector<T, Alloc>::~vector(){
        destroy(start, finish);
        deallocate();
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator  
    vector<T, Alloc>::begin(){
        return start;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator  
    vector<T, Alloc>::end(){
        return finish;
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::size_type 
    vector<T, Alloc>::size(){
        return size_type(finish - start);
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::size_type 
    vector<T, Alloc>::capacity(){
        return size_type(end_of_storage - start);
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::empty(){
        return (begin() == end());
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::resize(size_type new_size, const T& value){
        // if(new_size < size()){
        //     erase(begin() + new_size, end());
        // }else{
        //     insert(end(), new_size - size(), value);
        // }
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::reference
    vector<T, Alloc>::front(){
        return *(begin());
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::reference
    vector<T, Alloc>::back(){
        return *(end() - 1);
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::reference 
    vector<T, Alloc>::operator[](const difference_type i){
        return *(begin() + i);
    }

    template <class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& v){
        if(this != &v){
            allocate_and_copy(v.start, v.finish);
        }
        return *this;
    }

    //右值引用，减少内存损耗
    template <class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator = (vector&& v){
        if(this != &v){
            destroy_and_deallocate_all();
            start = v.start;
            finish = v.finish;
            end_of_storage = v.end_of_storage;
            v.start = 0;
            v.finish = 0;
            v.end_of_storage = 0;
        }
        return *this;
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::operator == (const vector& v){
        if(size() != v.size()){
            return false;
        }else{
            iterator ptr1 = start;
            iterator ptr2 = v.start;
            for(; ptr1 != finish && ptr2 != v.finish; ++ptr1, ++ptr2){
                if(*ptr1 != *ptr2){
                    return false;
                }
            }
            return true;
        }
    }

    template <class T, class Alloc>
    bool vector<T, Alloc>::operator != (const vector& v){
        return !(*this == v);
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::push_back(const T& value){
        if(finish != end_of_storage){
            construct(finish, value);
            ++finish;
        }else{
            insert_aux(end(), value);
        }
    }

    template <class T, class Alloc>
    void vector<T, Alloc>::pop_back(){
        --finish;
        destroy(finish);
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::insert(iterator position, const T& value){

    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::insert(iterator position, iterator first, iterator last){
        
    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator position){

    }

    template <class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator position, iterator first, iterator last){
        
    }

}

#endif