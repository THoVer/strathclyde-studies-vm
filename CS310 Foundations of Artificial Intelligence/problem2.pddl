(define (problem transportation2) 
    (:domain transportation)
    (:objects T1 T2 C1 C2 C3 L1 L2 L3 P1 P2
    )

    (:init
        ; Objects
        (truck T1)
        (truck T2)
        (capacity C1)
        (capacity C2)
        (capacity C3)
        (location L1)
        (location L2)
        (location L3)
        (package P1)
        (package P2)
        ; Constants
        (road L1 L2)
        (road L2 L1)
        (road L1 L3)
        (road L3 L1)
        ; Variables
        (capacity-empty C1)
        (capacity-empty C2)
        (capacity-empty C3)
        (truck-capacity T1 C1)
        (truck-capacity T2 C2)
        (truck-capacity T2 C3)
        (truck-at T1 L1)
        (truck-at T2 L1)
        (package-at P1 L2)
        (package-at P2 L2)
    )

    (:goal (and
            (package-at P1 L3)
            (package-at P2 L3)
        )
    )
)
