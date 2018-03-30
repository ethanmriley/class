#include "Matrix.h"
#include <cmath>
//matrix class implementation
//ethan riley, 2017

//constructs new Matrix object using nested pointers
Matrix::Matrix(unsigned int rows, unsigned int cols, double fill) {
	if (rows <= 0 || cols <= 0) {
		row_count = 0;
		col_count = 0;
		Matrix_pointer = NULL;
	}
	else {
		row_count = rows;
		col_count = cols;
		Matrix_pointer = new double*[rows];
		for (unsigned int i = 0; i < rows; i++) {
			Matrix_pointer[i] = new double[cols];
			for (unsigned int j = 0; j < cols; j++) {
				Matrix_pointer[i][j] = fill;            //populates Matrix with the fill value
			} 
		}
	}
}

//default constructor
//creates a null Matrix
Matrix::Matrix() {
	row_count = 0;
	col_count = 0;
	Matrix_pointer = NULL; 
}

//default destructor
//deallocates memory and sets pointer to null
Matrix::~Matrix() {
	for (unsigned int i = 0; i < row_count; i++) {
		delete [] Matrix_pointer[i]; 
	}
	delete [] Matrix_pointer;;
	row_count = 0;
	col_count = 0;
	Matrix_pointer = NULL;
}	

//copy constructor 
//creates new, separate matrix from a matrix object
Matrix::Matrix(const Matrix& M) {
	row_count = M.num_rows();
	col_count = M.num_cols();
	if (row_count <= 0 || col_count <= 0) {
		Matrix_pointer = NULL;
	}
	else {
		double set_val;
		Matrix_pointer = new double*[row_count];
		for (unsigned int i = 0; i < row_count; i++) {
			Matrix_pointer[i] = new double[col_count];
			for (unsigned int j = 0; j < col_count; j++) {
				M.get(i, j, set_val);                      //uses Matrix::get() to obtain val
				Matrix_pointer[i][j] = set_val;            //sets appropriate location in new matrix to that val
			} 
		}
	}
}

//this clear() function is exactly identical to the default destructor
void Matrix::clear() {
	for (unsigned int i = 0; i < row_count; i++) {
		delete [] Matrix_pointer[i]; 
	}
	delete [] Matrix_pointer;
	row_count = 0;
	col_count = 0;
	Matrix_pointer = NULL;
}

//these are simple accessor functions
//they return, respectively, row_count and col_count
unsigned int Matrix::num_rows() const {
	return row_count;
}

unsigned int Matrix::num_cols() const {
	return col_count;
}


//outputs value at location
bool Matrix::get(unsigned int row_num, unsigned int col_num, double &get_value) const{
	if (row_num >= 0 && row_num < row_count && col_num >= 0 && col_num < col_count) {
		get_value = Matrix_pointer[row_num][col_num];
		return true;	
	}
	else {
		return false;
	}
}

//sets value at location
bool Matrix::set(unsigned int row_num, unsigned int col_num, double set_value) {
	if (row_num >= 0 && row_num < row_count && col_num >= 0 && col_num < col_count) {
		Matrix_pointer[row_num][col_num] = set_value;
		return true;	
	}
	else {
		return false;
	}
}

//simple matrix operation which multiplies all values in a Matrix by a coefficient 
void Matrix::multiply_by_coefficient(double coefficient) {
	for (unsigned int i = 0; i < row_count; i++) {
		for (unsigned int j = 0; j < col_count; j++) {
			Matrix_pointer[i][j] *= coefficient;
		}
	}	
}

//swaps two rows in a Matrix
bool Matrix::swap_row(unsigned int source_row_num, unsigned int target_row_num) {
	//read source row into a temp array
	//overwrite source row with target row directly
	//overwrite target with stored source
	if (source_row_num >= 0 && source_row_num < row_count && target_row_num >= 0 && target_row_num < row_count) {
		double* source_row = new double[col_count];
		for (unsigned int i = 0; i < col_count; i++) {
			source_row[i] = Matrix_pointer[source_row_num][i];
			Matrix_pointer[source_row_num][i] = Matrix_pointer[target_row_num][i];
			Matrix_pointer[target_row_num][i] = source_row[i];
		}
		return true;
	}
	else {
		return false;
	}
}

