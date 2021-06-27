#include "SmallGrid.h"

namespace mito {

    // typedef for real values
    using real = double;

    // typedef for scalars
    template <typename T = real>
    using scalar_t = SmallGrid<T, 1>;

    // typedef for vectors
    template <int D, typename T = real>
    using vector_t = SmallGrid<T, D>;

    // typedef for tensors
    template <int D1, int D2 = D1, typename T = real>
    using tensor_t = SmallGrid<T, D1, D2>;

    // helper function
    template <typename... Args>
    bool all(Args... args)
    {
        return (args && ...);
    }

    // helper function
    template <typename T, int... I, size_t... J>
    bool operatorEqualEqual(
        std::index_sequence<J...>, const SmallGrid<T, I...> & lhs, const SmallGrid<T, I...> & rhs)
    {
        if (all((lhs[J] == rhs[J])...))
            return true;
        return false;
    }

    template <typename T, int... I>
    inline bool operator==(const SmallGrid<T, I...> & lhs, const SmallGrid<T, I...> & rhs)
    {
        constexpr int D = SmallGrid<T, I...>::S;
        // all done
        return operatorEqualEqual(std::make_index_sequence<D> {}, lhs, rhs);
    }


    // Algebraic operations on vectors, tensors, ...
    // TOFIX: generalize with respect to scalar type (in this case real)

