#ifndef LSTACK_HPP
#define LSTACK_HPP
#include <utility>
#include "List.hpp"

template<typename Object>
class LinkedStack{

    public: 

        LinkedStack() = default;

        bool empty() const{
            return myList.empty();
        }

        int size() const{
            return myList.size();
        }

        Object& top(){
            return myList.front();
        }

        const Object& top() const{
            return myList.front();
        }

        void push(const Object& obj){
            myList.push_front(obj);
        }

        void push(Object&& obj){
            myList.push_front(std::move(obj));
        }

        void pop(){
            myList.pop_front();
        }

        using iterator = typename List<Object>::iterator;
        using const_iterator = typename List<Object>::const_iterator;

        iterator begin(){
            return ++(myList.begin());
        }

        const_iterator begin() const{
            return ++(myList.begin());
        }

    private:

        List<Object> myList;
};

#endif
