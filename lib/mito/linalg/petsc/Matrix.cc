// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


// support
#include "public.h"


// constructor
mito::linalg::petsc::Matrix::Matrix(index_type size, assembly_format format) :
    _matrix(nullptr),
    _format(format),
    _n_entries(0)
{
    // create the matrix
    PetscCallVoid(MatCreate(PETSC_COMM_WORLD, &_matrix));
    PetscCallVoid(MatSetSizes(_matrix, PETSC_DECIDE, PETSC_DECIDE, size, size));

    // set the default options (do not allow the user to control the options for matrix)
    PetscCallVoid(MatSetFromOptions(_matrix));
}

// destructor
mito::linalg::petsc::Matrix::~Matrix()
{
    // destroy the matrix
    PetscCallVoid(MatDestroy(&_matrix));
}

// copy constructor
mito::linalg::petsc::Matrix::Matrix(const Matrix & other) :
    _matrix(nullptr),
    _format(other._format),
    _n_entries(other._n_entries),
    _rows(other._rows),
    _cols(other._cols),
    _segments(other._segments),
    _pattern_set(other._pattern_set)
{
    // duplicate matrix
    PetscCallVoid(MatDuplicate(other._matrix, MAT_COPY_VALUES, &_matrix));
}

// move constructor
mito::linalg::petsc::Matrix::Matrix(Matrix && other) noexcept :
    _matrix(std::exchange(other._matrix, nullptr)),
    _format(other._format),
    _n_entries(other._n_entries),
    _rows(std::move(other._rows)),
    _cols(std::move(other._cols)),
    _segments(std::move(other._segments)),
    _pattern_set(other._pattern_set)
{}

// copy assignment operator
auto
mito::linalg::petsc::Matrix::operator=(const Matrix & other) -> Matrix &
{
    // prevent self-copy
    if (this == &other) {
        return *this;
    }

    // copy and swap
    Matrix tmp(other);
    _swap(tmp);

    // returning deletes {tmp}
    return *this;
}

// move assignment operator
auto
mito::linalg::petsc::Matrix::operator=(Matrix && other) noexcept -> Matrix &
{
    // prevent self-move
    if (this != &other) {
        // free the matrix
        MatDestroy(&_matrix);
        // swap the contents
        _swap(other);
    }

    return *this;
}

// assemble the matrix
auto
mito::linalg::petsc::Matrix::assemble() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FINAL_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FINAL_ASSEMBLY));

    // all done
    return;
}

// flush assemble the matrix
auto
mito::linalg::petsc::Matrix::assemble_flush() -> void
{
    // assemble matrix
    PetscCallVoid(MatAssemblyBegin(_matrix, MAT_FLUSH_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(_matrix, MAT_FLUSH_ASSEMBLY));

    // all done
    return;
}

// set the pattern based on the provided row and column indices
auto
mito::linalg::petsc::Matrix::set_pattern(
    std::vector<index_type> rows, std::vector<index_type> cols,
    std::vector<block_segment_type> segments) -> void
{
    // ensure the row and column vectors have the same size
    assert(
        std::size(rows) == std::size(cols)
        && "row and column index vectors must have the same size");

    // ensure the pattern has not been set yet
    assert(!_pattern_set && "set_pattern can only be called once on this matrix");

    // store the number of entries
    _n_entries = std::size(rows);

    // store the segments, if there are any; else store everything is a 1x1 block
    _segments = std::empty(segments) ? std::vector<block_segment_type>{ { 1, _n_entries } } :
                                       std::move(segments);

    // ensure the number of segments is consistent with the number of entries
    assert(
        std::transform_reduce(
            _segments.begin(), _segments.end(), std::size_t{ 0 }, std::plus<>{},
            [](const auto & s) { return s.n_blocks * s.block_size * s.block_size; })
        == _n_entries);

    switch (_format) {
        case assembly_format::hash:
            _set_pattern_hash(std::move(rows), std::move(cols));
            break;

        case assembly_format::coo:
            _set_pattern_coo(std::move(rows), std::move(cols));
            break;

        case assembly_format::csr:
            _set_pattern_csr(std::move(rows), std::move(cols));
            break;

        default:
            throw std::runtime_error("Unknown assembly format");
    }

    // set the pattern set flag
    _pattern_set = true;

    return;
}

// set the values based on the provided values vector
auto
mito::linalg::petsc::Matrix::set_values(const std::vector<scalar_type> & values) -> void
{
    // one value per entry of the index lists
    assert(
        std::size(values) == _n_entries
        && "number of values must match number of entries in the matrix");

    // ensure that the pattern has been set before setting values
    assert(_pattern_set > 0 && "set_values requires set_pattern on this matrix");

    switch (_format) {
        case assembly_format::hash:
            _set_values_csr(values);
            break;

        case assembly_format::coo:
            _set_values_coo(values);
            break;

        case assembly_format::csr:
            _set_values_csr(values);
            break;

        default:
            throw std::runtime_error("Unknown assembly format");
    }
    return;
}

// set the matrix entry at ({row}, {col}) to {value}
auto
mito::linalg::petsc::Matrix::insert_value(index_type row, index_type col, const scalar_type & value)
    -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, INSERT_VALUES));

    // all done
    return;
}

