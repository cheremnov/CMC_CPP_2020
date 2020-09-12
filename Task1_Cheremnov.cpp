#include <array>
#include <iostream>

template <int N>
/**
 * A recursive approach:
 * Calculate submatrixes determinants
 * Use a formula to calculate a matrix determinant
 */
constexpr int det(const std::array<std::array<int, N>, N>& a) {
	// A constexpr 
	if constexpr( N < 1 ){
		return 0;
	} else if constexpr( N == 1 ){
		return a[0][0];
	} else{
		int determinant = 0;
		std::array<std::array<int, N-1>, N-1> sub_matrix{ { 0 } };
		// What sign has an addendum in formula
		int sign = 1;
		// In the submatrixes the first row is always excluded
		for( int excluded_column_idx = 0; excluded_column_idx < N; ++excluded_column_idx ){
			int submatrix_column_idx = 0;
			for( int row_idx = 1; row_idx < N; ++row_idx ){
				for( int column_idx = 0; column_idx < N; ++column_idx ){
					if( column_idx != excluded_column_idx ){
						submatrix_column_idx = column_idx;
						if( column_idx > excluded_column_idx ){
							// An excluded row causes changes in the indexes
							--submatrix_column_idx;
						}
						sub_matrix[row_idx - 1][submatrix_column_idx] = a[row_idx][column_idx];
					}
				}
			}
			determinant += sign * a[0][excluded_column_idx] * det<N-1>( sub_matrix);
			// Flip a sign
			sign = -sign;
		}
		return determinant;
	}
}
int main() {
constexpr std::array<std::array<int, 3>, 3> A = {{
{0, 1, 2}
,{1, 2, 3}
,{2, 3, 7}
}};
constexpr int res = det<3>(A);
std::cout << res << std::endl;
}