(define-module (gameoflife)
    #:export (get-next-cell))

(define (get-next-cell board cell)
    (board-neighbor->cell cell (board-count-neighbor board)))


(define (board-count-neighbor board)
    (apply + (map (lambda (x y)
        (list-ref board (+ (* y 3) x)))
            '(0 1 2 0 2 0 1 2)
            '(0 0 0 1 1 2 2 2))))


(define (board-neighbor->cell c n)
    (if (= c 0)
        (if (= n 3)
            1
            0)
        (if (or (= n 2) (= n 3))
            1
            0)))
