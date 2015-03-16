#----------------------------------*-sh-*--------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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
#     etc/config/settings.csh
#
# Description
#     Startup file for OpenFOAM
#     Sourced from OpenFOAM-<VERSION>/etc/cshrc
#
#------------------------------------------------------------------------------

# Prefix to PATH
alias _foamAddPath 'setenv PATH \!*\:${PATH}'
# Prefix to LD_LIBRARY_PATH
alias _foamAddLib 'setenv LD_LIBRARY_PATH \!*\:${LD_LIBRARY_PATH}'
# Prefix to MANPATH
alias _foamAddMan 'setenv MANPATH \!*\:${MANPATH}'

#------------------------------------------------------------------------------
# Set environment variables according to system type
setenv WM_ARCH `uname -s`

# Default WM_COMPILER_LIB_ARCH for 32bit
setenv WM_COMPILER_LIB_ARCH

switch ($WM_ARCH)
case Linux:
    setenv WM_ARCH linux

    switch (`uname -m`)
    case i686:
        breaksw

    case x86_64:
        switch ($WM_ARCH_OPTION)
        case 32:
            setenv WM_COMPILER_ARCH 64
            setenv WM_CC 'gcc'
            setenv WM_CXX 'g++'
            setenv WM_CFLAGS '-m32 -fPIC'
            setenv WM_CXXFLAGS '-m32 -fPIC'
            setenv WM_LDFLAGS '-m32'
            breaksw

        case 64:
            setenv WM_ARCH linux64
            setenv WM_COMPILER_LIB_ARCH 64
            setenv WM_CC 'gcc'
            setenv WM_CXX 'g++'
            setenv WM_CFLAGS '-m64 -fPIC'
            setenv WM_CXXFLAGS '-m64 -fPIC'
            setenv WM_LDFLAGS '-m64'
            breaksw

        default:
            echo "Unknown WM_ARCH_OPTION '$WM_ARCH_OPTION', should be 32 or 64"
            breaksw

        endsw
        breaksw

    case ia64:
        setenv WM_ARCH linuxIA64
        setenv WM_COMPILER I64
        breaksw

    case mips64:
        setenv WM_ARCH SiCortex64
        setenv WM_MPLIB MPI
        setenv WM_COMPILER_LIB_ARCH 64
        setenv WM_CC 'gcc'
        setenv WM_CXX 'g++'
        setenv WM_CFLAGS '-mabi=64 -fPIC'
        setenv WM_CXXFLAGS '-mabi=64 -fPIC'
        setenv WM_LDFLAGS '-mabi=64 -G0'
        breaksw

    case ppc64:
        setenv WM_ARCH linuxPPC64
        setenv WM_COMPILER_LIB_ARCH 64
        setenv WM_CC 'gcc'
        setenv WM_CXX 'g++'
        setenv WM_CFLAGS '-m64 -fPIC'
        setenv WM_CXXFLAGS '-m64 -fPIC'
        setenv WM_LDFLAGS '-m64'
        breaksw

    default:
        echo Unknown processor type `uname -m` for Linux
        breaksw

    endsw
    breaksw

case SunOS:
    setenv WM_ARCH SunOS64
    setenv WM_MPLIB FJMPI
    setenv WM_COMPILER_LIB_ARCH 64
    setenv WM_CC 'gcc'
    setenv WM_CXX 'g++'
    setenv WM_CFLAGS '-mabi=64 -fPIC'
    setenv WM_CXXFLAGS '-mabi=64 -fPIC'
    setenv WM_LDFLAGS '-mabi=64 -G0'
    breaksw

default:
    echo
    echo "Your '$WM_ARCH' operating system is not supported by this release"
    echo "of OpenFOAM. For further assistance, please contact www.OpenFOAM.org"
    echo
    breaksw

endsw


#------------------------------------------------------------------------------

# Location of the jobControl directory
setenv FOAM_JOB_DIR $WM_PROJECT_INST_DIR/jobControl

