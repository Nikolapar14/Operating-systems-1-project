

#ifndef PROJEKAT1_1_STATICLIST2_HPP
#define PROJEKAT1_1_STATICLIST2_HPP

#include "memoryAllocator.hpp"

template<typename T>
class StaticList
{
private:

    int head,tail;
    bool inicijalizovano;
    static int constexpr velicina = 20;
    int brojElemenata;
    T** lista;

public:

    StaticList() : head(0), tail(0), inicijalizovano(false),brojElemenata(0), lista(nullptr) {}

    StaticList(const StaticList<T> &) = delete;

    StaticList<T> &operator=(const StaticList<T> &) = delete;


    bool dohvatiInizijalizovano(){
        return this->inicijalizovano;
    }

    void inicijalizuj(){
        MemoryAllocator* singleton = MemoryAllocator::getMemoryAllocator();
        lista = (T**)singleton->memory_alloc(velicina * sizeof(T*));
        head = 0;
        tail = 0;
        brojElemenata = 0;

    }

    int getBrojElemenata(){
        return this->brojElemenata;
    }

    void addLast(T *data)
    {

        if(brojElemenata >= velicina){

            return;
        }
        lista[tail] = data;
        brojElemenata++;
        tail = (tail+1)%velicina;

    }

    T *removeFirst()
    {

        if(brojElemenata == 0){
            return nullptr;
        }
        int  oldHead = head;
        head = (head+1)%velicina;
        brojElemenata--;
        return lista[oldHead];


    }

};

#endif //PROJEKAT1_1_STATICLIST2_HPP
