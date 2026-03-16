#pragma once

template <typename T>
class SimpleListNode
{
public:
    SimpleListNode(T value) : m_value(value), m_pNext(nullptr), m_pPrev(nullptr)
    {
    }

private:
    T m_value;
    SimpleListNode* m_pNext;
    SimpleListNode* m_pPrev;
};

template <typename T>
class SimpleListIterator
{
public:
    using Node = SimpleListNode<T>;
    SimpleListIterator(Node* node = nullptr) : m_pNode(node)
    {
    }

    T& operator*()
    {
        return m_pNode->m_value;
    }

    SimpleListIterator& operator++()
    {
        m_pNode = m_pNode->m_pNext;
        return *this;
    }

    bool operator!=(const SimpleListIterator& other) const
    {
        return m_pNode != other.m_pNode;
    }

    bool operator==(const SimpleListIterator& other) const
    {
        return m_pNode == other.m_pNode;
    }

private:
    Node* m_pNode;
};

template <typename T>
class SimpleList
{
public:
    using Iterator = SimpleListIterator<T>;
    using Node = SimpleListNode<T>;

public:
    SimpleList();
    ~SimpleList();

    void PushBack(T& value);
    void PushFront(T& value);

    SimpleListIterator<T> Insert(Iterator pos, T& value);
    SimpleListIterator<T> Erase(Iterator pos);

    void PopBack();
    void PopFrot();

    size_t Size();
    void Clear();

    Iterator begin()
    {
        return Iterator(m_pSentinel->m_pNext);
    }

    Iterator end()
    {
        return Iterator(m_pSentinel);
    }

private:
    //SimpleListNode<T>* m_pHead;
    //SimpleListNode<T>* m_pTail;
    Node* m_pSentinel;
    size_t m_size;
};

template <typename T>
inline SimpleList<T>::SimpleList()
    : m_pSentinel(nullptr), m_size(0)
{
    m_pSentinel = new Node(T());  // 创建哨兵节点
    m_pSentinel->m_pNext = m_pSentinel;  // 哨兵节点的下一个指向自己
    m_pSentinel->m_pPrev = m_pSentinel;  // 哨兵节点的上一个指向自己
}

template <typename T>
inline SimpleList<T>::~SimpleList()
{
    Clear();
    delete m_pSentinel;
}

template <typename T>
inline void SimpleList<T>::PushBack(T& value)
{
    Insert(end(), value);
}

template <typename T>
inline void SimpleList<T>::PushFront(T& value)
{
    Insert(begin(), value);
}

template <typename T>
inline SimpleListIterator<T> SimpleList<T>::Insert(Iterator pos, T& value)
{
    auto* pNode = pos.m_pNode;
    auto* pNewNode = new SimpleListNode<T>(value);

    Node* pPrev = pNode->m_pPrev;
    pPrev->m_pNext = pNewNode;
    pNewNode->m_pPrev = pPrev;

    pNewNode->m_pNext = pNode;
    pNode->m_pPrev = pNewNode;

    ++m_size;
    return Iterator(newNode);
}

template <typename T>
inline SimpleListIterator<T> SimpleList<T>::Erase(Iterator pos)
{
    auto* pNode = pos.m_pNode;

    auto* pPrev = node->m_pPrev;
    auto* pNext = node->m_pNext;

    pPrev->m_pNext = pNext;
    pNext->m_pPrev = pPrev;

    delete node;
    --m_size;
    return Iterator(pNext);
}

template <typename T>
inline size_t SimpleList<T>::Size()
{
    return m_size;
}

template <typename T>
inline void SimpleList<T>::Clear()
{
    SimpleListNode* current = m_pSentinel->m_pNext;
    while (current != m_pSentinel) {
        SimpleListNode* next = current->m_pNext;
        delete current;
        current = next;
    }
    m_pSentinel->m_pNext = m_pSentinel;
    m_pSentinel->m_pPrev = m_pSentinel;
    m_size = 0;
}

template <typename T>
inline void SimpleList<T>::PopFrot()
{
    Erase(begin());
}

template <typename T>
inline void SimpleList<T>::PopBack()
{
    Erase(--end());
}