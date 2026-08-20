#ifndef CQUEUE_HPP
#define CQUEUE_HPP
#include <stdexcept>


template <typename Object>
class Cqueue{

    public: 

        Cqueue(int n):
        array{new Object[n]}, begin{0}, end{-1}, theSize{0}, capacity{n}{}

        ~Cqueue(){
            delete[] array;
        }

        Object front(){
            if (empty()){
                return Object();
            }
            return array[begin];
        }

        Object back(){
            if (empty()){
                return Object();
            }
            return array[end];
        }

        void enqueue(Object n){
            if (full()){
                throw std::runtime_error("attempted enqueue on a full queue");
            }
            move(end);
            array[end] = n;
            theSize++;
        }

        void dequeue(){
            if (empty()){
                throw std::runtime_error("attempted dequeue on an empty queue");
            }
            move(begin);
            theSize--;
        }

        void move(int& index){
            index = (index + 1) % capacity;
        }

        int size() const{
            return theSize;
        }

        bool empty() const{
            return theSize == 0;
        }

        bool full() const{
            return theSize == capacity;
        }

        

    private:

        Object* array;
        int begin;
        int end;
    
        int theSize;
        int capacity;

};

#endif
