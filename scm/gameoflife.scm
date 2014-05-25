(define-module (gameoflife)
    #:export (get-next-cell))


(define (generate-cell current neighbors)
    (if (= current 0)
        (if (= neighbors 3)
            1
            0)
        (if (or (= neighbors 2) (= neighbors 3))
            1
            0)))


(define (cell-based-generator x y state-num neighbor-func rule-func)
    (rule-func (board-get-cell x y) (neighbor-func x y state-num)))


(define (get-next-cell x y)
    (cell-based-generator x y 1 board-moore-neighbors generate-cell))
