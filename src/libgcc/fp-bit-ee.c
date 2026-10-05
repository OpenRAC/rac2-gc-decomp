/* libgcc single-precision software floating point, EE build - vendored copy.
 *
 * This file is a distribution of GCC 2.95.3's libgcc fp-bit.c, and it is
 * licensed under the GNU General Public License version 2, with the libgcc
 * linking exception. It is NOT covered by this repository's MIT licence.
 * The exception permits this file's compiled output to be linked into a
 * program without that program falling under the GPL; the file itself stays
 * GPL v2.
 *
 * Provenance, all measured rather than inherited:
 *   upstream   rac1-decomp src/libgcc/fp-bit.c, commit
 *              cb22f0b0d3a171d1fd4b6851b86fe214a22c9822, sha256
 *              3069e3a1385e9b2d316929a676a71e2af8a834666b7388a89f10c52ae0e17336
 *   transform  one pass of the campaign's own reconstructed 2.9-ee cpp:
 *              cpp -P -DFLOAT -DFLOAT_BIT_ORDER_MISMATCH -DNO_DENORMALS
 *                  -DUS_SOFTWARE_GOFAST
 *   rename     ONE symbol name, applied to the cpp output below and
 *              reversible by substituting it back: __unpack_f becomes
 *              FUN_00123400, the name this campaign measured it under. A C
 *              symbol name changes no emitted byte.
 *
 * The rest of the cpp output, including the refused fptodp body, keeps its
 * original names and is not edited. See README.md in this directory and
 * docs/RAC1-FP-BIT-EVIDENCE.md.
 */
/* --- verbatim preprocessed fp-bit.c begins here --- */
 



























 






 






 


 
















































 

















 












typedef float SFtype __attribute__ ((mode (SF)));
typedef float DFtype __attribute__ ((mode (DF)));

typedef int HItype __attribute__ ((mode (HI)));
typedef int SItype __attribute__ ((mode (SI)));
typedef int DItype __attribute__ ((mode (DI)));

 




