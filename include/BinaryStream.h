//
// Created by Jack Noordhuis on 23/12/18.
//

#ifndef BINARY_STREAM_BINARYSTREAM_H
#define BINARY_STREAM_BINARYSTREAM_H

#ifndef TYPE_BYTE
typedef unsigned char byte;
#define TYPE_BYTE byte
#endif

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
    explicit BinaryStream(unsigned int size, bool resizable = false);
    BinaryStream(byte *data, unsigned int size, bool resizable = false);
    BinaryStream(std::unique_ptr<byte[]> buffer, unsigned int size, bool resizable = false) : buffer(std::move(buffer)), size(size), resizable(resizable) {}

    /**
     * Read bytes from the buffer.
     *
     * Copies size bytes from the buffer, starting from the offset, and returns the number of bytes read.
     */
    unsigned int read(byte *data, unsigned int size);

    /**
     * Write bytes to the buffer.
     *
     * Copies data into the buffer at the current offset position.
     */
    void write(const byte *data, unsigned int size);

    /**
     * Check if the buffer is resizable.
     */
    inline bool isResizable() const {
        return resizable;
    }

    /**
     * Get the current buffer position offset.
     */
    inline unsigned int getOffset() const {
        return offset;
    }

    /**
     * Get the actual size of the buffer.
     */
    inline unsigned int getBufferSize() const {
        return size;
    }

    /**
     * Get the remaining buffer size (in bytes). When using a resizable buffer, it'll return how many bytes
     * can be still written before a buffer resize/reallocation.
     */
    inline unsigned int getRemainingSize() const {
        return size - offset;
    }

    /**
     * Rewind the offset to the beginning of the buffer.
     */
    void rewind() {
        offset = 0;
    }

    /**
     * Move the buffer offset forward by the specified number of bytes.
     */
    unsigned int skip(unsigned int count);

    /**
     * Check if the offset has reached the end of the buffer.
     */
    bool feof() {
        return offset > size;
    }

    /**
     * Get the actual buffer.
     *
     * If you are not going to use this object later, pass 'true' to free the buffer (trying to read
     * or write will cause a crash).
     */
    std::unique_ptr<byte[]> getBuffer(bool release);

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

#endif //BINARY_STREAM_BINARYSTREAM_H
