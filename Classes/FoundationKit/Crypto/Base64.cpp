
#include "Base64.h"

NS_FK_BEGIN

/** The table used to encode a 6 bit value as an ascii character */
uint8 Base64::EncodingAlphabet[64] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e',
    'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

/** The table used to convert an ascii character into a 6 bit value */
uint8 Base64::DecodingAlphabet[256] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
    0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22,
    0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
    0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };


Base64::Base64()
{

}

/**
 * Encodes the source into a Base64 string
 *
 * @param Source the binary payload to stringify
 * @param Length the length of the payload that needs encoding
 *
 * @return the stringified form of the binary data
 */
std::string Base64::encode(const uint8* source, uint32 length)
{
    // Each 3 uint8 set of data expands to 4 bytes and must be padded to 4 bytes
    uint32 expectedLength = (length + 2) / 3 * 4;
    std::string outBuffer;
    outBuffer.reserve(expectedLength);

    char encodedBytes[5];
    // Null terminate this so we can append it easily as a string
    encodedBytes[4] = 0;
    // Loop through the buffer converting 3 bytes of binary data at a time
    while (length >= 3)
    {
        uint8 A = *source++;
        uint8 B = *source++;
        uint8 C = *source++;
        length -= 3;
        // The algorithm takes 24 bits of data (3 bytes) and breaks it into 4 6bit chunks represented as ascii
        uint32 byteTriplet = A << 16 | B << 8 | C;
        // Use the 6bit block to find the representation ascii character for it
        encodedBytes[3] = EncodingAlphabet[byteTriplet & 0x3F];
        // Shift each time to get the next 6 bit chunk
        byteTriplet >>= 6;
        encodedBytes[2] = EncodingAlphabet[byteTriplet & 0x3F];
        byteTriplet >>= 6;
        encodedBytes[1] = EncodingAlphabet[byteTriplet & 0x3F];
        byteTriplet >>= 6;
        encodedBytes[0] = EncodingAlphabet[byteTriplet & 0x3F];
        // Now we can append this buffer to our destination string
        outBuffer += encodedBytes;
    }
    // Since this algorithm operates on blocks, we may need to pad the last chunks
    if (length > 0)
    {
        uint8 A = *source++;
        uint8 B = 0;
        uint8 C = 0;
        // Grab the second character if it is a 2 uint8 finish
        if (length == 2)
        {
            B = *source;
        }
        uint32 byteTriplet = A << 16 | B << 8 | C;
        // Pad with = to make a 4 uint8 chunk
        encodedBytes[3] = '=';
        byteTriplet >>= 6;
        // If there's only one 1 uint8 left in the source, then you need 2 pad chars
        if (length == 1)
        {
            encodedBytes[2] = '=';
        }
        else
        {
            encodedBytes[2] = EncodingAlphabet[byteTriplet & 0x3F];
        }
        // Now encode the remaining bits the same way
        byteTriplet >>= 6;
        encodedBytes[1] = EncodingAlphabet[byteTriplet & 0x3F];
        byteTriplet >>= 6;
        encodedBytes[0] = EncodingAlphabet[byteTriplet & 0x3F];
        outBuffer += encodedBytes;
    }
    return outBuffer;
}

/**
 * Decodes a Base64 string into an array of bytes
 *
 * @param source the stringified data to convert
 * @param length the length of the buffer being converted
 * @param dest the out buffer that will be filled with the decoded data
 * @param padCount the out count of the padding on the orignal buffer (0 to 2)
 *
 * @return true if the buffer was decoded, false if it failed to decode
 */
bool Base64::decode(const char* source, uint32 length, uint8* dest, uint32& padCount)
{
    padCount = 0;
    uint8 DecodedValues[4];
    while (length)
    {
        // Decode the next 4 BYTEs
        for (int32 Index = 0; Index < 4; Index++)
        {
            // Tell the caller if there were any pad bytes
            if (*source == '=')
            {
                padCount++;
            }
            DecodedValues[Index] = DecodingAlphabet[(int32)(*source++)];
            // Abort on values that we don't understand
            if (DecodedValues[Index] == 0xFF)
            {
                return false;
            }
        }
        length -= 4;
        // Rebuild the original 3 bytes from the 4 chunks of 6 bits
        uint32 OriginalTriplet = DecodedValues[0];
        OriginalTriplet <<= 6;
        OriginalTriplet |= DecodedValues[1];
        OriginalTriplet <<= 6;
        OriginalTriplet |= DecodedValues[2];
        OriginalTriplet <<= 6;
        OriginalTriplet |= DecodedValues[3];
        // Now we can tear the uint32 into bytes
        dest[2] = OriginalTriplet & 0xFF;
        OriginalTriplet >>= 8;
        dest[1] = OriginalTriplet & 0xFF;
        OriginalTriplet >>= 8;
        dest[0] = OriginalTriplet & 0xFF;
        dest += 3;
    }
    return true;
}



/**
 * Encodes a binary uint8 array into a Base64 string
 *
 * @param source the binary data to convert
 *
 * @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
 */
std::string Base64::encode(const ustring& source)
{
    return encode(source.c_str(), static_cast<uint32>(source.size()));
}

/**
 * Decodes a Base64 string into an array of bytes
 *
 * @param source the stringified data to convert
 * @param dest the out buffer that will be filled with the decoded data
 *
 * @return true if the buffer was decoded, false if it failed to decode
 */
bool Base64::decode(const std::string& source, ustring& dest)
{
    uint32 length = static_cast<uint32>(source.length());
    // Size must be a multiple of 4
    if (length % 4)
    {
        return false;
    }
    // Each 4 uint8 chunk of characters is 3 bytes of data
    uint32 expectedLength = length / 4 * 3;
    // Add the number we need for output
    dest.assign(expectedLength,0x00);
    uint8* buffer = dest.get_allocator().address(dest.front());
    uint32 padCount = 0;
    bool bWasSuccessful = decode(source.c_str(), length, buffer, padCount);
    if (bWasSuccessful)
    {
        if (padCount > 0)
        {
            dest.erase(expectedLength - padCount, padCount);
        }
    }
    return bWasSuccessful;
}

/**
 * Encodes a std::string into a Base64 string
 *
 * @param source the string data to convert
 *
 * @return a string that encodes the binary data in a way that can be safely transmitted via various Internet protocols
 */
std::string Base64::encode(const std::string& source)
{
    return encode(reinterpret_cast<const uint8*>(source.c_str()), static_cast<uint32>(source.size()));
}

/**
 * Decodes a Base64 string into a std::string
 *
 * @param source the stringified data to convert
 * @param dest the out buffer that will be filled with the decoded data
 */
bool Base64::decode(const std::string& source, std::string& dest)
{
    uint32 length = static_cast<uint32>(source.size());
    // Size must be a multiple of 4
    if (length % 4)
    {
        return false;
    }
    // Each 4 uint8 chunk of characters is 3 bytes of data
    uint32 expectedLength = length / 4 * 3;
    dest.assign(expectedLength,0x00);
    uint8* buffer = reinterpret_cast<uint8*>(dest.get_allocator().address(dest.front()));
    uint32 padCount = 0;
    bool bWasSuccessful = decode(source.c_str(), length, buffer, padCount);
    if (bWasSuccessful)
    {
        if (padCount > 0)
        {
            buffer[expectedLength - padCount] = 0;
        }
        else
        {
            dest.push_back('\0');
        }
    }
    return bWasSuccessful;
}

NS_FK_END