typedef unsigned int UHItype __attribute__ ((mode (HI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef unsigned int UDItype __attribute__ ((mode (DI)));























	typedef USItype fractype;
	typedef UHItype halffractype;
	typedef SFtype FLO_type;
	typedef SItype intfrac;
























 


 
 










 

typedef enum
{
  CLASS_SNAN,
  CLASS_QNAN,
  CLASS_ZERO,
  CLASS_NUMBER,
  CLASS_INFINITY
} fp_class_type;

typedef struct
{

  fp_class_type class;
  unsigned int sign;
  int normal_exp;


  union
    {
      fractype ll;
      halffractype l[2];
    } fraction;
} fp_number_type;

typedef union
{
  FLO_type value;
  fractype value_raw;




  struct
    {
      fractype fraction: 23  __attribute__ ((packed));
      unsigned int exp: 8  __attribute__ ((packed));
      unsigned int sign:1 __attribute__ ((packed));
    }
  bits;



}
FLO_union_type;


 

 



__inline__ 
static fp_number_type *
nan ()
{
   




  extern fp_number_type __thenan_df;

  return &__thenan_df;
}

__inline__ 
static int
isnan ( fp_number_type *  x)
{
  return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}

__inline__ 
static int
isinf ( fp_number_type *  x)
{
  return x->class == CLASS_INFINITY;
}



__inline__ 
static int
iszero ( fp_number_type *  x)
{
  return x->class == CLASS_ZERO;
}

__inline__  
static void
flip_sign ( fp_number_type *  x)
{
  x->sign = !x->sign;
}

extern FLO_type __pack_f  ( fp_number_type * );


FLO_type
__pack_f  ( fp_number_type *  src)
{
  FLO_union_type dst;
  fractype fraction = src->fraction.ll;	 
  int sign = src->sign;
  int exp = 0;

  if (isnan (src))
    {
      exp = (0xff) ;
      if (src->class == CLASS_QNAN || 1)
	{
	  fraction |= 0x100000L ;
	}
    }
  else if (isinf (src))
    {
      exp = (0xff) ;
      fraction = 0;
    }
  else if (iszero (src))
    {
      exp = 0;
      fraction = 0;
    }
  else if (fraction == 0)
    {
      exp = 0;
    }
  else
    {
      if (src->normal_exp < (-(127 )+1) )
	{
	   



	  int shift = (-(127 )+1)  - src->normal_exp;

	  exp = 0;

	  if (shift > 32  - 7L )
	    {
	       
	      fraction = 0;
	    }
	  else
	    {
	       
	      fraction >>= shift;
	    }
	  fraction >>= 7L ;
	}
      else if (src->normal_exp > 127 )
	{
	  exp = (0xff) ;
	  fraction = 0;
	}
      else
	{
	  exp = src->normal_exp + 127 ;
	   


	  if ((fraction & 0x7f ) == 0x40 )
	    {
	      if (fraction & (1 << 7L ))
		fraction += 0x3f  + 1;
	    }
	  else
	    {
	       
	      fraction += 0x3f ;
	    }
	  if (fraction >= (1LL<<(23 +1+ 7L )) )
	    {
	      fraction >>= 1;
	      exp += 1;
	    }
	  fraction >>= 7L ;
	}
    }

   



  dst.bits.fraction = fraction;
  dst.bits.exp = exp;
  dst.bits.sign = sign;




  return dst.value;
}


extern void FUN_00123400  (FLO_union_type *, fp_number_type *);


void
FUN_00123400  (FLO_union_type * src, fp_number_type * dst)
{
   


  fractype fraction;
  int exp;
  int sign;


  

  fraction = src->bits.fraction;
  exp = src->bits.exp;
  sign = src->bits.sign;


  dst->sign = sign;
  if (exp == 0)
    {
       
       



      if (fraction == 0

	  || 1

	  )
	{
	   
	  dst->class = CLASS_ZERO;
	}
      else
	{
	   


	  dst->normal_exp = exp - 127  + 1;
	  fraction <<= 7L ;

	  dst->class = CLASS_NUMBER;

	  while (fraction < (1LL<<(23 + 7L )) )
	    {
	      fraction <<= 1;
	      dst->normal_exp--;
	    }

	  dst->fraction.ll = fraction;
	}
    }
  else if (exp == (0xff) )
    {
       
      if (fraction == 0)
	{
	   
	  dst->class = CLASS_INFINITY;
	}
      else
	{
	   
	  if (fraction & 0x100000L )
	    {
	      dst->class = CLASS_QNAN;
	    }
	  else
	    {
	      dst->class = CLASS_SNAN;
	    }
	   
	  dst->fraction.ll = fraction;
	}
    }
  else
    {
       
      dst->normal_exp = exp - 127 ;
      dst->class = CLASS_NUMBER;
      dst->fraction.ll = (fraction << 7L ) | (1LL<<(23 + 7L )) ;
    }
}



static fp_number_type *
_fpadd_parts (fp_number_type * a,
	      fp_number_type * b,
	      fp_number_type * tmp)
{
  intfrac tfraction;

   
  int a_normal_exp;
  int b_normal_exp;
  fractype a_fraction;
  fractype b_fraction;

  if (isnan (a))
    {
      return a;
    }
  if (isnan (b))
    {
      return b;
    }
  if (isinf (a))
    {
       
      if (isinf (b) && a->sign != b->sign)
	return nan ();
      return a;
    }
  if (isinf (b))
    {
      return b;
    }
  if (iszero (b))
    {
      if (iszero (a))
	{
	  *tmp = *a;
	  tmp->sign = a->sign & b->sign;
	  return tmp;
	}
      return a;
    }
  if (iszero (a))
    {
      return b;
    }

   

  {
    int diff;

    a_normal_exp = a->normal_exp;
    b_normal_exp = b->normal_exp;
    a_fraction = a->fraction.ll;
    b_fraction = b->fraction.ll;

    diff = a_normal_exp - b_normal_exp;

    if (diff < 0)
      diff = -diff;
    if (diff < 32 )
      {
	 
	while (a_normal_exp > b_normal_exp)
	  {
	    b_normal_exp++;
	    {  b_fraction  = ( b_fraction  & 1) | ( b_fraction  >> 1); } ;
	  }
	while (b_normal_exp > a_normal_exp)
	  {
	    a_normal_exp++;
	    {  a_fraction  = ( a_fraction  & 1) | ( a_fraction  >> 1); } ;
	  }
      }
    else
      {
	 
	if (a_normal_exp > b_normal_exp)
	  {
	    b_normal_exp = a_normal_exp;
	    b_fraction = 0;
	  }
	else
	  {
	    a_normal_exp = b_normal_exp;
	    a_fraction = 0;
	  }
      }
  }

  if (a->sign != b->sign)
    {
      if (a->sign)
	{
	  tfraction = -a_fraction + b_fraction;
	}
      else
	{
	  tfraction = a_fraction - b_fraction;
	}
      if (tfraction >= 0)
	{
	  tmp->sign = 0;
	  tmp->normal_exp = a_normal_exp;
	  tmp->fraction.ll = tfraction;
	}
      else
	{
	  tmp->sign = 1;
	  tmp->normal_exp = a_normal_exp;
	  tmp->fraction.ll = -tfraction;
	}
       

      while (tmp->fraction.ll < (1LL<<(23 + 7L ))  && tmp->fraction.ll)
	{
	  tmp->fraction.ll <<= 1;
	  tmp->normal_exp--;
	}
    }
  else
    {
      tmp->sign = a->sign;
      tmp->normal_exp = a_normal_exp;
      tmp->fraction.ll = a_fraction + b_fraction;
    }
  tmp->class = CLASS_NUMBER;
   


  if (tmp->fraction.ll >= (1LL<<(23 +1+ 7L )) )
    {
      {  tmp->fraction.ll  = ( tmp->fraction.ll  & 1) | ( tmp->fraction.ll  >> 1); } ;
      tmp->normal_exp++;
    }
  return tmp;

}

FLO_type
fpadd  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type tmp;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  res = _fpadd_parts (&a, &b, &tmp);

  return __pack_f  (res);
}

FLO_type
fpsub  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type tmp;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  b.sign ^= 1;

  res = _fpadd_parts (&a, &b, &tmp);

  return __pack_f  (res);
}



static __inline__  fp_number_type *
_fpmul_parts ( fp_number_type *  a,
	       fp_number_type *  b,
	       fp_number_type * tmp)
{
  fractype low = 0;
  fractype high = 0;

  if (isnan (a))
    {
      a->sign = a->sign != b->sign;
      return a;
    }
  if (isnan (b))
    {
      b->sign = a->sign != b->sign;
      return b;
    }
  if (isinf (a))
    {
      if (iszero (b))
	return nan ();
      a->sign = a->sign != b->sign;
      return a;
    }
  if (isinf (b))
    {
      if (iszero (a))
	{
	  return nan ();
	}
      b->sign = a->sign != b->sign;
      return b;
    }
  if (iszero (a))
    {
      a->sign = a->sign != b->sign;
      return a;
    }
  if (iszero (b))
    {
      b->sign = a->sign != b->sign;
      return b;
    }

   

  {

    {
       


      DItype answer = (DItype)(a->fraction.ll) * (DItype)(b->fraction.ll);
      
      high = answer >> 32;
      low = answer;
    }

  }

  tmp->normal_exp = a->normal_exp + b->normal_exp;
  tmp->sign = a->sign != b->sign;

  tmp->normal_exp += 2;		 

  while (high >= (1LL<<(23 +1+ 7L )) )
    {
      tmp->normal_exp++;
      if (high & 1)
	{
	  low >>= 1;
	  low |= 0x80000000L ;
	}
      high >>= 1;
    }
  while (high < (1LL<<(23 + 7L )) )
    {
      tmp->normal_exp--;

      high <<= 1;
      if (low & 0x80000000L )
	high |= 1;
      low <<= 1;
    }
   

  if ((high & 0x7f ) == 0x40 )
    {
      if (high & (1 << 7L ))
	{
	   
	  high += 0x3f  + 1;
	}
      else if (low)
	{
	   
	  high += 0x3f  + 1;
	}
    }
  tmp->fraction.ll = high;
  tmp->class = CLASS_NUMBER;
  return tmp;
}

FLO_type
fpmul  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type tmp;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  res = _fpmul_parts (&a, &b, &tmp);

  return __pack_f  (res);
}



