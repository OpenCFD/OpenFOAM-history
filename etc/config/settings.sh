#----------------------------------*-sh-*--------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
#    \\/     M anipulation  |
#------------------------------------------------------------------------------
# License
#     This file is part of OpenFOAM.
#
#     OpenFOAM is free software: you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
#     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#     for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
#
# File
#     etc/config/settings.sh
#
# Description
#     Startup file for OpenFOAM
#     Sourced from OpenFOAM-<VERSION>/etc/bashrc
#
#------------------------------------------------------------------------------

# prefix to PATH
_foamAddPath()
{
    while [ $# -ge 1 ]
    do
        export PATH=$1:$PATH
        shift
    done
}

# prefix to LD_LIBRARY_PATH
_foamAddLib()
{
    while [ $# -ge 1 ]
    do
        export LD_LIBRARY_PATH=$1:$LD_LIBRARY_PATH
        shift
    done
}

# prefix to MANPATH
_foamAddMan()
{
    while [ $# -ge 1 ]
    do
        export MANPATH=$1:$MANPATH
        shift
    done
}

#------------------------------------------------------------------------------
# Set environment variables according to system type
export WM_ARCH=`uname -s`

case "$WM_ARCH" in
Linux)
    WM_ARCH=linux

    # compiler specifics
    case `uname -m` in
    i686)
        ;;

    x86_64)
        case "$WM_ARCH_OPTION" in
        32)
            export WM_COMPILER_ARCH=64
            export WM_CC='gcc'
            export WM_CXX='g++'
            export WM_CFLAGS='-m32 -fPIC'
            export WM_CXXFLAGS='-m32 -fPIC'
            export WM_LDFLAGS='-m32'
            ;;
        64)
            WM_ARCH=linux64
            export WM_COMPILER_LIB_ARCH=64
            export WM_CC='gcc'
            export WM_CXX='g++'
            export WM_CFLAGS='-m64 -fPIC'
            export WM_CXXFLAGS='-m64 -fPIC'
            export WM_LDFLAGS='-m64'
            ;;
        *)
            echo "Unknown WM_ARCH_OPTION '$WM_ARCH_OPTION', should be 32 or 64"
            ;;
        esac
        ;;

    ia64)
        WM_ARCH=linuxIA64
        export WM_COMPILER=I64
        ;;

    mips64)
        WM_ARCH=SiCortex64
        WM_MPLIB=MPI
        export WM_COMPILER_LIB_ARCH=64
        export WM_CC='gcc'
        export WM_CXX='g++'
        export WM_CFLAGS='-mabi=64 -fPIC'
        export WM_CXXFLAGS='-mabi=64 -fPIC'
        export WM_LDFLAGS='-mabi=64 -G0'
        ;;

    ppc64)
        WM_ARCH=linuxPPC64
        export WM_COMPILER_LIB_ARCH=64
        export WM_CC='gcc'
        export WM_CXX='g++'
        export WM_CFLAGS='-m64 -fPIC'
        export WM_CXXFLAGS='-m64 -fPIC'
        export WM_LDFLAGS='-m64'
        ;;

    *)
        echo Unknown processor type `uname -m` for Linux
        ;;
    esac
    ;;

SunOS)
    WM_ARCH=SunOS64
    WM_MPLIB=FJMPI
    export WM_COMPILER_LIB_ARCH=64
    export WM_CC='gcc'
    export WM_CXX='g++'
    export WM_CFLAGS='-mabi=64 -fPIC'
    export WM_CXXFLAGS='-mabi=64 -fPIC'
    export WM_LDFLAGS='-mabi=64 -G0'
    ;;

*)    # an unsupported operating system
    cat <<USAGE

    Your "$WM_ARCH" operating system is not supported by this release
    of OpenFOAM. For further assistance, please contact www.OpenFOAM.org

USAGE
    ;;
esac


#------------------------------------------------------------------------------

