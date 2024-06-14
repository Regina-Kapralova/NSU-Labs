#include "TritSet.h"
#include "Trit.h"


// sizeof(int) = 4 байт или 32 бит. 
// Trit имеет 3 значения => sizeof(Trit) = 2 бит.
// => sizeof(int) = 4 байт или 16 Trit. 
// (в 1 байт влезает 4 Trit'а)
TritSet::TritSet(uint trits_count) {
    size = trits_count / (4 * uint_sizeof) + (trits_count % (4 * uint_sizeof) ? 1 : 0);
    trits = new uint[size];
    for (int i = 0; i < size; ++i)
        trits[i] = 2863311530;
}
TritSet::TritSet(const TritSet& other) {
    size = other.size;
    trits = new uint[size];
    for (int i = 0; i < size; ++i)
        this->trits[i] = other.trits[i];
}
TritSet::TritSet() {
    this->size = 0;
    this->trits = nullptr;
}


size_t TritSet::length() const {
    for (uint i = capacity() - 1; i > 0; i--)
        if (getTritValue(i) != UNKNOWN)
            return i + 1;
    return 1;
}


Trit TritSet::operator[] (int index) const {
    Trit t(index, this);
    return t;
}


const TritSet TritSet::operator&(const TritSet& ts) const {
    TritSet new_set(this->size >= ts.size ? *this : ts);
    uint N = this->capacity() >= ts.capacity() ? ts.capacity() : this->capacity();
    for (uint i = 0; i < N; ++i)
        new_set.setTrit(i, ((*this)[i] & ts[i]));
    return std::move(new_set);
}

const TritSet TritSet::operator|(const TritSet& ts) const {
    TritSet new_set(this->size >= ts.size ? *this : ts);
    uint N = this->capacity() >= ts.capacity() ? ts.capacity() : this->capacity();
    for (uint i = 0; i < N; ++i)
        new_set.setTrit(i, ((*this)[i] | ts[i]));
    return std::move(new_set);
}

const TritSet TritSet::operator!() const {
    TritSet new_set(*this);
    for (int i = 0; i < capacity(); ++i)
        new_set.setTrit(i, (!(*this)[i]));
    return std::move(new_set);
}


TritSet& TritSet::operator=(const TritSet& other) {
    this->size = other.size;
    delete this->trits;
    trits = new uint[this->size];
    for (uint i = 0; i < capacity(); i++)
        (*this)[i] = other[i];
    return *this;
}

TritSet& TritSet::operator&=(const TritSet& other) {
    *this = (*this & other);
    return *this;
}

TritSet& TritSet::operator|=(const TritSet& other) {
    this->size = other.size;
    delete this->trits;
    trits = new uint[this->size];
    for (uint i = 0; i < capacity(); i++)
        (*this)[i] |= other[i];
    return *this;
}


uint TritSet::cardinality(TritValue tv) const {
    uint count_trits = 0;
    for (uint i = capacity() - 1; i > 0; i--) {
        if (getTritValue(i) == tv)
            count_trits++;
    }
    return count_trits;
}

void TritSet::trim(uint lastIndex) {
    for (int i = lastIndex; i < lastIndex + uint_sizeof * 4; ++i)
        this->setTrit(i, UNKNOWN);
    uint byte_number = lastIndex / 4;
    uint array_position = byte_number / uint_sizeof + 1;
    for (uint i = array_position; i < size; ++i)
        trits[i] = 2863311530;
}

void TritSet::shrink() {
    uint new_size = (length() - 1) / (4 * uint_sizeof) + ((length() - 1) % (4 * uint_sizeof) ? 1 : 0);
    uint* trits1 = new uint[new_size];
    for (uint i = 0; i < new_size; ++i)
        trits1[i] = trits[i];
    delete trits;
    trits = trits1;
    size = new_size;
}



TritValue TritSet::getTritValue(uint trit_index) const {
    uint byte_index = trit_index / 4; //индекс байта
    uint array_index = byte_index / uint_sizeof; //индекс uint 
    uint array_elem = this->trits[array_index];
    uint first_position = (trit_index - 4 * uint_sizeof * (array_index)) * 2; //позиция бита в uint
    return createTrirValue(getBit(first_position, array_elem), getBit(first_position + 1, array_elem));
}

TritValue TritSet::createTrirValue(bool first_bit, bool second_bit) const {
    if (first_bit == 1 && second_bit == 1)
        return TRUE;
    else if (first_bit == 0 && second_bit == 0)
        return FALSE;
    else return UNKNOWN;
}

TritValue TritSet::smartGetTritValue(int index) const {
    if (isMore(index))
        return UNKNOWN;
    return this->getTritValue(index);
}



void TritSet::setTrit(uint trit_index, TritValue tv) {
    uint byte_index = trit_index / 4; //индекс байта
    uint array_index = byte_index / uint_sizeof; //индекс uint 
    uint first_position = (trit_index - 4 * uint_sizeof * (array_index)) * 2; //позиция в uint
    if (tv == TRUE) {
        trits[array_index] = std::bitset<uint_sizeof * CHAR_BIT>(trits[array_index]).set(uint_sizeof * 8 - first_position - 1, true).to_ulong();
        trits[array_index] = std::bitset<uint_sizeof * CHAR_BIT>(trits[array_index]).set(uint_sizeof * 8 - first_position - 2, true).to_ulong();
    }
    else {
        if (tv == FALSE)
            trits[array_index] = std::bitset<uint_sizeof * CHAR_BIT>(trits[array_index]).set(uint_sizeof * 8 - first_position - 1, false).to_ulong();
        else
            trits[array_index] = std::bitset<uint_sizeof * CHAR_BIT>(trits[array_index]).set(uint_sizeof * 8 - first_position - 1, true).to_ulong();
        trits[array_index] = std::bitset<uint_sizeof * CHAR_BIT>(trits[array_index]).set(uint_sizeof * 8 - first_position - 2, false).to_ulong();
    }
}

void TritSet::takeMoreMemory(uint index) {
    uint new_size = index / (4 * uint_sizeof) + (index % (4 * uint_sizeof) ? 1 : 0);;
    uint* trits1 = new uint[new_size / (4 * uint_sizeof) + (new_size % (4 * uint_sizeof) ? 1 : 0)];
    for (uint i = 0; i < size; ++i)
        trits1[i] = trits[i];
    delete trits;
    trits = trits1;
    size = new_size;
}

void TritSet::smartSetTrit(int index, TritValue tv) {
    if (index == -1)
        return;
    if (isMore(index) && tv != UNKNOWN) {
        this->takeMoreMemory(index);
        this->setTrit(index, tv);
    }
    else
        if (!isMore(index))
            this->setTrit(index, tv);
}
