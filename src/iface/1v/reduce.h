#ifndef _TBLIS_IFACE_1V_REDUCE_H_
#define _TBLIS_IFACE_1V_REDUCE_H_

#include "../../util/thread.h"
#include "../../util/basic_types.h"

#ifdef __cplusplus

#include <utility>

namespace tblis
{

extern "C"
{

#endif

void tblis_vector_reduce(const tblis_comm* comm, const tblis_config* cfg,
                         reduce_t op, const tblis_vector* A,
                         tblis_scalar* result, len_type* idx);

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) && !defined(TBLIS_DONT_USE_CXX11)

template <typename T>
void reduce(reduce_t op, const_row_view<T> A, T& result, len_type& idx)
{
    tblis_vector A_s(A);
    tblis_scalar result_s(result);
    tblis_vector_reduce(nullptr, nullptr, op, &A_s, &result_s, &idx);
    result = result_s.get<T>();
}

template <typename T>
void reduce(single_t, reduce_t op, const_row_view<T> A,
            T& result, len_type& idx)
{
    tblis_vector A_s(A);
    tblis_scalar result_s(result);
    tblis_vector_reduce(tblis_single, nullptr, op, &A_s, &result_s, &idx);
    result = result_s.get<T>();
}

template <typename T>
void reduce(const communicator& comm, reduce_t op, const_row_view<T> A,
            T& result, len_type& idx)
{
    tblis_vector A_s(A);
    tblis_scalar result_s(result);
    tblis_vector_reduce(comm, nullptr, op, &A_s, &result_s, &idx);
    result = result_s.get<T>();
}

template <typename T>
std::pair<T,len_type> reduce(reduce_t op, const_row_view<T> A)
{
    std::pair<T,len_type> result;
    reduce(op, A, result.first, result.second);
    return result;
}

template <typename T>
std::pair<T,len_type> reduce(single_t, reduce_t op, const_row_view<T> A)
{
    std::pair<T,len_type> result;
    reduce(single, op, A, result.first, result.second);
    return result;
}

template <typename T>
std::pair<T,len_type> reduce(const communicator& comm, reduce_t op,
                             const_row_view<T> A)
{
    std::pair<T,len_type> result;
    reduce(comm, op, A, result.first, result.second);
    return result;
}

#endif

#ifdef __cplusplus
}
#endif

#endif