    // vector_t times scalar
    template <typename T, int... I, size_t... J>
    inline void _vector_times_scalar(
        const real & a, const SmallGrid<T, I...> & y, SmallGrid<T, I...> & result,
        std::index_sequence<J...>)
    {
        ((result[J] = y[J] * a), ...);
        return;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator*(const real & a, SmallGrid<T, I...> && y) requires(
        SmallGrid<T, I...>::S != 1)
    {
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_times_scalar(a, y, y, std::make_index_sequence<D> {});
        return std::move(y);
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> operator*(const real & a, const SmallGrid<T, I...> & y) requires(
        SmallGrid<T, I...>::S != 1)
    {
        SmallGrid<T, I...> result;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_times_scalar(a, y, result, std::make_index_sequence<D> {});
        return result;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator*(SmallGrid<T, I...> && y, const real & a) requires(
        SmallGrid<T, I...>::S != 1)
    {
        return a * std::move(y);
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> operator*(const SmallGrid<T, I...> & y, const real & a) requires(
        SmallGrid<T, I...>::S != 1)
    {
        return a * y;
    }

    // vector_t inner product
    template <typename T, int... I, std::size_t... J>
    inline T _vector_inner_product(
        const SmallGrid<T, I...> & y1, const SmallGrid<T, I...> & y2, std::index_sequence<J...>)
    {
        return ((y1[J] * y2[J]) + ...);
    }
    template <typename T, int... I>
    inline T operator*(const SmallGrid<T, I...> & y1, const SmallGrid<T, I...> & y2)
    {
        constexpr int D = SmallGrid<T, I...>::S;
        return _vector_inner_product(y1, y2, std::make_index_sequence<D> {});
    }
    template <typename T, int... I>
    inline T operator*(SmallGrid<T, I...> && y1, const SmallGrid<T, I...> & y2)
    {
        constexpr int D = SmallGrid<T, I...>::S;
        return _vector_inner_product(std::move(y1), y2, std::make_index_sequence<D> {});
    }
    template <typename T, int... I>
    inline T operator*(const SmallGrid<T, I...> & y1, SmallGrid<T, I...> && y2)
    {
        constexpr int D = SmallGrid<T, I...>::S;
        return _vector_inner_product(y1, std::move(y2), std::make_index_sequence<D> {});
    }
    template <typename T, int... I>
    inline T operator*(SmallGrid<T, I...> && y1, SmallGrid<T, I...> && y2)
    {
        constexpr int D = SmallGrid<T, I...>::S;
        return _vector_inner_product(std::move(y1), std::move(y2), std::make_index_sequence<D> {});
    }

    // sum of vector_ts
    template <typename T, int... I, std::size_t... J>
    inline void _vector_sum(
        const SmallGrid<T, I...> & y1, const SmallGrid<T, I...> & y2, SmallGrid<T, I...> & result,
        std::index_sequence<J...>)
    {
        ((result[J] = y1[J] + y2[J]), ...);
        return;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> operator+(
        const SmallGrid<T, I...> & y1, const SmallGrid<T, I...> & y2)
    {
        // std::cout << "operator+ new temp" << std::endl;
        SmallGrid<T, I...> result;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_sum(y1, y2, result, std::make_index_sequence<D> {});
        return result;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator+(SmallGrid<T, I...> && y1, const SmallGrid<T, I...> & y2)
    {
        // std::cout << "operator+ no temp && &" << std::endl;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_sum(y1, y2, y1, std::make_index_sequence<D> {});
        return std::move(y1);
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator+(const SmallGrid<T, I...> & y1, SmallGrid<T, I...> && y2)
    {
        // std::cout << "operator+ no temp & &&" << std::endl;
        return std::move(y2) + y1;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator+(SmallGrid<T, I...> && y1, SmallGrid<T, I...> && y2)
    {
        // std::cout << "operator+ no temp && &&" << std::endl;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_sum(y1, y2, y1, std::make_index_sequence<D> {});
        return std::move(y1);
    }

    // vector_t operator-
    template <typename T, int... I, std::size_t... J>
    inline void _vector_minus(
        const SmallGrid<T, I...> & y, SmallGrid<T, I...> & result, std::index_sequence<J...>)
    {
        ((result[J] = -y[J]), ...);
        return;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> operator-(const SmallGrid<T, I...> & y)
    {
        // std::cout << "unary operator- new temp" << std::endl;
        SmallGrid<T, I...> result;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_minus(y, result, std::make_index_sequence<D> {});
        return result;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator-(SmallGrid<T, I...> && y)
    {
        // std::cout << "unary operator- no temp &&" << std::endl;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_minus(y, y, std::make_index_sequence<D> {});
        return std::move(y);
    }
    template <typename T, int... I, std::size_t... J>
    inline void _vector_minus(
        const SmallGrid<T, I...> & y1, const SmallGrid<T, I...> & y2, SmallGrid<T, I...> & result,
        std::index_sequence<J...>)
    {
        ((result[J] = y1[J] - y2[J]), ...);
        return;
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> operator-(
        const SmallGrid<T, I...> & y1, const SmallGrid<T, I...> & y2)
    {
        // std::cout << "binary operator- new temp" << std::endl;
        SmallGrid<T, I...> result;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_minus(y1, y2, result, std::make_index_sequence<D> {});
        return result;
        // return y1 + (-y2);
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator-(SmallGrid<T, I...> && y1, const SmallGrid<T, I...> & y2)
    {
        // std::cout << "binary operator- no temp && &" << std::endl;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_minus(y1, y2, y1, std::make_index_sequence<D> {});
        return std::move(y1);
        // return std::move(y1) + (-y2);
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator-(const SmallGrid<T, I...> & y1, SmallGrid<T, I...> && y2)
    {
        // std::cout << "binary operator- no temp & &&" << std::endl;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_minus(y1, y2, y2, std::make_index_sequence<D> {});
        return std::move(y2);
        // return y1 + (-std::move(y2));
    }
    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator-(SmallGrid<T, I...> && y1, SmallGrid<T, I...> && y2)
    {
        // std::cout << "binary operator- no temp && &&" << std::endl;
        constexpr int D = SmallGrid<T, I...>::S;
        _vector_minus(y1, y2, y1, std::make_index_sequence<D> {});
        return std::move(y1);
        // return std::move(y1) + (-std::move(y2));
    }

    template <typename T, int... I>
    inline SmallGrid<T, I...> operator/(const SmallGrid<T, I...> & y, const real & a) requires(
        SmallGrid<T, I...>::S != 1)
    {
        return (1.0 / a) * y;
    }

    template <typename T, int... I>
    inline SmallGrid<T, I...> && operator/(SmallGrid<T, I...> && y, const real & a) requires(
        SmallGrid<T, I...>::S != 1)
    {
        return (1.0 / a) * std::move(y);
    }

    // matrix-vector multiplication
    // row-vector product
    template <int D1, int D2, typename T, size_t... J>
    inline T _row_times_vector(
        const tensor_t<D1, D2, T> & A, const vector_t<D2, T> & x, size_t row,
        std::index_sequence<J...>)
    {
        return ((A[row * D2 + J] /*A[{ row, J }]*/ * x[J]) + ...);
    }
    // matrix-vector product
    template <int D1, int D2, typename T, size_t... J>
    inline vector_t<D1, T> _matrix_times_vector(
        const tensor_t<D1, D2, T> & A, const vector_t<D2, T> & x, std::index_sequence<J...>)
    {
        vector_t<D1, T> result;
        ((result[J] = _row_times_vector(A, x, J, std::make_index_sequence<D2> {})), ...);
        return result;
    }
    template <int D1, int D2, typename T>
    inline vector_t<D1, T> operator*(const tensor_t<D1, D2, T> & A, const vector_t<D2, T> & x)
    {
        return _matrix_times_vector(A, x, std::make_index_sequence<D1> {});
    }

    // factorial
    template <int D>
    int Factorial()
    {
        return D * Factorial<int(D - 1)>();
    }
    template <>
    int Factorial<1>()
    {
        return 1;
    }

    real ComputeDeterminant(const tensor_t<4> & A)
    {
        return A[1] * A[11] * A[14] * A[4] - A[1] * A[10] * A[15] * A[4]
             - A[11] * A[13] * A[2] * A[4] + A[10] * A[13] * A[3] * A[4]
             - A[0] * A[11] * A[14] * A[5] + A[0] * A[10] * A[15] * A[5]
             + A[11] * A[12] * A[2] * A[5] - A[10] * A[12] * A[3] * A[5]
             - A[1] * A[11] * A[12] * A[6] + A[0] * A[11] * A[13] * A[6]
             + A[1] * A[10] * A[12] * A[7] - A[0] * A[10] * A[13] * A[7]
             - A[15] * A[2] * A[5] * A[8] + A[14] * A[3] * A[5] * A[8] + A[1] * A[15] * A[6] * A[8]
             - A[13] * A[3] * A[6] * A[8] - A[1] * A[14] * A[7] * A[8] + A[13] * A[2] * A[7] * A[8]
             + A[15] * A[2] * A[4] * A[9] - A[14] * A[3] * A[4] * A[9] - A[0] * A[15] * A[6] * A[9]
             + A[12] * A[3] * A[6] * A[9] + A[0] * A[14] * A[7] * A[9] - A[12] * A[2] * A[7] * A[9];
    }

    real ComputeDeterminant(const tensor_t<3> & A)
    {
        return A[0] * (A[4] * A[8] - A[5] * A[7]) - A[1] * (A[3] * A[8] - A[5] * A[6])
             + A[2] * (A[3] * A[7] - A[4] * A[6]);
    }

    real ComputeDeterminant(const tensor_t<2> & A) { return A[0] * A[3] - A[1] * A[2]; }

    real ComputeInverse(const tensor_t<3> & A, tensor_t<3> & invA)
    {
        real det = ComputeDeterminant(A);
        assert(det != 0.0);

        real detinv = 1.0 / det;
        invA[0] = detinv * (A[4] * A[8] - A[5] * A[7]);
        invA[1] = detinv * (-A[1] * A[8] + A[2] * A[7]);
        invA[2] = detinv * (A[1] * A[5] - A[2] * A[4]);
        invA[3] = detinv * (-A[3] * A[8] + A[5] * A[6]);
        invA[4] = detinv * (A[0] * A[8] - A[2] * A[6]);
        invA[5] = detinv * (-A[0] * A[5] + A[2] * A[3]);
        invA[6] = detinv * (A[3] * A[7] - A[4] * A[6]);
        invA[7] = detinv * (-A[0] * A[7] + A[1] * A[6]);
        invA[8] = detinv * (A[0] * A[4] - A[1] * A[3]);

        return det;
    }

    real ComputeInverse(const tensor_t<2> & A, tensor_t<2> & invA)
    {
        real det = ComputeDeterminant(A);
        assert(det != 0.0);

        real detinv = 1.0 / det;
        invA[0] = detinv * (A[3]);
        invA[1] = detinv * (-A[1]);
        invA[2] = detinv * (-A[2]);
        invA[3] = detinv * (A[0]);

        return det;
    }
}

// overload operator<< for vectors and tensors
std::ostream &
operator<<(std::ostream & os, const mito::vector_t<3> & x)
{
    os << "(" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    return os;
}

std::ostream &
operator<<(std::ostream & os, const mito::vector_t<2> & x)
{
    os << "(" << x[0] << ", " << x[1] << ")";
    return os;
}

std::ostream &
operator<<(std::ostream & os, const mito::tensor_t<3> & x)
{
    os << "(" << x[0] << ", " << x[1] << ", " << x[2] << "; " << x[3] << ", " << x[4] << ", "
       << x[5] << "; " << x[6] << ", " << x[7] << ", " << x[8] << ")";
    return os;
}

std::ostream &
operator<<(std::ostream & os, const mito::tensor_t<2> & x)
{
    os << "(" << x[0] << ", " << x[1] << "; " << x[2] << ", " << x[3] << ")";
    return os;
}

// end of file
