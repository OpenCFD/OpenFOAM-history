dnl
dnl stolen from tclobj-1.x, which in turn stole it from ncftp-2.3.x
dnl
AC_DEFUN(wi_EXTRA_IDIR, [
incdir="$1"
if test -r $incdir ; then
	case "$CPPFLAGS" in
		*-I${incdir}*)
			# echo "   + already had $incdir" 1>&6
			;;
		*)
			if test "$CPPFLAGS" = "" ; then
				CPPFLAGS="-I$incdir"
			else
				CPPFLAGS="$CPPFLAGS -I$incdir"
			fi
			echo "   + found $incdir" 1>&6
			;;
	esac
fi
])
dnl
dnl
dnl
dnl
AC_DEFUN(wi_EXTRA_LDIR, [
libdir="$1"
if test -r $libdir ; then
	case "$LDFLAGS" in
		*-L${libdir}*)
			# echo "   + already had $libdir" 1>&6
			;;
		*)
			if test "$LDFLAGS" = "" ; then
				LDFLAGS="-L$libdir"
			else
				LDFLAGS="$LDFLAGS -L$libdir"
			fi
			echo "   + found $libdir" 1>&6
			;;
	esac
fi
])
dnl
dnl __FP__
dnl
dnl
AC_DEFUN(wi_EXTRA_PDIR, [
progdir="$1"
if test -r $progdir ; then
	case "$PATH" in
		*:${progdir}*)
			# echo "   + already had $progdir" 1>&6
			;;
		*${progdir}:*)
			# echo "   + already had $progdir" 1>&6
			;;
		*)
			if test "$PATH" = "" ; then
				PATH="$progdir"
			else
				PATH="$PATH:$progdir"
			fi
			echo "   + found $progdir" 1>&6
			;;
	esac
fi
])
dnl
dnl
dnl If you want to also look for include and lib subdirectories in the
dnl $HOME tree, you supply "yes" as the first argument to this macro.
dnl
dnl If you want to look for subdirectories in include/lib directories,
dnl you pass the names in argument 3, otherwise pass a dash.
dnl
AC_DEFUN(wi_EXTRA_DIRS, [echo "checking for extra include and lib directories..." 1>&6
ifelse([$1], yes, [dnl
b1=`cd .. ; pwd`
b2=`cd ../.. ; pwd`
exdirs="$HOME $j $b1 $b2 $2"
],[dnl
exdirs="$2"
])
subexdirs="$3"
if test "$subexdirs" = "" ; then
	subexdirs="-"
fi
for subexdir in $subexdirs ; do
if test "$subexdir" = "-" ; then
	subexdir=""
else
	subexdir="/$subexdir"
fi
for exdir in $exdirs ; do
	if test "$exdir" != "/usr" || test "$subexdir" != ""; then
		incdir="${exdir}/include${subexdir}"
		wi_EXTRA_IDIR($incdir)

		libdir="${exdir}/lib${subexdir}"
		wi_EXTRA_LDIR($libdir)

		progdir="${exdir}/bin${subexdirr}"
		wi_EXTRA_PDIR($progdir)
	fi
done
done
])
dnl
dnl
dnl Check for the type of the third argument of getsockname
dnl
dnl
AC_DEFUN(AC_CHECK_SOCKET_SIZE_T,
[AC_MSG_CHECKING(for the third argument of getsockname)
dnl
AC_CACHE_VAL(ac_cv_socket_size_t,
[AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>],[socklen_t a=0; getsockname(0,0, &a);],
eval "ac_cv_socket_socklen_t=yes",
eval "ac_cv_socket_socklen_t=no")
dnl
ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>],[int a=0; getsockname(0,0, &a);],
eval "ac_cv_socket_size_t_int=yes",
eval "ac_cv_socket_size_t_int=no")
CXXFLAGS=$ac_cv_tmp_old_cxxflags
dnl
if test X"$ac_cv_socket_socklen_t" = X"yes"; then
  ac_cv_socket_size_t=socklen_t
elif test X"$ac_cv_socket_size_t_int" = X"yes"; then
  ac_cv_socket_size_t=int
