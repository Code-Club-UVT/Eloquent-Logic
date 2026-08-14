target_sources(liblogicTruthTable PRIVATE and_op.cpp iff_op.cpp implies_op.cpp not_op.cpp or_op.cpp val_op.cpp)
target_sources(liblogicTruthTable
        PUBLIC
        FILE_SET operations_headers
        TYPE HEADERS
        FILES
        and_op.h
        executor_base.h
        iff_op.h
        implies_op.h
        not_op.h
        operations.h
        or_op.h
        val_op.h
)