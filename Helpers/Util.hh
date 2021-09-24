#pragma once

//	Copy/move initialization/assignment
#define DEFAULT_COPY_MOVE_INIT_ASSIGN(x) \
    x(const x&) = default; \
    x(x&&)      = default; \
    x& operator=(const x&) = default; \
    x& operator=(x&&) = default;