// add {value} to matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::add_value(index_type row, index_type col, const scalar_type & value)
    -> void
{
    // delegate to PETSc
    PetscCallVoid(MatSetValue(_matrix, row, col, value, ADD_VALUES));

    // all done
    return;
}

// get the value of the matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::get_value(index_type row, index_type col, scalar_type & value) const
    -> void
{
    // delegate to PETSc
    PetscCallVoid(MatGetValue(_matrix, row, col, &value));

    // all done
    return;
}

// return the value of the matrix entry at ({row}, {col})
auto
mito::linalg::petsc::Matrix::get_value(index_type row, index_type col) const -> scalar_type
{
    // the value to return
    scalar_type value;

    // delegate to PETSc
    get_value(row, col, value);

    // all done
    return value;
}

// duplicate another matrix
auto
mito::linalg::petsc::Matrix::duplicate(const Matrix & X, duplicate_policy policy) -> void
{
    // check if we are not duplicating ourselves
    if (this == &X) {
        return;
    }

    // release the matrix we currently own
    PetscCallVoid(MatDestroy(&_matrix));

    // reset the matrix properties; note that this means that one has to first call set_pattern and
    // then set_values on the new matrix, if one wants to use those methods
    _n_entries = X._n_entries;
    _rows = X._rows;
    _cols = X._cols;
    _segments = X._segments;
    _pattern_set = true;

    // duplicate {X}'s structure without its values
    PetscCallVoid(MatDuplicate(X._matrix, static_cast<MatDuplicateOption>(policy), &_matrix));
}

// copy values from another matrix
auto
mito::linalg::petsc::Matrix::copy_values(const Matrix & X, sparsity_pattern pattern) -> void
{
    // delegate to PETSc
    PetscCallVoid(MatCopy(X._matrix, _matrix, static_cast<MatStructure>(pattern)));

    // all done
    return;
}

// add scaled matrix using MatAXPY
auto
mito::linalg::petsc::Matrix::add(
    const scalar_type & alpha, const Matrix & X, sparsity_pattern pattern) -> void
{
    // delegate to PETsc
    PetscCallVoid(MatAXPY(_matrix, alpha, X._matrix, static_cast<MatStructure>(pattern)));
}

// get the number of rows/columns of the matrix
auto
mito::linalg::petsc::Matrix::size(index_type & matrix_size) const -> void
{
    // delegate to PETSc
    PetscCallVoid(MatGetSize(_matrix, &matrix_size, nullptr));

    // all done
    return;
}

// return the number of rows/columns of the matrix
auto
mito::linalg::petsc::Matrix::size() const -> index_type
{
    // the value to return
    index_type matrix_size;

    // delegate to PETSc
    size(matrix_size);

    // all done
    return matrix_size;
}

// print the matrix
auto
mito::linalg::petsc::Matrix::print() const -> void
{
    // create a reporting channel
    journal::info_t channel("mito.solvers.petsc.Matrix");

    // print the matrix
    channel << "Matrix:" << journal::endl;
    PetscCallVoid(MatView(_matrix, PETSC_VIEWER_STDOUT_WORLD));

    // all done
    return;
}

// get petsc matrix
auto
mito::linalg::petsc::Matrix::get() const -> matrix_type const &
{
    return _matrix;
}

// swap the contents of two matrices
auto
mito::linalg::petsc::Matrix::_swap(Matrix & other) noexcept -> void
{
    // swap the matrix handles
    std::swap(_matrix, other._matrix);

    // swap the assembly formats
    std::swap(_format, other._format);

    // swap the number of entries
    std::swap(_n_entries, other._n_entries);

    // swap the row and column index lists
    std::swap(_rows, other._rows);
    std::swap(_cols, other._cols);

    // swap the block segments
    std::swap(_segments, other._segments);

    // swap the pattern set flags
    std::swap(_pattern_set, other._pattern_set);

    // all done
    return;
}

