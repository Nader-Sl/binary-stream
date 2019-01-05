//
// Created by Jack Noordhuis on 23/12/18.
//

#include "BinaryStream.h"

#include <memory>
#include <sstream>

BinaryStream::BinaryStream(unsigned int size, bool resizable) : size(size), resizable(resizable) {
    buffer = std::make_unique<byte[]>(size);
}

BinaryStream::BinaryStream(byte *data, unsigned int size, bool resizable) {
    auto newBuf = std::make_unique<byte[]>(size);
    memcpy(&newBuf[0], &data[0], size);

    buffer = std::move(newBuf);
    this->size = size;
    this->resizable = resizable;
}

unsigned int BinaryStream::read(byte *data, unsigned int size) {
    if(size == 0) {
        return 0;
    }

    unsigned int remaining = getRemainingSize();
    if(size > remaining) {
        std::stringstream err;
        err << "Not enough bytes in buffer to read: need " << size << ", have " << remaining;
        throw std::out_of_range(err.str());
    }

    memcpy(data, &buffer[offset], sizeof(byte) * size);
    offset += size;

    return size;
}

void BinaryStream::write(const byte *data, unsigned int size) {
    unsigned int remaining = getRemainingSize();
    if(size > remaining) {
        if(!resizable) {
            std::stringstream err;
            err << "Not enough bytes in buffer to write: need " << size << ", have " << remaining;
            throw std::out_of_range(err.str());
        }

        resize(offset + size);
    }

    memcpy(&buffer[offset], data, size);
    offset += size;
}

void BinaryStream::resize(unsigned int minimalSize) {
    if(!resizable) {
        throw std::bad_function_call();
    }

    unsigned int newSize = size;
    while(newSize < minimalSize) {
        newSize *= 2;
    }

    if(newSize == size) {
        return;
    }

    byte *newBuf = new byte[newSize];
    memcpy(&newBuf[0], &buffer[0], size);

    buffer.reset(newBuf);
    size = newSize;
}

unsigned int BinaryStream::skip(unsigned int count) {
    if(offset + count > size) {
        count = (size - offset);
    }
    offset += count;
    return count;
}

std::unique_ptr<byte[]> BinaryStream::getBuffer(bool release) {
    auto b = std::make_unique<byte[]>(size);
    memcpy(&b[0], &buffer[0], size);

    if(release) {
        buffer.reset();
    }

    return std::move(b);
}

void BinaryStream::setBuffer(byte *data, unsigned int size) {
    byte *newBuf = new byte[size];
    memcpy(&newBuf[0], &data[0], size);

    buffer.reset(newBuf);
    this->size = size;
}

void BinaryStream::swapBytes(byte *bytes, size_t size) {
    byte temp_bytes[size];
    memcpy(temp_bytes, bytes, size);
    for (ssize_t i = size - 1; i >= 0; i--) {
        bytes[size - 1 - i] = temp_bytes[i];
    }
}