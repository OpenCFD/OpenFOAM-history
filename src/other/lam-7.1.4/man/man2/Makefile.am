# -*- makefile -*-
#
# Copyright (c) 2001-2002 The Trustees of Indiana University.  
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
# $Id: Makefile.am,v 1.11 2002/10/09 20:56:12 brbarret Exp $
#

include $(top_srcdir)/config/Makefile.options

trillium_man		= \
			  atkexit.2 \
			  drecv.2 \
			  dsend.2 \
			  GALL.2 \
			  GCOMPS.2 \
			  getall.2 \
			  getcomps.2 \
			  getjones.2 \
			  getnall.2 \
			  getnjones.2 \
			  getnodeid.2 \
			  getnodes.2 \
			  getnodetype.2 \
			  getnotb.2 \
			  getntype.2 \
			  getorigin.2 \
			  getotbs.2 \
			  getrent.2 \
			  getrentc.2 \
			  getroute.2 \
			  getroute2.2 \
			  getrtype.2 \
			  GJONES.2 \
			  GNODES.2 \
			  GOTBS.2 \
			  IGNALL.2 \
			  IGNCMP.2 \
			  IGNDID.2 \
			  IGNDTP.2 \
			  IGNJON.2 \
			  IGNOTB.2 \
			  IGNTP.2 \
			  IGORGN.2 \
			  IGRTP.2 \
			  introc.2 \
			  INTROF.2 \
			  kattach.2 \
			  kcreate.2 \
			  kdetach.2 \
			  kdoom.2 \
			  kenter.2 \
			  KENTR.2 \
			  _kexit.2 \
			  kexit.2 \
			  kinit.2 \
			  krecv.2 \
			  ksend.2 \
			  ksr.2 \
			  kstate.2 \
			  KXIT.2 \
			  lamf_rfclose.2 \
			  lamf_rfopen.2 \
			  lamf_rfread.2 \
			  lamf_rfwrite.2 \
			  lam_kpause.2 \
			  lam_ksigblock.2 \
			  lam_ksignal.2 \
			  lam_ksigretry.2 \
			  lam_ksigsetmask.2 \
			  lam_ksigsetretry.2 \
			  lam_rfclose.2 \
			  lam_rfincr.2 \
			  lam_rflseek.2 \
			  lam_rfopen.2 \
			  lam_rfposix.2 \
			  lam_rfread.2 \
			  lam_rfrmfd.2 \
			  lam_rfstat.2 \
			  lam_rfstate.2 \
			  lam_rfwrite.2 \
			  lam_rtrfforget.2 \
			  lam_rtrfget.2 \
			  lam_rtrforget.2 \
			  lam_rtrget.2 \
			  lam_rtrstore.2 \
			  lam_rtrsweep.2 \
			  lam_rtrudie.2 \
			  lam_rtrwipe.2 \
			  lpattach.2 \
			  lpdetach.2 \
			  NPROB.2 \
			  nprobe.2 \
			  NRCV.2 \
			  nrecv.2 \
			  nsend.2 \
			  NSND.2 \
			  PRCV.2 \
			  PRCVC.2 \
			  PRCVO.2 \
			  precv.2 \
			  psend.2 \
			  PSND.2 \
			  PSNDC.2 \
			  PSNDO.2 \
			  rbflook.2 \
			  rbfparms.2 \
			  rbfstate.2 \
			  rbfsweep.2 \
			  rbfudie.2 \
			  rbfwipe.2 \
			  recho.2 \
			  rflat.2 \
			  rflclean.2 \
			  rforget.2 \
			  rget.2 \
			  rload.2 \
			  rpcreate.2 \
			  rpdoom.2 \
			  rpgo.2 \
			  RPLDGO.2 \
			  rploadgo.2 \
			  rpspawn.2 \
			  rpstate.2 \
			  rpwait.2 \
			  RPWT.2 \
			  rrsetrents.2 \
			  TRCV.2 \
			  trecv.2 \
			  TRROR.2 \
			  tsend.2 \
			  TSND.2

# Odd.  Automake 1.4 won't allow us to put $(trillium_man) directly in
# man_MANS, but it will let us put it in a macro that goes into
# man_MANS.

if WANT_TRILLIUM_INSTALL
install_trill_man	= $(trillium_man)
else
install_trill_man	=
endif

install_man		= $(install_trill_man)
man_MANS		= $(install_man)

EXTRA_DIST		= $(trillium_man)

# Prevent some duplication

