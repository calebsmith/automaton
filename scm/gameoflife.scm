(define-module (gameoflife)
    #:export (generate-next))


(define (generate-cell current neighbors)
    (if (= current 0)
        (if (= neighbors 3)
            1
            0)
        (if (or (= neighbors 2) (= neighbors 3))
            1
            0)))


(define (cell-based-generator x y state-num neighbor-func rule-func)
    (board-set-cell x y (rule-func
        (board-get-cell x y)
        (neighbor-func x y state-num))))


(define (generate-next)
    (map (lambda (y)
        (map (lambda (x)
            (cell-based-generator x y 1 board-moore-neighbors generate-cell))
            (iota (board-get-width))))
        (iota (board-get-height))))
