#ifndef LOSEMYMIND_BASE64_H
#define LOSEMYMIND_BASE64_H
#pragma once
#include <string>
#include <vector>
#include "FoundationKit/Base/Types.h"

NS_FK_BEGIN

    /**
     * Class for encoding/decoding Base64 data (RFC 4648)
     */
class Base64
{
    /**
     * Base64 supports encoding any 3 uint8 chunk of binary data into a 4 uint8 set of characters from the alphabet below
     */
    static uint8 EncodingAlphabet[64];

    /**
     * Used to do a reverse look up from the encoded alphabet to the original 6 bit value
     */
    static uint8 DecodingAlphabet[256];

    /** Ctor hidden on purpose, use static methods only */
    Base64();

    /**
     * Encodes the source into a Base64 string
     *
     * @param source the binary payload to stringify
     * @param length the length of the payload that needs encoding
     *
     * @return the stringified form of the binary data
     */
    static std::string encode(const uint8* source, uint32 length);

    /**
     * Decodes a Base64 string into an array of bytes
     *
     * @param source the stringified data to convert
     * @param length the length of the buffer being converted
     * @param dest the out buffer that will be filled with the decoded data
     * @param padCount the out count of the padding on the orignal buffer (0 to 2)
     *
     * @return TRUE if the buffer was decoded, FALSE if it failed to decode
     */
    static bool decode(const char* source, uint32 length, uint8* dest, uint32& padCount);

public:

    /**
     * Encodes a binary uint8 array into a Base64 string
     *
     * @param source the binary data to convert
     *
     * @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
     */
    static std::string encode(const ustring& source);

    /**
     * Decodes a Base64 string into an array of bytes
     *
     * @param source the stringified data to convert
     * @param dest the out buffer that will be filled with the decoded data
     */
    static bool decode(const std::string& source, ustring& dest);

    /**
     * Encodes a std::string into a Base64 string
     *
     * @param source the string data to convert
     *
     * @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
     */
    static std::string encode(const std::string& source);

    /**
     * Decodes a Base64 string into a std::string
     *
     * @param source the stringified data to convert
     * @param dest the out buffer that will be filled with the decoded data
     */
    static bool decode(const std::string& source, std::string& dest);
};

NS_FK_END

#endif // LOSEMYMIND_BASE64_H




