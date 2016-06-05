/****************************************************************************
Copyright (c) 2015 libo.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef LOSEMYMIND_PROTECTEDMEMORYALLOCATOR_H
#define LOSEMYMIND_PROTECTEDMEMORYALLOCATOR_H
#pragma once
#include <mutex>

typedef uintptr_t     vm_offset_t;
typedef uintptr_t     vm_size_t;
typedef vm_offset_t   vm_address_t;

class ProtectedMemoryAllocator
{
public:
	ProtectedMemoryAllocator() = delete;
	ProtectedMemoryAllocator(const ProtectedMemoryAllocator&) = delete;
	ProtectedMemoryAllocator& operator = (const ProtectedMemoryAllocator&) = delete;

	explicit ProtectedMemoryAllocator(size_t pool_size);
	~ProtectedMemoryAllocator();

	char*       allocate(vm_size_t size);

	char*       getBaseAddress(){ return reinterpret_cast<char*>(_baseAddress); }

	vm_size_t   getTotalSize(){ return _poolSize; }

	vm_size_t   getAllocatedSize(){ return _nextAllocOffset; }

	bool        protect();

	bool        unProtect();

	static bool protect(vm_address_t address, vm_size_t size);

	static bool unProtect(vm_address_t address, vm_size_t size);

private:
	vm_size_t         _poolSize;
	vm_address_t      _baseAddress;
	vm_size_t         _nextAllocOffset;
	bool              _valid;
};


class ProtectedMemoryLocker
{
public:
	ProtectedMemoryLocker() = delete;
	ProtectedMemoryLocker(const ProtectedMemoryLocker&) = delete;
	ProtectedMemoryLocker& operator = (const ProtectedMemoryLocker&) = delete;

	explicit ProtectedMemoryLocker(ProtectedMemoryAllocator* allocator) :_allocator(allocator)
	{
		_mutex.lock();
		_allocator->unProtect();
	}

	~ProtectedMemoryLocker()
	{
		_allocator->protect();
		_mutex.unlock();
	}

private:
	std::mutex                _mutex;
	ProtectedMemoryAllocator *_allocator;
};
#endif // LOSEMYMIND_PROTECTEDMEMORYALLOCATOR_H


