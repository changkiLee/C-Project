#pragma once
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdarg>
#include <iomanip>
#include <vector>

using namespace std;

// macro
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define SQRT(X)		sqrt((X))
#define SQR(X)		((X) * (X))
#define DEG2RAD(X)	((X) * (M_PI) / (180.0))
#define RAD2DEG(X)	((X) * (180.0) / (M_PI))
#define SWAP(type, x, y) { type temp = (x); (x) = (y); (y) = temp; }
#define MIN(x, y)	((x) > (y) ? (y) : (x))
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define ABS(X)		(((X) > 0.0) ? (X) : (-(X)))
#define SIGN(a)		((a) > 0.0 ? (1.0) : (-1.0))
#define SIGN1(a, b) ((b) > 0.0 ? ABS(a) : -ABS(a))
#define SIGN2(a, b)	((b) >= 0.0 ? fabs(a) : -fabs(a))
#define PYTHAG(a, b) SQRT((SQR(a) + SQR(b)))
#define EQ(X, Y, EPS)	(ABS((X) - (Y)) < EPS)
#define EQ_ZERO(X, EPS) (ABS(X) < EPS)
#define ARR_ZERO(A, N) memset((A), 0, sizeof(A[0]) * (N))
#define ARR_COPY(D, S, N) memmove((D), (S), sizeof(S[0]) * (N))

// classes
class GVector3;
class GVector;
class GPoint3;
class GMatrix;
class GLine;
class GPlane;

/*!
*	\class GVector3
*	\brief 3D Vector class
*/
class GVector3
{
	friend GVector3 operator +(const GVector3 &lhs, const GVector3 &rhs);
	friend GPoint3 operator +(const GVector3 &lhs, const GPoint3 &rhs);
	friend GVector3 operator -(const GVector3 &lhs, const GVector3 &rhs);
	friend GVector3 operator *(const GVector3 &lhs, const double &s);
	friend GVector3 operator *(const double &s, const GVector3 &rhs);
	friend double operator *(const GVector3 &lhs, const GVector3 &rhs);	// dot product
	friend GVector3 operator /(const GVector3 &lhs, const double &s);
	friend GVector3 operator ^(const GVector3 &lhs, const GVector3 &rhs);	// cross product
	friend bool operator ==(const GVector3 &lhs, const GVector3 &rhs);
	friend bool operator !=(const GVector3 &lhs, const GVector3 &rhs);
	friend ostream &operator <<(ostream &os, const GVector3 &v);
	friend istream &operator >> (istream &is, GVector3 &v);
	friend GVector3 proj(const GVector3 &v, const GVector3 &w);
	friend double dist(const GVector3 &v, const GVector3 &w);
	friend double norm(const GVector3 &v);
	friend double angle(const GVector3 &v, const GVector3 &w, bool radian = false);
	friend GPoint3 cast_pt3(const GVector3 &v);
	friend GPoint3 cast_pt3(const double *v);
	friend GVector cast_vec(const GVector3 &v);

public:
	GVector3(double x = 0.0, double y = 0.0, double z = 0.0);
	GVector3(const GVector3 &cpy);
	virtual ~GVector3();

	GVector3 &operator =(const GVector3 &rhs);
	GVector3 &operator +=(const GVector3 &rhs);
	GVector3 &operator -=(const GVector3 &rhs);
	GVector3 &operator *=(const double &s);
	GVector3 &operator /=(const double &s);
	GVector3 &operator ^=(const GVector3 &rhs);

	GVector3 operator +() const;
	GVector3 operator -() const;

	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	GVector3 &Set(const double &x, const double &y, const double &z);
	GVector3 &Normalize();

	static void SetPrecision(double error);
	static double GetPrecision();
	bool outside;
protected:
	double V[3];
	static double Precision;
};

inline
GVector3::GVector3(double x, double y, double z)
{
	V[0] = x;
	V[1] = y;
	V[2] = z;
}
inline
GVector3::GVector3(const GVector3 &cpy)
{
	V[0] = cpy.V[0];
	V[1] = cpy.V[1];
	V[2] = cpy.V[2];
}
inline
GVector3::~GVector3()
{
}
inline
GVector3 &GVector3::operator =(const GVector3 &rhs)
{
	V[0] = rhs.V[0];
	V[1] = rhs.V[1];
	V[2] = rhs.V[2];
	return *this;
}

inline
GVector3 &GVector3::operator +=(const GVector3 &rhs)
{
	V[0] += rhs.V[0];
	V[1] += rhs.V[1];
	V[2] += rhs.V[2];
	return *this;
}

inline
GVector3 &GVector3::operator -=(const GVector3 &rhs)
{
	V[0] -= rhs.V[0];
	V[1] -= rhs.V[1];
	V[2] -= rhs.V[2];
	return *this;
}
inline
GVector3 &GVector3::operator *=(const double &s)
{
	V[0] *= s;
	V[1] *= s;
	V[2] *= s;
	return *this;
}

inline
GVector3 &GVector3::operator /=(const double &s)
{
	V[0] /= s;
	V[1] /= s;
	V[2] /= s;
	return *this;
}

inline
GVector3 &GVector3::operator ^=(const GVector3 &rhs)
{
	double x = V[0], y = V[1], z = V[2];
	V[0] = y * rhs.V[2] - z * rhs.V[1];
	V[1] = z * rhs.V[0] - x * rhs.V[2];
	V[2] = x * rhs.V[1] - y * rhs.V[0];
	return *this;
}
inline
GVector3 GVector3::operator +() const
{
	return *this;
}

inline
GVector3 GVector3::operator -() const
{
	return *this * -1;
}

inline
GVector3 operator +(const GVector3 &lhs, const GVector3 &rhs)
{
	return GVector3(lhs.V[0] + rhs.V[0], lhs.V[1] + rhs.V[1], lhs.V[2] + rhs.V[2]);
}