static __inline__  fp_number_type *
_fpdiv_parts (fp_number_type * a,
	      fp_number_type * b)
{
  fractype bit;
  fractype numerator;
  fractype denominator;
  fractype quotient;

  if (isnan (a))
    {
      return a;
    }
  if (isnan (b))
    {
      return b;
    }

  a->sign = a->sign ^ b->sign;

  if (isinf (a) || iszero (a))
    {
      if (a->class == b->class)
	return nan ();
      return a;
    }

  if (isinf (b))
    {
      a->fraction.ll = 0;
      a->normal_exp = 0;
      return a;
    }
  if (iszero (b))
    {
      a->class = CLASS_INFINITY;
      return a;
    }

   

  {
     



    a->normal_exp = a->normal_exp - b->normal_exp;
    numerator = a->fraction.ll;
    denominator = b->fraction.ll;

    if (numerator < denominator)
      {
	 
	numerator *= 2;
	a->normal_exp--;
      }
    bit = (1LL<<(23 + 7L )) ;
    quotient = 0;
     
    while (bit)
      {
	if (numerator >= denominator)
	  {
	    quotient |= bit;
	    numerator -= denominator;
	  }
	bit >>= 1;
	numerator *= 2;
      }

    if ((quotient & 0x7f ) == 0x40 )
      {
	if (quotient & (1 << 7L ))
	  {
	     
	    quotient += 0x3f  + 1;
	  }
	else if (numerator)
	  {
	     
	    quotient += 0x3f  + 1;
	  }
      }

    a->fraction.ll = quotient;
    return (a);
  }
}

