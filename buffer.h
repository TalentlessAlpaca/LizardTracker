#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

template <class T>
class Buffer
{
public:
    Buffer();
    Buffer(int _size);
    ~Buffer();
    void push_back(T element);
    int size();
    void clear();
    T at(int i);
    T get_middle();

private:
    int pushes = 0;
    int bSize = 21;
    std::vector<T> buffer;
};


template <class T>
Buffer<T>::Buffer() : buffer(new std::vector<T>(bSize))
{

}

template <class T>
Buffer<T>::Buffer(int _size){
    bSize = _size;
    pushes = 0;
    buffer = std::vector<T>(bSize);
}

template <class T>
Buffer<T>::~Buffer(){
    delete buffer;
}

template <class T>
void Buffer<T>::push_back(T element){
    buffer[pushes] = element;
    pushes = ((pushes+1) >= bSize) ? 0:(pushes+1);
}

template <class T>
int Buffer<T>::size(){
    return bSize;
}

template <class T>
void Buffer<T>::clear(){
    pushes = 0;
    buffer = std::vector<T> (bSize);
}

template <class T>
T Buffer<T>::at(int i){
    int idx = ((i+pushes) >= bSize)? (i+pushes-bSize):(i+pushes);
    return buffer[idx];
}

template <class T>
T Buffer<T>::get_middle(){
    return buffer[(bSize/2)+pushes];
}

#endif // BUFFER_H
