#ifndef LOSEMYMIND_PROTECTEDMEMORYALLOCATORTEST_H
#define LOSEMYMIND_PROTECTEDMEMORYALLOCATORTEST_H



#pragma once


#include "ProtectedMemoryAllocator.h"

class MemObj
{
public:
    MemObj(int val):mem(val){}
    ~MemObj(){};
    void set(int val){ mem = val; }
    int  get(){ return mem; }
private:
    int mem;
};


void TestProtectedMemoryAllocator()
{
    ProtectedMemoryAllocator  mem_pool(2048);

    MemObj* pMemObj = new (mem_pool.allocate(sizeof(MemObj)))MemObj(100);
    int v0 = pMemObj->get(); // 100
    pMemObj->set(200);
    v0 = pMemObj->get(); // 200

    mem_pool.protect();
    //pMemObj->set(300); //crash! because pMemObj memory is readonly.
    {
        ProtectedMemoryLocker lock(&mem_pool);
        pMemObj->set(400);
    }

    //pMemObj->set(500);//crash! because pMemObj memory is readonly.

}


#endif // LOSEMYMIND_PROTECTEDMEMORYALLOCATORTEST_H


