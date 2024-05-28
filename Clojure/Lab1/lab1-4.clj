(ns lab1-4
  (:require [clojure.string :as str]))

(defn add-letter [string alphabet]
  (let
   [new-alphabet (filter
                  (fn [x] (not (str/ends-with? string x)))
                  alphabet)]
    (map (fn [x] (.concat string x)) new-alphabet)))

(defn step [strings alphabet]
  (reduce
   (fn [acc x] (concat acc (add-letter x alphabet)))
   '()
   strings))

(defn create-words [alphabet n]
  (cond
    (or (<= n 0) (<= (count alphabet) 0)) '()
    (and (= (count alphabet) 1) (> n 1)) '()
    :else (nth (iterate #(step %1 alphabet) alphabet) (dec n))))

(println (create-words '("a" "b" "c") 4))