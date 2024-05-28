(ns tests4
  (:use lab4)
  (:require [clojure.test :as test]))

(test/deftest defpackage-test
  (test/testing "DNF"
    ;; ((-x) -> y) = (x | y)
    (test/is (= (dnf (logic-impl (logic-not (variable :x)) (variable :y)))
                (logic-or (variable :x) (variable :y))))))

(test/run-tests 'tests4)