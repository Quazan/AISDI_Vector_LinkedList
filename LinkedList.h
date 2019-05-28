#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  class Node
  {
    public:
        Node* next;
        Node* prev;

        Type value;

        Node()
        {
            next = nullptr;
            prev = nullptr;

            value = Type();
        }

        Node(const Type& item)
        {
            next = nullptr;
            prev = nullptr;

            value = item;
        }
  };

  Node* head;
  Node* sentinel;

  int Size;

  LinkedList()
  {
    Node* tmp = new Node;
    head = tmp;
    sentinel = tmp;
    Size = 0;
  }

  LinkedList(std::initializer_list<Type> l):LinkedList()
  {
    for(auto i = l.begin(); i != l.end(); ++i)
    {
        append(*i);
    }
  }

  LinkedList(const LinkedList& other):LinkedList()
  {
    for(auto i = other.begin(); i != other.end(); ++i)
    {
        append(*i);
    }
  }

  LinkedList(LinkedList&& other)
  {
    head = nullptr;
    sentinel = nullptr;
    std::swap(head, other.head);
    std::swap(sentinel, other.sentinel);
    std::swap(Size, other.Size);
  }

  ~LinkedList()
  {
    Node* tmp;
    while(head != sentinel)
    {
        tmp = head;
        head = head->next;

        delete tmp;
    }

    if(sentinel != nullptr) delete sentinel;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(begin() == other.begin() && end() == other.end() && Size == other.Size) return *this;

    erase(begin(), end());

    for(auto i = other.begin(); i != other.end(); ++i)
    {
        append(*i);
    }

    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    erase(begin(), end());

    std::swap(head, other.head);
    std::swap(sentinel, other.sentinel);
    std::swap(Size, other.Size);

    return *this;
  }

  bool isEmpty() const
  {
    if(Size == 0) return true;
    else return false;
  }

  size_type getSize() const
  {
    return Size;
  }

  void append(const Type& item)
  {
    Node* tmp = new Node(item);
    Node* prev_node;
    if(Size == 0)
    {
        head = tmp;
    }
    else
    {
        prev_node = sentinel->prev;
        tmp->prev = prev_node;
        prev_node->next = tmp;
    }

    tmp->next = sentinel;
    sentinel->prev = tmp;

    Size++;
  }

  void prepend(const Type& item)
  {
    Node* tmp = new Node(item);

    if(Size==0)
    {
        sentinel->prev = tmp;

        tmp->next = sentinel;
    }
    else
    {
        head->prev = tmp;
        tmp->next = head;
    }

    head = tmp;
    Size++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(insertPosition == begin())
    {
        prepend(item);
        return;
    }

    if(insertPosition == end())
    {
        append(item);
        return;
    }

    Node* tmp = new Node(item);
    Node* old_node = insertPosition.node;
    Node* old_node_prev = old_node->prev;

    tmp->next = old_node;
    tmp->prev = old_node_prev;

    old_node->prev = tmp;
    old_node_prev->next = tmp;

    Size++;
    return;
  }

  Type popFirst()
  {
    if(Size == 0) throw std::out_of_range("");

    Type val = head->value;

    Node* tmp = head;

    head = head->next;
    head->prev = nullptr;

    delete tmp;
    Size--;
    return val;
  }

  Type popLast()
  {
    if(Size == 0) throw std::out_of_range("");

    if(Size == 1)
    {
       return popFirst();
    }

    Node* tmp = sentinel;
    Node* temporary = tmp->prev;
    tmp = tmp->prev;

    Type val = tmp->value;

    tmp = tmp->prev;

    tmp->next = sentinel;
    sentinel->prev = tmp;

    delete temporary;
    Size--;

    return val;
  }

  void erase(const const_iterator& possition)
  {
    if(Size == 0) throw std::out_of_range("");
    else if(possition == end()) throw std::out_of_range("");

    if(possition == begin())
    {
        popFirst();
        return;
    }

    Node* tmp = possition.node;
    Node* left_node = tmp->prev;
    Node* right_node = tmp->next;

    left_node->next = right_node;
    right_node->prev = left_node;

    delete tmp;
    Size--;
    return;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded == lastExcluded) return;

    Node* tmp = firstIncluded.node;
    Node* next;
    Node* prev = tmp->prev;
    while(tmp != lastExcluded.node)
    {
        next = tmp->next;
        delete tmp;
        tmp = next;
        Size--;
    }

    if(prev == nullptr)
    {
        head = tmp;
    }
    else
    {
        prev->next = tmp;
        tmp->prev = prev;
    }

    if(Size == 0)
    {
        sentinel->prev = nullptr;
    }
  }

  iterator begin()
  {
    Iterator it;
    it.node = head;
    return iterator(it);
  }

  iterator end()
  {
    Iterator it;
    it.node = sentinel;
    return iterator(it);
  }

  const_iterator cbegin() const
  {
    ConstIterator it;
    it.node = head;
    return const_iterator(it);
  }

  const_iterator cend() const
  {
    ConstIterator it;
    it.node = sentinel;
    return const_iterator(it);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  Node* node;

  explicit ConstIterator()
  {}

  reference operator*() const
  {
    if(node->next == nullptr) throw std::out_of_range("not a thing");

    return node->value;
  }

  ConstIterator& operator++()
  {
    if(node->next == nullptr) throw std::out_of_range("");

    node = node->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if(node->next == nullptr) throw std::out_of_range("");

    ConstIterator it;
    it.node = node;

    node = node->next;

    return it;
  }

  ConstIterator& operator--()
  {
    if(node->prev == nullptr) throw std::out_of_range("");

    node = node->prev;
    return *this;
  }

  ConstIterator operator--(int)
  {
    if(node->prev == nullptr) throw std::out_of_range("");

    ConstIterator it;
    it.node = node;

    node = node->prev;

    return it;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator it;
    it.node = node;

    for(difference_type i = 0; i < d; i++)
    {
        it++;
    }

    return it;
  }

  ConstIterator operator-(difference_type d) const
  {
    ConstIterator it;
    it.node = node;

    for(difference_type i = 0; i < d; i++)
    {
        it--;
    }

    return it;
  }

  bool operator==(const ConstIterator& other) const
  {
    if(node == other.node) return true;
    else return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if(node != other.node) return true;
    else return false;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
