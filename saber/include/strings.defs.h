#ifndef STRINGS_DEFS_H
#define STRINGS_DEFS_H

#define C8_TEMPLATE_ARG_PREFIX "Template argument"
#define C8_TYPENAME_T_PREFIX "T"
#define C8_TYPENAME_ITERATOR_PREFIX "Iterator"

#define C8_STAT__TEMPLATE_ARG__T__COPY_CONSTRUCT_ERROR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_T_PREFIX " cannot be copy-constructed"

#define C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_T_PREFIX " cannot be copy-assigned"

#define C8_STAT__TEMPLATE_ARG__T__MOVE_ASSIGN_ERROR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_T_PREFIX " cannot be move-assigned"

#define C8_STAT__TEMPLATE_ARG__T__DESTROY_ERROR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_T_PREFIX " cannot be destroyed"

#define C8_STAT__TEMPLATE_ARG__ITER__NOT_ITERATOR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_ITERATOR_PREFIX " is not an iterator type"

#define C8_STAT__TEMPLATE_ARG__ITER__NOT_INPUT_ITERATOR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_ITERATOR_PREFIX " is not an input iterator type"

#define C8_STAT__TEMPLATE_ARG__ITER__NOT_OUTPUT_ITERATOR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_ITERATOR_PREFIX " is not an output iterator type"

#define C8_STAT__TEMPLATE_ARG__ITER__NOT_FORWARD_ITERATOR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_ITERATOR_PREFIX " is not a forward iterator type"

#define C8_STAT__TEMPLATE_ARG__ITER__NOT_BIDIRECTIONAL_ITERATOR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_ITERATOR_PREFIX " is not a bidirectional iterator type"

#define C8_STAT__TEMPLATE_ARG__ITER__NOT_RANDOM_ACCESS_ITERATOR\
    C8_TEMPLATE_ARG_PREFIX " " C8_TYPENAME_ITERATOR_PREFIX " is not a random-access iterator type"


#define C8_DYN__ITER__UNINITIALIZED_ITERATOR\
    "This iterator did not get initialized"

#define C8_DYN__ITER__OLD_ITERATOR\
    "A manipulating operation has been performed on container object"

#define C8_DYN__ITER__SUBSCRIPT_OVERFLOW\
    "Accessing an invalid subscript"

#define C8_DYN__ITER__ITERATOR_OVERFLOW\
    "This iterator is now pointing to an invalid position, for example, end()"

#define C8_DYN__ITER__DELETED_CONTAINER\
    "Container has been destroyed but left this iterator alone"

#define C8_DYN__ITER__UNKNOWN_REGION_ITERATOR\
    "This iterator does not come from this container"

#define C8_DYN__CONT__SPLICE_BETWEEN_UNEQUAL_ALLOC_CONTAINERS\
    "Splicing between two containers which have unequal allocators"

#define C8_DYN__CONT__CONTAINER_COPY\
    "Copying container"

#endif // STRINGS_DEFS_H