// set the pattern based on the provided row and column indices, for no preallocation
auto
mito::linalg::petsc::Matrix::_set_pattern_hash(
    std::vector<index_type> rows, std::vector<index_type> cols) -> void
{
    // keep the index lists to insert values later
    _rows = std::move(rows);
    _cols = std::move(cols);

    // all done
    return;
}

// set the pattern based on the provided row and column indices, for coo assembly
auto
mito::linalg::petsc::Matrix::_set_pattern_coo(
    std::vector<index_type> rows, std::vector<index_type> cols) -> void
{
    // set the pattern using PETSc
    PetscCallVoid(MatSetPreallocationCOO(
        _matrix, static_cast<PetscCount>(_n_entries), rows.data(), cols.data()));

    // all done
    return;
}

// set the pattern based on the provided row and column indices, for csr assembly
auto
mito::linalg::petsc::Matrix::_set_pattern_csr(
    std::vector<index_type> rows, std::vector<index_type> cols) -> void
{
    // record the pattern in a preallocator matrix with the same size and distribution
    Mat prealloc;
    index_type size;
    PetscCallVoid(MatGetSize(_matrix, &size, nullptr));
    PetscCallVoid(MatCreate(PETSC_COMM_WORLD, &prealloc));
    PetscCallVoid(MatSetSizes(prealloc, PETSC_DECIDE, PETSC_DECIDE, size, size));
    PetscCallVoid(MatSetType(prealloc, MATPREALLOCATOR));
    PetscCallVoid(MatSetUp(prealloc));

    // set the values using PETSc
    std::vector<index_type> block_rows, block_cols;
    std::vector<scalar_type> zeros;
    std::size_t offset = 0;

    for (const auto & [block_size, n_blocks] : _segments) {
        const auto n = static_cast<index_type>(block_size);
        block_rows.resize(block_size);
        block_cols.resize(block_size);
        zeros.assign(block_size * block_size, 0.0);

        for (std::size_t e = 0; e < n_blocks; ++e, offset += block_size * block_size) {
            for (std::size_t i = 0; i < block_size; ++i) {
                block_rows[i] = rows[offset + i * block_size];
                block_cols[i] = cols[offset + i];
            }

            PetscCallVoid(MatSetValues(
                prealloc, n, block_rows.data(), n, block_cols.data(), zeros.data(), ADD_VALUES));
        }
    }

    // assemble the preallocator matrix
    PetscCallVoid(MatAssemblyBegin(prealloc, MAT_FINAL_ASSEMBLY));
    PetscCallVoid(MatAssemblyEnd(prealloc, MAT_FINAL_ASSEMBLY));

    // exact preallocation of the real matrix, with the pattern filled in
    PetscCallVoid(MatPreallocatorPreallocate(prealloc, PETSC_TRUE, _matrix));
    PetscCallVoid(MatDestroy(&prealloc));

    // keep the index lists to insert values later
    _rows = std::move(rows);
    _cols = std::move(cols);

    // all done
    return;
}

// set the values for coo assembly
auto
mito::linalg::petsc::Matrix::_set_values_coo(const std::vector<scalar_type> & values) -> void
{
    // set the values using PETSc
    PetscCallVoid(MatSetValuesCOO(_matrix, values.data(), INSERT_VALUES));

    // all done
    return;
}

// set the values for csr assembly
auto
mito::linalg::petsc::Matrix::_set_values_csr(const std::vector<scalar_type> & values) -> void
{
    // start from zero, keeping the pattern
    PetscCallVoid(MatZeroEntries(_matrix));

    // set the values using PETSc
    std::vector<index_type> block_rows, block_cols;
    std::size_t offset = 0;

    for (const auto & [block_size, n_blocks] : _segments) {
        const auto n = static_cast<index_type>(block_size);
        block_rows.resize(block_size);
        block_cols.resize(block_size);

        for (std::size_t e = 0; e < n_blocks; ++e, offset += block_size * block_size) {
            for (std::size_t i = 0; i < block_size; ++i) {
                block_rows[i] = _rows[offset + i * block_size];
                block_cols[i] = _cols[offset + i];
            }

            PetscCallVoid(MatSetValues(
                _matrix, n, block_rows.data(), n, block_cols.data(), values.data() + offset,
                ADD_VALUES));
        }
    }

    // all done
    return;
}


// end of file
