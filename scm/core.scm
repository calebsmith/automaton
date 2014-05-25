(define (cell-based-generator x y state-num neighbor-func rule-func)
    (rule-func (board-get-cell x y) (neighbor-func x y state-num)))
