target_sources(eloquent_logic
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