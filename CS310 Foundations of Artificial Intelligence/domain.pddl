(define (domain transportation)

    (:predicates 
        (location ?l) 
        (road ?l1 ?l2)
        (package ?p)
        (at-truck ?l)
        (at ?p ?l)
        (carry ?p)
    )

    (:action move
        :parameters (?from ?to)
        :precondition (and (location ?from) (location ?to) (road ?from ?to) (at-truck ?from))
        :effect (and (at-truck ?to) (not (at-truck ?from)))
    )

    (:action pick
        :parameters (?package ?location)
        :precondition (and (package ?package) (at ?package ?location) (at-truck ?location))
        :effect (and (carry ?package) (not (at ?package ?location)))
    )

    (:action drop
        :parameters (?package ?location)
        :precondition (and (package ?package) (carry ?package) (at-truck ?location))
        :effect (and (at ?package ?location) (not (carry ?package)))
    )
)