else
  ac_cv_socket_size_t=size_t
fi])
AC_MSG_RESULT($ac_cv_socket_size_t)
case $ac_cv_socket_size_t in
socklen_t)
  AC_DEFINE(socket_size_t, socklen_t)
  ;;
int)
  AC_DEFINE(socket_size_t, int)
  ;;
size_t)
  AC_DEFINE(socket_size_t, size_t)
  ;;
esac
])   
dnl
dnl
dnl Check for the type of the second argument of getsockname
dnl
dnl
AC_DEFUN(AC_CHECK_SOCKET_ADDR_T,
[AC_MSG_CHECKING(for the second argument of getsockname)
dnl
AC_CACHE_VAL(ac_cv_socket_addr_caddr_t,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>],[getsockname(0,(caddr_t)0, 0);],
eval "ac_cv_socket_addr_caddr_t=yes",
eval "ac_cv_socket_addr_caddr_t=no")
CXXFLAGS=$ac_cv_tmp_old_cxxflags])
dnl
if test "$ac_cv_socket_addr_caddr_t" = "yes"; then
  AC_MSG_RESULT(caddr_t)
  AC_DEFINE(socket_addr_t, caddr_t)
else
  AC_MSG_RESULT(struct sockaddr *)
  AC_DEFINE(socket_addr_t, struct sockaddr *)
fi
])
dnl
dnl
dnl Check for the type of the select() filemask arguments
dnl
dnl
AC_DEFUN(AC_CHECK_SELECT_ADDR_T,
[AC_MSG_CHECKING(for select argument type)
dnl
AC_CACHE_VAL(ac_cv_select_int,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif],[select(0, (int *)0, (int *)0, (int *)0, 0);],
eval "ac_cv_select_int=yes",
eval "ac_cv_select_int=no")
CXXFLAGS=$ac_cv_tmp_old_cxxflags])
dnl
if test "$ac_cv_select_int" = "yes"; then
  AC_MSG_RESULT(int *)
  AC_DEFINE(select_addr_t, int *)
else
  AC_MSG_RESULT(fd_set *)
  AC_DEFINE(select_addr_t, fd_set *)
