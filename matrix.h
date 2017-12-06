#ifndef __Complex_H__
#define __Complex_H__
#include <iomanip>
using namespace std;

class CMatrix
{
	private:
		struct matrix;
		matrix* data;
	public:
	CMatrix();
	CMatrix(int rows,int columns,double diagonal,double nondiagonal);
	CMatrix(const CMatrix&);
	CMatrix(istream& f1);
	~CMatrix();
	CMatrix operator*(const CMatrix&);
	CMatrix& operator=(const CMatrix&);
	double* operator[](int i) const;
	friend ostream& operator<<(ostream& o, const CMatrix& c);
};
struct CMatrix::matrix
{
	int rows,columns;
	int i,j;
	int Refs;
	double diagonal,nondiagonal;
	double **ArrMat;
	
	matrix(int r, int c, double d, double nd)
	{
		rows=r;
		columns=c;
		diagonal=d;
		nondiagonal=nd;
		Refs=1;

		ArrMat=new double*[rows];
		for(i=0;i<rows;i++)
			ArrMat[i]=new double[columns];

		matrix_fill();
	};
	void matrix_fill()
	{
		for(i=0;i<rows;i++)
		{
			for(j=0;j<columns;j++)
			{
				if(i==j)ArrMat[i][j]=diagonal;
				else ArrMat[i][j]=nondiagonal;
			}
		}	
	};
	~matrix()
	{
		for(i=0;i<rows;i++)	
			delete [] ArrMat[i];
		delete [] ArrMat;
	}
	matrix* detach()
  	{
    		if(Refs==1)
      			return this;
    		matrix* m=new matrix(rows,columns,diagonal,nondiagonal);
    		Refs--;
    		return m;
  	};
};
CMatrix::CMatrix()
{
	data=new matrix(0,0,0,0);
}
CMatrix::CMatrix(int rows=1,int columns=1,double diagonal=0,double nondiagonal=0)
{
	data=new matrix(rows,columns,diagonal,nondiagonal);
}
CMatrix::CMatrix(const CMatrix& c)
{
	c.data->Refs++;
	data=c.data;
}
CMatrix::~CMatrix()
{
	if(--data->Refs==0)
		delete data;
}
CMatrix& CMatrix::operator=(const CMatrix& c)
{
	c.data->Refs++;
	if(--data->Refs==0)
		delete data;
	data=c.data;
	data=data->detach();
	return *this;
}
CMatrix CMatrix::operator*(const CMatrix& c)
{
	if((data->columns)!=(c.data->rows))
	{
		cout << "Sizes of matrixes are not compatible" << endl;
		return *this;
	}
	else
	{
		CMatrix r (data->rows,c.data->columns,0,0);
		for(int i=0;i<r.data->rows;i++)
		{
			for(int j=0;j<r.data->columns;j++)
			{
				for(int k=0;k<data->columns;k++)
				{
					r.data->ArrMat[i][j]+=data->ArrMat[i][k]*c.data->ArrMat[k][j];
				}
			}
		}
		return r;
	}
}
double* CMatrix::operator[](int i) const
{
	return data->ArrMat[i];
}
CMatrix::CMatrix(istream& f1)
{
	int rows,columns,i,j;
	double number;
	f1 >> rows;
	f1 >> columns;
	data=new matrix(rows,columns,0,0);
	for(i=0;i<rows;i++)
	{
		for(j=0;j<columns;j++)
		{
			if(f1 >> number)
				data->ArrMat[i][j]=number;
			else
				cout << "Wrong matrix size" << endl;
		}
	}
}
ostream& operator<<(ostream& o, const CMatrix& c)
{
	cout << setprecision(1) << fixed;
	for(int i=0;i<c.data->rows;i++)
	{
		for(int j=0;j<c.data->columns;j++)
		{
			o << c.data->ArrMat[i][j] << " " ;
		}
		o << endl;
	}
	return o;
}

#endif
