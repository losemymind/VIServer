/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef LOSEMYMIND_NONCOPYABLE_H
#define LOSEMYMIND_NONCOPYABLE_H

#pragma once
#include "FoundationKit/GenericPlatformMacros.h"

NS_FK_BEGIN
//  Private copy constructor and copy assignment ensure classes derived from
//  class noncopyable cannot be copied.

//  Contributed by libo

namespace noncopyable_  // protection from unintended ADL
{
    class noncopyable
    {
    protected:
        constexpr noncopyable() = default;
        ~noncopyable() = default;
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
}

typedef noncopyable_::noncopyable noncopyable;

NS_FK_END
#endif // LOSEMYMIND_NONCOPYABLE_H







