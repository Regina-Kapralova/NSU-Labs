;; (def threads (.availableProcessors (Runtime/getRuntime)))
(def n-per-thread 4)

(defn heavy-pred? [x] (Thread/sleep 10) (= 0 (mod x 3)))

(defn my-partition [n coll]
  (when (not-empty coll)
    (lazy-seq (cons
               (take n coll)
               (my-partition n (drop n coll))))))

(defn my-lazy-pfilter [pred coll threads]
  (->> coll
       (my-partition (* threads n-per-thread))
       (map #(my-partition n-per-thread %))
       (mapcat (fn [x] (->> x
                            (map #(future (doall (filter pred %))))
                            (doall))))
       (mapcat deref)))

(defn -main []
  (my-lazy-pfilter heavy-pred? (range) 1)
  (time (nth (my-lazy-pfilter heavy-pred? (range) 1) 64))
  (time (nth (my-lazy-pfilter heavy-pred? (range) 2) 64))
  (time (nth (my-lazy-pfilter heavy-pred? (range) 4) 64)))

(-main)