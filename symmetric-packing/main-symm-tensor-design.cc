#include<iostream>

// values for template parameter packing 
class Canonical {};
class Symmetric {};
class Diagonal {};

// class tensor
template <typename T, class packing, int... I>
class Tensor {
  public:
    Tensor(){
        std::cout << "New tensor" << std::endl;
    };
};

// typedef for matrices
template <int D1, int D2 = D1, typename T = double, class packing = Canonical>
using matrix_t = Tensor<T, packing, D1, D2>;

// typedef for symmetric matrices
template <int D, typename T = double>
using symmetric_matrix_t = matrix_t<D, D, T, Symmetric>;

// typedef for diagonal matrices
template <int D, typename T = double>
using diagonal_matrix_t = matrix_t<D, D, T, Diagonal>;

// TODO: cast diagonal_matrix_t -> symmetric_matrix_t 
// constructor c

// adds two generic tensors
template <typename T, class packing1, class packing2, int... I>
constexpr inline auto operator+(
    const Tensor<T, packing1, I...> & y1, const Tensor<T, packing2, I...> & y2) 
        -> Tensor<T, repacking<packing1, packing2>, I...> 
{ 
    // TODO: repacking<matrix_t, symmetric_matrix_t> -> matrix_t
    // TODO: repacking<matrix_t, matrix_t> -> matrix_t
    // TODO: repacking<symmetric_matrix_t, symmetric_matrix_t> -> symmetric_matrix_t 
    std::cout << "regular operator+" << std::endl;
    return;
}

// adds two symmetric tensors (don't think we need it, just as a proof of concept) 
template <int D, typename T>
constexpr inline auto operator+(
    const symmetric_matrix_t<D, T> & y1, const symmetric_matrix_t<D, T> & y2) 
        -> symmetric_matrix_t<D, T>
{
    std::cout << "symmetric operator+" << std::endl;
    return;
}

// eigenvalues only for symmetric matrices
template <typename T, int... I>
constexpr inline void eigenvalues(const symmetric_matrix_t<2, T> & A)
{
    std::cout << "eigenvalues 2D" << std::endl;
    return;
}

// symmetrize a generic matrix
template <int D, typename T, class packing = Canonical>
constexpr symmetric_matrix_t<D, T> symmetric(const matrix_t<D, D, T, packing> & A)
{
    return symmetric_matrix_t<D, T>();
}

int main () {

    // a matrix
    matrix_t<2, 2> A;

    // a symmetric matrix
    symmetric_matrix_t<2> B;

    A + B; // calls regular operator+

    eigenvalues(A);             // compile-time error
    eigenvalues(symmetric(A));  // OK
    eigenvalues(B);             // OK

    symmetric(symmetric(A));    // OK

    B + symmetric(A);           // calls symmetric operator+

    // all done
    return 0;
}