fi
])
dnl
dnl
dnl Check for signature of gettimeofday()
dnl
dnl we fall back to the two-args-version when we cannot compile the
dnl test prog due to missing gettimeofday() prototype. For example
dnl Cygwin32 doesnt have this prototype but expects the two-args-version.
dnl
dnl
AC_DEFUN(AC_CHECK_GETTIMEOFDAY,
[AC_MSG_CHECKING(whether gettimeofday has two args)
dnl
AC_CACHE_VAL(ac_cv_gtod_twoargs,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE([
#include <sys/time.h>
#include <unistd.h>],[gettimeofday (0);],
eval "ac_cv_gtod_twoargs=no",
eval "ac_cv_gtod_twoargs=yes")
CXXFLAGS=$ac_cv_tmp_old_cxxflags])
dnl
if test "$ac_cv_gtod_twoargs" = "yes"; then
  AC_MSG_RESULT(yes)
  AC_DEFINE(HAVE_GETTIMEOFDAY_TWOARGS)
else
  AC_MSG_RESULT(no)
fi
])   
dnl
dnl
dnl get a define out of a header file
dnl
dnl
AC_DEFUN(AC_GET_DEFINE,
[cat > conftest.$ac_ext <<EOF
#include "confdefs.h"
#include <$3>
__grepline__:$2
EOF
eval "$ac_cpp conftest.$ac_ext 2>&AC_FD_CC | grep __grepline__ | cut -d: -f2  2>/dev/null > conftest.out"
if test $? = 0; then
  result=`cat conftest.out`
  if test X"$result" != X"$2"; then
    $1=$result
  fi
fi
rm -rf conftest*])
dnl
dnl
dnl try compiling something outside of functions
dnl
dnl
AC_DEFUN(AC_TRY_COMPILE_GLOBAL,
[cat > conftest.$ac_ext <<EOF
[#]line __oline__ "configure"
#include "confdefs.h"
[$1]
[$2]
int main() { return 0; }
EOF
if AC_TRY_EVAL(ac_compile); then
    ifelse([$3], , :, [rm -rf conftest*
    $3])
ifelse([$4], , , [else
    rm -rf conftest*
    $4
])dnl
fi
rm -f conftest*]
)
dnl
dnl
dnl try compiling and link something outside of functions
dnl
dnl
AC_DEFUN(AC_TRY_LINK_GLOBAL,
[cat > conftest.$ac_ext <<EOF
dnl This sometimes fails to find confdefs.h, for some reason.
dnl [#]line __oline__ "[$]0"
[#]line __oline__ "configure"
#include "confdefs.h"
[$1]
[$2]
int main() { return 0; }
EOF
if AC_TRY_EVAL(ac_link) && test -s conftest; then
  ifelse([$3], , :, [rm -rf conftest*
  $3])
else
  echo "configure: failed program was:" >&AC_FD_CC
  cat conftest.$ac_ext >&AC_FD_CC
ifelse([$4], , , [  rm -rf conftest*
  $4
])dnl
fi
rm -f conftest*])
dnl
dnl
dnl check for bool
dnl
dnl
AC_DEFUN(AC_CHECK_BOOL,
[AC_MSG_CHECKING(for bool)
AC_CACHE_VAL(ac_cv_have_bool_type,
[AC_TRY_COMPILE(,[bool b = true;],
eval "ac_cv_have_bool_type=yes",
eval "ac_cv_have_bool_type=no")])
if test $ac_cv_have_bool_type = yes; then
  AC_DEFINE(HAVE_BOOL_TYPE)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi
])
dnl
dnl
dnl check whether C++ compiler supports const overloding of conversion
dnl operators
dnl
dnl
AC_DEFUN(AC_CONST_OVERLOAD,
[AC_MSG_CHECKING(for const overload)
AC_CACHE_VAL(ac_cv_have_const_overload,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE_GLOBAL(,[
  struct S {
    int *i;
    operator const int *() const { return i; }
    operator int *&() { return i; }
    operator const int &() const { return *i; }
    operator int &() { return *i; }
  };

  void bar (const int &, int &, const int *, int *, int *&);

  void foo (S &s, const S &cs)
  {
     int i = s; i = cs; i = *s; i = *cs;
     bar (s, s, s, s, s);
     bar (cs, s, cs, s, s);
  }
],
eval "ac_cv_have_const_overload=yes",
eval "ac_cv_have_const_overload=no",)
CXXFLAGS=$ac_cv_tmp_old_cxxflags
])
if test $ac_cv_have_const_overload = yes; then
  AC_DEFINE(HAVE_CONST_OVERLOAD)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi
])
dnl
dnl
dnl check whether a virtual method defined in both base classes of a
dnl class has to be explicitely overridden.
dnl
dnl
AC_DEFUN(AC_EXPLICIT_METHOD_OVERRIDE,
[AC_MSG_CHECKING(for explicit method override)
AC_CACHE_VAL(ac_cv_have_explicit_method_override,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE_GLOBAL(,[
struct Base {
  virtual void f () {}
};
struct Skel1 : virtual public Base {
  virtual void f () {}
};
struct Skel2 : virtual public Base {
  virtual void f () {}
};
struct Skel3 : virtual public Skel1, virtual public Skel2 {
  virtual void f () {}
};
struct Impl1 : virtual public Skel1 {
};
struct Impl2 : virtual public Skel2 {
};
struct Impl3 : virtual public Impl1,
               virtual public Impl2,
               virtual public Skel3 {
};
void m () {
  Impl3 i;
  i.f();
}
],
eval "ac_cv_have_explicit_method_override=no",
eval "ac_cv_have_explicit_method_override=yes",)
CXXFLAGS=$ac_cv_tmp_old_cxxflags
])
if test $ac_cv_have_explicit_method_override = yes; then
  AC_DEFINE(HAVE_EXPLICIT_METHOD_OVERRIDE)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi
])
dnl
dnl
dnl check for typename
dnl
dnl
AC_DEFUN(AC_TYPENAME,
[AC_MSG_CHECKING(for typename)
AC_CACHE_VAL(ac_cv_have_typename,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE_GLOBAL(,[
  struct foo { typedef int Type; };

  template<class T>
  struct bar {
    void f (typename T::Type x) {}
  };

  bar<foo> i;
],
eval "ac_cv_have_typename=yes",
eval "ac_cv_have_typename=no",)
CXXFLAGS=$ac_cv_tmp_old_cxxflags
])
if test $ac_cv_have_typename = yes; then
  AC_DEFINE(HAVE_TYPENAME)
  AC_MSG_RESULT(yes)
else
  AC_DEFINE(typename,)
  AC_MSG_RESULT(no)
fi
])
dnl
dnl
dnl check for guiding declarations
dnl
dnl
AC_DEFUN(AC_GUIDING_DECLS,
[AC_MSG_CHECKING(for guiding declarations)
AC_CACHE_VAL(ac_cv_have_guiding_decls,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE_GLOBAL(,[
  template<class T>
  void func (T &x)
  {
  }

  template<class T>
  class foo {
  public:
    friend void func<> (T &x);
  };

  foo<int> i;
],
eval "ac_cv_have_guiding_decls=yes",
eval "ac_cv_have_guiding_decls=no",)
CXXFLAGS=$ac_cv_tmp_old_cxxflags
])
if test $ac_cv_have_guiding_decls = yes; then
  AC_DEFINE(HAVE_GUIDING_DECLS)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi
])
dnl
dnl
dnl check for namespaces
dnl
dnl
AC_DEFUN(AC_NAMESPACE,
[AC_MSG_CHECKING(for namespaces)
AC_CACHE_VAL(ac_cv_have_namespace,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE_GLOBAL(,[
// cause an error for gcc 2.7.2 ...
#if __GNUG__ == 2 && __GNUC_MINOR__ == 7
#error "gcc 2.7.2 doesnt have namespaces"
#endif
  namespace foo { struct bar{ bar(); }; };
  foo::bar::bar() {}

  struct bar{ bar(); };
  bar::bar() {}

  foo::bar i;
  bar j;
],
eval "ac_cv_have_namespace=yes",
eval "ac_cv_have_namespace=no",)
CXXFLAGS=$ac_cv_tmp_old_cxxflags
])
AC_SUBST(HAVE_NAMESPACE)
HAVE_NAMESPACE=$ac_cv_have_namespace
if test $ac_cv_have_namespace = yes; then
  AC_DEFINE(HAVE_NAMESPACE)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi
])
dnl
dnl
dnl check whether c++ compiler fails to automatically generate
dnl copy ctors, assigment op and dtor for structs with template members
dnl
dnl
AC_DEFUN(AC_EXPLICIT_STRUCT_OPS,
[AC_MSG_CHECKING(whether explicit struct methods are needed)
AC_CACHE_VAL(ac_cv_have_explicit_struct_ops,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_LINK_GLOBAL(,[
// cause an error for gcc 2.7.2 ...
#if __GNUG__ == 2 && __GNUC_MINOR__ == 7
#error "gcc 2.7.2 needs struct ops"
#endif
  template<class T>
  struct S {
    S ();
    S (const S<T> &);
    S<T> &operator= (const S<T> &);
    ~S ();
  };

  typedef S<int> Sint;

  struct A {
    Sint s;
  };

  void f (const A &a1)
  {
    A a2 (a1), a3;
    a3 = a2;
  }

  template<class T>
  S<T>::S ()
  {
  }

  template<class T>
  S<T>::S (const S<T> &s)
  {
  }

  template<class T>
  S<T> &S<T>::operator= (const S<T> &s)
  {
    return *this;
  }

  template<class T>
  S<T>::~S ()
  {
  }
],
eval "ac_cv_have_explicit_struct_ops=no",
eval "ac_cv_have_explicit_struct_ops=yes",)
CXXFLAGS=$ac_cv_tmp_old_cxxflags
])
if test $ac_cv_have_explicit_struct_ops = yes; then
  AC_DEFINE(HAVE_EXPLICIT_STRUCT_OPS)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi
])
dnl
dnl
dnl check whether dynamic_cast<> is supported
dnl
dnl
AC_DEFUN(AC_CXX_DYNAMIC_CAST,
[AC_MSG_CHECKING(for dynamic_cast support)
AC_CACHE_VAL(ac_cv_cxx_dynamic_cast,
[AC_TRY_COMPILE(,[
  class A {
  public:
    virtual int f() = 0;
  };

  class B : virtual public A {
  public:
    int f() { return 1; }
  };

  class C : virtual public A {
  public:
    int f() { return 2; }
  };

  class D : virtual public B, virtual public C {
  public:
    int f() { return 3; }
  };

  B * b = new D;
  C * c = dynamic_cast<C*> (b);
],
eval "ac_cv_cxx_dynamic_cast=yes",
eval "ac_cv_cxx_dynamic_cast=no")])
AC_MSG_RESULT($ac_cv_cxx_dynamic_cast)
if test "x$ac_cv_cxx_dynamic_cast" = "xyes" ; then
  AC_DEFINE(HAVE_DYNAMIC_CAST)
fi
])
dnl
dnl
dnl check for "overloaded typedefs"
dnl
dnl
AC_DEFUN(AC_TYPEDEF_OVERLOAD,
[AC_MSG_CHECKING(for overloaded typedefs)
AC_CACHE_VAL(ac_cv_have_typedef_overload,
[ac_cv_tmp_old_cxxflags="$CXXFLAGS"
if test X"$GXX" = Xyes; then
  CXXFLAGS="-Werror $CXXFLAGS"
fi
AC_TRY_COMPILE_GLOBAL(,[
  struct A {
    struct D {
      typedef D *T;
    };
  };
  struct B {
    struct D {
      typedef D *T;
    };
  };
],
eval "ac_cv_have_typedef_overload=yes",
eval "ac_cv_have_typedef_overload=no",)
CXXFLAGS=$ac_cv_tmp_old_cxxflags
])
AC_SUBST(HAVE_TYPEDEF_OVERLOAD)
HAVE_TYPEDEF_OVERLOAD=$ac_cv_typedef_overload
if test $ac_cv_have_typedef_overload = yes; then
  AC_DEFINE(HAVE_TYPEDEF_OVERLOAD)
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi
])

