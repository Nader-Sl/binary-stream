//
// Created by Jack Noordhuis on 23/12/18.
//

#ifndef CESTIA_BINARYSTREAM_H
#define CESTIA_BINARYSTREAM_H

#include "../../includes.h"

#include <cstring>
#include <string>

/**
 * This class allows reading and writing binary data.
 */
class BinaryStream {

protected:

    unsigned int size = 0;
    std::unique_ptr<byte[]> buffer;
    unsigned int offset = 0;
    bool resizable;

    void resize(unsigned int minimalSize);

    static void swapBytes(byte *bytes, size_t size);

public:

    bool swapEndian = false;

    explicit BinaryStream(bool resizable = false) : resizable(resizable) {}
    BinaryStream(byte *data, unsigned int size, bool resizable = false);
    BinaryStream(std::unique_ptr<byte[]> buffer, unsigned int size, bool resizable = false) : buffer(std::move(buffer)), size(size), resizable(resizable) {}

    unsigned int read(byte *data, unsigned int size);
    void write(const byte *data, unsigned int size);

    /**
     * This function is used to check if the buffer is resizable.
     */
    inline bool isResizable() const {
        return resizable;
    }

    /**
     * This function returns the current used buffer size when writing or the current buffer position when reading.
     */
    inline unsigned int getOffset() const {
        return offset;
    }

    /**
     * This function returns the actual size of the buffer.
     */
    inline unsigned int getBufferSize() const {
        return size;
    }

    /**
     * This function returns the remaining buffer size (in bytes). When using a resizable buffer, it'll return how many bytes
     * can be still written before a buffer reallocation.
     */
    inline unsigned int getRemainingSize() const {
        return size - offset;
    }

    unsigned int skip(unsigned int count);

    /**
     * Pass 'true' if you want to avoid freeing the buffer but are not going to use this object later (trying to read
     * or write on it will result in a crash!).
     */
    byte *getBuffer(bool release);

    void setBuffer(byte *data, unsigned int size);

    static const int LONG_SIZE = 8;
    static const int INT_SIZE = 4;
    static const int SHORT_SIZE = 2;
    static const int BYTE_SIZE = 1;
    static const int FLOAT_SIZE = 4;
    static const int DOUBLE_SIZE = 8;

    static inline unsigned int getStringSize(const std::string &str) {
        return (unsigned int) (INT_SIZE + str.length());
    }

    BinaryStream &operator<<(float val) {
        if(swapEndian) swapBytes((byte*) &val, FLOAT_SIZE);
        this->write((byte *) &val, FLOAT_SIZE);
        return *this;
    }

    BinaryStream &operator<<(double val) {
        if(swapEndian) swapBytes((byte*) &val, DOUBLE_SIZE);
        this->write((byte *) &val, DOUBLE_SIZE);
        return *this;
    }

    BinaryStream &operator<<(long long val) {
        if(swapEndian) swapBytes((byte*) &val, LONG_SIZE);
        this->write((byte *) &val, LONG_SIZE);
        return *this;
    }

    BinaryStream &operator<<(unsigned long long val) {
        *this << (long long) val;
        return *this;
    }

    BinaryStream &operator<<(int val) {
        if(swapEndian) swapBytes((byte*) &val, INT_SIZE);
        this->write((byte *) &val, INT_SIZE);
        return *this;
    }

    BinaryStream &operator<<(unsigned int val) {
        *this << (int) val;
        return *this;
    }

    BinaryStream &operator<<(short val) {
        if(swapEndian) swapBytes((byte*) &val, SHORT_SIZE);
        this->write((byte *) &val, SHORT_SIZE);
        return *this;
    }

    BinaryStream &operator<<(unsigned short val) {
        *this << (short) val;
        return *this;
    }

    BinaryStream &operator<<(char val) {
        this->write((byte *) &val, BYTE_SIZE);
        return *this;
    }

    BinaryStream &operator<<(unsigned char val) {
        *this << (char) val;
        return *this;
    }

    BinaryStream &operator<<(std::string val) {
        *this << (int) val.length();
        this->write((const byte *) val.c_str(), (unsigned int) val.length());
        return *this;
    }

    BinaryStream &operator>>(float &val) {
        this->read((byte *) &val, FLOAT_SIZE);
        if(swapEndian) swapBytes((byte *) &val, FLOAT_SIZE);
        return *this;
    }

    BinaryStream &operator>>(double &val) {
        this->read((byte *) &val, DOUBLE_SIZE);
        if(swapEndian) swapBytes((byte *) &val, DOUBLE_SIZE);
        return *this;
    }

    BinaryStream &operator>>(long long &val) {
        this->read((byte *) &val, LONG_SIZE);
        if(swapEndian) swapBytes((byte *) &val, LONG_SIZE);
        return *this;
    }

    BinaryStream &operator>>(unsigned long long &val) {
        *this >> (long long&) val;
        return *this;
    }

    BinaryStream &operator>>(int &val) {
        this->read((byte *) &val, INT_SIZE);
        if(swapEndian) swapBytes((byte *) &val, INT_SIZE);
        return *this;
    }

    BinaryStream &operator>>(unsigned int &val) {
        *this >> (int&) val;
        return *this;
    }

    BinaryStream &operator>>(short &val) {
        this->read((byte *) &val, SHORT_SIZE);
        if(swapEndian) swapBytes((byte *) &val, SHORT_SIZE);
        return *this;
    }

    BinaryStream &operator>>(unsigned short &val) {
        *this >> (short&) val;
        return *this;
    }

    BinaryStream &operator>>(char &val) {
        this->read((byte *) &val, BYTE_SIZE);
        return *this;
    }

    BinaryStream &operator>>(unsigned char &val) {
        *this >> (char&) val;
        return *this;
    }

    BinaryStream &operator>>(std::string &val) {
        unsigned int strLen;
        *this >> strLen;
        byte buf[strLen];
        this->read(&buf[0], strLen);
        val.assign((char *) buf, strLen);
        return *this;
    }

};

#endif //CESTIA_BINARYSTREAM_H
