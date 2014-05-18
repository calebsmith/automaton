(define-module (gameoflife)
    #:export (get-next-cell))


(define (board-neighbor->cell c n)
    (if (= c 0)
        (if (= n 3)
            1
            0)
        (if (or (= n 2) (= n 3))
            1
            0)))

(define (get-next-cell x y)
    (board-set-cell x y (board-neighbor->cell (board-get-cell x y) (board-neighbor-count x y 1))))


;(define (generate width height)
;    (define new-board (make-vector (* width height)))
;    (map (lambda (y)
;       (map (lambda (x)
;            (let ((num-neighbors (board-neighbor-count x y 1))
;                  (current (board-get-cell x y)))
;                (board-set-cell x y (board-neighbor->cell current num-neighbors))))
;            (iota width)))
;       (iota height))
;    new-board)