inline
GVector3 operator -(const GVector3 &lhs, const GVector3 &rhs)
{
	return GVector3(lhs.V[0] - rhs.V[0], lhs.V[1] - rhs.V[1], lhs.V[2] - rhs.V[2]);
}

inline
double operator *(const GVector3 &lhs, const GVector3 &rhs)
{
	return lhs.V[0] * rhs.V[0] + lhs.V[1] * rhs.V[1] + lhs.V[2] * rhs.V[2];
}

inline
GVector3 operator /(const GVector3 &lhs, const double &s)
{
	return GVector3(lhs.V[0] / s, lhs.V[1] / s, lhs.V[2] / s);
}

inline
GVector3 operator ^(const GVector3 &lhs, const GVector3 &rhs)
{
	return GVector3(lhs.V[1] * rhs.V[2] - lhs.V[2] * rhs.V[1], lhs.V[2] * rhs.V[0] - lhs.V[0] * rhs.V[2], lhs.V[0] * rhs.V[1] - lhs.V[1] * rhs.V[0]);
}

inline
bool operator ==(const GVector3 &lhs, const GVector3 &rhs)
{
	double error = GVector3::Precision;
	return (EQ(lhs.V[0], rhs.V[0], error) && EQ(lhs.V[1], rhs.V[1], error) && EQ(lhs.V[2], rhs.V[2], error));
}

inline
bool operator !=(const GVector3 &lhs, const GVector3 &rhs)
{
	double error = GVector3::Precision;
	return (!EQ(lhs.V[0], rhs.V[0], error) || !EQ(lhs.V[1], rhs.V[1], error) || !EQ(lhs.V[2], rhs.V[2], error));
}

inline
double &GVector3::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 3);
	return V[idx];
}

inline
const double &GVector3::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 3);
	return V[idx];
}

inline
GVector3 &GVector3::Set(const double &x, const double &y, const double &z)
{
	V[0] = x;
	V[1] = y;
	V[2] = z;
	return *this;
}

inline
GVector3 &GVector3::Normalize()
{
	double len = norm(*this);
	if (EQ_ZERO(len, Precision))
		return *this;
	V[0] /= len;
	V[1] /= len;
	V[2] /= len;
	return *this;
}

inline
void GVector3::SetPrecision(double error)
{
	Precision = error;
}

inline
double GVector3::GetPrecision()
{
	return Precision;
}

inline
GVector3 operator *(const GVector3 &lhs, const double &s)
{
	GVector3 ret(lhs);
	ret *= s;
	return ret;
}

inline
GVector3 operator *(const double &s, const GVector3 &rhs)
{
	GVector3 ret(rhs);
	ret *= s;
	return ret;
}

inline
ostream &operator <<(ostream &os, const GVector3 &v)
{
	os << "(" << setw(10) << v.V[0] << ", " << setw(10) << v.V[1] << ", " << setw(10) << v.V[2] << ")";
	return os;
}

inline
istream &operator >> (istream &is, GVector3 &v)
{
	is >> v.V[0] >> v.V[1] >> v.V[2];
	return is;
}

inline
GVector3 proj(const GVector3 &v, const GVector3 &w)
{
	return (v * w / (w.V[0] * w.V[0] + w.V[1] * w.V[1] + w.V[2] * w.V[2])) * w;
}

inline
double dist(const GVector3 &v, const GVector3 &w)
{
	return norm(v - w);
}


inline double norm(const GVector3 &v)
{
	return SQRT(SQR(v.V[0]) + SQR(v.V[1]) + SQR(v.V[2]));
}

inline
double angle(const GVector3 &v, const GVector3 &w, bool radian)
{
	GVector3 p(v);
	GVector3 q(w);
	double cs, sn, theta;

	p.Normalize();
	q.Normalize();

	cs = p * q;
	sn = norm(p ^ q);

	theta = radian ? atan2(sn, cs) : RAD2DEG(atan2(sn, cs));
	return theta;
}

/*!
*	\class GVector
*	\brief N-D Vector Class
*/
class GVector
{
	friend GVector operator +(const GVector &lhs, const GVector &rhs);
	friend GVector operator -(const GVector &lhs, const GVector &rhs);
	friend double operator *(const GVector &lhs, const GVector &rsh);
	friend GVector operator *(const double &s, const GVector &rhs);
	friend GVector operator *(const GVector &lhs, const double &s);
	friend GVector operator *(const GMatrix &lhs, const GVector &rhs);
	friend GMatrix operator *(const GVector &lhs, const GMatrix &rhs);
	friend GVector operator /(const GVector &lhs, const double &s);
	friend bool operator ==(const GVector &lhs, const GVector &rhs);
	friend bool operator !=(const GVector &lhs, const GVector &rhs);
	friend ostream &operator <<(ostream &os, const GVector &v);
	friend istream &operator >> (istream &is, GVector &v);
	friend double norm(const GVector &v);
	friend double dist(const GVector &v, const GVector &w);
	friend double *cast_arr(const GVector &v);
	friend GPoint3 cast_pt3(const GVector &v);
	friend GVector3 cast_vec3(const GVector &v);
	friend class GMatrix;

public:
	GVector(int dim = 3);
	GVector(int dim, double x, ...);
	GVector(int dim, int x, ...);
	GVector(const GVector &cpy);
	virtual ~GVector();

	GVector &operator =(const GVector &rhs);
	GVector &operator +=(const GVector &rhs);
	GVector &operator -=(const GVector &rhs);
	GVector &operator *=(const double &s);
	GVector	&operator /=(const double &s);

	GVector operator +() const;
	GVector operator -() const;

	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	GVector	&Set(double x, ...);
	GVector	&Set(int x, ...);
	GVector &Set(double *pVal);
	GVector	&Normalize();
	GVector &SetZeros();
	int GetDim() const;

