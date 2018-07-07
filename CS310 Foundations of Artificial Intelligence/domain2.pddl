(define (domain transportation)

    (:predicates
        (truck ?t) 
        (capacity ?c)
        (location ?l) 
        (package ?p)

        (road ?l1 ?l2)
        (truck-capacity ?t ?c)
        (truck-at ?t ?l)
        (package-at ?p ?l)
        (capacity-empty ?c)
        (carry-package ?c ?p)
    )

    (:action move
        :parameters (?truck ?from ?to)
        :precondition (and 
            (truck ?truck) (location ?from) (location ?to) 
            (road ?from ?to) (truck-at ?truck ?from)
        )
        :effect (and (truck-at ?truck ?to) (not (truck-at ?truck ?from)))
    )

    (:action pick
        :parameters (?truck ?capacity ?package ?location)
        :precondition (and 
            (truck ?truck) (capacity ?capacity) (package ?package) (location ?location)
            (truck-at ?truck ?location) 
            (truck-capacity ?truck ?capacity)
            (capacity-empty ?capacity) 
            (package-at ?package ?location) 
        )
        :effect (and (carry-package ?capacity ?package) 
            (not (package-at ?package ?location))
            (not (capacity-empty ?capacity))
        )
    )

    (:action drop
        :parameters (?truck ?capacity ?package ?location)
        :precondition (and 
            (truck ?truck) (capacity ?capacity) (package ?package) (location ?location)
            (truck-at ?truck ?location) 
            (truck-capacity ?truck ?capacity)
            (carry-package ?capacity ?package)

        )
        :effect (and 
            (package-at ?package ?location) 
            (capacity-empty ?capacity)
            (not (carry-package ?capacity ?package))
        )
    )
)