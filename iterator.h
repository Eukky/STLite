#ifndef _STLITE_ITERATOR_H_
#define _STLITE_ITERATOR_H_

namespace STLite{

    //定义5种迭代器类型
    //input_iterator:这种迭代器所指对象不能被外界所改变（只读）
    //output_iterator:只写
    //forward_iterator:允许写入型算法在这种迭代器上进行读写操作
    //bidirectional_iterator:双向移动迭代器
    //random_acess_iterator:前四种迭代器只供应一份指针算术能力，前三种支持operator++，第四种加上operator--
    //这种迭代器支持所有指针算术能力，包括p+n, p-n, p[n], p1-p2, p1<p2
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public random_access_iterator_tag {};
 
    //为每种迭代器定义5种型别
    template <class T, class Distance> struct input_iterator
    {
        typedef input_iterator_tag iterator_category;
        typedef T                  value_type;
        typedef Distance           difference_type;
        typedef T*                 pointer;
        typedef T&                 reference;
    };

    struct output_iterator
	{
		typedef output_iterator_tag iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;
	};

    template <class T, class Distance> struct forward_iterator
	{
		typedef forward_iterator_tag iterator_category;
		typedef T					 value_type;
		typedef Distance			 difference_type;
		typedef T*					 pointer;
		typedef T&					 reference;
	};

    template <class T, class Distance> struct bidirectional_iterator
	{
		typedef bidirectional_iterator_tag iterator_category;
		typedef T						   value_type;
		typedef Distance				   difference_type;
		typedef T*						   pointer;
		typedef T&						   reference;
	};

    template <class T, class Distance> struct random_access_iterator
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T						   value_type;
		typedef Distance				   difference_type;
		typedef T*						   pointer;
		typedef T&						   reference;
	};

    //为迭代器设计的模版保证
    //自行设计的迭代器都要继承此类
    template <class Category, class T, class Distance = ptrdiff_t,
              class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef Category    iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
    };
    
    //迭代器类型萃取
    template <class Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::difference_type   difference_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
    };

    //针对原生指针设计的萃取偏特化版
    template <class T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef ptrdiff_t                  difference_type;
        typedef T*                         pointer;
        typedef T&                         reference;
    };

    //针对原生常量指针设计的萃取便特化版
    template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T 						   value_type;
		typedef ptrdiff_t 				   difference_type;
		typedef const T*				   pointer;
		typedef const T& 				   reference;
	};
}

#endif