# location of the jobControl directory
export FOAM_JOB_DIR=$WM_PROJECT_INST_DIR/jobControl

# wmake configuration
export WM_DIR=$WM_PROJECT_DIR/wmake
export WM_LINK_LANGUAGE=c++
export WM_OPTIONS=$WM_ARCH$WM_COMPILER$WM_PRECISION_OPTION$WM_COMPILE_OPTION

# base executables/libraries
export FOAM_APPBIN=$WM_PROJECT_DIR/platforms/$WM_OPTIONS/bin
export FOAM_LIBBIN=$WM_PROJECT_DIR/platforms/$WM_OPTIONS/lib

# external (ThirdParty) libraries
export FOAM_EXT_LIBBIN=$WM_THIRD_PARTY_DIR/platforms/$WM_OPTIONS/lib

# shared site executables/libraries
# similar naming convention as ~OpenFOAM expansion
if [ -n "$WM_PROJECT_SITE" ]
then
    export FOAM_SITE_APPBIN=$WM_PROJECT_SITE/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/bin
    export FOAM_SITE_LIBBIN=$WM_PROJECT_SITE/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/lib
else
    export FOAM_SITE_APPBIN=$WM_PROJECT_INST_DIR/site/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/bin
    export FOAM_SITE_LIBBIN=$WM_PROJECT_INST_DIR/site/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/lib
fi

# user executables/libraries
export FOAM_USER_APPBIN=$WM_PROJECT_USER_DIR/platforms/$WM_OPTIONS/bin
export FOAM_USER_LIBBIN=$WM_PROJECT_USER_DIR/platforms/$WM_OPTIONS/lib

# dynamicCode templates
# - default location is the "~OpenFOAM/codeTemplates/dynamicCode" expansion
# export FOAM_CODE_TEMPLATES=$WM_PROJECT_DIR/etc/codeTemplates/dynamicCode

# convenience
export FOAM_APP=$WM_PROJECT_DIR/applications
export FOAM_SRC=$WM_PROJECT_DIR/src
export FOAM_TUTORIALS=$WM_PROJECT_DIR/tutorials
export FOAM_UTILITIES=$FOAM_APP/utilities
export FOAM_SOLVERS=$FOAM_APP/solvers
export FOAM_RUN=$WM_PROJECT_USER_DIR/run

# add wmake to the path - not required for runtime only environment
[ -d "$WM_DIR" ] && PATH=$WM_DIR:$PATH
# add OpenFOAM scripts to the path
export PATH=$WM_PROJECT_DIR/bin:$PATH

_foamAddPath $FOAM_USER_APPBIN:$FOAM_SITE_APPBIN:$FOAM_APPBIN
# Make sure to pick up dummy versions of external libraries last
_foamAddLib  $FOAM_USER_LIBBIN:$FOAM_SITE_LIBBIN:$FOAM_LIBBIN:$FOAM_EXT_LIBBIN:$FOAM_LIBBIN/dummy

# Compiler settings
# ~~~~~~~~~~~~~~~~~
unset gcc_version gmp_version mpfr_version mpc_version
unset MPFR_ARCH_PATH GMP_ARCH_PATH

# Location of compiler installation
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if [ -z "$foamCompiler" ]
then
    foamCompiler=system
    echo "Warning in $WM_PROJECT_DIR/etc/config/settings.sh:" 1>&2
    echo "    foamCompiler not set, using '$foamCompiler'" 1>&2
fi

