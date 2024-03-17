#ifndef QUEUE_H_
#define QUEUE_H_

#include <assert.h>

typedef unsigned int ui;

template<class T>
class Queue{
    public:
        Queue();
        ~Queue();
        void Push(const T& data);
        void Pop();
        void Extend();
        T Front();
        T Back();
        const T Front()const;
        const T Back()const;
        bool Empty()const;
        ui Size()const;
        Queue& operator=(const Queue& q);
        Queue& operator=(Queue&& q);

    private:
        T* Data;
        ui Begin;
        ui End;
        ui Capacity;
};

template<class T>
Queue<T>::Queue(){
    Capacity=2;
    Data=new T[Capacity];
    Begin=0;
    End=0;
}

template<class T>
Queue<T>::~Queue(){
    assert(Data!=nullptr);
    delete[] Data;
}

template<class T>
void Queue<T>::Push(const T& data){
    if(Size()==Capacity)Extend();
    Data[End]=data;
    End=(End+1)%Capacity;
}

template<class T>
void Queue<T>::Pop(){
    assert(Begin!=End);
    Begin=(Begin+1)%Capacity;
}

template<class T>
void Queue<T>::Extend(){
    Capacity*=2;
    T* temp=new T[Capacity];
    for(ui i=Begin, j=0;i<End;i=(i+1)%Capacity, j++)
        temp[j]=Data[i];
    Begin=0;
    End=Size();
    delete[] Data;
    Data=temp;
}

template<class T>
T Queue<T>::Front(){
    assert(Begin!=End);
    return Data[Begin];
}

template<class T>
T Queue<T>::Back(){
    assert(Begin!=End);
    return Data[(End-1+Capacity)%Capacity];
}

template<class T>
const T Queue<T>::Front()const{
    assert(Begin!=End);
    return Data[Begin];
}

template<class T>
const T Queue<T>::Back()const{
    assert(Begin!=End);
    return Data[(End-1+Capacity)%Capacity];
}

template<class T>
bool Queue<T>::Empty()const{
    return Begin==End;
}

template<class T>
ui Queue<T>::Size()const{
    return (End-Begin+Capacity)%Capacity;
}

template<class T>
Queue<T>& Queue<T>::operator=(const Queue& q){
    if(this==&q)return *this;
    delete[] Data;
    Capacity=q.Capacity;
    Data=new T[Capacity];
    Begin=q.Begin;
    End=q.End;
    for(ui i=Begin, j=0;i<End;i=(i+1)%Capacity, j++)
        Data[j]=q.Data[i];
    return *this;
}

template<class T>
Queue<T>& Queue<T>::operator=(Queue&& q){
    if(this==&q)return *this;
    delete[] Data;
    Capacity=q.Capacity;
    Data=q.Data;q.Data=nullptr;
    Begin=q.Begin;q.Begin=0;
    End=q.End;q.End=0;
    return *this;
}

#endif