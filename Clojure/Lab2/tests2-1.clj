(ns tests2-1
  (:use lab2-1)
  (:require [clojure.test :as test]))

(test/deftest defpackage-test
  (test/testing "Testing defpackage"
    (test/is (= 0 (integral-mem f 0)))
    (test/is (= 0 (integral-mem f -3)))
    (test/is (= 250 (integral-mem f 10)))
    (test/is (= -10 (integral-mem f-2 2)))
    (test/is (= -250 (integral-mem f-2 10)))))

(test/run-tests 'tests2-1)