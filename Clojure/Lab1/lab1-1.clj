(ns lab1-1
  (:require [clojure.string :as str]))

(defn add-letter [string alphabet]
  (if (> (count alphabet) 0)
    (if (str/ends-with? string (first alphabet))
      (add-letter string (rest alphabet))
      (cons
       (.concat string (first alphabet))
       (add-letter string (rest alphabet))))
    '()))

(defn step [strings alphabet]
  (if (> (count strings) 0)
    (concat (add-letter (first strings) alphabet)
            (step (rest strings) alphabet))
    '()))

(defn create-words [alphabet n]
  (cond
    (or (<= n 0) (<= (count alphabet) 0)) '()
    (and (= (count alphabet) 1) (> n 1)) '()
    (= n 1) alphabet
    :else (step (create-words alphabet (dec n)) alphabet)))

(println (create-words '("a" "b" "c") 4))