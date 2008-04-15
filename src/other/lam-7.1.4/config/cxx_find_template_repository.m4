dnl -*- shell-script -*-
dnl
dnl Copyright (c) 2001-2002 The Trustees of Indiana University.  
dnl                         All rights reserved.
dnl Copyright (c) 1998-2001 University of Notre Dame. 
dnl                         All rights reserved.
dnl Copyright (c) 1994-1998 The Ohio State University.  
dnl                         All rights reserved.
dnl 
dnl This file is part of the LAM/MPI software package.  For license
dnl information, see the LICENSE file in the top level directory of the
dnl LAM/MPI source distribution.
dnl
dnl $Id: cxx_find_template_repository.m4,v 1.3 2002/10/09 20:55:09 brbarret Exp $
dnl

define([LSC_CXX_FIND_TEMPLATE_REPOSITORY],[
#
# Arguments: prefix (optional)
#
# Dependencies: None
#
# See if the compiler makes template repository directories
# Warning: this is a really screwy example! -JMS
#
# Sets prefix_CXX_TEMPLATE_REPOSITORY and LSC_CXX_TEMPLATE_REPOSITORY
# to the template repository, or blank.
# Must call AC_SUBST manually
#

# Figure out what prefix to use
lsc_prefix="$1"
if test "$lsc_prefix" = ""; then
    lsc_prefix="LSC"
fi

# Find the repository
AC_MSG_CHECKING([for C++ template repository directory])
mkdir conf_tmp_$$
cd conf_tmp_$$
cat > conftest.h <<EOF
template <class T>
class foo {
public:
  foo(T yow) : data(yow) { yow.member(3); };
  void member(int i);
private:
  T data;
};

class bar {
public:
  bar(int i) { data = i; };
  void member(int j) { data = data * j; };

private:
  int data;
};
EOF

cat > conftest2.C <<EOF
#include "conftest.h"

void
some_other_function(void)
{
  foo<bar> var1(6);
  foo< foo<bar> > var2(var1);
}
EOF

cat > conftest1.C <<EOF
#include "conftest.h"

void some_other_function(void);

template <class T>
void
foo<T>::member(int i)
{
  i += 2;
}

int
main(int argc, char *argv[])
{
  foo<bar> var1(6);
  foo< foo<bar> > var2(var1);

  some_other_function();
  return 0;
}
EOF

lsc_template_failed=
echo configure:__oline__: $CXX $CXXFLAGS -c conftest1.C >&5
$CXX $CXXFLAGS -c conftest1.C >&5 2>&5
if test ! -f conftest1.o ; then
    AC_MSG_RESULT([templates not supported?])
    echo configure:__oline__: here is the program that failed: >&5
    cat conftest1.C >&5
    echo configure:__oline__: here is conftest.h: >&5
    cat conftest.h >&5
    lsc_template_failed=1
else
    echo configure:__oline__: $CXX $CXXFLAGS -c conftest2.C >&5
    $CXX $CXXFLAGS -c conftest2.C >&5 2>&5
    if test ! -f conftest2.o ; then
	AC_MSG_RESULT([unknown error])
	echo configure:__oline__: here is the program that failed: >&5
	cat conftest2.C >&5
	echo configure:__oline__: here is conftest.h: >&5
	cat conftest.h >&5
    else
	rm -rf conftest*

	for lsc_file in `ls`
	do
	    if test "$lsc_file" != "." -a "$lsc_file" != ".."; then
		# Is it a directory?
		if test -d "$lsc_file"; then
		    lsc_template_dir="$lsc_file $lsc_template_dir"
		    
		# Or is it a file?
		else
		    name="`echo $lsc_file | cut -d. -f1`"
		    
		    temp_mask=
		    if test "$name" = "main" -o "$name" = "other"; then
			temp_mask="`echo $lsc_file | cut -d. -f2`"
			if test "$lsc_template_filemask" = ""; then
			lsc_template_filemask="$temp_mask";
			elif test "`echo $lsc_template_filemask | grep $temp_mask`" = ""; then
			lsc_template_filemask="$lsc_template_filemask $temp_mask"
			fi
		    fi
		fi
	    fi
	done
	if test "$lsc_template_filemask" != ""; then
	    temp_mask=
	    for mask in $lsc_template_filemask
	    do
		temp_mask="*.$mask $temp_mask"
	    done
	    lsc_template_filemask=$temp_mask
	fi
    fi
fi
LSC_CXX_TEMPLATE_REPOSITORY="$lsc_template_dir $lsc_template_filemask"

if test "`echo $LSC_CXX_TEMPLATE_REPOSITORY`" != ""; then
    AC_MSG_RESULT([$LSC_CXX_TEMPLATE_REPOSITORY])
else
    AC_MSG_RESULT([not used])
fi
cd ..
rm -rf conf_tmp_$$

# Set the result
lsc_name="${lsc_prefix}_CXX_TEMPLATE_REPOSITORY"
lsc_str="${lsc_name}=\"$LSC_CXX_TEMPLATE_REPOSITORY\""
eval $lsc_str

# Clean up
unset lsc_prefix lsc_file lsc_template_failed lsc_template_dir \
    lsc_name lsc_str])dnl
