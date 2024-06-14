#include <iostream>
#include "TritSet.h"
#include "Trit.h"

int main()
{
    TritSet setA(3);
    setA[0] = FALSE;
    setA[1] = TRUE;
    setA[2] = UNKNOWN;

    TritSet setB(3);
    setB[0] = TRUE;
    setB[1] = TRUE;
    setB[2] = TRUE;

    std::cout << (setA & setB)[0] << std::endl;
    std::cout << (setA & setB)[1] << std::endl;
    std::cout << (setA & setB)[2] << std::endl;
}