	static void	SetPrecision(double error);
	static double GetPrecision();

protected:
	int N;
	double *V;
	static double Precision;
};

inline
GVector::GVector(int dim)
{
	this->N = dim;
	V = new double[N];
	ARR_ZERO(V, N);
}

inline
GVector::GVector(int dim, double x, ...)
{
	this->N = dim;
	V = new double[N];
	va_list ap;
	va_start(ap, dim);
	for (int i = 0; i < N; i++)
		V[i] = va_arg(ap, double);
	va_end(ap);
}

inline
GVector::GVector(int dim, int x, ...)
{
	this->N = dim;
	V = new double[N];
	va_list ap;
	va_start(ap, dim);
	for (int i = 0; i < N; i++)
		V[i] = (double)va_arg(ap, int);
	va_end(ap);
}

inline
GVector::GVector(const GVector &cpy)
{
	N = cpy.N;
	V = new double[N];
	memcpy(V, cpy.V, N * sizeof(double));
}

inline
GVector::~GVector()
{
	if (V)
		delete[] V;
	V = NULL;
}

inline
GVector &GVector::operator =(const GVector &rhs)
{
	if (V)
		delete[] V;
	N = rhs.N;
	V = new double[N];
	memcpy(V, rhs.V, N * sizeof(double));
	return *this;
}

inline
GVector &GVector::operator +=(const GVector &rhs)
{
	assert(N == rhs.N);
	for (int i = 0; i != N; ++i)
		V[i] += rhs.V[i];
	return *this;
}

inline
GVector &GVector::operator -=(const GVector &rhs)
{
	assert(N == rhs.N);
	for (int i = 0; i != N; ++i)
		V[i] -= rhs.V[i];
	return *this;
}

inline
GVector &GVector::operator *=(const double &s)
{
	for (int i = 0; i != N; ++i)
		V[i] *= s;
	return *this;
}

inline
GVector &GVector::operator /=(const double &s)
{
	assert(s != 0);
	for (int i = 0; i != N; ++i)
		V[i] /= s;
	return *this;
}

inline
GVector GVector::operator +() const
{
	return *this;
}

inline
GVector GVector::operator -() const
{
	return *this * -1;
}

inline
GVector operator +(const GVector &lhs, const GVector &rhs)
{
	assert(lhs.N == rhs.N);
	GVector ret(lhs.N);
	for (int i = 0; i != lhs.N; ++i)
		ret.V[i] = lhs.V[i] + rhs.V[i];
	return ret;
}

inline
GVector operator -(const GVector &lhs, const GVector &rhs)
{
	assert(lhs.N == rhs.N);
	GVector ret(lhs.N);
	for (int i = 0; i != lhs.N; ++i)
		ret.V[i] = lhs.V[i] - rhs.V[i];
	return ret;
}

inline
double operator *(const GVector &lhs, const GVector &rhs)
{
	assert(lhs.N == rhs.N);
	double ret = 0;
	for (int i = 0; i != lhs.N; ++i)
		ret += lhs.V[i] * rhs.V[i];
	return ret;
}

inline
GVector operator /(const GVector &lhs, const double &s)
{
	GVector ret(lhs.N);
	for (int i = 0; i != lhs.N; ++i)
		ret.V[i] = lhs.V[i] / s;
	return ret;
}

inline
bool operator ==(const GVector &lhs, const GVector &rhs)
{
	assert(lhs.N == rhs.N);
	double error = GVector::Precision;
	for (int i = 0; i != lhs.N; ++i)
		if (!EQ(lhs.V[i], rhs.V[i], error))
			return false;
	return true;
}

inline
bool operator !=(const GVector &lhs, const GVector &rhs)
{
	assert(lhs.N == rhs.N);
	double error = GVector::Precision;
	for (int i = 0; i != lhs.N; ++i)
		if (!EQ(lhs.V[i], rhs.V[i], error))
			return true;
	return false;
}

inline
double &GVector::operator [](const int &idx)
{
	assert(idx >= 0 && idx < N);
	return V[idx];
}

inline
const double &GVector::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < N);
	return V[idx];
}

inline
GVector &GVector::Set(double x, ...)
{
	V[0] = x;
	va_list ap;
	va_start(ap, x);
	for (int i = 1; i != N; ++i)
		V[i] = va_arg(ap, double);
	va_end(ap);
	return *this;
}

inline
GVector &GVector::Set(int x, ...)
{
	V[0] = (double)x;
	va_list ap;
	va_start(ap, x);
	for (int i = 1; i != N; ++i)
		V[i] = (double)va_arg(ap, int);
	va_end(ap);
	return *this;
}

inline
GVector &GVector::Set(double *pVal)
{
	memcpy(V, pVal, sizeof(double) * N);
	return *this;
}

inline
GVector &GVector::Normalize()
{
	double len = norm(*this);
	for (int i = 0; i != N; ++i)
		V[i] /= len;
	return *this;
}

inline
GVector &GVector::SetZeros()
{
	for (int i = 0; i != N; ++i)
		V[i] = 0.0;
	return *this;
}

inline
int GVector::GetDim() const
{
	return N;
}

inline
void GVector::SetPrecision(double error)
{
	Precision = error;
}

inline
double GVector::GetPrecision()
{
	return Precision;
}

inline
GVector operator *(const GVector &lhs, const double &s)
{
	GVector ret(lhs.N);
	for (int i = 0; i != lhs.N; ++i)
		ret.V[i] = lhs.V[i] * s;
	return ret;
}

inline
GVector operator *(const double &s, const GVector &rhs)
{
	GVector ret(rhs.N);
	for (int i = 0; i != rhs.N; ++i)
		ret.V[i] = rhs.V[i] * s;
	return ret;
}

