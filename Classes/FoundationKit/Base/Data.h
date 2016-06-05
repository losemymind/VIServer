/****************************************************************************
Copyright (c) 2013-2014 libo.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef LOSEMYMIND_DATA_H
#define LOSEMYMIND_DATA_H

#pragma once
#include "FoundationKit/GenericPlatformMacros.h"
#include <string>

NS_FK_BEGIN

class Data
{
public:
    static const Data Null;
    
    Data();
    Data(const Data& other);
    Data(Data&& other);
    ~Data();
    
    // Assignment operator
    Data& operator= (const Data& other);
    Data& operator= (Data&& other);

    unsigned char* getBytes() const;

    size_t getSize() const;
    
    /** Copies the buffer pointer and its size.
     *  @note This method will copy the whole buffer.
     *        Developer should free the pointer after invoking this method.
     *  @see Data::fastSet
     */
    void copy(unsigned char* bytes, const size_t size);
    
    /** Fast set the buffer pointer and its size. Please use it carefully.
     *  @param bytes The buffer pointer, note that it have to be allocated by 'malloc' or 'calloc',
     *         since in the destructor of Data, the buffer will be deleted by 'free'.
     *  @note 1. This method will move the ownship of 'bytes'pointer to Data,
     *        2. The pointer should not be used outside after it was passed to this method.
     *  @see Data::copy
     */
    void fastSet(unsigned char* bytes, const size_t size, bool needDelete = false);
    
    /** Clears data, free buffer and reset data size */
    void clear();
    
    /** Check whether the data is null. */
    bool isNull() const;
    
private:
    void move(Data& other);
    
private:
    unsigned char* _bytes;
    size_t         _size;
    bool           _ownMem;
};

NS_FK_END
#endif // LOSEMYMIND_DATA_H






