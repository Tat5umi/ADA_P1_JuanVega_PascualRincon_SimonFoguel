#ifndef QUEUE_LIST_HPP
#define QUEUE_LIST_HPP

#include <utility>
#include "List.hpp"

template<typename Object>
class LinkedQueue{

    public:

        LinkedQueue() = default;

        void enqueue(const Object& obj){
            myList.push_back(obj);
        }

        void enqueue(Object&& obj){
            myList.push_back(std::move(obj));
        }

        void dequeue(){
            myList.pop_front();
        }

        Object& front(){
            return myList.front();
        }

        const Object& front() const{
            return myList.front();
        }

        Object& back(){
            return myList.back();
        }

        const Object& back() const{
            return myList.back();
        }

        bool empty() const{
            return myList.empty();
        }

        int size() const{
            return myList.size();
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