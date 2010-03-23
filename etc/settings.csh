#----------------------------------*-sh-*--------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
#    \\/     M anipulation  |
#------------------------------------------------------------------------------
# License
#     This file is part of OpenFOAM.
#
#     OpenFOAM is free software; you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by the
#     Free Software Foundation; either version 2 of the License, or (at your
#     option) any later version.
#
#     OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
#     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#     for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with OpenFOAM; if not, write to the Free Software Foundation,
#     Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
#
# File
#     etc/settings.csh
#
# Description
#     Startup file for OpenFOAM
#     Sourced from OpenFOAM-??/etc/cshrc
#
#------------------------------------------------------------------------------

# prefix to PATH
alias _foamAddPath 'setenv PATH \!*\:${PATH}'
# prefix to LD_LIBRARY_PATH
alias _foamAddLib 'setenv LD_LIBRARY_PATH \!*\:${LD_LIBRARY_PATH}'
# prefix to MANPATH
alias _foamAddMan 'setenv MANPATH \!*\:${MANPATH}'

# location of the jobControl directory
setenv FOAM_JOB_DIR $WM_PROJECT_INST_DIR/jobControl

# wmake configuration
setenv WM_DIR $WM_PROJECT_DIR/wmake
setenv WM_LINK_LANGUAGE c++
setenv WM_OPTIONS $WM_ARCH$WM_COMPILER$WM_PRECISION_OPTION$WM_COMPILE_OPTION

# base configuration
setenv FOAM_APP $WM_PROJECT_DIR/applications
setenv FOAM_APPBIN $WM_PROJECT_DIR/applications/bin/$WM_OPTIONS
setenv FOAM_LIB $WM_PROJECT_DIR/lib
setenv FOAM_LIBBIN $WM_PROJECT_DIR/lib/$WM_OPTIONS
setenv FOAM_SRC $WM_PROJECT_DIR/src

# shared site configuration - similar naming convention as ~OpenFOAM expansion
setenv FOAM_SITE_APPBIN $WM_PROJECT_INST_DIR/site/$WM_PROJECT_VERSION/bin/$WM_OPTIONS
setenv FOAM_SITE_LIBBIN $WM_PROJECT_INST_DIR/site/$WM_PROJECT_VERSION/lib/$WM_OPTIONS

# user configuration
setenv FOAM_USER_APPBIN $WM_PROJECT_USER_DIR/applications/bin/$WM_OPTIONS
setenv FOAM_USER_LIBBIN $WM_PROJECT_USER_DIR/lib/$WM_OPTIONS

# convenience
setenv FOAM_TUTORIALS $WM_PROJECT_DIR/tutorials
setenv FOAM_UTILITIES $FOAM_APP/utilities
setenv FOAM_SOLVERS $FOAM_APP/solvers
setenv FOAM_RUN $WM_PROJECT_USER_DIR/run

# add OpenFOAM scripts and wmake to the path
setenv PATH ${WM_DIR}:${WM_PROJECT_DIR}/bin:${PATH}

_foamAddPath ${FOAM_USER_APPBIN}:${FOAM_SITE_APPBIN}:${FOAM_APPBIN}
 # Make sure to pick up dummy versions of external libraries last
_foamAddLib  ${FOAM_USER_LIBBIN}:${FOAM_SITE_LIBBIN}:${FOAM_LIBBIN}:${FOAM_LIBBIN}/dummy


# Select compiler installation
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# compilerInstall = OpenFOAM | system
if ( ! $?compilerInstall ) set compilerInstall=OpenFOAM

switch ("$compilerInstall")
case OpenFOAM:
    switch ("$WM_COMPILER")
    case Gcc:
        setenv WM_COMPILER_DIR $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/gcc-4.4.3
        setenv MPFR_HOME $WM_THIRD_PARTY_DIR/mpfr-2.4.2
        setenv MPFR_ARCH_PATH $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/mpfr-2.4.2
        _foamAddLib $MPFR_ARCH_PATH/lib
        _foamAddLib $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/gmp-5.0.1/lib
    breaksw
    case Gcc442:
        setenv WM_COMPILER_DIR $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/gcc-4.4.2
        setenv MPFR_HOME $WM_THIRD_PARTY_DIR/mpfr-2.4.1
        setenv MPFR_ARCH_PATH $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/mpfr-2.4.1
        _foamAddLib $MPFR_ARCH_PATH/lib
        _foamAddLib $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/gmp-4.2.4/lib
    breaksw
    case Gcc43:
        setenv WM_COMPILER_DIR $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/gcc-4.3.3
        setenv MPFR_HOME $WM_THIRD_PARTY_DIR/mpfr-2.4.1
        setenv MPFR_ARCH_PATH $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/mpfr-2.4.1
        _foamAddLib $MPFR_ARCH_PATH/lib
        _foamAddLib $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/gmp-4.2.4/lib
    breaksw
    endsw

    # Check that the compiler directory can be found
    if ( ! -d "$WM_COMPILER_DIR" ) then
        echo
        echo "Warning in $WM_PROJECT_DIR/etc/settings.csh:"
        echo "    Cannot find $WM_COMPILER_DIR installation."
        echo "    Please install this compiler version or if you wish to use the system compiler,"
        echo "    change the 'compilerInstall' setting to 'system' in this file"
        echo
    endif

    _foamAddPath    ${WM_COMPILER_DIR}/bin
    _foamAddLib     ${WM_COMPILER_DIR}/lib${WM_COMPILER_LIB_ARCH}:${WM_COMPILER_DIR}/lib
    _foamAddMan     ${WM_COMPILER_DIR}/man

    breaksw
