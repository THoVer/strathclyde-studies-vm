(define (problem transportation2) 
    (:domain transportation)
    (:objects L1 L2 L3 T P1 P2
    )

    (:init
        (location L1)
        (location L2)
        (location L3)
        (road L1 L2)
        (road L2 L1)
        (road L1 L3)
        (road L3 L1)
        (package P1)
        (package P2)
        (at-truck L1)
        (at P1 L2)
        (at P2 L2)
    )

    (:goal (and
            (at P1 L3)
            (at P2 L3)
        )
    )
)
