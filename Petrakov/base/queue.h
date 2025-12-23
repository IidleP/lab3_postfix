#pragma once

const int MaxQueueSize = 100;

template <class T>
class TQueue
{
    T* pMem;
    int size;
    int next;
    int back;
    int count;

public:
    TQueue(int _size = 50)
    {
        size = _size;
        next = 0;
        back = -1;
        count = 0;

        if ((size < 1) || (size > MaxQueueSize))
            throw size;

        pMem = new T[size];
    }

    TQueue(const TQueue& other)
    {
        size = other.size;
        next = other.next;
        back = other.back;
        count = other.count;

        pMem = new T[size];

        for (int i = 0; i < size; i++)
        {
            pMem[i] = other.pMem[i];
        }
    }

    ~TQueue()
    {
        delete[] pMem;
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    bool isFull() const
    {
        return count == size;
    }

    void Push(T el)
    {
        if (isFull())
            throw "Queue overflow";

        back = (back + 1) % size;
        pMem[back] = el;
        count++;
    }

    T Pop()
    {
        if (isEmpty())
            throw "Queue is empty";

        T item = pMem[next];
        next = (next + 1) % size;
        count--;

        return item;
    }

    T Front() const
    {
        if (isEmpty())
            throw "Queue is empty";

        return pMem[next];
    }

    int GetSize() const
    {
        return count;
    }

    TQueue& operator=(const TQueue& other)
    {
        if (this != &other)
        {
            T* tempMem = new T[other.size];

            for (int i = 0; i < other.size; i++)
            {
                tempMem[i] = other.pMem[i];
            }

            delete[] pMem;
            pMem = tempMem;
            size = other.size;
            next = other.next;
            back = other.back;
            count = other.count;
        }
        return *this;
    }
    int GetMaxSize() const
    {
        return size;
    }
};
