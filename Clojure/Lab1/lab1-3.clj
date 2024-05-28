(defn my-filter [pred coll]
  (reduce
   (fn [acc x]
     (if (pred x)
       (concat acc (list x))
       acc))
   '()
   coll))

(defn my-map [f coll]
  (reduce
   (fn [acc x]
     (concat acc (list (f x))))
   '()
   coll))

(defn my-even? [x]
  (= 0 (mod x 2)))

(defn square [x]
  (* x x))

(println (my-filter my-even? '(0 1 2 3 4 5 6)))
(println (my-map square '(0 1 2 3 4 5 6)))