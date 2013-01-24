#define DELDATA if (data && m_size) { \
                for (uint i = 0; i < m_size; i++) \
                data[i].~T(); \
                delete[] data; }

template<typename T>
Vector<T>::Vector() {
    data = 0;
    m_size = 0;
}

template<typename T>
Vector<T>::Vector(unsigned int size) {
    data = new T[size];
    this->m_size = size;
}

template<typename T>
Vector<T>::Vector(unsigned int size, const T& value) {
    data = new T[size];
    this->m_size = size;
    
    for (uint i = 0; i < size; i++)
        new (&data[i]) T(value);
}

template<typename T>
Vector<T>::Vector(const Vector<T>& other) {
    this->m_size = other.m_size;
    data = new T[m_size];
    
    for (uint i = 0; i < m_size; i++)
        new (&data[i]) T(other.data[i]);
}

template<typename T>
Vector<T>& Vector<T>::operator =(const Vector<T>& other) {
    DELDATA;
    m_size = other.m_size;
    data = new T[m_size];
    
    for (uint i = 0; i < m_size; i++)
        new (&data[i]) T(other.data[i]);
    return *this;
}

template<typename T>
Vector<T>::~Vector() {
    DELDATA;
}

template<typename T>
T& Vector<T>::operator [](unsigned int idx) const {
    return data[idx];
}

template<typename T>
void Vector<T>::push(const T& element) {
    T* newData = new T[m_size + 1];
    if (m_size && data)
        Memory::copy(data, newData, m_size);
    new (&newData[m_size]) T(element);
    
    m_size++;
    if (data)
        delete[] data;
    data = newData;
}

template<typename T>
void Vector<T>::pop() {
    if (empty())
        return;
    
    m_size--;
    data[m_size].~T();
    T* newData = new T[m_size];
    Memory::copy(data, newData, m_size);
    
    delete[] data;
    data = newData;
}

template<typename T>
T& Vector<T>::back() const {
    return data[m_size - 1];
}

template<typename T>
T& Vector<T>::front() const {
    return data[0];
}

template<typename T>
uint Vector<T>::size() const {
    return m_size;
}

template<typename T>
bool Vector<T>::empty() const {
    return m_size == 0;
}

template<typename T>
void Vector<T>::clear() {
    if (empty())
        return;
    
    DELDATA;
    data = 0;
    m_size = 0;
}