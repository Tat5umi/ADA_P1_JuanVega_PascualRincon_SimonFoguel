#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <utility>
#include <stdexcept>

template<typename Object>
class Vector{

    public:

        Vector():
        theCapacity{16}, theSize{0}, array{new Object[16]}{}

        explicit Vector(int capacity):
        theCapacity{capacity + 16}, theSize{0}, array{new Object[capacity + 16]}{}

        Vector(const Vector& rhs):
        theCapacity{rhs.theCapacity}, theSize{rhs.thesize}, array{new Object[theCapacity]}{
            for (int i = 0; i < rhs.theSize; ++i){
                array[i] = rhs.array[i];
            }
        }

        Object& operator[](int index){
            return array[index];
        }

        const Object& operator[](int index) const{
            return array[index];
        }

        Vector(Vector&& rhs):
        theCapacity{std::exchange(rhs.theCapacity, 0)},
        theSize{std::exchange(rhs.theSize, 0)},
        array{std::exchange(rhs.array, nullptr)}
        {}

        ~Vector(){
            theCapacity = 0;
            theSize = 0;
            delete[] array;
        }

        Vector& operator= (const Vector& rhs){
            if (this == &rhs) return *this;
            Vector temp = rhs;
            std::swap(theSize, temp.theSize);
            std::swap(theCapacity, temp.theCapacity);
            std::swap(array, temp.array);
            return *this;
        }

        Vector& operator= (Vector&& rhs){
            std::swap(theSize, rhs.theSize);
            std::swap(theCapacity, rhs.theCapacity);
            std::swap(array, rhs.array);
            return *this;
        }

        void resize(){
            Object* newArray = new Object[theCapacity*2];
            for(int i = 0; i < theSize; ++i){
                newArray[i] = std::move(array[i]);
            }

            theCapacity = theCapacity*2;
            delete[] array;
            array = newArray;
        }

        bool empty() const{
            return theSize == 0;
        }

        int size() const{
            return theSize;
        }

        void push_back(const Object& obj){
            if (theSize == theCapacity){
                resize();
            }
            array[theSize++] = obj;
        }

        void push_back(Object&& obj){
            if (theSize == theCapacity){
                resize();
            }
            array[theSize++] = std::move(obj);
        }

        void pop_back(){
            if (empty()){
                throw std::runtime_error("cannot pop on an empty vector");
            }
            --theSize;
        }

        Object& back(){
            return array[theSize - 1];
        }

        const Object& back() const{
            return array[theSize - 1];
        }

        using iterator = Object*;
        using const_iterator = const Object*;

        iterator begin(){
            return &array[0];
        }

        const_iterator begin() const{
            return &array[0];
        }

        iterator end(){
            return &array[theSize];
        }

        const_iterator end() const{
            return &array[theSize];
        }

        

    private:

        int theCapacity;
        int theSize;
        Object* array;


};




#endif