# wmake configuration
setenv WM_DIR $WM_PROJECT_DIR/wmake
setenv WM_LINK_LANGUAGE c++
setenv WM_LABEL_OPTION Int$WM_LABEL_SIZE
setenv WM_OPTIONS $WM_ARCH$WM_COMPILER$WM_PRECISION_OPTION$WM_LABEL_OPTION$WM_COMPILE_OPTION

# Base executables/libraries
setenv FOAM_APPBIN $WM_PROJECT_DIR/platforms/$WM_OPTIONS/bin
setenv FOAM_LIBBIN $WM_PROJECT_DIR/platforms/$WM_OPTIONS/lib

# External (ThirdParty) libraries
setenv FOAM_EXT_LIBBIN $WM_THIRD_PARTY_DIR/platforms/$WM_OPTIONS/lib

# Site-specific directory
if ( $?WM_PROJECT_SITE ) then
    set siteDir=$WM_PROJECT_SITE
else
    set siteDir=$WM_PROJECT_INST_DIR/site
endif

# Shared site executables/libraries
# Similar naming convention as ~OpenFOAM expansion
setenv FOAM_SITE_APPBIN $siteDir/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/bin
setenv FOAM_SITE_LIBBIN $siteDir/$WM_PROJECT_VERSION/platforms/$WM_OPTIONS/lib

# User executables/libraries
setenv FOAM_USER_APPBIN $WM_PROJECT_USER_DIR/platforms/$WM_OPTIONS/bin
setenv FOAM_USER_LIBBIN $WM_PROJECT_USER_DIR/platforms/$WM_OPTIONS/lib

# DynamicCode templates
# - default location is the "~OpenFOAM/codeTemplates/dynamicCode" expansion
# setenv FOAM_CODE_TEMPLATES $WM_PROJECT_DIR/etc/codeTemplates/dynamicCode

# Convenience
setenv FOAM_APP $WM_PROJECT_DIR/applications
setenv FOAM_SRC $WM_PROJECT_DIR/src
setenv FOAM_TUTORIALS $WM_PROJECT_DIR/tutorials
setenv FOAM_UTILITIES $FOAM_APP/utilities
setenv FOAM_SOLVERS $FOAM_APP/solvers
setenv FOAM_RUN $WM_PROJECT_USER_DIR/run

# Add wmake to the path - not required for runtime-only environment
if ( -d "${WM_DIR}" ) setenv PATH ${WM_DIR}:${PATH}
# Add OpenFOAM scripts to the path
setenv PATH ${WM_PROJECT_DIR}/bin:${PATH}

# Add site-specific scripts to path - only if they exist
if ( -d "$siteDir/bin" ) then                       # generic
    _foamAddPath "$siteDir/bin"
endif
if ( -d "$siteDir/$WM_PROJECT_VERSION/bin" ) then   # version-specific
    _foamAddPath "$siteDir/$WM_PROJECT_VERSION/bin"
endif
unset siteDir

_foamAddPath ${FOAM_USER_APPBIN}:${FOAM_SITE_APPBIN}:${FOAM_APPBIN}
# Make sure to pick up dummy versions of external libraries last
_foamAddLib  ${FOAM_USER_LIBBIN}:${FOAM_SITE_LIBBIN}:${FOAM_LIBBIN}:${FOAM_EXT_LIBBIN}:${FOAM_LIBBIN}/dummy

# Compiler settings
# ~~~~~~~~~~~~~~~~~
unset gcc_version gmp_version mpfr_version mpc_version
unsetenv MPFR_ARCH_PATH GMP_ARCH_PATH


# Location of compiler installation
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if ( ! $?foamCompiler ) then
    set foamCompiler=system
    echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
    echo "    foamCompiler not set, using '$foamCompiler'"
endif

