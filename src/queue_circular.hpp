#ifndef QUEUE_CIRCULAR_HPP
#define QUEUE_CIRCULAR_HPP
#include <stdexcept>
#include <utility>


template <typename Object>
class CircularQueue{

    public: 

        CircularQueue(int n):
        array{new Object[validar(n)]}, begin{0}, end{-1}, theSize{0}, capacity{n}{}
        
        CircularQueue(const CircularQueue& rhs):
        array{new Object[rhs.capacity]}, begin{rhs.begin}, end{rhs.end}, theSize{rhs.theSize}, capacity{rhs.capacity}{
            for (int i=0; i < capacity; ++i){
                array[i] = rhs.array[i];
                    }
        }

        CircularQueue& operator=(const CircularQueue& rhs){
             if (this == &rhs) return *this;
             CircularQueue temp = rhs;
             std::swap(array, temp.array);
             std::swap(begin, temp.begin);
             std::swap(end, temp.end);
             std::swap(theSize, temp.theSize);
             std::swap(capacity, temp.capacity);
             return *this;
        }
        

        ~CircularQueue(){
            delete[] array;
        }

        const Object& front() const{
            if (empty()){
                throw std::runtime_error("front sobre una cola vacia");
                    }
            return array[begin];
        }
        

         const Object& back() const{
            if (empty()){
                throw std::runtime_error("back sobre una cola vacia");
                    }
            return array[end];
         }

        void enqueue(const Object& n){
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

        static int validar(int n){
            if (n <= 0){
                throw std::invalid_argument("la capacidad de la cola debe ser positiva");
            }
            return n;
        }

        void move(int& index){
            index = (index + 1) % capacity;
        }
                
        Object* array;
        int begin;
        int end;
    
        int theSize;
        int capacity;

};

#endif