inline
ostream &operator <<(ostream &os, const GVector &v)
{
	for (int i = 0; i != v.N; ++i)
		if (i != v.N - 1)
			os << "| " << setw(10) << v.V[i] << " |" << endl;
		else
			os << "| " << setw(10) << v.V[i] << " |";
	return os;
}

inline
istream &operator >> (istream &is, GVector &v)
{
	for (int i = 0; i != v.N; ++i)
		is >> v.V[i];
	return is;
}

inline
double norm(const GVector &v)
{
	double len = 0;
	for (int i = 0; i != v.N; ++i)
		len += SQR(v.V[i]);
	len = SQRT(len);
	return len;
}

inline
double dist(const GVector &v, const GVector &w)
{
	return norm(v - w);
}

/*!
*	\class GPoint3
*	\brief 3D Point Class
*/
class GPoint3
{
	friend GVector3 operator -(const GPoint3 &lhs, const GPoint3 &rhs);
	friend GPoint3 operator -(const GPoint3 &lhs, const GVector3 &rhs);
	friend GPoint3 operator +(const GPoint3 &lhs, const GVector3 &rhs);
	friend GPoint3 operator +(const GVector3 &lhs, const GPoint3 &rhs);
	friend bool operator ==(const GPoint3 &lhs, const GPoint3 &rhs);
	friend bool operator !=(const GPoint3 &lhs, const GPoint3 &rhs);
	friend ostream &operator <<(ostream &os, const GPoint3 &p);
	friend istream &operator >> (istream &is, GPoint3 &p);
	friend double norm(const GPoint3 &p);
	friend double dist(const GPoint3 &p, const GPoint3 &q);
	friend GPoint3 barycentric_combination(GPoint3 *Points, double *Weights, const int Size);
	friend GVector3 cast_vec3(const GPoint3 &p);
	friend GVector  cast_vec(const GPoint3 &p);
	friend GVector3 cast_vec3(const double *p);

public:
	GPoint3(double x = 0.0, double y = 0.0, double z = 0.0);
	GPoint3(const GPoint3 &cpy);
	virtual ~GPoint3();

	GPoint3 &operator =(const GPoint3 &rhs);

	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	GPoint3 &Set(const double &x, const double &y, const double &z);

	static void SetPrecision(double error);
	static double GetPrecision();

protected:
	double V[3];
	static double Precision;
};

inline
GPoint3::GPoint3(double x, double y, double z)
{
	V[0] = x;
	V[1] = y;
	V[2] = z;
}

inline
GPoint3::GPoint3(const GPoint3 &cpy)
{
	V[0] = cpy.V[0];
	V[1] = cpy.V[1];
	V[2] = cpy.V[2];
}

inline
GPoint3::~GPoint3()
{
}

inline
GPoint3 &GPoint3::operator =(const GPoint3 &rhs)
{
	V[0] = rhs.V[0];
	V[1] = rhs.V[1];
	V[2] = rhs.V[2];
	return *this;
}

inline
GVector3 operator -(const GPoint3 &lhs, const GPoint3 &rhs)
{
	return GVector3(lhs.V[0] - rhs.V[0], lhs.V[1] - rhs.V[1], lhs.V[2] - rhs.V[2]);
}

inline
GPoint3 operator -(const GPoint3 &lhs, const GVector3 &rhs)
{
	return GPoint3(lhs.V[0] - rhs[0], lhs.V[1] - rhs[1], lhs.V[2] - rhs[2]);
}

inline
GPoint3 operator +(const GPoint3 &lhs, const GVector3 &rhs)
{
	return GPoint3(lhs.V[0] + rhs[0], lhs.V[1] + rhs[1], lhs.V[2] + rhs[2]);
}

inline
GPoint3 operator +(const GVector3 &lhs, const GPoint3 &rhs)
{
	return GPoint3(lhs.V[0] + rhs[0], lhs.V[1] + rhs[1], lhs.V[2] + rhs[2]);
}

inline
bool operator ==(const GPoint3 &lhs, const GPoint3 &rhs)
{
	double error = GPoint3::Precision;
	return (EQ(lhs.V[0], rhs.V[0], error) && EQ(lhs.V[1], rhs.V[1], error) && EQ(lhs.V[2], rhs.V[2], error));
}

inline
bool operator !=(const GPoint3 &lhs, const GPoint3 &rhs)
{
	double error = GPoint3::Precision;
	return (!EQ(lhs.V[0], rhs.V[0], error) || !EQ(lhs.V[1], rhs.V[1], error) || !EQ(lhs.V[2], rhs.V[2], error));
}

inline
double &GPoint3::operator [](const int &idx)
{
	assert(idx >= 0 && idx < 3);
	return V[idx];
}

inline
const double &GPoint3::operator [](const int &idx) const
{
	assert(idx >= 0 && idx < 3);
	return V[idx];
}

inline
GPoint3 &GPoint3::Set(const double &x, const double &y, const double &z)
{
	V[0] = x;
	V[1] = y;
	V[2] = z;
	return *this;
}

inline
void GPoint3::SetPrecision(double error)
{
	Precision = error;
}

inline
double GPoint3::GetPrecision()
{
	return Precision;
}

inline
ostream &operator <<(ostream &os, const GPoint3 &p)
{
	os << "(" << setw(5) << p.V[0] << ", " << setw(5) << p.V[1] << ", " << setw(5) << p.V[2] << ")";
	return os;
}

inline
istream &operator >> (istream &is, GPoint3 &p)
{
	is >> p.V[0] >> p.V[1] >> p.V[2];
	return is;
}

inline
double norm(const GPoint3 &p)
{
	return SQRT(SQR(p.V[0]) + SQR(p.V[1]) + SQR(p.V[2]));
}

inline
double dist(const GPoint3 &p, const GPoint3 &q)
{
	return SQRT(SQR(p.V[0] - q.V[0]) + SQR(p.V[1] - q.V[1]) + SQR(p.V[2] - q.V[2]));
}

