#include "pch.h"
#include "../BitArray/Trit.cpp"
#include "../BitArray/TritSet.cpp"

TEST(TritSet, length) {
    std::cout << std::endl << "Start testing length : " << std::endl;
    TritSet set(1000);
    set[60] = TRUE;
    ASSERT_TRUE(set.length() == 60 + 1);

    set[60] = UNKNOWN;
    ASSERT_TRUE(set.length() == 0 + 1);

    TritSet set1(100000);
    set1[5000] = TRUE;
    ASSERT_TRUE(set1.length() == 5000 + 1);
}

TEST(TritSet, capacity) {
    std::cout << std::endl << "Start testing capacity : " << std::endl;
    TritSet set(1);
    size_t trits_count = set.capacity();
    ASSERT_TRUE(trits_count == 16);

    set[5000] = TRUE;
    trits_count = set.capacity();
    ASSERT_TRUE(trits_count == 16 * (5000 * 2 / 8 / uint_sizeof + (((5000 * 2 / 8) % uint_sizeof) ? 1 : 0)));

    set[6000] = UNKNOWN;
    size_t new_trits_count = set.capacity();
    ASSERT_TRUE(trits_count == new_trits_count);

    TritSet set1(100000);
    trits_count = set1.capacity();
    ASSERT_TRUE(trits_count == 16 * (100000 * 2 / 8 / uint_sizeof + (((100000 * 2 / 8) % uint_sizeof) ? 1 : 0)));
}

TEST(TritSet, shrink) {
    std::cout << std::endl << "Start testing shrink : " << std::endl;
    TritSet set(1000);
    size_t trits_count = set.capacity();
    set[50] = TRUE;
    ASSERT_TRUE(trits_count == set.capacity());
    set.shrink();
    ASSERT_TRUE(trits_count > set.capacity());
    ASSERT_TRUE(set.capacity() == 16 * (50 * 2 / 8 / uint_sizeof + (((50 * 2 / 8) % uint_sizeof) ? 1 : 0) + 1));
}

TEST(TritSet, trim) {
    std::cout << std::endl << "Start testing trim : " << std::endl;
    TritSet set(1000);
    set[50] = set[51] = TRUE;
    set.trim(51);
    ASSERT_TRUE(set[50] == TRUE);
    ASSERT_TRUE(set[51] == UNKNOWN);
}

TEST(TritSet, cardinality) {
    std::cout << std::endl << "Start testing cardinality : " << std::endl;
    TritSet set(1000);
    set[1] = set[2] = set[3] = set[4] = TRUE;
    set[5] = set[6] = set[7] = set[8] = set[9] = FALSE;
    ASSERT_TRUE(set.cardinality(TRUE) == 4);
    ASSERT_TRUE(set.cardinality(FALSE) == 5);
    ASSERT_EQ(set.cardinality(UNKNOWN), 998);
}


TEST(TritSet, operator_and) {
    std::cout << std::endl << "Start testing operator and : " << std::endl;
    TritSet set(100);
    set[0] = FALSE;
    set[1] = UNKNOWN;
    set[2] = TRUE;
    ASSERT_TRUE((set[0] & TRUE) == FALSE);
    ASSERT_TRUE((set[0] & set[0]) == FALSE);
    ASSERT_TRUE((set[0] & set[1]) == FALSE);
    ASSERT_TRUE((set[1] & set[0]) == FALSE);
    ASSERT_TRUE((set[1] & set[2]) == UNKNOWN);
    ASSERT_TRUE((set[2] & set[1]) == UNKNOWN);
    ASSERT_TRUE((set[1] & set[1]) == UNKNOWN);
    ASSERT_TRUE((set[2] & set[2]) == TRUE);
    ASSERT_TRUE((set[2] & set[0]) == FALSE);
    ASSERT_TRUE((set[0] & set[2]) == FALSE);

    TritSet setA(1000);
    setA[5] = FALSE;
    setA[6] = TRUE;
    setA[7] = UNKNOWN;
    TritSet setB(2000);
    setB[5] = TRUE;
    setB[6] = TRUE;
    setB[7] = TRUE;
    setB[1500] = TRUE;
    setB[1501] = FALSE;
    setB[1502] = UNKNOWN;
    setA &= setB;
    ASSERT_TRUE(setA.capacity() == setB.capacity());
    ASSERT_TRUE(setA[5] == FALSE);
    ASSERT_TRUE(setA[6] == TRUE);
    ASSERT_TRUE(setA[7] == UNKNOWN);
    ASSERT_TRUE(setA[1500] == TRUE);
    ASSERT_TRUE(setA[1501] == FALSE);
    ASSERT_TRUE(setA[1502] == UNKNOWN);

    ASSERT_FALSE(setA[3000] == TRUE);
}

TEST(TritSet, operator_or) {
    std::cout << std::endl << "Start testing operator or : " << std::endl;
    TritSet set(100);
    set[0] = FALSE;
    set[1] = UNKNOWN;
    set[2] = TRUE;
    ASSERT_TRUE((set[0] | set[0]) == FALSE);
    ASSERT_TRUE((set[0] | set[1]) == UNKNOWN);
    ASSERT_TRUE((set[1] | set[0]) == UNKNOWN);
    ASSERT_TRUE((set[1] | set[2]) == TRUE);
    ASSERT_TRUE((set[2] | set[1]) == TRUE);
    ASSERT_TRUE((set[1] | set[1]) == UNKNOWN);
    ASSERT_TRUE((set[2] | set[2]) == TRUE);
    ASSERT_TRUE((set[2] | set[0]) == TRUE);
    ASSERT_TRUE((set[0] | set[2]) == TRUE);

    TritSet setA(1000);
    setA[5] = FALSE;
    setA[6] = TRUE;
    setA[7] = UNKNOWN;
    TritSet setB(2000);
    setB[5] = TRUE;
    setB[6] = TRUE;
    setB[7] = TRUE;
    setB[1500] = TRUE;
    setB[1501] = FALSE;
    setB[1502] = UNKNOWN;
    TritSet setC(2000);
    setC = setA | setB;
    ASSERT_TRUE(setC.capacity() == setB.capacity());
    ASSERT_TRUE(setC[5] == TRUE);
    ASSERT_TRUE(setC[6] == TRUE);
    ASSERT_TRUE(setC[7] == TRUE);
    ASSERT_TRUE(setC[1500] == TRUE);
    ASSERT_TRUE(setC[1501] == FALSE);
    ASSERT_TRUE(setC[1502] == UNKNOWN);
}

TEST(TritSet, operator_ne) {
    std::cout << std::endl << "Start testing operator ne : " << std::endl;
    TritSet set(1000);
    set[0] = TRUE;
    set[1] = UNKNOWN;
    set[2] = FALSE;
    TritSet neSet = !set;
    ASSERT_TRUE(!TRUE == FALSE);
    ASSERT_TRUE(neSet[0] == FALSE);
    ASSERT_TRUE(neSet[1] == UNKNOWN);
    ASSERT_TRUE(neSet[2] == TRUE);
}


