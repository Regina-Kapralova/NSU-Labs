#pragma once
#include "TritValue.h"

class Trit;
class TritSet {
    uint* trits;        // массив тритов
    uint size;

    void setTrit(uint trit_index, TritValue trit); // установить трит
    void takeMoreMemory(uint index);              //увеличить размер trits
    bool isMore(int index) const { return index + 1 > capacity(); }  //если индекс больше числа тритов в контейнере
    void smartSetTrit(int index, TritValue tv);

    TritValue getTritValue(uint trit_index) const;              // взять значение трита
    bool getBit(uint position, uint elem) const { return (elem >> (uint_sizeof * 8 - position - 1)) & 1; }
    TritValue createTrirValue(bool first_bit, bool second_bit) const; //сгенерировать трит из двух битов
    TritValue smartGetTritValue(int index) const;

public:
    friend class Trit;
    TritSet(uint trits_count);     // конструктор выделяет память под trits_count тритов
    TritSet();
    TritSet(const TritSet& other);      //конструктор копирования 

    size_t capacity() const { return size * uint_sizeof * 4; }    // размер памяти в тритах
    size_t length() const;                  //количество значащих тритов

    const TritSet operator&(const TritSet& ts) const;
    const TritSet operator|(const TritSet& ts) const;
    const TritSet operator!() const;

    TritSet& operator=(const TritSet& other);
    TritSet& operator&=(const TritSet& other);
    TritSet& operator|=(const TritSet& other);
    Trit operator[](int index) const;

    uint cardinality(TritValue tv) const;   // сколько таких тритов
    void trim(uint lastIndex);            // забыть содержимое от lastIndex и дальше
    void shrink();                          //освобождение памяти после последнего установленного трита

};