case "${foamCompiler}" in
OpenFOAM | ThirdParty)
    case "$WM_COMPILER" in
    Gcc | Gcc++0x | Gcc46 | Gcc46++0x)
        gcc_version=gcc-4.6.1
        gmp_version=gmp-5.0.2
        mpfr_version=mpfr-3.0.1
        mpc_version=mpc-0.9
        gmpPACKAGE=gmp-5.0.2
        ;;
    Gcc45 | Gcc45++0x)
        gcc_version=gcc-4.5.2
        gmp_version=gmp-5.0.1
        mpfr_version=mpfr-2.4.2
        mpc_version=mpc-0.8.1
        ;;
    Gcc44 | Gcc44++0x)
        gcc_version=gcc-4.4.3
        gmp_version=gmp-5.0.1
        mpfr_version=mpfr-2.4.2
        ;;
    Gcc43)
        gcc_version=gcc-4.3.3
        gmp_version=gmp-4.2.4
        mpfr_version=mpfr-2.4.1
        ;;
    Clang)
        # using clang - not gcc
        export WM_CC='clang'
        export WM_CXX='clang++'
        #clang_version=llvm-2.9
        clang_version=llvm-svn
        ;;
    *)
        echo
        echo "Warning in $WM_PROJECT_DIR/etc/config/settings.sh:"
        echo "    Unknown OpenFOAM compiler type '$WM_COMPILER'"
        echo "    Please check your settings"
        echo
        ;;
    esac

    # optional configuration tweaks:
    _foamSource `$WM_PROJECT_DIR/bin/foamEtcFile config/compiler.sh`

    if [ -n "$gcc_version" ]
    then
        gccDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$gcc_version
        gmpDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$gmp_version
        mpfrDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$mpfr_version
        mpcDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$mpc_version

        # Check that the compiler directory can be found
        [ -d "$gccDir" ] || {
            echo
            echo "Warning in $WM_PROJECT_DIR/etc/config/settings.sh:"
            echo "    Cannot find $gccDir installation."
            echo "    Please install this compiler version or if you wish to use the system compiler,"
            echo "    change the 'foamCompiler' setting to 'system'"
            echo
        }

        _foamAddMan     $gccDir/man
        _foamAddPath    $gccDir/bin

        # add compiler libraries to run-time environment
        # 64-bit needs lib64, but 32-bit needs lib (not lib32)
        if [ "$WM_ARCH_OPTION" = 64 ]
        then
            _foamAddLib     $gccDir/lib$WM_COMPILER_LIB_ARCH
        else
            _foamAddLib     $gccDir/lib
        fi


        # add gmp/mpfr libraries to run-time environment
        _foamAddLib     $gmpDir/lib
        _foamAddLib     $mpfrDir/lib

        # add mpc libraries (not need for older gcc) to run-time environment
        if [ -n "$mpc_version" ]
        then
            _foamAddLib     $mpcDir/lib
        fi

        # used by boost/CGAL:
        export MPFR_ARCH_PATH=$mpfrDir
        export GMP_ARCH_PATH=$gmpDir
    fi
    unset gcc_version gccDir
    unset gmp_version gmpDir  mpfr_version mpfrDir  mpc_version mpcDir

    if [ -n "$clang_version" ]
    then
        clangDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$clang_version

        # Check that the compiler directory can be found
        [ -d "$clangDir" ] || {
            echo
            echo "Warning in $WM_PROJECT_DIR/etc/config/settings.sh:"
            echo "    Cannot find $clangDir installation."
            echo "    Please install this compiler version or if you wish to use the system compiler,"
            echo "    change the 'foamCompiler' setting to 'system'"
            echo
        }

        _foamAddMan     $clangDir/share/man
        _foamAddPath    $clangDir/bin
    fi
    unset clang_version clangDir
    ;;
system)
    # okay, use system compiler
    ;;
*)
    echo "Warn: foamCompiler='$foamCompiler' is unsupported" 1>&2
    echo "   treating as 'system' instead" 1>&2
    ;;
esac


#
# add c++0x flags for external programs
#
if [ -n "$WM_CXXFLAGS" ]
then
    case "$WM_COMPILER" in
    Gcc*++0x)
        WM_CXXFLAGS="$WM_CXXFLAGS -std=c++0x"
        ;;
    esac
fi


# boost and CGAL
# ~~~~~~~~~~~~~~

boost_version=boost_1_45_0
cgal_version=CGAL-3.8

