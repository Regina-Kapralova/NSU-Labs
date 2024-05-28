(def delta 0.01)

(defn trapezium [f x-1 x-2]
  (* delta (/ (+ (f x-1) (f x-2)) 2)))

(defn lazy-integral [f]
  (let [integral (fn [a] (trapezium f a (+ a delta)))
        steps (iterate (fn [x] (+ x delta)) 0)]
    (reductions
     + 0
     (map integral steps))))

(defn f [x] (* 3 x))

(defn -main []
  (let [integral-l (lazy-integral f)
        N (quot 10 delta)]
    (time (nth integral-l N))
    (time (nth integral-l N))
    (time (nth integral-l N))
    (time (nth integral-l N))
    (time (nth integral-l N))
    (time (nth integral-l N))
    (time (nth integral-l (+ N 1)))))
(-main)