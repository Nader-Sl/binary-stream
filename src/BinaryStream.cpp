//
// Created by Jack Noordhuis on 23/12/18.
//

#include "BinaryStream.h"

#include <memory>
#include <sstream>

unsigned int BinaryStream::read(byte *data, unsigned int size) {
    if(size == 0) {
        return 0;
    }

    unsigned int remaining = this->size - offset;
    if(size > remaining) {
        std::stringstream err;
        err << "Not enough bytes in buffer to read: need " << size << ", have " << remaining;
        throw std::out_of_range(err.str());
    }

    memcpy(data, &this->data[offset], sizeof(byte) * size);
    offset += size;

    return size;
}

void BinaryStream::write(const byte *data, unsigned int size) {
    unsigned int remaining = offset + size;
    if(size > remaining) {
        if(resizable) {
            resize(offset + size);
        } else {
            std::stringstream err;
            err << "Not enough bytes in buffer to write: need " << size << ", have " << remaining;
            throw std::out_of_range(err.str());
        }
    }

    memcpy(&this->data[offset], data, size);
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
    memcpy(&newBuf[0], &data[0], size);
    delete data;
    data = newBuf;
    size = newSize;
}

unsigned int BinaryStream::skip(unsigned int count) {
    if(offset + count > size) {
        count = (size - offset);
    }
    offset += count;
    return count;
}

byte* BinaryStream::getBuffer(bool release) {
    byte *b = data;
    if(release) {
        data = nullptr;
    }

    return b;
}

void BinaryStream::swapBytes(byte *array, size_t size) {
    byte* array2 = new byte[size];
    memcpy(array2, array, size);
    for (ssize_t i = size - 1; i >= 0; i--) {
        array[size - 1 - i] = array2[i];
    }
}