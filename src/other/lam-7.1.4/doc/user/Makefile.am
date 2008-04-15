# LaTeX -*-Makefile-*-
#
# Copyright (c) 2001-2003 The Trustees of Indiana University.  
#                         All rights reserved.
# Copyright (c) 1998-2001 University of Notre Dame. 
#                         All rights reserved.
# Copyright (c) 1994-1998 The Ohio State University.  
#                         All rights reserved.
# 
# This file is part of the LAM/MPI software package.  For license
# information, see the LICENSE file in the top level directory of the
# LAM/MPI source distribution.
#
# $Id: Makefile.am,v 1.7 2003/09/25 16:15:04 jsquyres Exp $
#
# This first section contains macros whose values that you need to
# fill in.
#

include $(top_srcdir)/config/Makefile.options

# The majority of stuff to build the PS/PDF files are included in a
# separate Makefile so that they can be invoked directly at "make
# dist" time without needing to generate a real "Makefile" from
# Makefile.am.

include Makefile.latex

# Now ensure that all the relevant files get picked up in the tarball.

EXTRA_DIST      = $(MAIN_TEX) $(OTHER_SRC_FILES) $(FIG_FILES) $(PNG_FILES) $(BIBTEX_SOURCES) Makefile.latex

dist-hook: pdf
	mv user.pdf $(DESTDIR)$(distdir)/../.
