#include <iostream>
//matrix class header file
//ethan riley, 2017

class Matrix {
	private:
		unsigned int row_count;
		unsigned int col_count;
		double** Matrix_pointer;	

	public:
		//constructor
		Matrix(unsigned int rows, unsigned int cols, double fill);
		//default constructor
		Matrix();
		//copy constructor
		Matrix(const Matrix&);
		//default destructor
		~Matrix();
		//destructor
		void clear();
		//returns number of rows
		unsigned int num_rows() const;
		//returns number of columns
		unsigned int num_cols() const;
		//outputs value at Matrix[row_num][col_num]
		bool get(unsigned int row_num, unsigned int col_num, double &get_value) const;
		//sets a value in the Matrix
		bool set(unsigned int row_num, unsigned int col_num, double set_value);
		//mutliplies matrix by a constant
		void multiply_by_coefficient(double coefficient);
		//swaps two rows in a matrix
		bool swap_row(unsigned int source_row_num, unsigned int target_row_num);
		//"flips" a matrix
		void transpose();
		//adds two matrices
		bool add(const Matrix m2);
		//subtracts two matrices
		bool subtract(const Matrix m2);
		//returns a subscriptable pointer to a row
		double* get_row(unsigned int row_num) const;
		//like get_row(), but with columns
		double* get_col(unsigned int col_num) const;
		//returns the quarters of a matrix in an array, i.e an array of matrices
		Matrix* quarter() const;
		//equality operator
		bool operator==(const Matrix M);
		//inequality operator
		bool operator!=(const Matrix M);
		//assignment operator
		void operator=(const Matrix M);
		//resize a matrix to different dimensions, copying over valid values
		void resize(unsigned int new_num_rows, unsigned int new_num_cols, double fill);
};

//overloads << operator for Matrix printing
std::ostream& operator<<(std::ostream& out, const Matrix& m);