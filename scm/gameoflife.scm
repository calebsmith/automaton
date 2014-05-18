(define-module (gameoflife)
    #:export (get-next-cell))


(define (board-neighbor->cell current neighbors)
    (if (= current 0)
        (if (= neighbors 3)
            1
            0)
        (if (or (= neighbors 2) (= neighbors 3))
            1
            0)))


(define (get-next-cell x y)
    (board-set-cell x y (board-neighbor->cell (board-get-cell x y) (board-neighbor-count x y 1))))
