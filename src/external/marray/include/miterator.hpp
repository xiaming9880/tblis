#ifndef _MARRAY_MITERATOR_HPP_
#define _MARRAY_MITERATOR_HPP_

#include "utility.hpp"

namespace MArray
{

template <unsigned NDim, unsigned N=1>
class miterator
{
    public:
        miterator(const miterator&) = default;

        miterator(miterator&&) = default;

        template <typename Len, typename... Strides,
                  typename=typename std::enable_if<detail::is_container_of<Len, len_type>::value &&
                                                   detail::are_containers_of<stride_type, Strides...>::value &&
                                                   sizeof...(Strides) == N>::type>
        miterator(const Len& len, const Strides&... strides)
        : pos_{}, first_(true), empty_(false)
        {
            assert(len.size() == NDim);
            for (unsigned i = 0;i < NDim;i++) if (len[i] == 0) empty_ = true;
            std::copy_n(len.begin(), NDim, len_.begin());
            detail::set_strides(strides_, strides...);
        }

        miterator& operator=(const miterator&) = default;

        miterator& operator=(miterator&&) = default;

        void reset()
        {
            pos_.fill(0);
            first_ = true;
        }

        template <typename... Offsets,
                  typename=typename std::enable_if<sizeof...(Offsets) == N>::type>
        bool next(Offsets&... off)
        {
            if (empty_) return false;

            if (first_)
            {
                first_ = false;
                return true;
            }

            if (NDim == 0)
            {
                first_ = true;
                return false;
            }

            for (unsigned i = 0;i < NDim;i++)
            {
                if (pos_[i] == len_[i]-1)
                {
                    detail::dec_offsets(i, pos_, strides_, off...);
                    pos_[i] = 0;

                    if (i == NDim-1)
                    {
                        first_ = true;
                        return false;
                    }
                }
                else
                {
                    detail::inc_offsets(i, strides_, off...);
                    pos_[i]++;
                    return true;
                }
            }

            return true;
        }

        template <typename... Offsets,
                  typename=typename std::enable_if<sizeof...(Offsets) == N>::type>
        void position(stride_type pos, Offsets&... off)
        {
            for (size_t i = 0;i < NDim;i++)
            {
                pos_[i] = pos%len_[i];
                pos = pos/len_[i];
            }
            assert(pos == 0);

            position(pos_, off...);
        }

        template <typename Pos, typename... Offsets,
                  typename=typename std::enable_if<detail::is_container_of<Pos, len_type>::value &&
                                                   sizeof...(Offsets) == N>::type>
        void position(const Pos& pos, Offsets&... off)
        {
            assert(pos.size() == NDim);

            std::copy_n(pos.begin(), NDim, pos_.begin());

            for (size_t i = 0;i < NDim;i++)
            {
                assert(pos_[i] >= 0 && pos_[i] < len_[i]);
            }

            detail::move_offsets(pos_, strides_, off...);

            first_ = true;
        }

        unsigned dimension() const
        {
            return NDim;
        }

        len_type position(unsigned dim) const
        {
            return pos_[dim];
        }

        const std::array<len_type,NDim>& position() const
        {
            return pos_;
        }

        len_type length(unsigned dim) const
        {
            return len_[dim];
        }

        const std::array<len_type,NDim>& lengths() const
        {
            return len_;
        }

        stride_type stride(unsigned i, unsigned dim) const
        {
            return strides_[i][dim];
        }

        const std::array<stride_type,NDim>& strides(unsigned i) const
        {
            return strides_[i];
        }

        void swap(miterator& i2)
        {
            using std::swap;
            swap(pos_, i2.pos_);
            swap(len_, i2.len_);
            swap(strides_, i2.strides_);
            swap(first_, i2.first_);
            swap(empty_, i2.empty_);
        }

        friend void swap(miterator& i1, miterator& i2)
        {
            i1.swap(i2);
        }

    private:
        std::array<len_type,NDim> pos_;
        std::array<len_type,NDim> len_;
        std::array<std::array<stride_type,NDim>,N> strides_;
        bool first_;
        bool empty_;
};

template <typename len_type, typename stride_type, size_t NDim, typename... Strides,
          typename=typename std::enable_if<detail::are_arrays_of<stride_type, NDim, Strides...>::value>::type>
miterator<NDim, 1+sizeof...(Strides)>
make_iterator(const std::array<len_type, NDim>& len,
              const std::array<stride_type, NDim>& stride0,
              const Strides&... strides)
{
    return {len, stride0, strides...};
}

}

#endif
