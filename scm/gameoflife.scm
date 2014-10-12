(define-module (gameoflife)
    #:export (generate-next))

(use-modules (ice-9 poe))


(define (generate-cell current neighbors)
    (if (= current 0)
        (if (= neighbors 3)
            1
            0)
        (if (or (= neighbors 2) (= neighbors 3))
            1
            0)))

; Memoize pure functions and primitive procs that return a constant value
(define M_generate-cell (pure-funcq generate-cell))
(define M_width-range (pure-funcq (lambda () (iota (board-get-width)))))
(define M_height-range (pure-funcq (lambda () (iota (board-get-height)))))


(define (cell-based-generator x y state-num neighbor-func rule-func)
    (board-set-cell x y (rule-func
        (board-get-cell x y)
        (neighbor-func x y state-num))))


(define (generate-next)
    (map (lambda (y)
        (map (lambda (x)
            (cell-based-generator x y 1 board-moore-neighbors M_generate-cell))
            (M_width-range)))
        (M_height-range)))
