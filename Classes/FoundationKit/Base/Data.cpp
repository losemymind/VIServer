/****************************************************************************
Copyright (c) 2013-2014 libo.

losemymind.libo@gmail.com

****************************************************************************/
#include <string>
#include "Data.h"

NS_FK_BEGIN
const Data Data::Null;

Data::Data()
:_bytes(nullptr)
,_size(0)
,_ownMem(false)
{
}

Data::Data(Data&& other)
:_bytes(nullptr)
,_size(0)
,_ownMem(false)
{
    move(other);
}

Data::Data(const Data& other)
:_bytes(nullptr)
,_size(0)
,_ownMem(false)
{
    copy(other._bytes, other._size);
}

Data::~Data()
{
    clear();
}

Data& Data::operator= (const Data& other)
{
    copy(other._bytes, other._size);
    return *this;
}

Data& Data::operator= (Data&& other)
{
    move(other);
    return *this;
}

void Data::move(Data& other)
{
    _ownMem = other._ownMem;
    _bytes = other._bytes;
    _size = other._size;
    other._bytes = nullptr;
    other._size = 0;
}

bool Data::isNull() const
{
    return (_bytes == nullptr || _size == 0);
}

unsigned char* Data::getBytes() const
{
    return _bytes;
}

size_t Data::getSize() const
{
    return _size;
}

void Data::copy(unsigned char* bytes, const size_t size)
{
    clear();
    
    if (size > 0)
    {
        _ownMem = true;
        _size = size;
        _bytes = new unsigned char[sizeof(unsigned char) * _size];
        memcpy(_bytes, bytes, _size);
    }
}

void Data::fastSet(unsigned char* bytes, const size_t size, bool needDelete /*= false*/)
{
    clear();
    _ownMem = needDelete;
    _bytes = bytes;
    _size = size;
}

void Data::clear()
{
    if (_ownMem)
        delete[] _bytes;
    _bytes = nullptr;
    _size = 0;
}

NS_FK_END

