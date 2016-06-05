/****************************************************************************
  Copyright (c) 2015 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef LOSEMYMIND_DATASTREAM_H
#define LOSEMYMIND_DATASTREAM_H

#pragma once

#include <cstdint>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include "FoundationKit/GenericPlatformMacros.h"
#include "FoundationKit/Base/Types.h"
NS_FK_BEGIN

class  DataStream
{
public:
    typedef uint32 size_type;
	DataStream();
	DataStream(const DataStream& pDataStream);
	DataStream(DataStream&& pDataStream);
	DataStream& operator=(const DataStream& pDataStream);
	DataStream& operator=(DataStream&& pDataStream);

    template<typename T, typename = typename std::enable_if<std::is_fundamental<T>::value>::type >
    DataStream& operator<<(T data)
    {
        _buffer.append((uint8*)&data, sizeof(T));
        return *this;
    }

    DataStream& operator<<(const uint8* data);
    DataStream& operator<<(const std::string& data);
    DataStream& operator<<(const ustring& data);
   
    template<typename T, typename = typename std::enable_if<std::is_fundamental<T>::value>::type >
    DataStream& operator>>(T& data)
    {
        read(data);
        return *this;
    }

    DataStream& operator>>(uint8* data);
    DataStream& operator>>(std::string& data);
    DataStream& operator>>(ustring& data);
    
	template <typename K, typename V>
	DataStream& operator<<(const std::map<K, V>& data)
	{
		return writeAssociativeContainer(data);
	}

	template <typename K, typename V>
	DataStream& operator>>(std::map<K, V>& data)
	{
		return readAssociativeContainer<std::map<K, V>, K, V>(data);
	}

	template <typename K, typename V>
	DataStream& operator<<(const std::unordered_map<K, V>& data)
	{
		return writeAssociativeContainer(data);
	}

	template <typename K, typename V>
	DataStream& operator>>(std::unordered_map<K, V>& data)
	{
		return readAssociativeContainer<std::unordered_map<K, V>, K, V>(data);
	}

	template <typename V>
	DataStream& operator<<(const std::vector<V>& data)
	{
		return writeSequenceContainer(data);
	}

	template <typename V>
	DataStream& operator>>(std::vector<V>& data)
	{
		return readSequenceContainer<std::vector<V>, V>(data);
	}

	template <typename V>
	DataStream& operator<<(const std::list<V>& data)
	{
		return writeSequenceContainer(data);
	}

	template <typename V>
	DataStream& operator>>(std::list<V>& data)
	{
		return readSequenceContainer<std::list<V>, V>(data);
	}

    void write(const uint8_t* data, size_type pSize);

	template< typename T >
	void read(T& data)
	{
		if (size() < sizeof(T))
		{
			data = T();
			return;
		}
        memcpy(&data, &_buffer[getReadIndex()], sizeof(T));
        readIndexIncrement(sizeof(T));
		//memcpy(&data, &_buffer[0], sizeof(T));
		//_buffer.erase(0, sizeof(T));
	}

	template< typename T >
	inline T read()
	{
		T ret;
		read(ret);
		return ret;
	}

    void read(uint8_t* data, size_type dataSize);


	void   clear();
    void   reset(const ustring& data);
    void   reset(const uint8* data, size_type size);
	size_t size();
	const uint8* c_str();
    const ustring&	getBuffer()const;

    void  setBurnAfterReading(bool val){ _burnAfterReading = val; }
private:
	template<typename C>
	DataStream& writeSequenceContainer(const C& data)
	{
		*this << data.size();
		for(auto iter : data)
		{
			*this << iter;
		}
		return *this;
	}

	template<typename C, typename V>
	DataStream& readSequenceContainer(C& data)
	{
		size_t size=0;
		*this >> size;

		for(size_t i = 0; i < size; ++i)
		{
			V value;
			*this >> value;
			data.push_back(value);
		}

		return *this;
	}

	template<typename C>
	DataStream& writeAssociativeContainer(const C& data)
	{
		*this << data.size();
		for(auto& iter : data)
		{
			*this << iter.first << iter.second;
		}
		return *this;
	}

	template<typename C ,typename K, typename V>
	DataStream& readAssociativeContainer(C& data)
	{
		size_t size=0;
		*this >> size;
		for(size_t i = 0; i < size; ++i)
		{
			K key;
			V value;
			*this >> key >> value;
			data.insert(std::pair<K,V>(key, value));
		}
		return *this;
	}

    size_type getReadIndex();

    void readIndexIncrement(size_type count);

protected:
    ustring     _buffer;
    size_type   _readIndex;
    bool        _burnAfterReading;
};

NS_FK_END
#endif // LOSEMYMIND_DATASTREAM_H





