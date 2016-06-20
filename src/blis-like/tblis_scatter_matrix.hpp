#ifndef _TBLIS_SCATTER_MATRIX_HPP_
#define _TBLIS_SCATTER_MATRIX_HPP_

#include "tblis.hpp"

namespace tblis
{
namespace blis_like
{

enum scatter_t
{
    SCATTER_NONE = 0x0,
    SCATTER_ROWS = 0x1,
    SCATTER_COLS = 0x2,
    SCATTER_BOTH = SCATTER_ROWS|SCATTER_COLS
};

template <typename T>
class ScatterMatrix
{
    public:
        typedef T type;
        typedef typename real_type<T>::type real_type;

    private:
        trans_op_t _conjtrans;
        type* _ptr;
        dim_t _m;
        dim_t _n;
        inc_t _rs;
        inc_t _cs;
        inc_t* _rscat;
        inc_t* _cscat;

    protected:
        void create()
        {
            _conjtrans = BLIS_NO_TRANSPOSE;
            _ptr = NULL;
            _m = 0;
            _n = 0;
            _rs = 0;
            _cs = 0;
            _rscat = NULL;
            _cscat = NULL;
        }

        void create(const ScatterMatrix& other)
        {
            _conjtrans = other._conjtrans;
            _ptr = other._ptr;
            _m = other._m;
            _n = other._n;
            _rs = other._rs;
            _cs = other._cs;
            _rscat = other._rscat;
            _cscat = other._cscat;
        }

        void create(dim_t m, dim_t n, type* p, inc_t rs, inc_t cs)
        {
            _conjtrans = BLIS_NO_TRANSPOSE;
            _ptr = p;
            _m = m;
            _n = n;
            _rs = rs;
            _cs = cs;
            _rscat = NULL;
            _cscat = NULL;
        }

        void create(dim_t m, dim_t n, type* p, inc_t rs, inc_t* cscat)
        {
            _conjtrans = BLIS_NO_TRANSPOSE;
            _ptr = p;
            _m = m;
            _n = n;
            _rs = rs;
            _cs = 0;
            _rscat = NULL;
            _cscat = cscat;
        }

        void create(dim_t m, dim_t n, type* p, inc_t* rscat, inc_t cs)
        {
            _conjtrans = BLIS_NO_TRANSPOSE;
            _ptr = p;
            _m = m;
            _n = n;
            _rs = 0;
            _cs = cs;
            _rscat = rscat;
            _cscat = NULL;
        }

        void create(dim_t m, dim_t n, type* p, inc_t* rscat, inc_t* cscat)
        {
            _conjtrans = BLIS_NO_TRANSPOSE;
            _ptr = p;
            _m = m;
            _n = n;
            _rs = 0;
            _cs = 0;
            _rscat = rscat;
            _cscat = cscat;
        }

    public:
        ScatterMatrix()
        {
            create();
        }

        ScatterMatrix(const ScatterMatrix& other)
        {
            create(other);
        }

        explicit ScatterMatrix(type* p)
        {
            create(1, 1, p, 1, 1);
        }

        ScatterMatrix(dim_t m, dim_t n, type* p)
        {
            create(m, n, p, 1, m);
        }

        ScatterMatrix(dim_t m, dim_t n, type* p, inc_t rs, inc_t cs)
        {
            create(m, n, p, rs, cs);
        }

        ScatterMatrix(dim_t m, dim_t n, type* p, inc_t rs, inc_t* cscat)
        {
            create(m, n, p, rs, cscat);
        }

        ScatterMatrix(dim_t m, dim_t n, type* p, inc_t* rscat, inc_t cs)
        {
            create(m, n, p, rscat, cs);
        }

        ScatterMatrix(dim_t m, dim_t n, type* p, inc_t* rscat, inc_t* cscat)
        {
            create(m, n, p, rscat, cscat);
        }

        ScatterMatrix& operator=(const ScatterMatrix& other)
        {
            reset(other);
            return *this;
        }

        void reset()
        {
            create();
        }

        void reset(const ScatterMatrix& other)
        {
            if (&other == this) return;
            create(other);
        }

        void reset(type* p)
        {
            create(1, 1, p, 1, 1);
        }

        void reset(dim_t m, dim_t n, type* p)
        {
            create(m, n, p, 1, m);
        }

        void reset(dim_t m, dim_t n, type* p, inc_t rs, inc_t cs)
        {
            create(m, n, p, rs, cs);
        }

        void reset(dim_t m, dim_t n, type* p, inc_t rs, inc_t* cscat)
        {
            create(m, n, p, rs, cscat);
        }

        void reset(dim_t m, dim_t n, type* p, inc_t* rscat, inc_t cs)
        {
            create(m, n, p, rscat, cs);
        }

        void reset(dim_t m, dim_t n, type* p, inc_t* rscat, inc_t* cscat)
        {
            create(m, n, p, rscat, cscat);
        }

