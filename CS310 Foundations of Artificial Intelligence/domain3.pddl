(define (domain transportation)

    (:predicates
        (truck ?t) 
        (capacity ?c)
        (location ?l) 
        (package ?p)
        (fuel ?f)

        (road ?l1 ?l2)
        (truck-capacity ?t ?c)
        (truck-at ?t ?l)
        (package-at ?p ?l)
        (capacity-empty ?c)
        (carry-package ?c ?p)
        (fuel-at ?f ?l)
        (truck-fuel ?t ?f)
    )

    (:action move
        :parameters (?truck ?from ?to ?fuel)
        :precondition (and 
            (truck ?truck) (location ?from) (location ?to) (fuel ?fuel)
            (road ?from ?to) (truck-at ?truck ?from) (truck-fuel ?truck ?fuel)
        )
        :effect (and 
            (truck-at ?truck ?to) 
            (not (truck-at ?truck ?from))
            (not (truck-fuel ?truck ?fuel))
        )
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

    (:action refill
        :parameters (?truck ?location ?fuel)
        :precondition (and
            (truck ?truck) (location ?location) (fuel ?fuel)
            (truck-at ?truck ?location)
            (fuel-at ?fuel ?location)
        )    
        :effect (and
            (truck-fuel ?truck ?fuel)
            (not (fuel-at ?fuel ?location))
        )
    )
)