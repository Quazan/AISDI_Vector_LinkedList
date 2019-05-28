#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
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

  Type* tab;

  int SIZE;
  int MAX_SIZE;

  Vector()
  {
    SIZE = 0;
    MAX_SIZE = 2;

    tab = new Type[MAX_SIZE];
  }

  Vector(std::initializer_list<Type> l):Vector()
  {
    for(auto i = l.begin(); i != l.end(); ++i)
    {
        append(*i);
    }
  }

  Vector(const Vector& other):Vector()
  {
    for(auto i = other.begin(); i != other.end(); ++i)
    {
        append(*i);
    }
  }

  Vector(Vector&& other):Vector()
  {
    std::swap(SIZE, other.SIZE);
    std::swap(MAX_SIZE, other.MAX_SIZE);
    std::swap(tab, other.tab);
  }

  ~Vector()
  {
    if(tab != nullptr) delete[] tab;
  }

  Vector& operator=(const Vector& other)
  {
    if(tab == other.tab) return *this;

    if(tab != nullptr) delete[] tab;
    SIZE = 0;
    MAX_SIZE = other.MAX_SIZE;
    tab = new Type[MAX_SIZE];

    for(auto i = other.begin(); i != other.end(); ++i)
    {
        append(*i);
    }

    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    std::swap(SIZE, other.SIZE);
    std::swap(MAX_SIZE, other.MAX_SIZE);
    std::swap(tab, other.tab);

    return *this;
  }

  bool isEmpty() const
  {
    if(SIZE == 0) return true;
    else return false;
  }

  size_type getSize() const
  {
    return SIZE;
  }

  void re_alloc()
  {
    MAX_SIZE *= 2;
    Type* tmp = new Type[MAX_SIZE];

    for(int i = 0; i < SIZE; i++)
    {
        tmp[i] = tab[i];
    }

    delete[] tab;
    tab = tmp;
  }

  void check_size()
  {
    if(SIZE + 1 == MAX_SIZE)
    {
        re_alloc();
    }
  }

  void append(const Type& item)
  {
    check_size();

    tab[SIZE] = item;
    SIZE++;
  }

  void move_right(int firstExcluded)
  {
    for(int i = SIZE; i > firstExcluded; i--)
    {
        tab[i] = tab[i-1];
    }
  }

  void prepend(const Type& item)
  {
    check_size();

    move_right(0);

    tab[0] = item;
    SIZE++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(insertPosition == begin())
    {
        prepend(item);
        return;
    }
    else if(insertPosition == end())
    {
        append(item);
        return;
    }

    check_size();

    move_right(insertPosition.id);

    tab[insertPosition.id] = item;
    SIZE++;

    return;
  }

  void move_left(int firstIncluded, int lastExcluded)
  {
    for(int i = firstIncluded; i < lastExcluded; i++)
    {
        tab[i] = tab[i+1];
    }
  }
  Type popFirst()
  {
    if(SIZE == 0) throw std::out_of_range("");

    Type tmp = tab[0];

    move_left(0, SIZE);

    SIZE--;
    return tmp;
  }

  Type popLast()
  {
    if(SIZE == 0) throw std::out_of_range("");

    Type tmp = tab[SIZE-1];
    SIZE--;
    return tmp;
  }

  void erase(const const_iterator& possition)
  {
    if(possition == end()) throw std::out_of_range("");

    move_left(possition.id, SIZE);
    SIZE--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded == lastExcluded) return;

    auto it = firstIncluded;

    for(int i = 0; i < (lastExcluded.id-firstIncluded.id); i++)
    {
        erase(it);
    }
  }

  iterator begin()
  {
    Iterator it;
    it.id = 0;
    it.tab = tab;
    it.SIZE = SIZE;

    return it;
  }

  iterator end()
  {
    Iterator it;
    it.id = SIZE;
    it.tab = tab;
    it.SIZE = SIZE;

    return it;
  }

  const_iterator cbegin() const
  {

    ConstIterator it;
    it.id = 0;
    it.tab = tab;
    it.SIZE = SIZE;

    return it;
  }

  const_iterator cend() const
  {
    ConstIterator it;
    it.id = SIZE;
    it.tab = tab;
    it.SIZE = SIZE;

    return it;
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
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

  int id;
  int SIZE;
  Type* tab;

  explicit ConstIterator()
  {}

  reference operator*() const
  {
    if(id >= SIZE || id < 0) throw std::out_of_range("");

    return tab[id];
  }

  ConstIterator& operator++()
  {
    if(id >= SIZE) throw std::out_of_range("");

    id++;
    return *this;
  }

  ConstIterator operator++(int)
  {
    if(id >= SIZE) throw std::out_of_range("");

    Iterator it;
    it = *this;
    id++;

    return it;
  }

  ConstIterator& operator--()
  {
    if(id <= 0) throw std::out_of_range("");
    id--;

    return *this;
  }

  ConstIterator operator--(int)
  {
    if(id <= 0) throw std::out_of_range("");
    Iterator it;
    it = *this;
    id--;

    return it;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator it;
    it = *this;

    for(difference_type i = 0; i < d; i++)
    {
        it++;
    }

    return it;
  }

  ConstIterator operator-(difference_type d) const
  {
    ConstIterator it;
    it = *this;

    for(difference_type i = 0; i < d; i++)
    {
        it--;
    }

    return it;
  }

  bool operator==(const ConstIterator& other) const
  {
    if(id == other.id && tab == other.tab) return true;
    else return false;

  }

  bool operator!=(const ConstIterator& other) const
  {
    if(id != other.id || tab != other.tab) return true;
    else return false;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

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

#endif // AISDI_LINEAR_VECTOR_H
