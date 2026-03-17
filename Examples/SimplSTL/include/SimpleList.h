#pragma once

template <typename>
class SimpleList;

template <typename T>
class SimpleListIterator;

template <typename T>
class SimpleListNode
{
public:
    SimpleListNode(const T& value)
        : m_value(value), m_pNext(nullptr), m_pPrev(nullptr)
    {
    }

private:
    T m_value;
    SimpleListNode* m_pNext;
    SimpleListNode* m_pPrev;

    template <typename>
    friend class SimpleList;

    template <typename>
    friend class SimpleListIterator;
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
    const T& operator*() const
    {
        return m_pNode->m_value;
    }

    SimpleListIterator& operator++()
    {
        m_pNode = m_pNode->m_pNext;
        return *this;
    }
    SimpleListIterator& operator--()
    {
        m_pNode = m_pNode->m_pPrev;
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
    template <typename>
    friend class SimpleList;
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

    void PushBack(const T& value);
    void PushFront(const T& value);

    SimpleListIterator<T> Insert(Iterator pos, const T& value);
    SimpleListIterator<T> Erase(Iterator pos);

    void PopBack();
    void PopFront();

    size_t Size() const;
    void Clear();

    Iterator Begin() const
    {
        return Iterator(m_pSentinel->m_pNext);
    }

    Iterator End() const
    {
        return Iterator(m_pSentinel);
    }
    /*把 other 中 it 指向的节点
    移动到当前链表 pos 位置之前*/
    void Splice(SimpleListIterator<T> pos, SimpleList& other,
                SimpleListIterator<T> it);

private:
    // SimpleListNode<T>* m_pHead;
    // SimpleListNode<T>* m_pTail;
    Node* m_pSentinel;
    size_t m_size;

private:
    // 禁止拷贝构造和赋值操作
    SimpleList(const SimpleList&) = delete;
    SimpleList& operator=(const SimpleList&) = delete;
};

template <typename T>
inline SimpleList<T>::SimpleList() : m_pSentinel(nullptr), m_size(0)
{
    m_pSentinel = new Node(T());         // 创建哨兵节点
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
inline void SimpleList<T>::PushBack(const T& value)
{
    Insert(End(), value);
}

template <typename T>
inline void SimpleList<T>::PushFront(const T& value)
{
    Insert(Begin(), value);
}

template <typename T>
inline SimpleListIterator<T> SimpleList<T>::Insert(Iterator pos, const T& value)
{
    auto* pNode = pos.m_pNode;
    auto* pNewNode = new SimpleListNode<T>(value);

    Node* pPrev = pNode->m_pPrev;
    pPrev->m_pNext = pNewNode;
    pNewNode->m_pPrev = pPrev;

    pNewNode->m_pNext = pNode;
    pNode->m_pPrev = pNewNode;

    ++m_size;
    return Iterator(pNewNode);
}

template <typename T>
inline SimpleListIterator<T> SimpleList<T>::Erase(Iterator pos)
{
    auto* pNode = pos.m_pNode;
    if (pNode == m_pSentinel) {
        return End();  // 不允许删除哨兵节点，直接返回原位置
    }
    auto* pPrev = pNode->m_pPrev;
    auto* pNext = pNode->m_pNext;

    pPrev->m_pNext = pNext;
    pNext->m_pPrev = pPrev;

    delete pNode;
    --m_size;
    return Iterator(pNext);
}

template <typename T>
inline size_t SimpleList<T>::Size() const
{
    return m_size;
}

template <typename T>
inline void SimpleList<T>::Clear()
{
    SimpleListNode<T>* current = m_pSentinel->m_pNext;
    while (current != m_pSentinel) {
        SimpleListNode<T>* next = current->m_pNext;
        delete current;
        current = next;
    }
    m_pSentinel->m_pNext = m_pSentinel;
    m_pSentinel->m_pPrev = m_pSentinel;
    m_size = 0;
}

template <typename T>
inline void SimpleList<T>::Splice(SimpleListIterator<T> pos, SimpleList& other,
                                  SimpleListIterator<T> it)
{
    auto* pOtherNode = it.m_pNode;
    if (pOtherNode == other.m_pSentinel) {
        return;  // it 指向的是 other 的哨兵节点，不进行操作
    }

    if (pos == it)
        return;  // pos 和 it 指向同一个位置，不进行操作
    if (this == &other && pos.m_pNode == it.m_pNode->m_pNext)
        return;  

    // 从 other 中移除 pOtherNode
    auto* pOtherPrev = pOtherNode->m_pPrev;
    auto* pOtherNext = pOtherNode->m_pNext;
    pOtherNext->m_pPrev = pOtherPrev;
    pOtherPrev->m_pNext = pOtherNext;

    // 将 pOtherNode 插入到当前链表 pos 位置之前
    auto* pPosNode = pos.m_pNode;
    auto* pPosPrev = pPosNode->m_pPrev;
    pPosPrev->m_pNext = pOtherNode;
    pOtherNode->m_pPrev = pPosPrev;
    pOtherNode->m_pNext = pPosNode;
    pPosNode->m_pPrev = pOtherNode;

    if (this != &other) {
        --other.m_size;
        ++m_size;
    }
}

template <typename T>
inline void SimpleList<T>::PopFront()
{
    if (m_size == 0)
        return;  // 链表为空，不进行操作
    Erase(Begin());
}

template <typename T>
inline void SimpleList<T>::PopBack()
{
    if (m_size == 0)
        return;  // 链表为空，不进行操作
    Erase(--End());
}