//	point.hpp

#ifndef POINT_HPP
#define POINT_HPP

typedef int	Coord;

#ifdef FLOATINGPOINT
class FPoint;
#endif

#define sign(x) (x>0 ? 1:x<0 ? -1:0)
#define signF(x) (x>0.0 ? 1.0:x<0.0 ? -1.0:0.0)

class SOL_Point {
public:

	enum { DefaultCoord = 0x89ABCDEF };

	SOL_Point() : x(DefaultCoord), y(DefaultCoord) {}
	SOL_Point(Coord x, Coord y) : x(x), y(y) {}
	SOL_Point(const SOL_Point& p) : x(p.x), y(p.y) {}

   // copy operator
	SOL_Point&	operator=(const SOL_Point&);

   #ifdef FLOATINGPOINT
   // conversion operators
	SOL_Point&	operator=(const FPoint&);
   #endif

	SOL_Point&	operator+=(const SOL_Point&);
	SOL_Point&	operator-=(const SOL_Point&);
	Boolean		operator==(const SOL_Point&) const;
	Boolean		operator!=(const SOL_Point&) const;

	SOL_Point	operator+(const SOL_Point&) const;
	SOL_Point	operator-(const SOL_Point&) const;
	SOL_Point	operator-() const;
	SOL_Point	operator*(const int&) const;
	int         operator*(const SOL_Point&) const;
	int         operator^(const SOL_Point&) const;

	Coord	x;
	Coord	y;
};

struct SCI_Point {

	enum { DefaultCoord = 0xABCD };

	SCI_Point() : x((SCIWord) DefaultCoord), y((SCIWord) DefaultCoord) {}
	SCI_Point(Coord x, Coord y) : x(x), y(y) {}
	SCI_Point(const SCI_Point& p) : x(p.x), y(p.y) {}

   // this function was defined outside of struct, but CodeWarrior 1.1.1.2
   // complains about it being an illegal inline function definition.
	SCI_Point&	operator=(const SCI_Point& p) {x = p.x; y = p.y; return *this;}

   #ifdef FLOATINGPOINT
   // conversion operators
	SCI_Point&	operator=(const FPoint&);
   #endif

	SCI_Point&	operator+=(const SCI_Point&);
	SCI_Point&	operator-=(const SCI_Point&);
	Boolean			operator==(const SCI_Point&) const;
	Boolean			operator!=(const SCI_Point&) const;

	SCI_Point	operator+(const SCI_Point&) const;
	SCI_Point	operator-(const SCI_Point&) const;
	SCI_Point	operator-() const;
	SCI_Point	operator*(const int&) const;
	int         operator*(const SCI_Point&) const;
	int         operator^(const SCI_Point&) const;

	//	a coordinate must fit in a property
	SCIWord	x;
	SCIWord	y;
};

class FPoint {
public:

	#define DefaultCoord 7777.0

	FPoint() : x(DefaultCoord), y(DefaultCoord) {}
	FPoint(double x, double y) : x(x), y(y) {}
	FPoint(const FPoint& p) : x(p.x), y(p.y) {}

   // copy operator
	FPoint&	operator=(const FPoint&);

   // conversion operators
	FPoint&	operator=(const SOL_Point&);
	FPoint&	operator=(const SCI_Point&);

	FPoint&	operator+=(const FPoint&);
	FPoint&	operator-=(const FPoint&);
	Boolean	operator==(const FPoint&) const;
	Boolean	operator!=(const FPoint&) const;

	FPoint	operator+(const FPoint&) const;
	FPoint	operator-(const FPoint&) const;
	FPoint	operator-() const;
	FPoint	operator*(const double&) const;
	double   operator*(const FPoint&) const;
	int      operator^(const FPoint&) const;
	FPoint   operator|(const FPoint&) const;

	double	x;
	double	y;
};

inline SOL_Point&
SOL_Point::operator=(const SOL_Point& p)
{
	x = p.x;
	y = p.y;
	return *this;
}

inline SOL_Point
SOL_Point::operator+(const SOL_Point& p) const
{
   return SOL_Point(x+p.x,y+p.y);
}

inline SOL_Point
SOL_Point::operator-(const SOL_Point& p) const
{
   return SOL_Point(x-p.x,y-p.y);
}

inline SOL_Point
SOL_Point::operator-() const
{
   return SOL_Point(-x,-y);
}

// Scalar product
inline SOL_Point
SOL_Point::operator*(const int& p) const
{
   return SOL_Point(x*p,y*p);
}

// Dot product
inline int
SOL_Point::operator*(const SOL_Point& p) const
{
   return ((int)x*(int)p.x+(int)y*(int)p.y);
}

