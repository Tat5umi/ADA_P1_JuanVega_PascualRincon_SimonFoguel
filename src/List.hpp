#ifndef LIST_HPP
#define LIST_HPP


#include <utility>
#include <stdexcept>


template<typename Object>
class List{

  private:

    struct Node;

  public:

    class iterator;
    class const_iterator;

    List():
    head{new Node}, tail{nullptr}, theSize{0}{}

    List(const List& rhs):
    head{new Node}, tail{nullptr}, theSize{0}{
      for (auto itr1 = ++(rhs.begin()); itr1.current != nullptr; ++itr1){
        push_back(*itr1);
      }  
    }

    List(List&& rhs):
    head{std::exchange(rhs.head, nullptr)},
    tail{std::exchange(rhs.tail, nullptr)},
    theSize{std::exchange(rhs.theSize, 0)}{}

    ~List(){
        while(!empty()){
            pop_front();
        }
        delete head;
    }

    List& operator= (const List& rhs){
        List temp = rhs;
        std::swap(head, temp.head);
        std::swap(tail, temp.tail);
        std::swap(theSize, temp.theSize);
        return *this;
    }

    List& operator= (List&& rhs){
      std::swap(head, rhs.head);
      std::swap(tail, rhs.tail);
      std::swap(theSize, rhs.theSize);
      return *this;
    }

    void push_front(const Object& obj){
      Node* newNode = new Node(obj, head->next);
      head->next = newNode;
      if (empty()){
        tail = newNode;
      }
      theSize++;
    }

    void push_front(Object&& obj){
      Node* newNode = new Node(std::move(obj), head->next);
      head->next = newNode;
      if (empty()){
        tail = newNode;
      }
      theSize++;
    }

    void push_back(const Object& obj){
      if (empty()){
        push_front(obj);
        return;
      }
      tail->next = new Node(obj, nullptr);
      tail = tail->next;
      theSize++;
    }

    void push_back(Object&& obj){
      if (empty()){
        push_front(std::move(obj));
        return;
      }
      tail->next = new Node(std::move(obj), nullptr);
      tail = tail->next;
      theSize++;
    }

    void pop_front(){
      if (empty()) {
        throw std::runtime_error("attempted pop on an empty list");}
      Node* old = head->next;
      head->next = old->next;

      delete old;
      theSize--;
      if (empty()){
        tail = nullptr;
      }
    }

    Object& front(){
      if (empty()){
        throw std::runtime_error("front sobre una lista vacia");
      }
      return *(++begin());
    }

     const Object& front() const{
      if (empty()){
        throw std::runtime_error("front sobre una lista vacia");
      }
      return *(++begin());
    }    

      Object& back(){
       if (empty()){
         throw std::runtime_error("back sobre una lista vacia");
      }
      return tail->data;
    }

      const Object& back() const{
       if (empty()){
         throw std::runtime_error("back sobre una lista vacia");
      }
      return tail->data;
    }

    bool empty() const{
      return theSize == 0;
    }

    iterator begin(){
      return iterator(head);
    }

    const_iterator begin() const{
        return const_iterator(head);
    }

    iterator end(){
      return iterator(nullptr);
    }

    const_iterator end(){
      return const_iterator(nullptr);
    }

    int size() const{
      return theSize;
    }

    class iterator{

      public:

        friend class List;

        iterator(Node* n):
        current{n}{}

        Object& operator*(){
            return current->data;
        }

        iterator& operator++(){
            current = current->next; 
            return *this;
        }

        iterator operator++(int){
            iterator old = *this;
            ++(*this);
            return old;
        }

bool operator==(iterator& rhs){
          return current == rhs.current;
        }

        bool operator!=(iterator& rhs){
          return !(*this == rhs);
        }

      private:

        Node* current;

    };

    class const_iterator{

      public:

        friend class List;

        const_iterator(Node* n):
        current{n}{}

        const Object& operator*() const{
            return current->data;
        }

        const_iterator& operator++(){
            current = current->next; 
            return *this;
        }

        const_iterator operator++(int){
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        bool operator==(const const_iterator& rhs) const{
          return current == rhs.current;
        }

        bool operator!=(const const_iterator& rhs) const{
          return !(*this == rhs);
        }

      private:

        Node* current;

    };

  
  private:
  
    struct Node{
  
      Object data;
      Node* next;
  
      Node():
      data{Object{}}, next{nullptr}{}
  
      Node(const Object& d, Node* n):
      data{d}, next{n}{}
  
      Node(Object&& d, Node* n):
      data{std::move(d)}, next{n}{}

    };

    Node* head;
    Node* tail;
    int theSize;

};





#endif