inline
GVector3 cast_vec3(const GPoint3 &pt)
{
	return GVector3(pt[0], pt[1], pt[2]);
}

inline
GVector cast_vec(const GPoint3 &pt)
{
	return GVector(3, pt[0], pt[1], pt[2]);
}

inline GVector3 cast_vec3(const double *p)
{
	return GVector3(p[0], p[1], p[2]);
}

inline
GPoint3 cast_pt3(const GVector3 &v)
{
	return GPoint3(v[0], v[1], v[2]);
}

inline GPoint3 cast_pt3(const double * v)
{
	return GPoint3(v[0], v[1], v[2]);
}

inline
GVector cast_vec(const GVector3 &v)
{
	return GVector(3, v[0], v[1], v[2]);
}

inline
GPoint3 barycentric_combination(GPoint3 *Points, double *Weights, const int Size)
{
	GPoint3 ret;
	for (int i = 0; i < Size; i++)
	{
		ret.V[0] += Points[i][0] * Weights[i];
		ret.V[1] += Points[i][1] * Weights[i];
		ret.V[2] += Points[i][2] * Weights[i];
	}
	return ret;
}

/*!
*	\class GMatrix
*	\brief m-row n-col matrix class
*/
class GMatrix
{
	friend GMatrix operator +(const GMatrix &lhs, const GMatrix &rhs);
	friend GMatrix operator -(const GMatrix &lhs, const GMatrix &rhs);
	friend GMatrix operator *(const GMatrix &lhs, const GMatrix &rhs);
	friend GMatrix operator /(const GMatrix &lhs, const double &s);
	friend bool operator ==(const GMatrix &lhs, const GMatrix &rhs);
	friend bool operator !=(const GMatrix &lhs, const GMatrix &rhs);
	friend GVector operator *(const GMatrix &lhs, const GVector &rhs);
	friend GMatrix operator *(const GVector &lhs, const GMatrix &rhs);
	friend GMatrix operator *(const GMatrix &lhs, const double &s);
	friend GMatrix operator *(const double &s, const GMatrix &rhs);
	friend ostream &operator <<(ostream &os, const GMatrix &m);
	friend GMatrix tr(const GMatrix &m);
	friend double *cast_arr(const GMatrix &m);
	
public:
	GMatrix(int row = 4, int col = 4, double *elem = NULL);
	GMatrix(const GMatrix &cpy);
	virtual ~GMatrix();

	GMatrix &operator =(const GMatrix &rhs);
	GMatrix &operator +=(const GMatrix &rhs);
	GMatrix &operator -=(const GMatrix &rhs);
	GMatrix &operator *=(const GMatrix &rhs);
	GMatrix &operator *=(const double &s);
	GMatrix &operator /=(const double &s);

	GMatrix operator +() const;
	GMatrix operator -() const;

	double *operator [](const int idx);
	const double *operator [](const int idx) const;

	GMatrix &SetRowVec(const int idx, const GVector &v);
	GMatrix &SetColVec(const int idx, const GVector &v);
	GMatrix &SetTranspose();
	GMatrix &SetIdentity();
	GMatrix &SetZeros();
	int GetRowNum() const;
	int GetColNum() const;
	bool IsSquare() const;

	static void SetPrecision(double error);
	static double GetPrecision();

protected:
	int r;		// row
	int c;		// col
	double *M;	// elements
	static double Precision;
};

inline
GMatrix::GMatrix(int row, int col, double *elem)
{
	r = row;
	c = col;
	M = new double[r * c];
	if (elem)
		memcpy(M, elem, sizeof(double) * r * c);
	else
		memset(M, 0, sizeof(double) * r * c);
}

inline
GMatrix::GMatrix(const GMatrix &cpy)
{
	r = cpy.r;
	c = cpy.c;
	M = new double[r *c];
	memcpy(M, cpy.M, sizeof(double) * r * c);
}

inline
GMatrix::~GMatrix()
{
	if (M)
		delete[] M;
	M = NULL;
}

inline
GMatrix &GMatrix::operator =(const GMatrix &rhs)
{
	if (M)
		delete[] M;

	r = rhs.r;
	c = rhs.c;
	M = new double[r * c];

	memcpy(M, rhs.M, sizeof(double) * r * c);
	return *this;
}

inline
GMatrix &GMatrix::operator +=(const GMatrix &rhs)
{
	assert(r == rhs.r && c == rhs.c);
	for (int i = 0; i < r * c; ++i)
		M[i] += rhs.M[i];
	return *this;
}

inline
GMatrix &GMatrix::operator -=(const GMatrix &rhs)
{
	assert(r == rhs.r && c == rhs.c);
	for (int i = 0; i < r * c; ++i)
		M[i] -= rhs.M[i];
	return *this;
}

inline
GMatrix &GMatrix::operator *=(const GMatrix &rhs)
{
	assert(c == rhs.r);

	c = rhs.c;
	double *newM = new double[r * c];
	memset(newM, 0, sizeof(double) * r * c);

	for (int i = 0; i != r; ++i)
		for (int j = 0; j != c; ++j)
			for (int k = 0; k != rhs.r; ++k)
				newM[i * c + j] += M[i * rhs.r + k] * rhs.M[k * rhs.c + j];

	delete[] M;
	M = newM;

	return *this;
}

inline
GMatrix &GMatrix::operator *=(const double &s)
{
	for (int i = 0; i < r * c; i++)
		M[i] *= s;
	return *this;
}

inline
GMatrix &GMatrix::operator /=(const double &s)
{
	for (int i = 0; i < r * c; i++)
		M[i] /= s;
	return *this;
}

inline
GMatrix GMatrix::operator +() const
{
	return *this;
}

