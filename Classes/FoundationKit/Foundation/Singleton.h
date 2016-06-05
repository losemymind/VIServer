/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#pragma once

#include <mutex>
#include <stdlib.h>
#include "FoundationKit/Base/noncopyable.hpp"
#include "FoundationKit/GenericPlatformMacros.h"
NS_FK_BEGIN

template< typename T, bool destroy_on_exit = true >
class Singleton : noncopyable
{
    typedef T                         instance_type;
    typedef instance_type*            instance_pointer;
    typedef volatile instance_pointer volatile_instance_pointer;
    static std::once_flag            m_once;
    static volatile_instance_pointer m_instance;
    static void destroy();

public:
    inline static instance_pointer getInstance()
    {
        std::call_once(m_once, [&]{
            m_instance = new (std::nothrow) instance_type();
            if (destroy_on_exit)
            {
                ::atexit(destroy);
            }
        });
        return m_instance;
    }

    inline static void destroyInstance()
    {
        if (!destroy_on_exit)
        {
            destroy();
        }
    }
};


template< typename T, bool destroy_on_exit >
void Singleton< T, destroy_on_exit>::destroy()
{
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void)dummy;
    delete m_instance;
    m_instance = nullptr;
}

template< typename T, bool destroy_on_exit >
typename Singleton< T, destroy_on_exit >::volatile_instance_pointer
    Singleton<T, destroy_on_exit>::m_instance = nullptr;

template< class T, bool destroy_on_exit >
std::once_flag Singleton<T, destroy_on_exit>::m_once;

NS_FK_END