# Configure paths for GTK+
# Owen Taylor     97-11-3

dnl AM_PATH_GTK([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for GTK, and define GTK_CFLAGS and GTK_LIBS
dnl
AC_DEFUN(AM_PATH_GTK,
[dnl 
dnl Get the cflags and libraries from the gtk-config script
dnl
AC_ARG_WITH(gtk-prefix,[  --with-gtk-prefix=PFX   Prefix where GTK is installed (optional)],
            gtk_config_prefix="$withval", gtk_config_prefix="")
AC_ARG_WITH(gtk-exec-prefix,[  --with-gtk-exec-prefix=PFX Exec prefix where GTK is installed (optional)],
            gtk_config_exec_prefix="$withval", gtk_config_exec_prefix="")
AC_ARG_ENABLE(gtktest, [  --disable-gtktest       Do not try to compile and run a test GTK program],
		    , enable_gtktest=yes)

  if test x$gtk_config_exec_prefix != x ; then
     gtk_config_args="$gtk_config_args --exec-prefix=$gtk_config_exec_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_exec_prefix/bin/gtk-config
     fi
  fi
  if test x$gtk_config_prefix != x ; then
     gtk_config_args="$gtk_config_args --prefix=$gtk_config_prefix"
     if test x${GTK_CONFIG+set} != xset ; then
        GTK_CONFIG=$gtk_config_prefix/bin/gtk-config
     fi
  fi

  AC_PATH_PROG(GTK_CONFIG, gtk-config, no)
  min_gtk_version=ifelse([$1], ,0.99.7,$1)
  AC_MSG_CHECKING(for GTK - version >= $min_gtk_version)
  no_gtk=""
  if test "$GTK_CONFIG" = "no" ; then
    no_gtk=yes
  else
    GTK_CFLAGS=`$GTK_CONFIG $gtk_config_args --cflags`
    GTK_LIBS=`$GTK_CONFIG $gtk_config_args --libs`
    gtk_config_major_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    gtk_config_minor_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    gtk_config_micro_version=`$GTK_CONFIG $gtk_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_gtktest" = "xyes" ; then
      ac_save_CXXFLAGS="$CXXFLAGS"
      ac_save_LIBS="$LIBS"
      CXXFLAGS="$CXXFLAGS $GTK_CFLAGS"
      LIBS="$LIBS $GTK_LIBS"
dnl
dnl Now check if the installed GTK is sufficiently new. (Also sanity
dnl checks the results of gtk-config to some extent
dnl
      rm -f conf.gtktest
      AC_TRY_RUN([
#include <gtk/gtk.h>
#include <stdio.h>

int 
main ()
{
  int major, minor, micro;

  system ("touch conf.gtktest");

  if (sscanf("$min_gtk_version", "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_gtk_version");
     exit(1);
   }

  if ((gtk_major_version != $gtk_config_major_version) ||
      (gtk_minor_version != $gtk_config_minor_version) ||
      (gtk_micro_version != $gtk_config_micro_version))
    {
      printf("\n*** 'gtk-config --version' returned %d.%d.%d, but GTK+ (%d.%d.%d)\n", 
             $gtk_config_major_version, $gtk_config_minor_version, $gtk_config_micro_version,
             gtk_major_version, gtk_minor_version, gtk_micro_version);
      printf ("*** was found! If gtk-config was correct, then it is best\n");
      printf ("*** to remove the old version of GTK+. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If gtk-config was wrong, set the environment variable GTK_CONFIG\n");
      printf("*** to point to the correct copy of gtk-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    } 
  else
    {
      if ((gtk_major_version > major) ||
        ((gtk_major_version == major) && (gtk_minor_version > minor)) ||
        ((gtk_major_version == major) && (gtk_minor_version == minor) && (gtk_micro_version >= micro)))
      {
        return 0;
       }
     else
      {
        printf("\n*** An old version of GTK+ (%d.%d.%d) was found.\n",
               gtk_major_version, gtk_minor_version, gtk_micro_version);
        printf("*** You need a version of GTK+ newer than %d.%d.%d. The latest version of\n",
	       major, minor, micro);
        printf("*** GTK+ is always available from ftp://ftp.gtk.org.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the gtk-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of GTK+, but you can also set the GTK_CONFIG environment to point to the\n");
        printf("*** correct copy of gtk-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_gtk=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CXXFLAGS="$ac_save_CXXFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_gtk" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$GTK_CONFIG" = "no" ; then
       echo "*** The gtk-config script installed by GTK could not be found"
       echo "*** If GTK was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the GTK_CONFIG environment variable to the"
       echo "*** full path to gtk-config."
     else
       if test -f conf.gtktest ; then
        :
       else
          echo "*** Could not run GTK test program, checking why..."
          CXXFLAGS="$CXXFLAGS $GTK_CFLAGS"
          LIBS="$LIBS $GTK_LIBS"
          AC_TRY_LINK([
#include <gtk/gtk.h>
#include <stdio.h>
],      [ return ((gtk_major_version) || (gtk_minor_version) || (gtk_micro_version)); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding GTK or finding the wrong"
          echo "*** version of GTK. If it is not finding GTK, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"
          echo "***"
          echo "*** If you have a RedHat 5.0 system, you should remove the GTK package that"
          echo "*** came with the system with the command"
          echo "***"
          echo "***    rpm --erase --nodeps gtk gtk-devel" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means GTK was incorrectly installed"
          echo "*** or that you have moved GTK since it was installed. In the latter case, you"
          echo "*** may want to edit the gtk-config script: $GTK_CONFIG" ])
          CXXFLAGS="$ac_save_CXXFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     GTK_CFLAGS=""
     GTK_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(GTK_CFLAGS)
  AC_SUBST(GTK_LIBS)
  rm -f conf.gtktest
])
