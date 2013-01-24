#define BS_FREE if (this->string) delete[] this->string;
#define BS_ALLOC this->string = new T[this->length];
#define BS_VRFY if (!this->length) { this->string = 0; return; }

template<typename T>
BasicString<T>::BasicString() {
    string = 0;
    length = 0;
}

template<typename T>
BasicString<T>::BasicString(const T* string, unsigned int length) {
    string = 0;
    affect(string, length);
}

template<typename T>
BasicString<T>::BasicString(const BasicString<T>& other) {
    string = 0;
    affect(other);
}

template<typename T>
BasicString<T>::BasicString(const T value, unsigned int count) {
    string = 0;
    affect(value, count);
}

template<typename T>
BasicString<T>::~BasicString() {
   // BS_FREE;
}

template<typename T>
void BasicString<T>::affect(const BasicString<T>& other) {
    BS_FREE;
    length = other.length;
    BS_VRFY;
    BS_ALLOC;
    Memory::copy((char *)other.string, (char *)string, length * sizeof(T));
}

template<typename T>
void BasicString<T>::affect(const T* string, unsigned int length) {
    BS_FREE;
    this->length = length;
    BS_VRFY;
    BS_ALLOC;
    Memory::copy((char *)string, (char *)this->string, length * sizeof(T));
}

template<typename T>
void BasicString<T>::affect(const T value, unsigned int count) {
    BS_FREE;
    length = count;
    BS_VRFY;
    BS_ALLOC;
    for (uint i = 0; i < count; i++)
        string[i] = value;
}

template<typename T>
bool BasicString<T>::compare(const BasicString<T>& other) const {
    if (length != other.length)
        return false;
    for (uint i = 0; i < length; i++) {
        if (string[i] != other.string[i])
            return false;
    }
    return true;
}

template<typename T>
bool BasicString<T>::compare(const T* string, unsigned int length) const {
    if (this->length != length)
        return false;
    
    for (uint i = 0; i < length; i++) {
        if (this->string[i] != string[i])
            return false;
    }
    return true;
}

template<typename T>
BasicString<T>& BasicString<T>::append(const BasicString<T>& other) {
    T* newData = new T[length + other.length];
    
    Memory::copy(string, newData, length);
    Memory::copy(other.string, newData, other.length, length);
    
    BS_FREE;
    string = newData;
    length += other.length;
    return *this;
}

template<typename T>
BasicString<T>& BasicString<T>::append(const T* string, unsigned int length) {
    T* newData = new T[length + this->length];
    
    Memory::copy(this->string, newData, this->length);
    Memory::copy(string, newData, length, this->length);
    
    BS_FREE;
    this->string = newData;
    this->length += length;
    return *this;
}

template<typename T>
BasicString<T>& BasicString<T>::append(const T other) {
    T* newData = new T[length + 1];
    
    Memory::copy(string, newData, length);
    
    BS_FREE;
    string = newData;
    string[length] = other;
    length++;
    return *this;
}

template<typename T>
BasicString<T> BasicString<T>::concat(const BasicString<T>& other) const {
    BasicString<T> ret(*this);
    return (ret.append(other));
}

template<typename T>
BasicString<T> BasicString<T>::concat(const T* string, unsigned int length) const {
    BasicString<T> ret(*this);
    return (ret.append(string, length));
}

template<typename T>
BasicString<T> BasicString<T>::concat(const T other) const {
    BasicString<T> ret(*this);
    return (ret.append(other));
}

template<typename T>
void BasicString<T>::clear() {
    BS_FREE;
    length = 0;
    string = 0;
}

template<typename T>
bool BasicString<T>::contains(const T& c) const {
    for (uint i = 0; i < length; i++) {
        if (string[i] == c)
            return true;
    }
    return false;
}

template<typename T>
BasicString<T> BasicString<T>::substr(int start, unsigned int size) {
    if (!size)
        return BasicString<T>();
    
    if (size < 0)
        start = length - start;
    
    if (size == -1)
        size = length - start;
    
    BasicString<T> ret;
    ret.string = new T[size + 1];
    ret.length = size;
    Memory::copy((char *)ret.string, (char *)(&string[start]), size * sizeof(T));
    return ret;
}

template<typename T>
Vector<BasicString<T> > BasicString<T>::split(T sep) const {
    Vector<BasicString<T> > ret;
    ret.push(BasicString<T>());
    
    for (uint i = 0; i < length; i++) {
        if (string[i] == sep)
            ret.push(BasicString<T>());
        else
            ret.back().append(string[i]);
    }
    return ret;
}

template<typename T>
bool BasicString<T>::operator< (const BasicString<T>& other) const {
    for (uint i = 0; i < length && i < other.length; i++) {
        if (string[i] < other.string[i])
            return true;
    }
    
    if (length < other.length)
        return true;
    
    return false;
}