inline
GMatrix GMatrix::operator -() const
{
	return *this * -1;
}

inline
GMatrix operator +(const GMatrix &lhs, const GMatrix &rhs)
{
	assert(lhs.r == rhs.r && lhs.c == rhs.c);
	GMatrix ret(lhs.r, lhs.c);
	ret += rhs;
	return ret;
}

inline
GMatrix operator -(const GMatrix &lhs, const GMatrix &rhs)
{
	assert(lhs.r == rhs.r && lhs.c == rhs.c);
	GMatrix ret(lhs);
	ret -= rhs;
	return ret;
}

inline
GMatrix operator *(const GMatrix &lhs, const GMatrix &rhs)
{
	assert(lhs.c == rhs.r);
	GMatrix ret(lhs);
	ret *= rhs;
	return ret;
}

inline
GMatrix operator /(const GMatrix &lhs, const double &s)
{
	GMatrix ret(lhs);
	ret /= s;
	return ret;
}

inline
bool operator ==(const GMatrix &lhs, const GMatrix &rhs)
{
	assert(lhs.r == rhs.r && lhs.c == rhs.c);
	double error = GMatrix::Precision;
	for (int i = 0; i != lhs.r * lhs.c; ++i)
		if (!EQ(lhs.M[i], rhs.M[i], error))
			return false;
	return true;
}

inline
bool operator !=(const GMatrix &lhs, const GMatrix &rhs)
{
	assert(lhs.r == rhs.r && lhs.c == rhs.c);
	double error = GMatrix::Precision;
	for (int i = 0; i != lhs.r * lhs.c; ++i)
		if (EQ(lhs.M[i], rhs.M[i], error))
			return true;
	return false;
}

inline
double *GMatrix::operator [](const int idx)
{
	assert(idx >= 0 && idx < r);
	return &M[idx * c];
}

inline
const double *GMatrix::operator [](const int idx) const
{
	assert(idx >= 0 && idx < r);
	return &M[idx * c];
}
inline
GMatrix &GMatrix::SetRowVec(const int idx, const GVector &v)
{
	assert(idx < r);
	assert(v.N == c);
	for (int i = 0; i < c; i++)
		M[idx * c + i] = v.V[i];
	return *this;
}

inline
GMatrix &GMatrix::SetColVec(const int idx, const GVector &v)
{
	assert(idx < c);
	assert(v.N == r);
	for (int i = 0; i < r; i++)
		M[i * c + idx] = v.V[i];
	return *this;
}
inline
GMatrix &GMatrix::SetTranspose()
{
	int i, j;
	if (r == c)	// Square matrix
	{
		for (i = 0; i != r; ++i)
			for (j = i + 1; j != c; ++j)
				SWAP(double, M[i * c + j], M[j * c + i]);
	}
	else		// Rectangular matrix
	{
		double *buffer = new double[r * c];
		memcpy(buffer, M, sizeof(double) * r * c);
		SWAP(int, r, c);
		for (i = 0; i != r; ++i)
			for (j = 0; j != c; ++j)
				M[i * c + j] = buffer[j * r + i];
		delete[] buffer;
	}
	return *this;
}

inline
GMatrix &GMatrix::SetIdentity()
{
	memset(M, 0, sizeof(double) * r * c);
	int min = MIN(r, c);
	for (int i = 0; i < min; i++)
		M[i * c + i] = 1.0;
	return *this;
}

inline
GMatrix &GMatrix::SetZeros()
{
	memset(M, 0, sizeof(double) * r * c);
	return *this;
}

inline
int GMatrix::GetRowNum() const
{
	return r;
}

inline
int GMatrix::GetColNum() const
{
	return c;
}

inline
bool GMatrix::IsSquare() const
{
	return (r == c) ? true : false;
}

inline
void GMatrix::SetPrecision(double error)
{
	Precision = error;
}

inline
double GMatrix::GetPrecision()
{
	return Precision;
}

inline
GVector operator *(const GMatrix &lhs, const GVector &rhs)
{
	assert(lhs.c == rhs.N);
	GVector ret(lhs.r);
	for (int i = 0; i != lhs.r; ++i)		// for each row.
		for (int j = 0; j != lhs.c; ++j)	// for each col.
			ret.V[i] += lhs.M[i * lhs.c + j] * rhs.V[j];
	return ret;
}

inline
GMatrix operator *(const GVector &lhs, const GMatrix &rhs)
{
	assert(rhs.r == 1);
	GMatrix ret(lhs.N, rhs.c);
	for (int i = 0; i != lhs.N; ++i)		// for each row.
		for (int j = 0; j != rhs.c; ++j)	// for each col.
			ret.M[i * rhs.c + j] = lhs.V[i] * rhs.M[j];
	return ret;
}

inline
GMatrix operator *(const GMatrix &lhs, const double &s)
{
	GMatrix ret(lhs);
	ret *= s;
	return ret;
}

inline
GMatrix operator *(const double &s, const GMatrix &rhs)
{
	GMatrix ret(rhs);
	ret *= s;
	return ret;
}

inline
ostream &operator <<(ostream &os, const GMatrix &m)
{
	for (int i = 0; i != m.r; i++)
	{
		os << "|";
		for (int j = 0; j != m.c; j++)
			os << setw(5) << m.M[i * m.c + j] << " ";
		os << "|" << endl;
	}
	return os;
}

inline GMatrix tr(const GMatrix &m)
{
	GMatrix ret(m);
	ret.SetTranspose();
	return ret;
}

inline
double *cast_arr(const GMatrix &m)
{
	return m.M;
}

inline
double *cast_arr(const GVector &v)
{
	return v.V;
}

inline
GVector3 cast_vec3(const GVector &v)
{
	return GVector3(v[0], v[1], v[2]);
}

inline
GPoint3 cast_pt3(const GVector &v)
{
	return GPoint3(v[0], v[1], v[2]);
}

