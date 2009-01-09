# -----------------------------------------------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 1991-2009 OpenCFD Ltd.
#    \\/     M anipulation  |
# -----------------------------------------------------------------------------
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
# Script
#     doxyFilt.awk
#
# Description
#     Converts cocoon style sentinel strings into doxygen style strings
#
#     Assumes comment strings are formatted as follows
#         //- general description
#         //  more information
#         //  and even more information
#     This should be re-formatted as the following
#         //! general description
#         /*!
#         more information
#         and even more information
#         */
#     The intermediate "/*! ... */" block is left-justified to handle
#     possible verbatim text
# -----------------------------------------------------------------------------

BEGIN {
    state = 0
}

/^ *\/\/-/ {
    state = 1
    sub(/\/\/-/, "//!")
    print
    next
}


/^ *\/\// {
    # start comment block
    if (state == 1)
    {
        printf "/*!\n"
        state = 2
    }

    # inside comment block
    if (state == 2)
    {
        if (!sub(/^ *\/\/  /, ""))
        {
            sub(/^ *\/\//, "")
        }
    }

    print
    next
}


{
    # end comment block
    if (state == 2)
    {
        printf "*/\n"
    }
    state = 0
    print
    next
}

# -----------------------------------------------------------------------------