switch ("$foamCompiler")
case OpenFOAM:
case ThirdParty:
    switch ("$WM_COMPILER")
    case Gcc:
    case Gcc48:
        set gcc_version=gcc-4.8.4
        set gmp_version=gmp-5.1.2
        set mpfr_version=mpfr-3.1.2
        set mpc_version=mpc-1.0.1
        breaksw
    case Gcc45:
        set gcc_version=gcc-4.5.4
        set gmp_version=gmp-5.1.2
        set mpfr_version=mpfr-3.1.2
        set mpc_version=mpc-1.0.1
        breaksw
    case Gcc46:
        set gcc_version=gcc-4.6.4
        set gmp_version=gmp-5.1.2
        set mpfr_version=mpfr-3.1.2
        set mpc_version=mpc-1.0.1
        breaksw
    case Gcc47:
        set gcc_version=gcc-4.7.4
        set gmp_version=gmp-5.1.2
        set mpfr_version=mpfr-3.1.2
        set mpc_version=mpc-1.0.1
        breaksw
    case Gcc49:
        set gcc_version=gcc-4.9.2
        set gmp_version=gmp-5.1.2
        set mpfr_version=mpfr-3.1.2
        set mpc_version=mpc-1.0.1
        breaksw
    case Gcc50:
        set gcc_version=gcc-5.0.0
        set gmp_version=gmp-5.1.2
        set mpfr_version=mpfr-3.1.2
        set mpc_version=mpc-1.0.1
        breaksw
    case Clang:
        # Using clang - not gcc
        setenv WM_CC 'clang'
        setenv WM_CXX 'clang++'
        set clang_version=llvm-3.5.1
        breaksw
    default:
        echo
        echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
        echo "    Unknown OpenFOAM compiler type '$WM_COMPILER'"
        echo "    Please check your settings"
        echo
        breaksw
    endsw

    # Optional configuration tweaks:
    _foamSource `$WM_PROJECT_DIR/bin/foamEtcFile config/compiler.csh`

    if ( $?gcc_version ) then
        set gccDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$gcc_version
        set gmpDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$gmp_version
        set mpfrDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$mpfr_version
        if ( $?mpc_version ) then
            set mpcDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$mpc_version
        endif

        # Check that the compiler directory can be found
        if ( ! -d "$gccDir" ) then
            echo
            echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
            echo "    Cannot find $gccDir installation."
            echo "    Please install this compiler version or if you wish to" \
                 " use the system compiler,"
            echo "    change the 'foamCompiler' setting to 'system'"
            echo
        endif

        _foamAddMan     $gccDir/man
        _foamAddPath    $gccDir/bin

        # Add compiler libraries to run-time environment
        _foamAddLib     $gccDir/lib$WM_COMPILER_LIB_ARCH

        # Add gmp/mpfr libraries to run-time environment
        _foamAddLib     $gmpDir/lib
        _foamAddLib     $mpfrDir/lib

        # Add mpc libraries (not need for older gcc) to run-time environment
        if ( $?mpc_version ) then
            _foamAddLib     $mpcDir/lib
        endif

        # Used by boost/CGAL:
        setenv MPFR_ARCH_PATH $mpfrDir
        setenv GMP_ARCH_PATH $gmpDir
    endif
    unset gcc_version gccDir
    unset gmp_version gmpDir  mpfr_version mpfrDir  mpc_version mpcDir

    if ( $?clang_version ) then
        set clangDir=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$clang_version

        # Check that the compiler directory can be found
        if ( ! -d "$clangDir" ) then
            echo
            echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
            echo "    Cannot find $clangDir installation."
            echo "    Please install this compiler version or if you wish to" \
                 " use the system compiler,"
            echo "    change the 'foamCompiler' setting to 'system'"
            echo
        endif

        _foamAddMan     $clangDir/man
        _foamAddPath    $clangDir/bin
    endif
    unset clang_version clangDir

    breaksw

case system:
    # Use system compiler
    breaksw

default:
    echo "Warn: foamCompiler='$foamCompiler' is unsupported"
    echo "   treating as 'system' instead"
    breaksw
endsw


#
# Add c++0x flags for external programs
#
if ( $?WM_CXXFLAGS ) then
    switch ("$WM_COMPILER")
    case Gcc*++0x:
        setenv WM_CXXFLAGS "$WM_CXXFLAGS -std=c++0x"
        breaksw
    endsw
