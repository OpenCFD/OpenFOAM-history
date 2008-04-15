/*
 * Copyright (c) 2001-2002 The Trustees of Indiana University.  
 *                         All rights reserved.
 * Copyright (c) 1998-2001 University of Notre Dame. 
 *                         All rights reserved.
 * Copyright (c) 1994-1998 The Ohio State University.  
 *                         All rights reserved.
 * 
 * This file is part of the LAM/MPI software package.  For license
 * information, see the LICENSE file in the top level directory of the
 * LAM/MPI source distribution.
 * 
 * $HEADER$
 *	Ohio Trollius
 *	Copyright 1997 The Ohio State University
 *	RBD/NJN
 *
 *	$Id: blktype.c,v 6.12 2002/10/09 20:57:01 brbarret Exp $
 *
 *	Function:	- get description of blocking request type code
 *	Accepts:	- request type code
 *	Returns:	- description string or NULL
 */

#include <errno.h>

#include "blktype.h"
#include "lam.h"

/*
 * local variables
 */
static CONST char	*blkdesc[] = {
		"MPI_Buffer_attach",		/* BLKMPIBUFATTACH */
		"MPI_Buffer_detach",		/* BLKMPIBUFDETACH */
		"MPI_Initialized",		/* BLKMPIINITED */
		"MPI_Abort",			/* BLKMPIABORT */
		"MPI_Address",			/* BLKMPIADDRESS */
		"MPI_Allgather",		/* BLKMPIALLGATHER */
		"MPI_Allgatherv",		/* BLKMPIALLGATHERV */
		"MPI_Allreduce",		/* BLKMPIALLREDUCE */
		"MPI_Alltoall",			/* BLKMPIALLTOALL */
		"MPI_Alltoallv",		/* BLKMPIALLTOALLV */
		"MPI_Attr_delete",		/* BLKMPIATTRDEL */
		"MPI_Attr_get",			/* BLKMPIATTRGET */
		"MPI_Attr_put",			/* BLKMPIATTRPUT */
		"MPI_Barrier",			/* BLKMPIBARRIER */
		"MPI_Bcast",			/* BLKMPIBCAST */
		"MPI_Bsend",			/* BLKMPIBSEND */
		"MPI_Bsend_init",		/* BLKMPIBSINIT */
		"MPI_Cancel",			/* BLKMPICANCEL */
		"MPI_Cart_coords",		/* BLKMPICARTCOORDS */
		"MPI_Cart_create",		/* BLKMPICARTCREATE */
		"MPI_Cartdim_get",		/* BLKMPICARTDIMGET */
		"MPI_Cart_get",			/* BLKMPICARTGET */
		"MPI_Cart_map",			/* BLKMPICARTMAP */
		"MPI_Cart_rank",		/* BLKMPICARTRANK */
		"MPI_Cart_shift",		/* BLKMPICARTSHIFT */
		"MPI_Cart_sub",			/* BLKMPICARTSUB */
		"MPI_Comm_compare",		/* BLKMPICOMMCMP */
		"MPI_Comm_create",		/* BLKMPICOMMCREATE */
		"MPI_Comm_dup",			/* BLKMPICOMMDUP */
		"MPI_Comm_free",		/* BLKMPICOMMFREE */
		"MPI_Comm_group",		/* BLKMPICOMMGROUP */
		"MPI_Comm_rank",		/* BLKMPICOMMRANK */
		"MPI_Comm_remote_group",	/* BLKMPICOMMRGROUP */
		"MPI_Comm_remote_size",		/* BLKMPICOMMRSIZE */
		"MPI_Comm_size",		/* BLKMPICOMMSIZE */
		"MPI_Comm_split",		/* BLKMPICOMMSPLIT */
		"MPI_Comm_test_inter",		/* BLKMPICOMMTESTINTER */
		"MPI_Dims_create",		/* BLKMPIDIMSCREATE */
		"MPI_Error_class",		/* BLKMPIERRCLASS */
		"MPI_Errhandler_create",	/* BLKMPIERRCREATE */
		"MPI_Errhandler_free",		/* BLKMPIERRFREE */
		"MPI_Errhandler_get",		/* BLKMPIERRGET */
		"MPI_Errhandler_set",		/* BLKMPIERRSET */
		"MPI_Error_string",		/* BLKMPIERRSTRING */
		"MPI_Finalize",			/* BLKMPIFINAL */
		"MPI_Gather",			/* BLKMPIGATHER */
		"MPI_Gatherv",			/* BLKMPIGATHERV */
		"MPI_Get_count",		/* BLKMPIGETCOUNT */
		"MPI_Get_elements",		/* BLKMPIGETELEM */
		"MPI_Get_processor_name",	/* BLKMPIGETPNAME */
		"MPI_Graph_create",		/* BLKMPIGRAPHCREATE */
		"MPI_Graphdims_get",		/* BLKMPIGRAPHDIMSGET */
		"MPI_Graph_get",		/* BLKMPIGRAPHGET */
		"MPI_Graph_map",		/* BLKMPIGRAPHMAP */
		"MPI_Graph_neighbors",		/* BLKMPIGRAPHNBR */
		"MPI_Graph_neighbors_count",	/* BLKMPIGRAPHNBRCOUNT */
		"MPI_Group_compare",		/* BLKMPIGROUPCMP */
		"MPI_Group_difference",		/* BLKMPIGROUPDIFF */
		"MPI_Group_excl",		/* BLKMPIGROUPEXCL */
		"MPI_Group_free",		/* BLKMPIGROUPFREE */
		"MPI_Group_incl",		/* BLKMPIGROUPINCL */
		"MPI_Group_intersection",	/* BLKMPIGROUPINTER */
		"MPI_Group_rank",		/* BLKMPIGROUPRANK */
		"MPI_Group_range_excl",		/* BLKMPIGROUPREXCL */
		"MPI_Group_range_incl",		/* BLKMPIGROUPRINCL */
		"MPI_Group_size",		/* BLKMPIGROUPSIZE */
		"MPI_Group_translate_ranks",	/* BLKMPIGROUPTRANKS */
		"MPI_Group_union",		/* BLKMPIGROUPUNION */
		"MPI_Ibsend",			/* BLKMPIIBSEND */
		"MPI_Intercomm_create",		/* BLKMPIICOMMCREATE */
		"MPI_Intercomm_merge",		/* BLKMPIICOMMMERGE */
		"MPI_Init",			/* BLKMPIINIT */
		"MPI_Iprobe",			/* BLKMPIIPROBE */
		"MPI_Irecv",			/* BLKMPIIRECV */
		"MPI_Irsend",			/* BLKMPIIRSEND */
		"MPI_Isend",			/* BLKMPIISEND */
		"MPI_Issend",			/* BLKMPIISSEND */
		"MPI_Keyval_create",		/* BLKMPIKEYCREATE */
		"MPI_Keyval_free",		/* BLKMPIKEYFREE */
		"MPI_Op_create",		/* BLKMPIOPCREATE */
		"MPI_Op_free",			/* BLKMPIOPFREE */
		"MPI_Pack",			/* BLKMPIPACK */
		"MPI_Pack_size",		/* BLKMPIPACKSIZE */
		"MPI_Probe",			/* BLKMPIPROBE */
		"MPI_Recv",			/* BLKMPIRECV */
		"MPI_Reduce",			/* BLKMPIREDUCE */
		"MPI_Reduce_scatter",		/* BLKMPIREDUCESCATTER */
		"MPI_Request_free",		/* BLKMPIREQFREE */
		"MPI_Recv_init",		/* BLKMPIRINIT */
		"MPI_Rsend",			/* BLKMPIRSEND */
		"MPI_Rsend_init",		/* BLKMPIRSINIT */
		"MPI_Scan",			/* BLKMPISCAN */
		"MPI_Scatter",			/* BLKMPISCATTER */
		"MPI_Scatterv",			/* BLKMPISCATTERV */
		"MPI_Send",			/* BLKMPISEND */
		"MPI_Sendrecv",			/* BLKMPISENDRECV */
		"MPI_Sendrecv_replace",		/* BLKMPISENDRECVREP */
		"MPI_Send_init",		/* BLKMPISINIT */
		"MPI_Ssend",			/* BLKMPISSEND */
		"MPI_Ssend_init",		/* BLKMPISSINIT */
		"MPI_Start",			/* BLKMPISTART */
		"MPI_Startall",			/* BLKMPISTARTALL */
		"MPI_Type_commit",		/* BLKMPITCOMMIT */
		"MPI_Type_contiguous",		/* BLKMPITCONTIG */
		"MPI_Pcontrol",			/* BLKMPIPCONTROL */
		"MPI_Test",			/* BLKMPITEST */
		"MPI_Testall",			/* BLKMPITESTALL */
		"MPI_Testany",			/* BLKMPITESTANY */
		"MPI_Test_cancelled",		/* BLKMPITESTCANCEL */
		"MPI_Testsome",			/* BLKMPITESTSOME */
		"MPI_Type_extent",		/* BLKMPITEXTENT */
		"MPI_Type_free",		/* BLKMPITFREE */
		"MPI_Type_hindexed",		/* BLKMPITHINDEX */
		"MPI_Type_hvector",		/* BLKMPITHVECTOR */
		"MPI_Type_indexed",		/* BLKMPITINDEX */
		"MPI_Type_lb",			/* BLKMPITLB */
		"MPI_Topo_test",		/* BLKMPITOPOTEST */
		"MPI_Type_size",		/* BLKMPITSIZE */
		"MPI_Type_struct",		/* BLKMPITSTRUCT */
		"MPI_Type_ub",			/* BLKMPITUB */
		"MPI_Type_vector",		/* BLKMPITVECTOR */
		"MPI_Unpack",			/* BLKMPIUNPACK */
		"MPI_Wait",			/* BLKMPIWAIT */
		"MPI_Waitall",			/* BLKMPIWAITALL */
		"MPI_Waitany",			/* BLKMPIWAITANY */
		"MPI_Waitsome",			/* BLKMPIWAITSOME */
		"MPIL_Spawn",			/* BLKMPILSPAWN */
		"MPIL_Comm_parent",		/* BLKMPILCOMMPARENT */
		"MPIL_Trace_on",		/* BLKMPILTRACEON */
		"MPIL_Trace_off",		/* BLKMPILTRACEOFF */
		"MPIL_Comm_id",			/* BLKMPILCOMMID */
		"MPIL_Comm_gps",		/* BLKMPILCOMMGPS */
		"MPIL_Type_id",			/* BLKMPILTYPEID */
		"MPIL_Signal",			/* BLKMPILSIGNAL */
		"MPIL_Universe_size",		/* BLKMPILUNIVERSESIZE */

		"MPI_Get_version",		/* BLKMPIGETVERSION */
		"MPI_Wtick",			/* BLKMPIWTICK */
		"MPI_Wtime",			/* BLKMPIWTIME */

		"MPI_Comm_c2f",			/* BLKMPICOMMC2F */
		"MPI_Type_c2f",			/* BLKMPITYPEC2F */
		"MPI_Group_c2f",		/* BLKMPIGROUPC2F */
		"MPI_Op_c2f",			/* BLKMPIOPC2F */
		"MPI_Request_c2f",		/* BLKMPIREQC2F */
		"MPI_Comm_f2c",			/* BLKMPICOMMF2C */
		"MPI_Type_f2c",			/* BLKMPITYPEF2C */
		"MPI_Group_f2c",		/* BLKMPIGROUPF2C */
		"MPI_Op_f2c",			/* BLKMPIOPF2C */
		"MPI_Request_f2c",		/* BLKMPIREQF2C */
		"MPI_Status_c2f",		/* BLKMPISTATC2F */
		"MPI_Status_f2c",		/* BLKMPISTATF2C */

		"MPI_Type_get_true_extent",	/* BLKMPITTEXTENT */
		"MPI_Type_get_envelope",	/* BLKMPITGETENV */
		"MPI_Type_get_contents",	/* BLKMPITGETCONT */
		"MPI_Finalized",		/* BLKMPIFINALIZED */

		"MPI_Accumulate",		/* BLKMPIACCUMULATE */
		"",				/* BLKMPIADDERRCLASS */
		"MPI_Alloc_mem",		/* BLKMPIALLOCMEM */
		"",				/* BLKMPIFILEREADALL */
		"",				/* BLKMPIALLTOALLW */
		"MPI_Close_port",		/* BLKMPICLOSEPORT */
		"",				/* BLKMPICOMMCALLERRHDL */
		"",				/* BLKMPICOMMCLONE */
		"MPI_Comm_connect",		/* BLKMPICOMMCONNECT */
		"MPI_Comm_create_errhandler",	/* BLKMPICOMMCREATEERRHDL */
		"MPI_Comm_create_keyval",	/* BLKMPICOMMCREATEKEYVAL */
		"MPI_Comm_disconnect",		/* BLKMPICOMMDISCONNECT */
		"MPI_COMM_DUP_FN",		/* BLKMPICOMMDUPFN */
		"MPI_Comm_free_keyval",		/* BLKMPICOMMFREEKEYVAL */
		"MPI_Comm_get_errhandler",	/* BLKMPICOMMGETERRHDL */
		"MPI_Comm_get_name",		/* BLKMPICOMMGETNAME */
		"MPI_Comm_get_parent",		/* BLKMPICOMMGETPARENT */
		"MPI_Comm_join",		/* BLKMPICOMMJOIN */
		"MPI_COMM_NULL_COPY_FN",	/* BLKMPICOMMNULLCOPYFN */
		"MPI_Comm_set_errhandler",	/* BLKMPICOMMSETERRHDL */
		"MPI_Comm_set_name",		/* BLKMPICOMMSETNAME */
		"MPI_Comm_spawn",		/* BLKMPICOMMSPAWN */
		"MPI_Comm_spawn_multiple",	/* BLKMPICOMMSPAWNMULT */
		"",				/* BLKMPIEXSCAN */

		"",				/* BLKMPIFILEC2F */
		"",				/* BLKMPIFILECALLERRHDL */
		"",				/* BLKMPIFILECLOSE */
		"",				/* BLKMPIFILECREATEERRHDL */
		"",				/* BLKMPIFILEDELETE */
		"",				/* BLKMPIFILEF2C */
		"",				/* BLKMPIFILEGETAMODE */
		"",				/* BLKMPIFILEGETATOM */
		"",				/* BLKMPIFILEGETBYTEOFFSET */
		"",				/* BLKMPIFILEGETERRHDL */
		"",				/* BLKMPIFILEGETGROUP */
		"",				/* BLKMPIFILEGETINFO */
		"",				/* BLKMPIFILEGETPOS */
		"",				/* BLKMPIFILEGETPOSSHARED */
		"",				/* BLKMPIFILEGETSIZE */
		"",				/* BLKMPIFILEGETTEXTENT */
		"",				/* BLKMPIFILEGETVIEW */
		"",				/* BLKMPIFILEIREAD */
		"",				/* BLKMPIFILEIREADAT */
		"",				/* BLKMPIFILEIREADSHARED */
		"",				/* BLKMPIFILEIWRITE */
		"",				/* BLKMPIFILEIWRITEAT */
		"",				/* BLKMPIFILEIWRITESHARED */
		"",				/* BLKMPIFILEOPEN */
		"",				/* BLKMPIFILEPREALLOCATE */
		"",				/* BLKMPIFILEREAD */
		"",				/* BLKMPIFILEREADALLBEG */
		"",				/* BLKMPIFILEREADAT */
		"",				/* BLKMPIFILEREADATALLBEG */
		"",				/* BLKMPIFILEREADORD */
		"",				/* BLKMPIFILEREADORDBEG */
		"",				/* BLKMPIFILEREADSHARED */
		"",				/* BLKMPIFILESEEK */
		"",				/* BLKMPIFILESEEKSHARED */
		"",				/* BLKMPIFILESETATOM */
		"",				/* BLKMPIFILESETERRHDL */
		"",				/* BLKMPIFILESETINFO */
		"",				/* BLKMPIFILESETSIZE */
		"",				/* BLKMPIFILESETVIEW */
		"",				/* BLKMPIFILESYNC */
		"",				/* BLKMPIFILEWRITE */
		"",				/* BLKMPIFILEWRITEALLBEG */
		"",				/* BLKMPIFILEWRITEAT */
		"",				/* BLKMPIFILEWRITEATALLBEG */
		"",				/* BLKMPIFILEWRITEORD */
		"",				/* BLKMPIFILEWRITEORDBEG */
		"",				/* BLKMPIFILEWRITESHARED */

		"MPI_Free_mem",			/* BLKMPIFREEMEM */
		"MPI_Get",			/* BLKMPIGET */
		"MPI_Get_address",		/* BLKMPIGETADDRESS */
		"",				/* BLKMPIGREQCOMPLETE */
		"MPI_Info_create",		/* BLKMPIINFOCREATE */
		"MPI_Info_f2c",			/* BLKMPIINFOF2C */
		"MPI_Info_c2f",			/* BLKMPIINFOC2F */
		"MPI_Info_free",		/* BLKMPIINFOFREE */
		"MPI_Init_thread",		/* BLKMPIINITTHREAD */
		"MPI_Lookup_name",		/* BLKMPILOOKUPNAME */
		"MPI_Open_port",		/* BLKMPIOPENPORT */
		"MPI_Pack_external",		/* BLKMPIPACKEXTERNAL */
		"MPI_Publish_name",		/* BLKMPIPUBLISHNAME */
		"MPI_Put",			/* BLKMPIPUT */
		"MPI_Query_thread",		/* BLKMPIQUERYTHREAD */
		"MPI_Register_datarep",		/* BLKMPIREGISTERDATAREP */
		"MPI_Request_get_status",	/* BLKMPIREQGETSTAT */
		"MPI_SIZEOF",			/* BLKMPISIZEOF */
		"MPI_Status_set_cancelled",	/* BLKMPISTATSETCANCELLED */
		"MPI_Type_create_darray",	/* BLKMPITCREATEDARRAY */
		"",				/* BLKMPITCREATEF90COMPLEX */
		"MPI_Type_create_hindexed",	/* BLKMPITCREATEHINDEX */
		"",				/* BLKMPITCREATEINDEXBLOCK */
		"MPI_Type_create_keyval",	/* BLKMPITCREATEKEYVAL */
		"MPI_Type_create_resized",	/* BLKMPITCREATERESIZED */
		"MPI_Type_create_struct",	/* BLKMPITCREATESTRUCT */
		"MPI_Type_create_subarray",	/* BLKMPITCREATESUBARRAY */
		"MPI_Type_delete_attr",		/* BLKMPITDELETEATTR */
		"MPI_Type_dup",			/* BLKMPITDUP */
		"MPI_DUP_FN",			/* BLKMPITDUPFN */
		"MPI_Type_free_keyval",		/* BLKMPITFREEKEYVAL */
		"MPI_Type_get_extent",		/* BLKMPITGETEXTENT */
		"MPI_Type_get_name",		/* BLKMPITGETNAME */
		"",				/* BLKMPITMATCHSIZE */
		"MPI_TYPE_NULL_COPY_FN",	/* BLKMPITNULLCOPYFN */
		"MPI_Type_set_name",		/* BLKMPITSETNAME */
		"",				/* BLKMPIUNPACKEXTERNAL */
		"MPI_Unpublish_name",		/* BLKMPIUNPUBLISHNAME */
		"MPI_Win_c2f",			/* BLKMPIWINC2F */
		"MPI_Win_call_errhandler",	/* BLKMPIWINCALLERRHDL */
		"MPI_Win_complete",		/* BLKMPIWINCOMPLETE */
		"MPI_Win_create",		/* BLKMPIWINCREATE */
		"MPI_Win_create_errhandler",	/* BLKMPIWINCREATEERRHDL */
		"MPI_Win_create_keyval",	/* BLKMPIWINCREATEKEYVAL */
		"MPI_Win_f2c",			/* BLKMPIWINF2C */
		"MPI_Win_fence",		/* BLKMPIWINFENCE */
		"MPI_Win_free",			/* BLKMPIWINFREE */
		"MPI_Win_free_keyval",		/* BLKMPIWINFREEKEYVAL */
		"MPI_Win_get_errhandler",	/* BLKMPIWINGETERRHDL */
		"MPI_Win_get_group",		/* BLKMPIWINGETGROUP */
		"MPI_Win_get_name",		/* BLKMPIWINGETNAME */
		"MPI_Win_lock",			/* BLKMPIWINLOCK */
		"MPI_WIN_NULL_COPY_FN",		/* BLKMPIWINNULLCOPYFN */
		"MPI_Win_post",			/* BLKMPIWINPOST */
		"MPI_Win_set_attr",		/* BLKMPIWINSETATTR */
		"MPI_Win_set_errhandler",	/* BLKMPIWINSETERRHDL */
		"MPI_Win_set_name",		/* BLKMPIWINSETNAME */
		"MPI_Win_start",		/* BLKMPIWINSTART */
		"MPI_Win_test",			/* BLKMPIWINTEST */
		"MPI_Win_unlock",		/* BLKMPIWINUNLOCK */
		"MPI_Win_wait",			/* BLKMPIWINWAIT */
		"",				/* BLKMPIFILEWRITEALL */
		"",				/* BLKMPIOTEST */
		"",				/* BLKMPIOWAIT */
		"",				/* BLKMPIFILEREADATALL */
		"",				/* BLKMPIFILEWRITEATALL */
		"MPI_Info_set",			/* BLKMPIINFOSET */
		"MPI_Info_delete",		/* BLKMPIINFODEL */
		"MPI_Info_get",			/* BLKMPIINFOGET */
		"MPI_Info_get_valuelen",	/* BLKMPIINFOGETLEN */
		"MPI_Info_get_nkeys",		/* BLKMPIINFOGETNKEYS */
		"MPI_Info_get_nthkey",		/* BLKMPIINFOGETNTHKEY */
		"MPI_Info_dup",			/* BLKMPIINFODUP */
		"MPI_Is_thread_main",		/* BLKMPIISTHREADMAIN */
		"MPI_Status_set_elements",	/* BLKMPISTATSETELEM */
		"MPI_Comm_accept",		/* BLKMPICOMMACCEPT */
		"MPI_Comm_get_attr",		/* BLKMPICOMMGETATTR */
		"MPI_Comm_set_attr",		/* BLKMPICOMMSETATTR */
		"MPI_Type_get_attr",		/* BLKMPITGETATTR */
		"MPI_Type_set_attr",		/* BLKMPITSETATTR */
		"MPI_Win_get_attr",		/* BLKMPIWINGETATTR */
		"MPI_Comm_delete_attr",		/* BLKMPICOMMDELETEATTR */
		"MPI_Win_delete_attr",		/* BLKMPIWINDELETEATTR */
		"MPI_Type_create_hvector",	/* BLKMPITCREATEHVECTOR */

		"mpirun",                       /* BLKMPIRUN */

		"MPIL_Request_get_name",        /* BLKMPILREQUESTGETNAME */
		"MPIL_Request_set_name",        /* BLKMPILREQUESTSETNAME */

		"XMPI_Buoy",                    /* BLKXMPIBUOY */
		"XMPI_Coloron",                 /* BLKXMPICOLORON */
		"XMPI_Coloroff",                /* BLKXMPICOLOROFF */

		"LAM_IMPI_router_daemon",       /* BLKIMPIDAEMON */

		"IMPI_Barrier",                 /* BLKIMPIBARRIER */
		"IMPI_Bcast",                   /* BLKIMPIBCAST */
		"IMPI_Gather",                  /* BLKIMPIGATHER */
		"IMPI_Allgather"                /* BLKIMPIALLGATHER */
		"IMPI_Reduce",                  /* BLKIMPIREDUCE */
		"IMPI_Allreduce"                /* BLKIMPIALLREDUCE */
		};

char *
blktype(int4 type)
{
	char	*p;

	if ((type >= BLKLOW) && (type <= BLKHIGH)) {
		p = (char *) blkdesc[type - BLKLOW];
	} else {
		p = 0;
		errno = EINVAL;
	}

	return(p);
}
