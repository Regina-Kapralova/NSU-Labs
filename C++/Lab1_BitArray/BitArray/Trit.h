#pragma once
#include "TritValue.h"

class TritSet;
class Trit {
    TritSet* set;
    int index = -1;

public:

    Trit(int index, const TritSet* set);

    TritValue getTritValue() const;

    TritValue operator&(const Trit& t) const;
    TritValue operator&(TritValue tv) const;
    friend TritValue operator&(TritValue tv, const Trit& t) { return t & tv; }

    TritValue operator|(const Trit& t) const;
    TritValue operator|(TritValue tv) const;
    friend TritValue operator|(TritValue tv, const Trit& t) { return t | tv; }

    TritValue operator!() const;

    Trit& operator=(TritValue tv);
    Trit& operator=(const Trit& other);

    Trit& operator&=(TritValue tv);
    Trit& operator&=(const Trit& t);

    Trit& operator|=(TritValue tv);
    Trit& operator|=(const Trit& t);

    bool operator==(const Trit& t) const;
    bool operator==(TritValue tv) const;
    friend bool operator==(TritValue tv, const Trit& t) { return tv == t.getTritValue(); }

    friend std::ostream& operator<< (std::ostream& out, const Trit& t);
};