export BOOST_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$boost_version
export CGAL_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$cgal_version

# enabled if CGAL is available
if [ "$FOAM_VERBOSE" -a "$PS1" ]
then
    echo "Checking for"
    echo "    $cgal_version at $CGAL_ARCH_PATH"
    echo "    $boost_version at $BOOST_ARCH_PATH"
fi

if [ -d "$CGAL_ARCH_PATH" ]
then
    if [ -d "$BOOST_ARCH_PATH" ]
    then
        _foamAddLib $BOOST_ARCH_PATH/lib
    else
        unset BOOST_ARCH_PATH
    fi
    _foamAddLib $CGAL_ARCH_PATH/lib
else
    unset BOOST_ARCH_PATH CGAL_ARCH_PATH MPFR_ARCH_PATH GMP_ARCH_PATH
fi

unset boost_version cgal_version


# Communications library
# ~~~~~~~~~~~~~~~~~~~~~~

unset MPI_ARCH_PATH MPI_HOME FOAM_MPI_LIBBIN

case "$WM_MPLIB" in
OPENMPI)
    export FOAM_MPI=openmpi-1.5.3
    # optional configuration tweaks:
    _foamSource `$WM_PROJECT_DIR/bin/foamEtcFile config/openmpi.sh`

    export MPI_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$FOAM_MPI

    # Tell OpenMPI where to find its install directory
    export OPAL_PREFIX=$MPI_ARCH_PATH

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    _foamAddMan     $MPI_ARCH_PATH/man
    ;;

SYSTEMOPENMPI)
    # Use the system installed openmpi, get library directory via mpicc
    export FOAM_MPI=openmpi-system

    # Set compilation flags here instead of in wmake/rules/../mplibSYSTEMOPENMPI
    export PINC="`mpicc --showme:compile`"
    export PLIBS="`mpicc --showme:link`"
    libDir=`echo "$PLIBS" | sed -e 's/.*-L\([^ ]*\).*/\1/'`

    # Bit of a hack: strip off 'lib' and hope this is the path to openmpi
    # include files and libraries.
    export MPI_ARCH_PATH="${libDir%/*}"

    if [ "$FOAM_VERBOSE" -a "$PS1" ]
    then
        echo "Using system installed MPI:"
        echo "    compile flags : $PINC"
        echo "    link flags    : $PLIBS"
        echo "    libmpi dir    : $libDir"
    fi

    _foamAddLib     $libDir
    unset libDir
    ;;

MPICH)
    export FOAM_MPI=mpich2-1.1.1p1
    export MPI_HOME=$WM_THIRD_PARTY_DIR/$FOAM_MPI
    export MPI_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$FOAM_MPI

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    _foamAddMan     $MPI_ARCH_PATH/share/man
    ;;

MPICH-GM)
    export FOAM_MPI=mpich-gm
    export MPI_ARCH_PATH=/opt/mpi
    export MPICH_PATH=$MPI_ARCH_PATH
    export GM_LIB_PATH=/opt/gm/lib64

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    _foamAddLib     $GM_LIB_PATH
    ;;

HPMPI)
    export FOAM_MPI=hpmpi
    export MPI_HOME=/opt/hpmpi
    export MPI_ARCH_PATH=$MPI_HOME

    _foamAddPath $MPI_ARCH_PATH/bin

    case `uname -m` in
    i686)
        _foamAddLib $MPI_ARCH_PATH/lib/linux_ia32
        ;;

    x86_64)
        _foamAddLib $MPI_ARCH_PATH/lib/linux_amd64
        ;;
    ia64)
        _foamAddLib $MPI_ARCH_PATH/lib/linux_ia64
        ;;
    *)
        echo Unknown processor type `uname -m` for Linux
        ;;
    esac
    ;;

GAMMA)
    export FOAM_MPI=gamma
    export MPI_ARCH_PATH=/usr
    ;;

