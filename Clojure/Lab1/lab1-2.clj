(ns lab1-2
  (:require [clojure.string :as str]))

(defn add-letter
  ([string alphabet] (add-letter string alphabet '()))
  ([string alphabet acc]
   (if (> (count alphabet) 0)
     (if (str/ends-with? string (first alphabet))
       (recur string (rest alphabet) acc)
       (recur string (rest alphabet)
              (concat acc (list (.concat string (first alphabet))))))
     acc)))

(defn step
  ([strings alphabet] (step strings alphabet '()))
  ([strings alphabet acc]
   (if (> (count strings) 0)
     (recur (rest strings) alphabet
            (concat acc (add-letter (first strings) alphabet)))
     acc)))

(defn create-words
  ([alphabet n]
   (cond
     (or (<= n 0) (<= (count alphabet) 0)) '()
     (and (= (count alphabet) 1) (> n 1)) '()
     (= n 1) alphabet
     :else (create-words alphabet n alphabet)))
  ([alphabet n acc]
   (if (> n 0)
     (recur alphabet (dec n) (step acc alphabet))
     acc)))

(println (create-words '("a" "b" "c") 3))