/**
 * Calculate a determinant of a matrix,
 * using constexpr
 * Works on C++14.
 */
#include <array>
#include <iostream>
/**
 * Finish recursion
 * (C++ 14 doesn't support if constexpr)
 */
template <int N>
constexpr typename std::enable_if< (N <= 1), int>::type 
det(const std::array<std::array<int, N>, N>& a){
	if( N < 1 ){
		return 0;
	} else{
		return a[0][0];
	}
}
/**
 * A recursive approach:
 * Calculate submatrixes determinants
 * Use a formula to calculate a matrix determinant
 */
template <int N>
constexpr typename std::enable_if< (N > 1), int>::type
det(const std::array<std::array<int, N>, N>& a) {
	int determinant = 0;
	// What sign has an addendum in formula
	int sign = 1;
	std::array<std::array<int, N-1>, N-1> sub_matrix{ { 0 } };
	// In the submatrixes the first row is always excluded
	for( int excluded_column_idx = 0; excluded_column_idx < N; ++excluded_column_idx ){
		int submatrix_column_idx = 0;
		for( int row_idx = 1; row_idx < N; ++row_idx ){
			for( int column_idx = 0; column_idx < N; ++column_idx ){
				if( column_idx != excluded_column_idx ){
					submatrix_column_idx = column_idx;
					if( column_idx > excluded_column_idx ){
						// An excluded row causes changes in the indexation
						--submatrix_column_idx;
					}
					/**
					 * In C++ 14 a subscript operator can be used in the constexpr functions
					 * But constexpr overload of it requires const references
					 * So, cast the reference to a const one
					 */
					const_cast<int&>(static_cast<std::array<int, N-1> const&>
					(static_cast<std::array<std::array<int, N-1>, N-1> const&>(sub_matrix)[row_idx - 1])[submatrix_column_idx])
					= a[row_idx][column_idx];
				}
			}
		}
		determinant += sign * a[0][excluded_column_idx] * det<N-1>( sub_matrix);
		// Flip a sign
		sign = -sign;
	}
	return determinant;
}
int main() {
constexpr std::array<std::array<int, 3>, 3> A = {{
{0, 1, 2}
,{1, 2, 3}
,{2, 3, 7}
}};
constexpr int res = det<3>(A);
}