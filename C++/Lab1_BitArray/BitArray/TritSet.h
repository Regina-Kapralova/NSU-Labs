#pragma once
#include "TritValue.h"

class Trit;
class TritSet {
    uint* trits;        // ������ ������
    uint size;

    void setTrit(uint trit_index, TritValue trit); // ���������� ����
    void takeMoreMemory(uint index);              //��������� ������ trits
    bool isMore(int index) const { return index + 1 > capacity(); }  //���� ������ ������ ����� ������ � ����������
    void smartSetTrit(int index, TritValue tv);

    TritValue getTritValue(uint trit_index) const;              // ����� �������� �����
    bool getBit(uint position, uint elem) const { return (elem >> (uint_sizeof * 8 - position - 1)) & 1; }
    TritValue createTrirValue(bool first_bit, bool second_bit) const; //������������� ���� �� ���� �����
    TritValue smartGetTritValue(int index) const;

public:
    friend class Trit;
    TritSet(uint trits_count);     // ����������� �������� ������ ��� trits_count ������
    TritSet();
    TritSet(const TritSet& other);      //����������� ����������� 

    size_t capacity() const { return size * uint_sizeof * 4; }    // ������ ������ � ������
    size_t length() const;                  //���������� �������� ������

    const TritSet operator&(const TritSet& ts) const;
    const TritSet operator|(const TritSet& ts) const;
    const TritSet operator!() const;

    TritSet& operator=(const TritSet& other);
    TritSet& operator&=(const TritSet& other);
    TritSet& operator|=(const TritSet& other);
    Trit operator[](int index) const;

    uint cardinality(TritValue tv) const;   // ������� ����� ������
    void trim(uint lastIndex);            // ������ ���������� �� lastIndex � ������
    void shrink();                          //������������ ������ ����� ���������� �������������� �����

};