MPI)
    export FOAM_MPI=mpi
    export MPI_ARCH_PATH=/opt/mpi
    ;;

FJMPI)
    export FOAM_MPI=fjmpi
    export MPI_ARCH_PATH=/opt/FJSVmpi2

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib/sparcv9
    _foamAddLib     /opt/FSUNf90/lib/sparcv9
    _foamAddLib     /opt/FJSVpnidt/lib
    ;;

QSMPI)
    export FOAM_MPI=qsmpi
    export MPI_ARCH_PATH=/usr/lib/mpi

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    ;;

SGIMPI)
    lastCharID=$(( ${#MPI_ROOT} - 1 ))
    if [ "${MPI_ROOT:$lastCharID:1}" == '/' ]
    then
        MPI_ROOT=${MPI_ROOT:0:$lastCharID}
    fi

    export FOAM_MPI=${MPI_ROOT##*/}
    export MPI_ARCH_PATH=$MPI_ROOT

    if [ ! -d "$MPI_ROOT" -o -z "$MPI_ARCH_PATH" ]
    then
        echo "Warning in $WM_PROJECT_DIR/etc/config/settings.sh:" 1>&2
        echo "    MPI_ROOT not a valid mpt installation directory or ending in a '/'." 1>&2
        echo "    Please set MPI_ROOT to the mpt installation directory." 1>&2
        echo "    MPI_ROOT currently set to '$MPI_ROOT'" 1>&2
    fi

    if [ "$FOAM_VERBOSE" -a "$PS1" ]
    then
        echo "Using SGI MPT:"
        echo "    MPI_ROOT : $MPI_ROOT"
        echo "    FOAM_MPI : $FOAM_MPI"
    fi

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    ;;

INTELMPI)
    lastCharID=$(( ${#MPI_ROOT} - 1 ))
    if [ "${MPI_ROOT:$lastCharID:1}" == '/' ]
    then
        MPI_ROOT=${MPI_ROOT:0:$lastCharID}
    fi

    export FOAM_MPI=${MPI_ROOT##*/}
    export MPI_ARCH_PATH=$MPI_ROOT

    if [ ! -d "$MPI_ROOT" -o -z "$MPI_ARCH_PATH" ]
    then
        echo "Warning in $WM_PROJECT_DIR/etc/config/settings.sh:" 1>&2
        echo "    MPI_ROOT not a valid mpt installation directory or ending in a '/'." 1>&2
        echo "    Please set MPI_ROOT to the mpt installation directory." 1>&2
        echo "    MPI_ROOT currently set to '$MPI_ROOT'" 1>&2
    fi

    if [ "$FOAM_VERBOSE" -a "$PS1" ]
    then
        echo "Using INTEL MPI:"
        echo "    MPI_ROOT : $MPI_ROOT"
        echo "    FOAM_MPI : $FOAM_MPI"
    fi

    _foamAddPath    $MPI_ARCH_PATH/bin64
    _foamAddLib     $MPI_ARCH_PATH/lib64
    ;;
*)
    export FOAM_MPI=dummy
    ;;
esac

# add (non-dummy) MPI implementation
# dummy MPI already added to LD_LIBRARY_PATH and has no external libraries
if [ "$FOAM_MPI" != dummy ]
then
    _foamAddLib $FOAM_LIBBIN/$FOAM_MPI:$FOAM_EXT_LIBBIN/$FOAM_MPI
fi



# Set the minimum MPI buffer size (used by all platforms except SGI MPI)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
: ${minBufferSize:=20000000}

if [ "${MPI_BUFFER_SIZE:=$minBufferSize}" -lt $minBufferSize ]
then
    MPI_BUFFER_SIZE=$minBufferSize
fi
export MPI_BUFFER_SIZE


# cleanup environment:
# ~~~~~~~~~~~~~~~~~~~~
unset _foamAddPath _foamAddLib _foamAddMan foamCompiler minBufferSize

# ----------------------------------------------------------------- end-of-file
