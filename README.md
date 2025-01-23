# Sparse Matrix Implementation using Doubly Linked Lists

## Overview
A C++ implementation of a memory-efficient sparse matrix using nested doubly linked lists. Stores only non-zero elements while maintaining matrix operations capabilities. Designed for large matrices with predominantly zero elements.

## Features
- **Bi-dimensional Sparse Storage**:
  - Row-wise doubly linked list structure
  - Column-wise doubly linked list within rows
- **Matrix Operations**:
  - Element get/set by (row, column)
  - Matrix addition
  - Full matrix print with implicit zeros
  - Compact non-zero elements print
- **Efficient Insertion**: Maintains sorted indices (O(n) complexity)
- **Memory Safe**: Automatic cleanup through destructors

## Usage
```
// Create 100x100 sparse matrix
SparseMatrix matrix(100, 100);

// Set values
matrix.set_value(5, 10, 20);  // Row 10, Column 20 = 5
matrix.set_value(3, 42, 15);

// Get value
cout << matrix.get_value(10, 20); // Returns 5

// Create second matrix
SparseMatrix matrix2(100, 100);
matrix2.set_value(7, 10, 20);
matrix2.set_value(2, 30, 15);

// Matrix addition
matrix.add(matrix2);

// Print
matrix.print_matrix();        // Full 100x100 matrix display
matrix.print_matrix_nonzero();// Only non-zero elements
```
## Key Methods
- `set_value(int data, int row, int col)`: Set element at (row,col)
- `get_value(int row, int col)`:	Get element value (0 if not set)
- `print_matrix()`:	Print full matrix with zeros
- `print_matrix_nonzero()`:	Print only stored values
- `add(SparseMatrix& other)`:	Matrix addition

## Example
input:
```
SparseMatrix a(3, 3), b(3, 3);
a.set_value(3, 0, 2);
a.set_value(5, 1, 1);
b.set_value(4, 0, 2);
b.set_value(7, 2, 2);

a.add(b);
a.print_matrix();
```
output:
```
0 0 7 
0 5 0 
0 0 7
```
## Notes
- Fixed matrix dimensions set at construction
- Zero-based indexing (row 0, column 0)
- Default element value is 0
- Element setting overwrites existing values
- Matrix addition requires same dimensions
- Assertions validate row/column bounds
