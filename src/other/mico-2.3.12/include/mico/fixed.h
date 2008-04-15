// -*- c++ -*-
/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

#ifndef __mico_fixed_h__
#define __mico_fixed_h__

/*
 * This is a much simplified implementation of the IDL type fixed<d,s>.
 * the number of exact digits depends on the size of long double.
 * with the usual 64-bit mantissa you get no more than 16 exact digits,
 * a 114-bit mantissa allows for up to 31 exact digits.
 */

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

/*
 * On AIX, the div() function is implemented as a macro, which screws
 * our definition of div below.
 */

#ifdef div
#undef div
#endif

class FixedBase {
    MICO_UShort _digits;
    MICO_Short _scale;
    MICO_LongDouble _val;

    void adjust (MICO_Boolean round = TRUE);
    MICO_LongDouble transform (MICO_Short nscale) const;
public:
    typedef SequenceTmpl<MICO_Octet,MICO_TID_OCTET> FixedValue;
    typedef TSeqVar<FixedValue> FixedValue_var;

    FixedBase ();
    FixedBase (MICO_Long);
    FixedBase (MICO_ULong);
    FixedBase (MICO_LongLong);
    FixedBase (MICO_ULongLong);
    FixedBase (MICO_Double);
    FixedBase (MICO_LongDouble);
    FixedBase (const char *);
    FixedBase (MICO_UShort d, MICO_Short s);
    FixedBase (const FixedBase &);
    ~FixedBase ();

    operator MICO_LongDouble () const;
    operator MICO_LongLong () const;
    FixedBase round (MICO_UShort nscale) const;
    FixedBase truncate (MICO_UShort nscale) const;

    FixedBase &operator= (const FixedBase &);
    FixedBase &operator+= (const FixedBase &);
    FixedBase &operator-= (const FixedBase &);
    FixedBase &operator*= (const FixedBase &);
    FixedBase &operator/= (const FixedBase &);

    FixedBase &operator++ ();
    FixedBase operator++ (int);
    FixedBase &operator-- ();
    FixedBase operator-- (int);
    FixedBase operator+ () const;
    FixedBase operator- () const;
    MICO_Boolean operator!() const;
    
    static void compute_params (const char *str,
				MICO_UShort &digits, MICO_Short &scale,
				MICO_UShort max_significant_digits = 1000);
    /*
     * FixedValue contains '_digits' digits (starting with the most
     * significant one) plus one Octet at the end that is non-zero iff
     * the number is negative.
     */
    FixedValue* to_digits () const;
    void from_digits (const FixedValue &);

    std::istream &read (std::istream &);
    std::ostream &write (std::ostream &) const;

    static void add (FixedBase &res, const FixedBase &a1, const FixedBase a2);
    static void sub (FixedBase &res, const FixedBase &a1, const FixedBase a2);
    static void mul (FixedBase &res, const FixedBase &a1, const FixedBase a2);
    static void div (FixedBase &res, const FixedBase &a1, const FixedBase a2);
    static void neg (FixedBase &res, const FixedBase &a);

    static int eq (const FixedBase &a1, const FixedBase &a2);
    static int lt (const FixedBase &a1, const FixedBase &a2);
    static int le (const FixedBase &a1, const FixedBase &a2);

    MICO_UShort fixed_digits () const
    { return _digits; }

    MICO_Short fixed_scale () const
    { return _scale; }
};

static inline std::istream&
operator>> (std::istream &i, FixedBase &f)
{
    return f.read (i);
}

static inline std::ostream&
operator<< (std::ostream &o, const FixedBase &f)
{
    return f.write (o);
}


FixedBase operator+ (const FixedBase &v1, const FixedBase &v2);
FixedBase operator- (const FixedBase &v1, const FixedBase &v2);
FixedBase operator* (const FixedBase &v1, const FixedBase &v2);
FixedBase operator/ (const FixedBase &v1, const FixedBase &v2);

MICO_Boolean operator>  (const FixedBase &v1, const FixedBase &v2);
MICO_Boolean operator<  (const FixedBase &v1, const FixedBase &v2);
MICO_Boolean operator>= (const FixedBase &v1, const FixedBase &v2);
MICO_Boolean operator<= (const FixedBase &v1, const FixedBase &v2);
MICO_Boolean operator== (const FixedBase &v1, const FixedBase &v2);
MICO_Boolean operator!= (const FixedBase &v1, const FixedBase &v2);

template<MICO_UShort D, MICO_Short S>
class FixedTmpl : public FixedBase {
public:
    FixedTmpl ()
	: FixedBase (D, S)
    {
    }
    FixedTmpl (MICO_Long v)
	: FixedBase (D, S)
    {
	FixedBase::operator= (FixedBase (v));
    }
    FixedTmpl (MICO_ULong v)
	: FixedBase (D, S)
    {
	FixedBase::operator= (FixedBase (v));
    }
    FixedTmpl (MICO_LongLong v)
	: FixedBase (D, S)
    {
	FixedBase::operator= (FixedBase (v));
    }
    FixedTmpl (MICO_ULongLong v)
	: FixedBase (D, S)
    {
	FixedBase::operator= (FixedBase (v));
    }
    FixedTmpl (MICO_Double v)
	: FixedBase (D, S)
    {
	FixedBase::operator= (FixedBase (v));
    }
    FixedTmpl (MICO_LongDouble v)
	: FixedBase (D, S)
    {
	FixedBase::operator= (FixedBase (v));
    }
    FixedTmpl (const FixedBase &f)
        : FixedBase (D, S)
    {
	FixedBase::operator= (f);
    }
    FixedTmpl (const FixedTmpl<D,S> &f)
        : FixedBase (D, S)
    {
	FixedBase::operator= (f);
    }
    FixedTmpl<D,S> &operator= (MICO_Long v)
    {
	FixedBase::operator= (FixedBase (v));
	return *this;
    }
    FixedTmpl<D,S> &operator= (MICO_ULong v)
    {
	FixedBase::operator= (FixedBase (v));
	return *this;
    }
    FixedTmpl<D,S> &operator= (MICO_LongLong v)
    {
	FixedBase::operator= (FixedBase (v));
	return *this;
    }
    FixedTmpl<D,S> &operator= (MICO_ULongLong v)
    {
	FixedBase::operator= (FixedBase (v));
	return *this;
    }
    FixedTmpl<D,S> &operator= (MICO_Double v)
    {
	FixedBase::operator= (FixedBase (v));
	return *this;
    }
    FixedTmpl<D,S> &operator= (MICO_LongDouble v)
    {
	FixedBase::operator= (FixedBase (v));
	return *this;
    }
    FixedTmpl<D,S> &operator= (const FixedBase &f)
    {
	FixedBase::operator= (f);
	return *this;
    }
    FixedTmpl<D,S> &operator= (const FixedTmpl<D,S> &f)
    {
	FixedBase::operator= (f);
	return *this;
    }
    ~FixedTmpl ()
    {
    }
};

typedef FixedBase Fixed;

#endif