//this function switches the size of a matrix
//a 3 x 4 Matrix becomes a 4 x 3
void Matrix::transpose() {
	double** new_Matrix = new double*[col_count];        //create new Matrix in proper dimensions
	for (unsigned int i = 0; i < col_count; i++) {
		new_Matrix[i] = new double[row_count];
		for (unsigned int j = 0; j < row_count; j++) {
			new_Matrix[i][j] = Matrix_pointer[j][i];     //populate temp Matrix with appropriate values, switching rows and columns (i, j)
		}
	}
	unsigned int new_row_count = col_count;              //swaps old col/row and stores in var
	unsigned int new_col_count = row_count;
	clear();                                             //deletes old Matrix data
	row_count = new_row_count;	                         //updates Matrix variables
	col_count = new_col_count;
	Matrix_pointer = new_Matrix;                         //assigns pointer to new Matrix object
}

//binary matrix operation
//adds m2 to this Matrix objects value by value if dimensions are equal
bool Matrix::add(const Matrix m2) {
	if (row_count == m2.num_rows() && col_count == m2.num_cols()) {
		double m2_val;
		for (unsigned int i = 0; i  < row_count; i++) {
			for (unsigned int j = 0; j < col_count; j++) {
				m2.get(i, j, m2_val);
				Matrix_pointer[i][j] += m2_val;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

//binary matrix operation
//subtracts m2 from this value by value if dimensions are equal
bool Matrix::subtract(const Matrix m2) {
	if (row_count == m2.num_rows() && col_count == m2.num_cols()) {
		double m2_val;
		for (unsigned int i = 0; i  < row_count; i++) {
			for (unsigned int j = 0; j < col_count; j++) {
				m2.get(i, j, m2_val);
				Matrix_pointer[i][j] -= m2_val;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

//returns pointer to individual row 
double* Matrix::get_row(unsigned int row_num) const{
	double* row;                                        //constructs new array
	if (row_num >= 0 && row_num < row_count) {
		row = new double[col_count];
		for (unsigned int i = 0; i < col_count; i++) {
			row[i] = Matrix_pointer[row_num][i];        //populates row with a row of the Matrix
		}
	}
	else {
		row = NULL;                                     //sets row to null if row_num is invalid
	}
	return row;
}

//returns pointer to individual column
double* Matrix::get_col(unsigned int col_num) const{
	double* col;                                         //constructs new array
	if (col_num >= 0 && col_num < col_count) {
		col = new double[row_count];
		for (unsigned int i = 0; i < col_count; i++) {
			col[i] = Matrix_pointer[i][col_num];         //populates col with a column of the matrix
		}
	}
	else {
		col = NULL;                                      //sets col to null if col_num is invalid
	}
	return col;
}	

//creates and returns an array of Matrix objects from a base Matrix
//each object is one "quarter" of the base	
Matrix* Matrix::quarter() const {
	unsigned int col_val = std::ceil((double)col_count / 2); //sets the width of quarter matrices
	unsigned int row_val = std::ceil((double)row_count / 2); //sets the height of quarter matrices
	Matrix* quarters = new Matrix[4];
	double set_val;


	//each of the quarter matrices differs only in start and stop point in the base matrix
	//hardcoding in the desired pattern is the most efficient way
	for (unsigned int m = 0; m < 4; m++) {
		unsigned int row_start, row_end, col_start, col_end;
		row_end = row_val;
		col_end = col_val;
		if (m == 0) {      //upper left corner
			row_start = 0;
			col_start = 0;
		}
		else if (m == 1) { //upper right corner
			row_start = 0;
			col_start = col_count-col_val;                //this quantity is the number of columns minus the width of a quarter matrix
			col_end = col_count;						  //effectively, the start position of quarter values
		}
		else if (m == 2) { //lower left
			row_start = row_count-row_val;
			col_start = 0;
			row_end = row_end;
		}
		else if (m == 3) { //lower right
			row_start = row_count-row_val;
			col_start = col_count-col_val;
			row_end = row_count;
			col_end = col_count;
		}
		quarters[m] = Matrix(row_val, col_val, 0);
		unsigned int k = 0;                                              //i use k to keep track of the row in the quarter matrix, as i is initialized to a non-zero value half the time
		for (unsigned int i = row_start; i < row_count; i++, k++) {      //iterates through the rows of the base matrix accordinng to quarter start and end positions; simultaneously increments k
			unsigned int l = 0;                                          //l functions identically to k, but for columns 
			for (unsigned int j = col_start; j < col_count; j++, l++) {
				set_val = Matrix_pointer[i][j];                          //use eastablished set() method to get val at location in base matrix (determined by i and j)
				quarters[m].set(k, l, set_val);                          //then set the corresponding location in the quarter matric to that value (determined by k and l)
			}
		}
	}
	return quarters;
}

//overload == (equality) operator
//checks if dimensions and values in both Matrix objects are equal
bool Matrix::operator==(const Matrix M) {
	if (row_count == M.num_rows() && col_count == M.num_cols()) {
		double get_val_one;
		double get_val_two;
		for (unsigned int i = 0; i < row_count; i++) {
			for (unsigned int j = 0; j < col_count; j++) {
				get_val_one = Matrix_pointer[i][j];
				M.get(i, j, get_val_two); 
				if (get_val_one != get_val_two){
					return false;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

//overload != (inequality) operator
//identical to ==, except the return booleans are flipped
bool Matrix::operator!=(const Matrix M) {
	if (row_count == M.num_rows() && col_count == M.num_cols()) {
		double get_val_one;
		double get_val_two;
		for (unsigned int i = 0; i < row_count; i++) {
			for (unsigned int j = 0; j < col_count; j++) {
				get_val_one = Matrix_pointer[i][j];
				M.get(i, j, get_val_two); 
				if (get_val_one != get_val_two){
					return true;
				}
			}
			return false;
		}
	}
	else {
		return true;
	}
}

//overload = (assignment) operator
//identical to copy constructor
void Matrix::operator=(const Matrix M) {
	row_count = M.num_rows();
	col_count = M.num_cols();
	if (row_count == 0 || col_count == 0) {
		Matrix_pointer = NULL;
	}
	else {
		double set_val;
		Matrix_pointer = new double*[row_count];
		for (unsigned int i = 0; i < row_count; i++) {
			Matrix_pointer[i] = new double[col_count];
			for (unsigned int j = 0; j < col_count; j++) {
				M.get(i, j, set_val);
				Matrix_pointer[i][j] = set_val;
			} 
		}
	}

}

//overload << (stream output) operator
std::ostream& operator<<(std::ostream& out, const Matrix& M) {
	out << std::endl << M.num_rows() << " x " << M.num_cols() << " matrix:" << std::endl << "[ ";
	double get_val;
	for (unsigned int i = 0; i < M.num_rows(); i++) {
		for (unsigned int j = 0; j < M.num_cols(); j++) {
			M.get(i, j, get_val);
			out << get_val << " ";
		}
		if (i != M.num_rows() - 1) {                        //checks if on the last row
			out << std::endl << "  ";                       //if so, print newline and ready output for printing values
		}
		else {
			out << "]" << std::endl;                        //otherwise, print close bracket and end, returning the now-modified outstream
			return out;
		}
	}
	out << "]" << std::endl;                                //this handles empty Matrix objects by just printing a close bracket
	return out;                                             //and returning the stream
}

void Matrix::resize(unsigned int new_num_rows, unsigned int new_num_cols, double fill) {
	if (new_num_cols <= 0 || new_num_rows <= 0){
		new_num_rows = 0;
		new_num_cols = 0;
	}

	double set_val;
	double** new_Matrix = new double*[new_num_rows];        //create new Matrix in proper dimensions
	for (unsigned int i = 0; i < new_num_rows; i++) {
		new_Matrix[i] = new double[new_num_cols];
		for (unsigned int j = 0; j < new_num_cols; j++) {
			if (i >= row_count || j >= col_count) {         //checks for validity of location (in case resizing to a larger size)
				set_val = fill;                             //sets to fill if invalid
			}
			else {
				set_val = Matrix_pointer[i][j];             //sets to old matrix's val at that location if valid
			}
			new_Matrix[i][j] = set_val;                     //populate new Matrix with appropriate values
		}
	}
	clear();                                                //deletes old Matrix object
	Matrix_pointer = new_Matrix;                            //sets pointer to ew object
	row_count = new_num_rows;
	col_count = new_num_cols;
}