// Cross product
// Actually just returns the Z direction for (x1,y1,0) cross (x2,y2,0)
inline int
SOL_Point::operator^(const SOL_Point& p) const
{
   int direction;
   direction = (
               (((int) x) * ((int) p.y)) 
               - 
               (((int) y) * ((int) p.x))
               );
   if (direction < 0)
      return(-1);
   if (direction > 0)
      return(1);
   return(0);
}

inline SOL_Point&
SOL_Point::operator+=(const SOL_Point& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

inline SOL_Point&
SOL_Point::operator-=(const SOL_Point& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

inline Boolean
SOL_Point::operator==(const SOL_Point& p) const
{
	return x == p.x  &&  y == p.y;
}

inline Boolean
SOL_Point::operator!=(const SOL_Point& p) const
{
	return x != p.x  ||  y != p.y;
}

struct SCI_Rect {
	SCI_Point	ul;	// upper left
	SCI_Point	lr; // lower right
};

inline SCI_Point
SCI_Point::operator+(const SCI_Point& p) const
{
   return SCI_Point(x+p.x,y+p.y);
}

inline SCI_Point
SCI_Point::operator-(const SCI_Point& p) const
{
   return SCI_Point(x-p.x,y-p.y);
}

inline SCI_Point
SCI_Point::operator-() const
{
   return SCI_Point(-x,-y);
}

// Scalar product
inline SCI_Point
SCI_Point::operator*(const int& p) const
{
   return SCI_Point(x*p,y*p);
}

// Dot product
inline int
SCI_Point::operator*(const SCI_Point& p) const
{
   return ((int)x*(int)p.x+(int)y*(int)p.y);
}

// Cross product
// Actually just returns the Z direction for (x1,y1,0) cross (x2,y2,0)
inline int
SCI_Point::operator^(const SCI_Point& p) const
{
   int direction;
   direction = (
               (((int) x) * ((int) p.y)) 
               - 
               (((int) y) * ((int) p.x))
               );
   if (direction < 0)
      return(-1);
   if (direction > 0)
      return(1);
   return(0);
}

inline SCI_Point&
SCI_Point::operator+=(const SCI_Point& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

inline SCI_Point&
SCI_Point::operator-=(const SCI_Point& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

inline Boolean
SCI_Point::operator==(const SCI_Point& p) const
{
	return x == p.x  &&  y == p.y;
}

inline Boolean
SCI_Point::operator!=(const SCI_Point& p) const
{
	return x != p.x  ||  y != p.y;
}


#ifdef FLOATINGPOINT

// conversion operators
inline SOL_Point&
SOL_Point::operator=(const FPoint& p)
{
	x = (Coord) (p.x + signF(p.x)*0.5);
	y = (Coord) (p.y + signF(p.y)*0.5);
	return *this;
}

inline SCI_Point&
SCI_Point::operator=(const FPoint& p)
{
	x = (SCIWord) (p.x + signF(p.x)*0.5);
	y = (SCIWord) (p.y + signF(p.y)*0.5);
	return *this;
}

inline FPoint&
FPoint::operator=(const SOL_Point& p)
{
	x = (double) p.x;
	y = (double) p.y;
	return *this;
}

inline FPoint&
FPoint::operator=(const FPoint& p)
{
	x = p.x;
	y = p.y;
	return *this;
}

inline FPoint&
FPoint::operator=(const SCI_Point& p)
{
	x = (double) p.x;
	y = (double) p.y;
	return *this;
}

inline FPoint
FPoint::operator+(const FPoint& p) const
{
   return FPoint(x+p.x,y+p.y);
}

inline FPoint
FPoint::operator-(const FPoint& p) const
{
   return FPoint(x-p.x,y-p.y);
}

inline FPoint
FPoint::operator-() const
{
   return FPoint(-x,-y);
}

// Scalar product
inline FPoint
FPoint::operator*(const double& p) const
{
   return FPoint(x*p,y*p);
}

// Dot product
inline double
FPoint::operator*(const FPoint& p) const
{
   return x*p.x+y*p.y;
}

// Cross product
// Actually just returns the Z direction for (x1,y1,0) cross (x2,y2,0)
inline int
FPoint::operator^(const FPoint& p) const
{
   double direction = x*p.y - y*p.x;
   if (direction < 0.0)
      return(-1);
   if (direction > 0.0)
      return(1);
   return(0);
}

inline FPoint&
FPoint::operator+=(const FPoint& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

inline FPoint&
FPoint::operator-=(const FPoint& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

inline Boolean
FPoint::operator==(const FPoint& p) const
{
	return x == p.x  &&  y == p.y;
}

inline Boolean
FPoint::operator!=(const FPoint& p) const
{
	return x != p.x  ||  y != p.y;
}

#endif


#endif

