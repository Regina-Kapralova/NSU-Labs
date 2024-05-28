(ns lab2-1)

(def delta 1/20)

(defn trapezium [f x-1 x-2]
  (* delta (/ (+ (f x-1) (f x-2)) 2)))

(defn rec-integral [f x-1 x-2 rec-fn]
  (if (= x-1 x-2)
    0
    (+
     (rec-fn f x-1 (- x-2 delta) rec-fn)
     (trapezium f (- x-2 delta) x-2))))

(defn integral [f x]
  (rec-integral f 0 x rec-integral))

(def rec-integral-mem (memoize rec-integral))

(defn integral-mem [f x]
  (rec-integral-mem f 0 x rec-integral-mem))

(defn f [x] (* 5 x))
(defn f-2 [x] (* -5 x))

(defn -main []
  (time (integral f 10))
  (time (integral f 10))
  (time (integral f 11))
  (time (integral-mem f-2 10))
  (time (integral-mem f-2 10))
  (time (integral-mem f-2 9))
  (time (integral-mem f-2 11)))

(-main)

;; Example results:
;; 99.91 - разгод java-машины
;; 6.7
;; 6.9
;; 15.67
;; 0.07
;; 0.03
;; 1.04