endsw


# Communications library
# ~~~~~~~~~~~~~~~~~~~~~~

unsetenv MPI_ARCH_PATH MPI_HOME

switch ("$WM_MPLIB")
case OPENMPI:
    set mpi_version=openmpi-1.4.1
    setenv MPI_ARCH_PATH $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$mpi_version

    # Tell OpenMPI where to find its install directory
    setenv OPAL_PREFIX $MPI_ARCH_PATH

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    _foamAddMan     $MPI_ARCH_PATH/man

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/$mpi_version
    unset mpi_version
    breaksw

case SYSTEMOPENMPI:
    # This uses the installed openmpi. It needs mpicc installed!
    set mpi_version=openmpi-system

    # Set compilation flags here instead of in wmake/rules/../mplibSYSTEMOPENMPI
    setenv PINC `mpicc --showme:compile`
    setenv PLIBS `mpicc --showme:link`
    set libDir=`echo "$PLIBS" | sed -e 's/.*-L\([^ ]*\).*/\1/'`

    if ($?FOAM_VERBOSE && $?prompt) then
        echo "Using system installed MPI:"
        echo "    compile flags : $PINC"
        echo "    link flags    : $PLIBS"
        echo "    libmpi dir    : $libDir"
    endif

    _foamAddLib     $libDir

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/$mpi_version
    unset mpi_version libDir
    breaksw

case MPICH:
    set mpi_version=mpich2-1.1.1p1
    setenv MPI_HOME $WM_THIRD_PARTY_DIR/$mpi_version
    setenv MPI_ARCH_PATH $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$mpi_version

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    _foamAddMan     $MPI_ARCH_PATH/share/man

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/$mpi_version
    unset mpi_version
    breaksw

case MPICH-GM:
    setenv MPI_ARCH_PATH /opt/mpi
    setenv MPICH_PATH $MPI_ARCH_PATH
    setenv GM_LIB_PATH /opt/gm/lib64

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    _foamAddLib     $GM_LIB_PATH

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/mpich-gm
    breaksw

case HPMPI:
    setenv MPI_HOME /opt/hpmpi
    setenv MPI_ARCH_PATH $MPI_HOME

    _foamAddPath $MPI_ARCH_PATH/bin

    switch (`uname -m`)
    case i686:
        _foamAddLib $MPI_ARCH_PATH/lib/linux_ia32
        breaksw
    case x86_64:
        _foamAddLib $MPI_ARCH_PATH/lib/linux_amd64
        breaksw
    case ia64:
        _foamAddLib $MPI_ARCH_PATH/lib/linux_ia64
        breaksw
    default:
        echo Unknown processor type `uname -m` for Linux
        breaksw
    endsw

    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/hpmpi
    breaksw

case GAMMA:
    setenv MPI_ARCH_PATH /usr
    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/gamma
    breaksw

case MPI:
    setenv MPI_ARCH_PATH /opt/mpi
    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/mpi
    breaksw

case FJMPI:
    setenv MPI_ARCH_PATH /opt/FJSVmpi2
    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/mpi
    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib/sparcv9
    _foamAddLib     /opt/FSUNf90/lib/sparcv9
    _foamAddLib     /opt/FJSVpnidt/lib
    breaksw

case QSMPI:
    setenv MPI_ARCH_PATH /usr/lib/mpi
    setenv FOAM_MPI_LIBBIN FOAM_LIBBIN/qsmpi

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib

    breaksw

default:
    setenv FOAM_MPI_LIBBIN $FOAM_LIBBIN/dummy
    breaksw
endsw

_foamAddLib $FOAM_MPI_LIBBIN


# Set the minimum MPI buffer size (used by all platforms except SGI MPI)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if ( ! $?minBufferSize ) set minBufferSize=20000000

if ( $?MPI_BUFFER_SIZE ) then
    if ( $MPI_BUFFER_SIZE < $minBufferSize ) then
        setenv MPI_BUFFER_SIZE $minBufferSize
    endif
else
    setenv MPI_BUFFER_SIZE $minBufferSize
endif


# CGAL and boost
# ~~~~~~~~~~~~~~
set cgal_version=3.6
setenv CGAL_SRC $WM_THIRD_PARTY_DIR/CGAL-$cgal_version

set boost_version=1_42_0
setenv BOOST_ROOT $WM_THIRD_PARTY_DIR/boost_$boost_version

_foamAddLib $BOOST_ROOT/platforms/$WM_OPTIONS/lib
_foamAddLib $CGAL_SRC/lib

unset cgal_version boost_version


# Enable the hoard memory allocator if available
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#if ( -f $FOAM_LIBBIN/libhoard.so ) then
#    setenv LD_PRELOAD $FOAM_LIBBIN/libhoard.so:${LD_PRELOAD}
#endif


# cleanup environment:
# ~~~~~~~~~~~~~~~~~~~~
unalias _foamAddPath _foamAddLib _foamAddMan
unset compilerInstall minBufferSize

# ----------------------------------------------------------------- end-of-file
