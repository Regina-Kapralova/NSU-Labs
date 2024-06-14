#include "Trit.h"
#include "TritSet.h"

Trit::Trit(int index, const TritSet* set) {
    this->index = index;
    this->set = const_cast<TritSet*>(set);
}

TritValue Trit::getTritValue() const {
    if (index != -1)
        return  (*set).smartGetTritValue(index);
    else
        return UNKNOWN;
}


TritValue Trit::operator!() const {
    switch (this->getTritValue()) {
    case TRUE:
        return FALSE;
    case FALSE:
        return TRUE;
    default:
        return UNKNOWN;
    }
}


TritValue Trit::operator&(TritValue tv) const {
    switch (this->getTritValue()) {
    case TRUE:
        switch (tv) {
        case FALSE: return FALSE;
        case UNKNOWN: return UNKNOWN;
        case TRUE: return TRUE;
            break;
        }
    case UNKNOWN:
        switch (tv) {
        case FALSE: return FALSE;
        case UNKNOWN: return UNKNOWN;
        case TRUE: return UNKNOWN;
            break;
        }
    default:
        return FALSE;
    }
}
TritValue Trit::operator&(const Trit& t) const {
    return t & this->getTritValue();
}


TritValue Trit::operator|(TritValue tv) const {
    switch (this->getTritValue()) {
    case FALSE:
        switch (tv) {
        case FALSE: return FALSE;
        case UNKNOWN: return UNKNOWN;
        case TRUE: return TRUE;
            break;
        }
    case UNKNOWN:
        switch (tv) {
        case FALSE: return UNKNOWN;
        case UNKNOWN: return UNKNOWN;
        case TRUE: return TRUE;
            break;
        }
    default:
        return TRUE;
    }
}
TritValue Trit::operator|(const Trit& t) const {
    return t | this->getTritValue();
}


Trit& Trit::operator=(TritValue tv) {
    set->smartSetTrit(index, tv);
    return *this;
}
Trit& Trit::operator=(const Trit& t) {
    *this = t.getTritValue();
    return *this;
}


Trit& Trit::operator&=(TritValue tv) {
    *this = (*this & tv);
    return *this;
}
Trit& Trit::operator&=(const Trit& t) {
    *this &= t.getTritValue();
    return *this;
}

Trit& Trit::operator|=(TritValue tv) {
    *this = (*this | tv);
    return *this;
}
Trit& Trit::operator|=(const Trit& t) {
    *this |= t.getTritValue();
    return *this;
}


bool Trit::operator==(TritValue tv) const {
    return this->getTritValue() == tv;
}
bool Trit::operator==(const Trit& t) const {
    return this->getTritValue() == t.getTritValue();
}

std::ostream& operator<<(std::ostream& out, const Trit& t) {
    switch (t.getTritValue()) {
    case TRUE:
        out << "TRUE";
        break;
    case FALSE:
        out << "FALSE";
        break;
    default:
        out << "UNKNOWN";
        break;
    }
    return out;
}
