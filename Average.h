#ifndef _AVERAGE_H
#define _AVERAGE_H

template <typename T>
class Average {
  private:
    int    _count;
    int    _index;
    float  _sum;
    T     *_array;
    
  public:
    // Constructor
    Average(int count);
    
    T     getAverage();
    void  getAverage(T *);
    void  pushValue(T);
};

template<typename T>
Average<T>::Average(int count) {
   _count = count;
   _index = 0;
   _sum = 0;
   
   _array = new T[_count];
   
   for (int i = 0; i < _count; i++) {
     _array[i] = 0; 
   }
}

template<typename T>
T Average<T>::getAverage() {
  return (_sum / _count);
}

template<typename T>
void Average<T>::getAverage(T *avg) {
  avg = (_sum / _count); 
}

template<typename T>
void Average<T>::pushValue(T val) {
  // Subtract old value
  _sum -= _array[_index];
  
  // Update array to new value
  _array[_index] = val;
  
  // Add new value to sum
  _sum += _array[_index];
  
  // Increment index
  _index = (_index + 1) % _count;
}

#endif