clean-local:
	$(RM) -f *~ atkexit.2 drecv.2 GALL.2 GCOMPS.2 getall.2 \
		  getcomps.2 getjones.2 getnall.2 getnjones.2 \
		  getnodeid.2 getnodetype.2 getnotb.2 getorigin.2 \
		  getotbs.2 getrentc.2 getroute.2 getroute2.2 \
		  getrtype.2 GJONES.2 GNODES.2 GOTBS.2 IGNALL.2 \
		  IGNCMP.2 IGNDID.2 IGNDTP.2 IGNJON.2 IGNOTB.2 \
		  IGNTP.2 IGORGN.2 IGRTP.2 kdetach.2 KENTR.2 \
		  _kexit.2 krecv.2 ksr.2 KXIT.2 lamf_rfclose.2 \
		  lamf_rfwrite.2 lam_kpause.2 lam_ksigretry.2 \
		  lam_ksigsetmask.2 lam_ksigsetretry.2 lam_rfclose.2 \
		  lam_rfincr.2 lam_rflseek.2 lam_rfopen.2 \
		  lam_rfread.2 lam_rfstat.2 lam_rfwrite.2 \
		  lam_rtrfforget.2 lam_rtrfget.2 lam_rtrforget.2 \
		  lam_rtrudie.2 lam_rtrwipe.2 lpdetach.2 NPROB.2 \
		  NRCV.2 nrecv.2 NSND.2 PRCV.2 PRCVC.2 PRCVO.2 \
		  precv.2 PSND.2 PSNDC.2 PSNDO.2 rbfsweep.2 \
		  bfudie.2 rbfwipe.2 rflclean.2 rforget.2 RPLDGO.2 \
		  RPWT.2 TRCV.2 trecv.2 TSND.2 rbfudie.2

m2dir			= $(top_srcdir)/man/man2

atkexit.2: $(m2dir)/kexit.2
	cp -f $(m2dir)/kexit.2 atkexit.2
drecv.2: $(m2dir)/dsend.2
	cp -f $(m2dir)/dsend.2 drecv.2
GALL.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 GALL.2
GCOMPS.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 GCOMPS.2
getall.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 getall.2
getcomps.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 getcomps.2
getjones.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 getjones.2
getnall.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 getnall.2
getnjones.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 getnjones.2
getnodeid.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 getnodeid.2
getnodetype.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 getnodetype.2
getnotb.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 getnotb.2
getorigin.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 getorigin.2
getotbs.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 getotbs.2
getrentc.2: $(m2dir)/getrent.2
	cp -f $(m2dir)/getrent.2 getrentc.2
getroute.2: $(m2dir)/getrent.2
	cp -f $(m2dir)/getrent.2 getroute.2
getroute2.2: $(m2dir)/getrent.2
	cp -f $(m2dir)/getrent.2 getroute2.2
getrtype.2: $(m2dir)/getrent.2
	cp -f $(m2dir)/getrent.2 getrtype.2
GJONES.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 GJONES.2
GNODES.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 GNODES.2
GOTBS.2: $(m2dir)/getnodes.2
	cp -f $(m2dir)/getnodes.2 GOTBS.2
IGNALL.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGNALL.2
IGNCMP.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGNCMP.2
IGNDID.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGNDID.2
IGNDTP.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGNDTP.2
IGNJON.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGNJON.2
IGNOTB.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGNOTB.2
IGNTP.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGNTP.2
IGORGN.2: $(m2dir)/getntype.2
	cp -f $(m2dir)/getntype.2 IGORGN.2
IGRTP.2: $(m2dir)/getrent.2
	cp -f $(m2dir)/getrent.2 IGRTP.2
kdetach.2: $(m2dir)/kattach.2
	cp -f $(m2dir)/kattach.2 kdetach.2
KENTR.2: $(m2dir)/kenter.2
	cp -f $(m2dir)/kenter.2 KENTR.2
_kexit.2: $(m2dir)/kexit.2
	cp -f $(m2dir)/kexit.2 _kexit.2
krecv.2: $(m2dir)/ksend.2
	cp -f $(m2dir)/ksend.2 krecv.2
ksr.2: $(m2dir)/ksend.2
	cp -f $(m2dir)/ksend.2 ksr.2
KXIT.2: $(m2dir)/kexit.2
	cp -f $(m2dir)/kexit.2 KXIT.2
lamf_rfclose.2: $(m2dir)/lamf_rfopen.2
	cp -f $(m2dir)/lamf_rfopen.2 lamf_rfclose.2
lamf_rfwrite.2: $(m2dir)/lamf_rfread.2
	cp -f $(m2dir)/lamf_rfread.2 lamf_rfwrite.2
lam_kpause.2: $(m2dir)/lam_ksigblock.2
	cp -f $(m2dir)/lam_ksigblock.2 lam_kpause.2
lam_ksigretry.2: $(m2dir)/lam_ksigblock.2
	cp -f $(m2dir)/lam_ksigblock.2 lam_ksigretry.2
lam_ksigsetmask.2: $(m2dir)/lam_ksigblock.2
	cp -f $(m2dir)/lam_ksigblock.2 lam_ksigsetmask.2