endif



# Communications library
# ~~~~~~~~~~~~~~~~~~~~~~

unsetenv MPI_ARCH_PATH MPI_HOME FOAM_MPI_LIBBIN

switch ("$WM_MPLIB")
case SYSTEMOPENMPI:
    # Use the system installed openmpi, get library directory via mpicc
    setenv FOAM_MPI openmpi-system

    set libDir=`mpicc --showme:link | sed -e 's/.*-L\([^ ]*\).*/\1/'`

    # Bit of a hack: strip off 'lib' and hope this is the path to openmpi
    # Include files and libraries.
    setenv MPI_ARCH_PATH "${libDir:h}"

    _foamAddLib     $libDir
    unset libDir
    breaksw

case OPENMPI:
    setenv FOAM_MPI openmpi-1.8.4
    # Optional configuration tweaks:
    _foamSource `$WM_PROJECT_DIR/bin/foamEtcFile config/openmpi.csh`

    setenv MPI_ARCH_PATH $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$FOAM_MPI

    # Tell OpenMPI where to find its install directory
    setenv OPAL_PREFIX $MPI_ARCH_PATH

    _foamAddPath    $MPI_ARCH_PATH/bin

    # 64-bit on OpenSuSE 12.1 uses lib64 others use lib
    _foamAddLib     $MPI_ARCH_PATH/lib$WM_COMPILER_LIB_ARCH
    _foamAddLib     $MPI_ARCH_PATH/lib

    _foamAddMan     $MPI_ARCH_PATH/share/man
    breaksw

case SYSTEMMPI:
    setenv FOAM_MPI mpi-system

    if ( ! ($?MPI_ROOT) ) then
        echo
        echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
        echo "    Please set the environment variable MPI_ROOT to point to" \
             " the base folder for the system MPI in use."
        echo "    Example:"
        echo
        echo "        setenv MPI_ROOT /opt/mpi"
        echo
    else
        setenv MPI_ARCH_PATH $MPI_ROOT

        if ( ! ($?MPI_ARCH_FLAGS) ) then
            echo
            echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
            echo "    MPI_ARCH_FLAGS is not set. Example:"
            echo
            echo '        setenv MPI_ARCH_FLAGS "-DOMPI_SKIP_MPICXX"'
            echo
        endif

        if ( ! ($?MPI_ARCH_INC) ) then
            echo
            echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
            echo "    MPI_ARCH_INC is not set. Example:"
            echo
            echo '        setenv MPI_ARCH_INC "-I$MPI_ROOT/include"'
            echo
        endif

        if ( ! ($?MPI_ARCH_LIBS) ) then
            echo
            echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
            echo "    MPI_ARCH_LIBS is not set. Example:"
            echo
            echo '        setenv MPI_ARCH_LIBS "-L$MPI_ROOT/lib -lmpi"'
            echo
        endif
    endif
    breaksw

case MPICH:
    setenv FOAM_MPI mpich2-1.1.1p1
    setenv MPI_HOME $WM_THIRD_PARTY_DIR/$FOAM_MPI
    setenv MPI_ARCH_PATH $WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$FOAM_MPI

    _foamAddPath    $MPI_ARCH_PATH/bin

    # 64-bit on OpenSuSE 12.1 uses lib64 others use lib
    _foamAddLib     $MPI_ARCH_PATH/lib$WM_COMPILER_LIB_ARCH
    _foamAddLib     $MPI_ARCH_PATH/lib

    _foamAddMan     $MPI_ARCH_PATH/share/man
    breaksw

case MPICH-GM:
    setenv FOAM_MPI mpich-gm
    setenv MPI_ARCH_PATH /opt/mpi
    setenv MPICH_PATH $MPI_ARCH_PATH
    setenv GM_LIB_PATH /opt/gm/lib64

    _foamAddPath    $MPI_ARCH_PATH/bin

    # 64-bit on OpenSuSE 12.1 uses lib64 others use lib
    _foamAddLib     $MPI_ARCH_PATH/lib$WM_COMPILER_LIB_ARCH
    _foamAddLib     $MPI_ARCH_PATH/lib

    _foamAddLib     $GM_LIB_PATH
    breaksw

