;; (def threads (.availableProcessors (Runtime/getRuntime)))

(defn heavy-pred? [x] (Thread/sleep 100) (= 0 (mod x 3)))

(defn my-partition [parts coll]
  (when (not-empty coll)
    (let [n (if (= (mod (count coll) parts) 0)
              (quot (count coll) parts)
              (+ 1 (quot (count coll) parts)))]
      (cons
       (take n coll)
       (my-partition (dec parts) (drop n coll))))))

(defn pfilter [pred coll threads]
  (->> coll
       (my-partition threads)
       (map #(future (doall (filter pred %))))
       (doall)
       (mapcat deref)))

(defn -main []
  (time (doall (filter heavy-pred? (range 10))))
  (time (doall (pfilter heavy-pred? (range 10) 1)))
  (time (doall (pfilter heavy-pred? (range 10) 2)))
  (time (doall (pfilter heavy-pred? (range 10) 4))))

(-main)