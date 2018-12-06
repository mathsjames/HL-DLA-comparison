#include <math.h>
#include "cx.h"
#define TWO30  1073741824

/* Complex arithmetic and utilities:  
	add, sub, mult, div, recip, cabs, infnorm, polar, csqrt, 
	cexp, clog, cxrand, contsqrt, disk_to_sphere, mobius.
*/

complex add (z, w)
	complex z, w;
{
	complex t;
	t.x = z.x + w.x;
	t.y = z.y + w.y;
	return(t);
}

complex sub (z, w)
	complex z, w;
{
	complex t;
	t.x = z.x - w.x;
	t.y = z.y - w.y;
	return(t);
}

complex mult (z, w)
	complex z, w;
{
	complex t;
	t.x = z.x*w.x - z.y*w.y;
	t.y = z.x*w.y + z.y*w.x;
	return(t);
}


complex div (z, w)
	complex z, w;
{
	complex mult(), recip();
	return(mult(z,recip(w)));
}

complex recip (z)
	complex z;
{
	complex w;
	double r;
	r = z.x*z.x + z.y*z.y;
	w.x = z.x / r;
	w.y = -z.y / r;
	return(w);
}

complex conj(z)
	complex z;
{
	complex t;
	t = z;
	t.y = -t.y;
	return(t);
}

complex csqrt(z)
	complex z;
{
	complex w;
	double fabs(), sqrt();
/* Worry about numerical stability */
	if (z.x == 0.0 && z.y == 0.0) return(z); 
	else
	if (z.x > fabs(z.y))
		{
		w.x = sqrt((z.x+sqrt(z.x*z.x+z.y*z.y))/2);
		w.y = z.y/(2*w.x);
		}
	else
		{
		w.y = sqrt((-z.x+sqrt(z.x*z.x+z.y*z.y))/2);
		w.x = z.y/(2*w.y);
		}
	return(w);
}

/* Compute sqrt(z) in the half-plane perpendicular to w. */
complex contsqrt(z,w)
complex z,w;
{
	complex csqrt(), t;

	t = csqrt(z);
	if (0 > (t.x*w.x + t.y*w.y)) 
		{t.x = -t.x; t.y = -t.y;}
	return(t);
}

double cabs (z)       /* L 2 norm of z */
	complex z;
{
	return (sqrt (z.x*z.x + z.y*z.y));
}

double infnorm (z)   /* L infinity norm of z */
	complex z;
{
	double a,b;
	a = (z.x > 0) ? z.x : -z.x;
	b = (z.y > 0) ? z.y : -z.y;
	return ((a>b) ? a : b);
}

complex polar (radius, angle) /*Convert to complex. */
	double radius, angle;
{
	complex z;
	z.x = cos (angle) * radius;
	z.y = sin (angle) * radius;
	return(z);
}

/* Values in [-pi,pi]. */
double arg(z)  
	complex z;
{
	return(atan2(z.y,z.x));
}

complex cexp(z)
	complex z;
{
	complex w;
	double m;

	m = exp(z.x);
	w.x = m * cos(z.y);
	w.y = m * sin(z.y);
	return(w);
}

complex clog(z)
	complex z;
{
	complex w;

	w.x = log(cabs(z));
	w.y = arg(z);
	return(w);
}

complex csin(z)
	complex z;
{
	complex w;

	w.x = sin(z.x) * cosh(z.y);
	w.y = cos(z.x) * sinh(z.y);
	return(w);
}

complex ccos(z)
	complex z;
{
	complex w;

	w.x = cos(z.x) * cosh(z.y);
	w.y = -sin(z.x) * sinh(z.y);
	return(w);
}

complex csinh(z)
	complex z;
{
	complex w;

	w.x = sinh(z.x) * cos(z.y);
	w.y = cosh(z.x) * sin(z.y);
	return(w);
}

complex ccosh(z)
	complex z;
{
	complex w;

	w.x = cosh(z.x) * cos(z.y);
	w.y = sinh(z.x) * sin(z.y);
	return(w);
}

complex power(z,t)   /* Raise z to a real power t */
	complex z;
	double t;
{	double arg(), cabs(), pow();
	complex polar();

	return(polar(pow(cabs(z),t), t*arg(z)));
}

/*  Map points in the unit disk onto the lower hemisphere of the
    Riemann sphere by inverse stereographic projection.
    Projecting, r -> s = 2r/(r^2 + 1);  inverting this,
    s -> r = (1 - sqrt(1-s^2))/s.   */
 
complex disk_to_sphere(z)
	complex z;
{       complex w;
        double cabs(), r, s, sqrt();
 
        s = cabs(z);
        if (s == 0) return(z);
                else r = (1 - sqrt(1-s*s))/s;
        w.x = (r/s)*z.x;
        w.y = (r/s)*z.y;
        return(w);
}

complex mobius(a,b,c,d,z)
        complex a,b,c,d,z;

{       return(div(add(mult(a,z),b),
                   add(mult(c,z),d)));
}