/*!
*	\class GLine
*	\brief Line in 3D
*/
class GLine
{
	friend bool operator ==(const GLine &lhs, const GLine &rhs);
	friend bool operator !=(const GLine &lhs, const GLine &rhs);
	friend ostream &operator <<(ostream &os, const GLine &l);
	friend double dist(const GLine &l, const GPoint3 &q);
	friend bool intersect_line_plane(GPoint3 &p, const GLine &l, const GPlane &pi);
	friend bool intersect_line_triangle(GPoint3 &q, const GLine &l, const GPoint3 &p1, const GPoint3 &p2, const GPoint3 &p3);

public:
	GLine(const GPoint3 &_p = GPoint3(0, 0, 0), const GVector3 &_v = GVector3(0, 0, 0), const bool& _outside = true, const double& _idxr = 1.003);
	GLine(const GPoint3 &_p, const GPoint3 &_q, const bool& _outside = true, const double& _idxr = 1.003);
	GLine(const GLine &cpy);
	virtual ~GLine();

	GLine &operator =(const GLine &rhs);

	GLine &SetPt(const GPoint3 &_p);
	GLine &SetDir(const GVector3 &_v);
	GPoint3 GetPt() const;
	GVector3 GetDir() const;
	bool IsOnLine(const GPoint3 &q) const;
	GPoint3 Eval(const double t) const;

	static void SetPrecision(double err);
	static double GetPrecision();
	bool outside;
	double idxr;
protected:
	GPoint3 p;
	GVector3 v;		// l(t) = p + v * t
	static double Precision;
};

inline
GLine::GLine(const GPoint3 &_p, const GVector3 &_v, const bool& _outside, const double& _idxr)
	: p(_p), v(_v), outside(_outside), idxr(_idxr)
{

}

inline
GLine::GLine(const GPoint3 &_p, const GPoint3 &_q, const bool& _outside, const double& _idxr) : outside(_outside), idxr(_idxr)
{
	p = _p;
	v = _q - _p;
}

inline
GLine::GLine(const GLine &copy)
	: p(copy.p), v(copy.v), outside(copy.outside), idxr(copy.idxr)
{
}

inline
GLine::~GLine()
{
}

inline
GLine &GLine::operator =(const GLine &rhs)
{
	this->p = rhs.p;
	this->v = rhs.v;
	this->outside = rhs.outside;
	this->idxr = rhs.idxr;
	return *this;
}

inline
GPoint3 GLine::Eval(const double t) const
{
	return p + t * v;
}

inline
bool operator ==(const GLine &lhs, const GLine &rhs)
{
	if (lhs.IsOnLine(rhs.p) && EQ_ZERO(norm(lhs.v ^ rhs.v), GLine::Precision))
		return true;
	else
		return false;
}

inline
bool operator !=(const GLine &lhs, const GLine &rhs)
{
	return !(lhs == rhs);
}

inline
bool GLine::IsOnLine(const GPoint3 &q) const
{
	GVector3 w = q - p;
	double s = norm(v ^ w);
	if (EQ_ZERO(s, Precision))
		return true;
	else
		return false;
}

inline
ostream &operator <<(ostream &os, const GLine &l)
{
	os << "("
		<< l.p[0] << " + (" << l.v[0] << ") * t, "
		<< l.p[1] << " + (" << l.v[1] << ") * t, "
		<< l.p[2] << " + (" << l.v[2] << ") * t)";
	return os;
}

inline
double dist(const GLine &l, const GPoint3 &q)
{
	return norm(proj(q - l.p, l.v) - (q - l.p));
}

inline
GLine &GLine::SetPt(const GPoint3 &_p)
{
	p = _p;
	return *this;
}

inline
GLine &GLine::SetDir(const GVector3 &_v)
{
	v = _v;
	return *this;
}

inline
GPoint3 GLine::GetPt() const
{
	return p;
}

inline
GVector3 GLine::GetDir() const
{
	return v;
}

inline
void GLine::SetPrecision(double err)
{
	Precision = err;
}

inline
double GLine::GetPrecision()
{
	return Precision;
}

/*!
*	\class GPlane
*	\brief Plan Class
*/
class GPlane
{
	friend bool operator ==(const GPlane &lhs, const GPlane &rhs);
	friend bool operator !=(const GPlane &lhs, const GPlane &rhs);
	friend ostream &operator <<(ostream &os, const GPlane &pi);
	friend double dist(const GPlane &pi, const GPoint3 &p);
	friend GPoint3 proj(const GPoint3 &p, const GPlane &pi);
	friend bool intersect_line_plane(GPoint3 &p, const GLine &l, const GPlane &pi);

public:
	GPlane(const GVector3 &_n, const GPoint3 &_p);
	GPlane(const GPoint3 &p1, const GPoint3 &p2, const GPoint3 &p3);
	GPlane(const GVector3 &_n, const double &_d);
	GPlane(const double &a, const double &b, const double &c, const double &d);
	GPlane(const GPlane &cpy);
	virtual ~GPlane();

	GPlane &operator =(const GPlane &rhs);

	GVector3 GetNormal() const;
	double GetD() const;
	bool IsOnPlane(const GPoint3 &p) const;
	bool IsAbovePlane(const GPoint3 &p) const;
	bool IsBelowPlane(const GPoint3 &p) const;

	static void SetPrecision(const double err);
	static double GetPrecision();

protected:
	GVector3 n;		// n = (a, b, c) from ax + by + cz + d = 0 
	double d;		// d = -n * p (p: a point on the plane)
	static double Precision;
};

inline
double GPlane::GetD() const
{
	return d;
}
inline
GPlane::GPlane(const GVector3 &_n, const GPoint3 &_p)
{
	n = _n;
	d = -(n[0] * _p[0] + n[1] * _p[1] + n[2] * _p[2]);
}