lam_ksigsetretry.2: $(m2dir)/lam_ksigblock.2
	cp -f $(m2dir)/lam_ksigblock.2 lam_ksigsetretry.2
lam_rfclose.2: $(m2dir)/lam_rfposix.2
	cp -f $(m2dir)/lam_rfposix.2 lam_rfclose.2
lam_rfincr.2: $(m2dir)/lam_rfrmfd.2
	cp -f $(m2dir)/lam_rfrmfd.2 lam_rfincr.2
lam_rflseek.2: $(m2dir)/lam_rfposix.2
	cp -f $(m2dir)/lam_rfposix.2 lam_rflseek.2
lam_rfopen.2: $(m2dir)/lam_rfposix.2
	cp -f $(m2dir)/lam_rfposix.2 lam_rfopen.2
lam_rfread.2: $(m2dir)/lam_rfposix.2
	cp -f $(m2dir)/lam_rfposix.2 lam_rfread.2
lam_rfstat.2: $(m2dir)/lam_rfposix.2
	cp -f $(m2dir)/lam_rfposix.2 lam_rfstat.2
lam_rfwrite.2: $(m2dir)/lam_rfposix.2
	cp -f $(m2dir)/lam_rfposix.2 lam_rfwrite.2
lam_rtrfforget.2: $(m2dir)/lam_rtrget.2
	cp -f $(m2dir)/lam_rtrget.2 lam_rtrfforget.2
lam_rtrfget.2: $(m2dir)/lam_rtrget.2
	cp -f $(m2dir)/lam_rtrget.2 lam_rtrfget.2
lam_rtrforget.2: $(m2dir)/lam_rtrget.2
	cp -f $(m2dir)/lam_rtrget.2 lam_rtrforget.2
lam_rtrudie.2: $(m2dir)/lam_rtrsweep.2
	cp -f $(m2dir)/lam_rtrsweep.2 lam_rtrudie.2
lam_rtrwipe.2: $(m2dir)/lam_rtrsweep.2
	cp -f $(m2dir)/lam_rtrsweep.2 lam_rtrwipe.2
lpdetach.2: $(m2dir)/lpattach.2
	cp -f $(m2dir)/lpattach.2 lpdetach.2
NPROB.2: $(m2dir)/nprobe.2
	cp -f $(m2dir)/nprobe.2 NPROB.2
NRCV.2: $(m2dir)/nsend.2
	cp -f $(m2dir)/nsend.2 NRCV.2
nrecv.2: $(m2dir)/nsend.2
	cp -f $(m2dir)/nsend.2 nrecv.2
NSND.2: $(m2dir)/nsend.2
	cp -f $(m2dir)/nsend.2 NSND.2
PRCV.2: $(m2dir)/psend.2
	cp -f $(m2dir)/psend.2 PRCV.2
PRCVC.2: $(m2dir)/psend.2
	cp -f $(m2dir)/psend.2 PRCVC.2
PRCVO.2: $(m2dir)/psend.2
	cp -f $(m2dir)/psend.2 PRCVO.2
precv.2: $(m2dir)/psend.2
	cp -f $(m2dir)/psend.2 precv.2
PSND.2: $(m2dir)/psend.2
	cp -f $(m2dir)/psend.2 PSND.2
PSNDC.2: $(m2dir)/psend.2
	cp -f $(m2dir)/psend.2 PSNDC.2
PSNDO.2: $(m2dir)/psend.2
	cp -f $(m2dir)/psend.2 PSNDO.2
rbfsweep.2: $(m2dir)/rbfparms.2
	cp -f $(m2dir)/rbfparms.2 rbfsweep.2
rbfudie.2: $(m2dir)/rbfparms.2
	cp -f $(m2dir)/rbfparms.2 rbfudie.2
rbfwipe.2: $(m2dir)/rbfparms.2
	cp -f $(m2dir)/rbfparms.2 rbfwipe.2
rflclean.2: $(m2dir)/rflat.2
	cp -f $(m2dir)/rflat.2 rflclean.2
rforget.2: $(m2dir)/rget.2
	cp -f $(m2dir)/rget.2 rforget.2
RPLDGO.2: $(m2dir)/rploadgo.2
	cp -f $(m2dir)/rploadgo.2 RPLDGO.2
RPWT.2: $(m2dir)/rpwait.2
	cp -f $(m2dir)/rpwait.2 RPWT.2
TRCV.2: $(m2dir)/tsend.2
	cp -f $(m2dir)/tsend.2 TRCV.2
trecv.2: $(m2dir)/tsend.2
	cp -f $(m2dir)/tsend.2 trecv.2
TSND.2: $(m2dir)/tsend.2 
	cp -f $(m2dir)/tsend.2  TSND.2
