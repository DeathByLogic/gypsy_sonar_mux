#ifndef _MEMORY_
#define _MEMORY_

template <typename T>
class MemoryItem {
  private:
    T    _value;
    bool _clear;
    bool _readonly;

  public:
    
};

template <typename T>
MemoryItem::MemoryItem(bool ro, bool cor) {
  _clear = cor;
  _readonly = ro;
}

template <typename T>
MemoryItem::operator=() {
  
}
