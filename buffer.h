#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

/* ---------------------------------------------------
 *              BUFFER CLASS
 *
 *  This class is an implementation of a simple vector based buffer.
 *  As such, it implements the following methods:
 *    + CONSTRUCTORS
 *      - Buffer <T> ():        Default Constructor
 *
 *      - Buffer <T> (int):     Creates a buffer of an specified size
 *
 *    + Methods
 *      - void push_back(T):    Pushes the element T into the buffer.
 *                              The first element of the buffer is lost.
 *                              The order of elements is maintained and
 *                              the pushed element is located in the last
 *                              position of the buffer.
 *
 *      - void int size():      Returns the number of elements in the buffer.
 *                              This number remains constant since the creation
 *                              of the buffer.
 *
 *      - void clear():         Deletes all the elements in the buffer.
 *
 *      - T at(int):            Returns the element in the specified location.
 *
 *      - T get_middle():       Returns the element stored in the middle of the buffer.
 *
 *  USAGE:
 *
 *  Buffer<int> waka(10); // This creates a Buffer object which stores 10 int elements.
 *  // Here, the buffer will be an array of 10 0s
 *  for(int i = 0; i < waka.size(); i++)
 *      waka.push_back(i);
 *  // At this point, the buffer will contain numbers from 0 to 9
 *  for(int i = 10; i < 20; i++)
 *      waka.push_back(i);
 *  // At this point the buffer will contain the numbers from 10 to 19
 *  waka.clear();   // This will empty the buffer, and the underlying vectro will be all ceros again
 *
 * ---------------------------------------------------*/

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
    delete &buffer;
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