FLO_type
fpdiv  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  res = _fpdiv_parts (&a, &b);

  return __pack_f  (res);
}


int __fpcmp_parts_f  (fp_number_type * a, fp_number_type *b);


 





int
__fpcmp_parts_f  (fp_number_type * a, fp_number_type * b)
{


  if (isnan (a) || isnan (b))
    {
      return 1;			 
    }
  if (isinf (a) && isinf (b))
    {
       
       







      return b->sign - a->sign;
    }
   
  if (isinf (a))
    {
      return a->sign ? -1 : 1;
    }
  if (isinf (b))
    {
      return b->sign ? 1 : -1;
    }
  if (iszero (a) && iszero (b))
    {
      return 0;
    }
  if (iszero (a))
    {
      return b->sign ? 1 : -1;
    }
  if (iszero (b))
    {
      return a->sign ? -1 : 1;
    }
   
  if (a->sign != b->sign)
    {
       
      return a->sign ? -1 : 1;
    }
   
  if (a->normal_exp > b->normal_exp)
    {
      return a->sign ? -1 : 1;
    }
  if (a->normal_exp < b->normal_exp)
    {
      return a->sign ? 1 : -1;
    }
   
  if (a->fraction.ll > b->fraction.ll)
    {
      return a->sign ? -1 : 1;
    }
  if (a->fraction.ll < b->fraction.ll)
    {
      return a->sign ? 1 : -1;
    }
   
  return 0;
}



SItype 
fpcmp  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  return __fpcmp_parts_f  (&a, &b);
}





FLO_type
sitofp  (SItype arg_a)
{
  fp_number_type in;

  in.class = CLASS_NUMBER;
  in.sign = arg_a < 0;
  if (!arg_a)
    {
      in.class = CLASS_ZERO;
    }
  else
    {
      in.normal_exp = 23  + 7L ;
      if (in.sign) 
	{
	   

	  if (arg_a == (SItype) 0x80000000)
	    {
	      return -2147483648.0;
	    }
	  in.fraction.ll = (-arg_a);
	}
      else
	in.fraction.ll = arg_a;

      while (in.fraction.ll < (1LL << (23  + 7L )))
	{
	  in.fraction.ll <<= 1;
	  in.normal_exp -= 1;
	}
    }
  return __pack_f  (&in);
}



SItype
fptosi  (FLO_type arg_a)
{
  fp_number_type a;
  SItype tmp;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  if (iszero (&a))
    return 0;
  if (isnan (&a))
    return 0;
   
  if (isinf (&a))
    return a.sign ? (- ((SItype) ((unsigned) (~0)>>1)) )-1 : ((SItype) ((unsigned) (~0)>>1)) ;
   
  if (a.normal_exp < 0)
    return 0;
  if (a.normal_exp > 30)
    return a.sign ? (- ((SItype) ((unsigned) (~0)>>1)) )-1 : ((SItype) ((unsigned) (~0)>>1)) ;
  tmp = a.fraction.ll >> ((23  + 7L ) - a.normal_exp);
  return a.sign ? (-tmp) : (tmp);
}




 





USItype
fptoui  (FLO_type arg_a)
{
  fp_number_type a;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  if (iszero (&a))
    return 0;
  if (isnan (&a))
    return 0;
   
  if (a.sign)
    return 0;
   
  if (isinf (&a))
    return ((USItype) ~0) ;
   
  if (a.normal_exp < 0)
    return 0;
  if (a.normal_exp > 31)
    return ((USItype) ~0) ;
  else if (a.normal_exp > (23  + 7L ))
    return a.fraction.ll << (a.normal_exp - (23  + 7L ));
  else
    return a.fraction.ll >> ((23  + 7L ) - a.normal_exp);
}




FLO_type
__negsf2  (FLO_type arg_a)
{
  fp_number_type a;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  flip_sign (&a);
  return __pack_f  (&a);
}





SFtype
__make_fp(fp_class_type class,
	     unsigned int sign,
	     int exp, 
	     USItype frac)
{
  fp_number_type in;

  in.class = class;
  in.sign = sign;
  in.normal_exp = exp;
  in.fraction.ll = frac;
  return __pack_f  (&in);
}




 




extern DFtype __make_dp (fp_class_type, unsigned int, int, UDItype frac);


DFtype
fptodp  (SFtype arg_a)
{
  fp_number_type in;

  FUN_00123400  ((FLO_union_type *) & arg_a, &in);
  return __make_dp (in.class, in.sign, in.normal_exp,
		    ((UDItype) in.fraction.ll) << (52+8-(23+7)) );
}







/* --- verbatim preprocessed fp-bit.c ends here --- */