case HPMPI:
    setenv FOAM_MPI hpmpi
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
    breaksw

case MPI:
    setenv FOAM_MPI mpi
    setenv MPI_ARCH_PATH /opt/mpi
    breaksw

case FJMPI:
    setenv FOAM_MPI fjmpi
    setenv MPI_ARCH_PATH /opt/FJSVmpi2

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib/sparcv9
    _foamAddLib     /opt/FSUNf90/lib/sparcv9
    _foamAddLib     /opt/FJSVpnidt/lib
    breaksw

case QSMPI:
    setenv FOAM_MPI qsmpi
    setenv MPI_ARCH_PATH /usr/lib/mpi

    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    breaksw

case SGIMPI:
    if ( ! $?MPI_ROOT) setenv MPI_ROOT /dummy

    if ( ! -d "$MPI_ROOT" ) then
        echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
        echo "    MPI_ROOT not a valid mpt installation directory."
        echo "    Please set MPI_ROOT to the mpt installation directory."
        echo "    (usually done by loading the mpt module)"
        echo "    MPI_ROOT currently set to '$MPI_ROOT'"
    endif

    if ( "${MPI_ROOT:h}/" == $MPI_ROOT ) then
        setenv MPI_ROOT ${MPI_ROOT:h}
    endif

    setenv FOAM_MPI ${MPI_ROOT:t}
    setenv MPI_ARCH_PATH $MPI_ROOT


    if ($?FOAM_VERBOSE && $?prompt) then
        echo "Using SGI MPT:"
        echo "    MPI_ROOT : $MPI_ROOT"
        echo "    FOAM_MPI : $FOAM_MPI"
    endif


    _foamAddPath    $MPI_ARCH_PATH/bin
    _foamAddLib     $MPI_ARCH_PATH/lib
    breaksw

case INTELMPI:
    if ( ! $?MPI_ROOT) setenv MPI_ROOT /dummy

    if ( ! -d "$MPI_ROOT" ) then
        echo "Warning in $WM_PROJECT_DIR/etc/config/settings.csh:"
        echo "    MPI_ROOT not a valid mpt installation directory."
        echo "    Please set MPI_ROOT to the mpt installation directory."
        echo "    (usually done by loading the mpt module)"
        echo "    MPI_ROOT currently set to '$MPI_ROOT'"
    endif

    if ( "${MPI_ROOT:h}/" == $MPI_ROOT ) then
        setenv MPI_ROOT ${MPI_ROOT:h}
    endif

    setenv FOAM_MPI ${MPI_ROOT:t}
    setenv MPI_ARCH_PATH $MPI_ROOT


    if ($?FOAM_VERBOSE && $?prompt) then
        echo "Using INTEL MPT:"
        echo "    MPI_ROOT : $MPI_ROOT"
        echo "    FOAM_MPI : $FOAM_MPI"
    endif


    _foamAddPath    $MPI_ARCH_PATH/bin64
    _foamAddLib     $MPI_ARCH_PATH/lib64
    breaksw

default:
    setenv FOAM_MPI dummy
    breaksw
endsw


# Add (non-dummy) MPI implementation
# Dummy MPI already added to LD_LIBRARY_PATH and has no external libraries
if ( "$FOAM_MPI" != dummy ) then
    _foamAddLib ${FOAM_LIBBIN}/${FOAM_MPI}:${FOAM_EXT_LIBBIN}/${FOAM_MPI}
endif



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


# Cleanup environment:
# ~~~~~~~~~~~~~~~~~~~~
#- keep _foamAddPath _foamAddLib _foamAddMan
unset minBufferSize
unsetenv foamCompiler

# ----------------------------------------------------------------- end-of-file
