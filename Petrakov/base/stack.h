#ifndef __STACK_H__
#define __STACK_H__

const int MaxStackSize = 100;

template <class T>
class TStack
{
  T *pMem;
  int size;
  int top;
public:
  TStack(int _size)
  {
    size = _size;
    top = -1;
    if ((size < 1) || (size > MaxStackSize))
      throw size;
    pMem = new T[size];
  }
  TStack(const TStack& other) {
      size = other.size;
      top = other.top;
      pMem = new T[size];
      for (int i = 0; i <= top; i++) {
          pMem[i] = other.pMem[i];
      }
  }
  ~TStack()
  {
    delete [] pMem;
  }
  bool isEmpty() const {
      return top == -1;
  }
  bool isFull() const {
      return top == size - 1;
  }
  void Push(T el) {
      if (isFull()) throw "Stack overflow";
      pMem[++top] = el;
  }
  T Pop() {
      if (isEmpty()) throw "Stack is empty";
      return pMem[top--];
  }
  T Top() const{
      if (isEmpty()) throw "Stack is empty";
      return pMem[top];
  }
  int GetSize() const {
      return top + 1;
  }
  TStack& operator=(const TStack& other) {
      if (this != &other) {
          T* tempMem = new T[other.size];
          for (int i = 0; i <= other.top; i++)
              tempMem[i] = other.pMem[i];
          delete[] pMem;
          pMem = tempMem;
          size = other.size;
          top = other.top;
      }
      return *this;
  }
};

#endif