inline
GPlane::GPlane(const GPoint3 &p1, const GPoint3 &p2, const GPoint3 &p3)
{
	n = (p2 - p1) ^ (p3 - p1);
	d = -(n[0] * p1[0] + n[1] * p1[1] + n[2] * p1[2]);
}

inline
GPlane::GPlane(const GPlane &cpy)
	: n(cpy.n), d(cpy.d)
{
}

inline
GPlane::GPlane(const GVector3 &_n, const double &_d)
	: n(_n), d(_d)
{
}

inline
GPlane::GPlane(const double &a, const double &b, const double &c, const double &d)
{
	this->n = GVector3(a, b, c);
	this->n.Normalize();
	this->d = d;
}

inline
GPlane::~GPlane()
{
}

inline
GPlane &GPlane::operator =(const GPlane &rhs)
{
	this->n = rhs.n;
	this->d = rhs.d;
	return *this;
}

inline
bool operator ==(const GPlane &lhs, const GPlane &rhs)
{
	GVector pi1(4), pi2(4);
	pi1.Set(lhs.n[0], lhs.n[1], lhs.n[2], lhs.d);
	pi2.Set(rhs.n[0], rhs.n[1], rhs.n[2], rhs.d);
	double s1 = norm(pi1) * norm(pi2);
	double s2 = pi1 * pi2;

	if (EQ(s1, s2, GPlane::Precision))
		return true;
	else
		return false;
}

inline
bool operator !=(const GPlane &lhs, const GPlane &rhs)
{
	return !(lhs == rhs);
}

inline
GVector3 GPlane::GetNormal() const
{
	return n;
}

inline
bool GPlane::IsOnPlane(const GPoint3 &p) const
{
	double s;
	s = -(n[0] * p[0] + n[1] * p[1] + n[2] * p[2]);
	if (EQ(s, d, GPlane::Precision))
		return true;
	else
		return false;
}

inline
bool GPlane::IsAbovePlane(const GPoint3 &p) const
{
	double s;
	s = n[0] * p[0] + n[1] * p[1] + n[2] * p[2] + d;
	if (s > 0.0)
		return true;
	else
		return false;
}

inline
bool GPlane::IsBelowPlane(const GPoint3 &p) const
{
	double s;
	s = n[0] * p[0] + n[1] * p[1] + n[2] * p[2] + d;
	if (s < 0.0)
		return true;
	else
		return false;
}

inline
void GPlane::SetPrecision(const double err)
{
	Precision = err;
}

inline
double GPlane::GetPrecision()
{
	return Precision;
}

inline
ostream &operator <<(ostream &os, const GPlane &pi)
{
	os << "(" << pi.n[0] << ") * x + ("
		<< pi.n[1] << ") * y + ("
		<< pi.n[2] << ") * z + ("
		<< pi.d << ") = 0";
	return os;
}

inline
double dist(const GPlane &pi, const GPoint3 &p)
{
	double D;
	D = (p[0] * pi.n[0] + p[1] * pi.n[1] + p[2] * pi.n[2] + pi.d) / norm(pi.n);
	return D;
}

inline
GPoint3 proj(const GPoint3 &p, const GPlane &pi)
{
	double s = -dist(pi, p) / norm(pi.n);
	return p + s * pi.n;
}

inline
bool intersect_line_plane(GPoint3 &p, const GLine &l, const GPlane &pi)
{
	if (EQ_ZERO(l.v * pi.n, GPlane::Precision))
	{
		cout << "line is parallel to plane !" << endl;
		return false;
	}

	double t = -(l.p[0] * pi.n[0] + l.p[1] * pi.n[1] + l.p[2] * pi.n[2] + pi.d) / (l.v * pi.n);
	p = l.Eval(t);
	return true;
}

/*!
*	\brief	calculate an intersection between a line and a triangle(p1p2p3)
*
*	\param q intersect point
*	\param l line
*	\param p1 vertex of the triangle
*	\param p2 vertex of the triangle
*	\param p3 vertex of the triangle
*	\param t  q = l(t)
*	\param alpha barycentric coordinate
*	\param beta barycentric coordinate
*	\param bCull to avoid when if a line meets the back face of triangle (default: true).
*
*	\return true: success, false: parallel or out side of triangle
*/
inline
bool intersect_line_triangle(GPoint3 &q, const GLine &l, const GPoint3 &p1, const GPoint3 &p2, const GPoint3 &p3)
{
	//GVector3 e1 = p2 - p1;
	//GVector3 e2 = p3 - p1;

	//GVector3 u, v, w;
	//u = l.v ^ e2;
	//w = l.p - p1;
	//v = w ^ e1;
	//double det, alpha, beta, t;
	//det = e1 * u;

	//if (det > -GLine::Precision && det < GLine::Precision)
	//	return false;

	//alpha = w * u;
	//if (alpha < 0.0 || alpha > det)
	//	return false;

	//beta = l.v * v;
	//if (beta < 0.0 || alpha + beta > det)
	//	return false;

	//t = e2 * v / det;
	//q = l.Eval(t);

	GVector3 v0v1 = p2 - p1;
	GVector3 v0v2 = p3 - p1;
	GVector3 pvec = l.v ^ v0v2;
	float det = v0v1 * pvec;

	if (ABS(det) < GLine::Precision)
		return false;

	float invDet = 1 / det;
	GVector3 tvec = l.p - p1;
	float u = tvec * pvec * invDet;
	if (u < 0 || u > 1)
		return false;

	GVector3 qvec = tvec ^ v0v1;
	float v = l.v * qvec * invDet;
	if (v < 0 || u + v > 1)
		return false;

	float t = v0v2 * qvec * invDet;
	if (t < GLine::Precision)
		return false;

	q = l.Eval(t);

	return true;
}