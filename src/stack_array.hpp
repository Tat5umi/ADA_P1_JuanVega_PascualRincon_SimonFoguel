#ifndef STACK_ARRAY_HPP
#define STACK_ARRAY_HPP

#include "Vector.hpp"
#include <utility>

template<typename Object>
class ArrayStack{

    public:

        ArrayStack() = default;

        void pop(){
            vec.pop_back();
        }

        void push(const Object& obj){
            vec.push_back(obj);
        }

        void push(Object&& obj){
            vec.push_back(std::move(obj));
        }

        Object& top(){
            return vec.back();
        }

        const Object& top() const{
            return vec.back();
        }

        int size() const{
            return vec.size();
        }

        bool empty() const{
            return vec.empty();
        }

        using iterator = Object*;
        using const_iterator = const Object*;

        iterator begin(){
            return vec.begin();
        }

        iterator end(){
            return vec.end();
        }

        const_iterator begin() const{
            return vec.begin();
        }

        const_iterator end() const{
            return vec.end();
        }

    private:

        Vector<Object> vec;

};

#endif