        void reset(dim_t m, dim_t n, type* p, inc_t rs, inc_t cs, inc_t* rscat, inc_t* cscat)
        {
            if (rs == 0 && cs == 0)
                create(m, n, p, rscat, cscat);
            else if (rs == 0)
                create(m, n, p, rscat, cs);
            else if (cs == 0)
                create(m, n, p, rs, cscat);
            else
                create(m, n, p, rs, cs);
        }

        bool is_view() const
        {
            return true;
        }

        bool is_transposed() const
        {
            return _conjtrans.transpose();
        }

        bool transpose()
        {
            bool old = is_transposed();
            bli_obj_toggle_trans(_conjtrans);
            return old;
        }

        bool transpose(bool trans)
        {
            bool old = is_transposed();
            bli_obj_set_onlytrans(trans ? BLIS_TRANSPOSE : BLIS_NO_TRANSPOSE, _conjtrans);
            return old;
        }

        trans_t transpose(trans_t trans)
        {
            trans_t old = bli_obj_onlytrans_status(_conjtrans);
            bli_obj_set_onlytrans(trans, _conjtrans);
            return old;
        }

        bool is_conjugated() const
        {
            return _conjtrans.conjugate();
        }

        bool conjugate()
        {
            bool old = is_conjugated();
            bli_obj_toggle_conj(_conjtrans);
            return old;
        }

        bool conjugate(bool conj)
        {
            bool old = is_conjugated();
            bli_obj_set_conj(conj ? BLIS_CONJUGATE : BLIS_NO_CONJUGATE, _conjtrans);
            return old;
        }

        conj_t conjugate(conj_t conj)
        {
            conj_t old = bli_obj_conj_status(_conjtrans);
            bli_obj_set_conj(conj, _conjtrans);
            return old;
        }

        trans_op_t conjtrans()
        {
            return bli_obj_conjtrans_status(_conjtrans);
        }

        trans_op_t conjtrans(trans_op_t conjtrans)
        {
            trans_op_t old = this->conjtrans();
            bli_obj_set_conjtrans(conjtrans, _conjtrans);
            return old;
        }

        dim_t length() const
        {
            return _m;
        }

        dim_t length(dim_t m)
        {
            dim_t old = _m;
            _m = m;
            return old;
        }

        dim_t width() const
        {
            return _n;
        }

        dim_t width(dim_t n)
        {
            dim_t old = _n;
            _n = n;
            return old;
        }

        inc_t row_stride() const
        {
            return _rs;
        }

        inc_t row_stride(inc_t rs)
        {
            inc_t old = _rs;
            _rs = rs;
            if (_rs) _rscat = NULL;
            return old;
        }

        inc_t col_stride() const
        {
            return _cs;
        }

        inc_t col_stride(inc_t cs)
        {
            inc_t old = _cs;
            _cs = cs;
            if (_cs) _cscat = NULL;
            return old;
        }

        inc_t* row_scatter()
        {
            return _rscat;
        }

        const inc_t* row_scatter() const
        {
            return _rscat;
        }

        void row_scatter(inc_t* rscat)
        {
            _rscat = rscat;
            if (!_rscat) _rs = 0;
        }

        inc_t* col_scatter()
        {
            return _cscat;
        }

        const inc_t* col_scatter() const
        {
            return _cscat;
        }

        void col_scatter(inc_t* cscat)
        {
            _cscat = cscat;
            if (!_cscat) _cs = 0;
        }

        void shift_down(dim_t m)
        {
            if (_rs == 0)
            {
                _rscat += m;
            }
            else
            {
                _ptr += m*_rs;
            }
        }

        void shift_up(dim_t m)
        {
            shift_down(-m);
        }

        void shift_right(dim_t n)
        {
            if (_cs == 0)
            {
                _cscat += n;
            }
            else
            {
                _ptr += n*_cs;
            }
        }

        void shift_left(dim_t n)
        {
            shift_right(-n);
        }

        void shift_down()
        {
            shift_down(length());
        }

        void shift_up()
        {
            shift_up(length());
        }

        void shift_right()
        {
            shift_right(width());
        }

        void shift_left()
        {
            shift_left(width());
        }

        type* data()
        {
            return _ptr;
        }

        const type* data() const
        {
            return _ptr;
        }

        ScatterMatrix operator^(trans_op_t trans)
        {
            ScatterMatrix view(*this);

            if (trans.transpose()) view.transpose();
            if (trans.conjugate()) view.conjugate();

            return view;
        }

        friend void ViewNoTranspose(ScatterMatrix& A, ScatterMatrix& V)
        {
            blis::detail::AssertNotSelfView(A, V);

            if (A.is_conjugated()) V.conjugate();

            if (A.is_transposed())
            {
                V.reset(A.width(), A.length(), A.data(),
                        A.col_stride(), A.row_stride(),
                        A.col_scatter(), A.row_scatter());
            }
            else
            {
                V.reset(A.length(), A.width(), A.data(),
                        A.row_stride(), A.col_stride(),
                        A.row_scatter(), A.col_scatter());
            }
        }

        friend void ViewNoTranspose(ScatterMatrix&& A, ScatterMatrix& V)
        {
            ViewNoTranspose(A, V);
        }
};

}
}

#endif