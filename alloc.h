#ifndef _STLITE_ALLOC_H_
#define _STLITE_ALLOC_H_

#include <cstdlib>

namespace STLite{

    //空间配置器
    class alloc{
    private:
        enum Align{ ALIGN = 8 };//小型区块上调边界
        enum MaxBytes{ MAXBYTES = 128 };//小型区块上调上限，超过该大小的区块直接由malloc分配内存
        enum NFreeLists{ NFREELISTS = MaxBytes::MAXBYTES / Align::ALIGN };//FreeList节点数量
        enum NObjs{ NOBJS = 20 };//自由链表每次增加的节点数
    private:
        // Freelist节点构造
        union FreeList{
            union FreeList* next;
            char data[1];
        };

        static FreeList *free_list[NFREELISTS];
    private:
        static char *start_free;//内存池起始位置
        static char *end_free;//内存池技术位置
        static size_t heap_size;//内存池大小
    private:
        static size_t round_up(size_t bytes);
        static size_t freelist_index(size_t bytes);
        static void *refill(size_t bytes);
        static char *chunk_alloc(size_t size, size_t& nobj);
    public:
        static void *allocate(size_t bytes);
        static void deallocate(void *ptr, size_t bytes);
        static void *reallocate(void *ptr, size_t old_size, size_t new_size);
    };

    char *alloc::start_free = 0;
    char *alloc::end_free = 0;
    size_t alloc::heap_size = 0;

    alloc::FreeList *alloc::free_list[NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    //上调bytes至8的倍数
    inline size_t alloc::round_up(size_t bytes){
        return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
    }

    //计算在哪一个FreeList的节点处开始分配内存，节点由0开始计算
    inline size_t alloc::freelist_index(size_t bytes){
        return ((bytes + ALIGN - 1) / ALIGN - 1);
    }

    //返回一个大小为bytes的对象
    //假设bytes已经上调至8的倍数
    void *alloc::refill(size_t bytes){
        size_t nobjs = alloc::NOBJS;
        char *chunk = chunk_alloc(bytes, nobjs);
        FreeList **my_free_list, *result, *current_free_list, *next_free_list;
        int i;
        if(nobjs == 1){
            return chunk;
        }
        my_free_list = free_list + freelist_index(bytes);
        //在chunk空间内建立freelist
        result = (FreeList *)chunk;
        //将freelist指向新的空间
        *my_free_list = next_free_list = (FreeList *)(chunk + bytes);
        //将freelist串联起来
        //i从1开始，第0个区块将返回给客端（取20用1放19）
        for(i = 1; ; i++){
            current_free_list = next_free_list;
            next_free_list = (FreeList *)((char *)next_free_list + bytes);
            if(nobjs - 1 == i){
                current_free_list -> next = 0;
                break;
            }else{
                current_free_list -> next = next_free_list;
            }
        }
        return result;

    }
    //假设size已经上调至8的倍数
    //该函数从内存池中取出空间给freelist使用
    char *alloc::chunk_alloc(size_t size, size_t& nobjs){
        char *result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;//内存池剩余空间

        if(bytes_left >= total_bytes){
            //内存池剩余空间完全满足需求   
            result = start_free;
            start_free += total_bytes;
            return result;
        }else if(bytes_left >= size){
            //内存池不完全满足需求量，但能供应一个以上的区块
            nobjs = bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        }else{
            //内存池不足以供应1个区块
            //计算需要配置的空间为2倍需求空间大小加上随着配置次数增加的附加量
            size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
            //如果内存池还有剩余空间，将它分配给freelist合适的节点
            if(bytes_left > 0){
                FreeList **my_free_list = free_list + freelist_index(bytes_left);
                ((FreeList *)start_free) -> next = *my_free_list;
                *my_free_list = (FreeList *)start_free;
            }
            //扩充内存池空间
            start_free = (char *)malloc(bytes_to_get);
            if(!start_free){
                FreeList **my_free_list = 0, *p = 0;
                for(int i = 0; i <= MaxBytes::MAXBYTES; i += Align::ALIGN){
                    my_free_list = free_list + freelist_index(i);
                    p = *my_free_list;
                    //当freelist中还有未使用的区块时，调整freelist释放区块
                    if(p != 0){
                        *my_free_list = p -> next;
                        start_free = (char *)p;
                        end_free = start_free + i;
                        //递归调用自己，调整nobjs，任何残余零头最终都将被编入适当的freelist中备用
                        return chunk_alloc(bytes, nobjs);
                    }
                }
                std::printf("Out of memory");
                end_free = 0;
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            //递归调用自己，调整nobjs
            return chunk_alloc(bytes, nobjs);
        }
    }

    void *alloc::allocate(size_t bytes){
        //如果大小超过128，直接调用malloc进行内存分配
        if(bytes > MaxBytes::MAXBYTES){
            return malloc(bytes);
        }
        FreeList **my_free_list, *result;
        my_free_list = free_list + freelist_index(bytes);
        result = my_free_list;
        //如果freelist没有空间了，则重新填充
        if(result == 0){
            void *r = refill(round_up(bytes));
            return r;
        }
        my_free_list = result -> next;
        return result;
    }

    void alloc::deallocate(void *ptr, size_t bytes){
        //大小超过128，则直接调用free函数释放内存
        if(bytes > MaxBytes::MAXBYTES){
            free(ptr);
            return;
        }
        //将释放的节点重新接回freelist
        FreeList **my_free_list, *node;
        node = (FreeList *)ptr;
        my_free_list = free_list + freelist_index(bytes);
        node -> next = *my_free_list;
        *my_free_list = node;

    }

    void *alloc::reallocate(void *ptr, size_t old_size, size_t new_size){
        deallocate(ptr, old_size);
        ptr = allocate(new_size);
        return ptr;
    }
}

#endif
