#include <iostream>

class Matrix {
private:
	int *mtr;
	const size_t rows;
	const size_t cols;

public:
	class Row {
	private:
		int *arr;
		const size_t cols;

	public:
		Row (const size_t cols_, int *arr_)
			: cols(cols_), arr(arr_)
			{}

		int& operator[](const int jnd){
			if (jnd >= cols || jnd < 0)
				throw std::out_of_range("");

			return arr[jnd];
		}

		int operator[](const int jnd) const{
			if (jnd >= cols || jnd < 0)
				throw std::out_of_range("");

			return arr[jnd];
		}
	};

	Matrix (const size_t rows_, const size_t cols_)
			: rows(rows_), cols(cols_)
			{
				mtr = new int[rows * cols];
			}

	size_t getRows() const{
		return this -> rows;
	}
	size_t getColumns() const{
		return this -> cols;
	}

	const Row operator[](const int ind) const{
		if (ind >= rows || ind < 0) 
			throw std::out_of_range("");

		return Row(cols, mtr + ind * cols);
	}

	Row operator[](const int ind){
		if (ind >= rows || ind < 0) 
			throw std::out_of_range("");

		return Row(cols, mtr + ind * cols);
	}

	bool operator==(const Matrix& other) const {
		if (this == &other) {
			return true;
		}

		for (size_t i = 0; i < rows * cols; i++)
			if (mtr[i] != other.mtr[i])
				return false;

		return true;
	}

	bool operator!=(const Matrix& other) const {
		return !(*this == other);
	}

	Matrix& operator*=(const int num){
		for (size_t i = 0; i < rows * cols; i++)
			mtr[i] *= num;

		return *this;
	}

	~Matrix(){
		delete []mtr;
	}
};
