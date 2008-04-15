# -*- makefile -*-
#
# Copyright (c) 2001-2004 The Trustees of Indiana University.  
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
# $Id: Makefile.am,v 1.14 2002/10/09 20:56:13 brbarret Exp $
#

include $(top_srcdir)/config/Makefile.options

if CASE_SENSITIVE_FS
case_sens_man		= LTOT.3 \
			TTOL.3
else
case_sens_man		=
endif

trillium_man		= \
			  all_hash.3 \
			  all_list.3 \
			  all_opt.3 \
			  all_queue.3 \
			  FTTIME.3 \
			  ltot.3 \
			  tstdio.3 \
			  ttime.3 \
			  ttol.3 \
			  $(case_sens_man)

# Do not edit these next few lines without editing
# config/distscript.csh.  We have to split them into multiple lines
# because sed can't handle one really, really long line.
# #@$%#@%@#$%....
generated_man_0 =MPI_Free_mem.3 MPI_Finalized.3 MPI_Finalize.3 MPI_File_write_shared.3 MPI_File_write_ordered_end.3 MPI_File_write_ordered_begin.3 MPI_File_write_ordered.3 MPI_File_write_at_all_end.3 MPI_File_write_at_all_begin.3 MPI_File_write_at_all.3 MPI_File_write_at.3 MPI_File_write_all_end.3 MPI_File_write_all_begin.3 MPI_File_write_all.3 MPI_File_write.3 MPI_File_sync.3 MPI_File_set_view.3 MPI_File_set_size.3 MPI_File_set_info.3 MPI_File_set_errhandler.3 MPI_File_set_atomicity.3 MPI_File_seek_shared.3 MPI_File_seek.3 MPI_File_read_shared.3 MPI_File_read_ordered_end.3 MPI_File_read_ordered_begin.3 MPI_File_read_ordered.3 MPI_File_read_at_all_end.3 MPI_File_read_at_all_begin.3 MPI_File_read_at_all.3 MPI_File_read_at.3 MPI_File_read_all_end.3 MPI_File_read_all_begin.3 MPI_File_read_all.3 MPI_File_read.3 MPI_File_preallocate.3 MPI_File_open.3 MPI_File_iwrite_shared.3 MPI_File_iwrite_at.3 MPI_File_iwrite.3 MPI_File_iread_shared.3 MPI_File_iread_at.3 MPI_File_iread.3 MPI_File_get_view.3 MPI_File_get_type_extent.3 MPI_File_get_size.3 MPI_File_get_position_shared.3 MPI_File_get_position.3 MPI_File_get_info.3 MPI_File_get_group.3 MPI_File_get_errhandler.3 MPI_File_get_byte_offset.3 MPI_File_get_atomicity.3 MPI_File_get_amode.3 MPI_File_f2c.3 MPI_File_delete.3 MPI_File_close.3 MPI_File_c2f.3 MPI_Exscan.3 MPI_Error_string.3 MPI_Error_class.3 MPI_Errhandler_set.3 MPI_Errhandler_get.3 MPI_Errhandler_free.3 MPI_Errhandler_f2c.3 MPI_Errhandler_create.3 MPI_Errhandler_c2f.3 MPI_DUP_FN.3 MPI_Dims_create.3 MPI_Comm_test_inter.3 MPI_Comm_split.3 MPI_Comm_spawn_multiple.3 MPI_Comm_spawn.3 MPI_Comm_size.3 MPI_Comm_set_name.3 MPI_Comm_set_attr.3 MPI_Comm_ser_errhandler.3 MPI_Comm_remote_size.3 MPI_Comm_remote_group.3 MPI_Comm_rank.3 MPI_Comm_join.3 MPI_Comm_group.3 MPI_Comm_get_parent.3 MPI_Comm_get_name.3 MPI_Comm_get_errhandler.3 MPI_Comm_get_attr.3 MPI_Comm_free_keyval.3 MPI_Comm_free.3 MPI_Comm_f2c.3 MPI_COMM_DUP_FN.3 MPI_Comm_dup.3 MPI_Comm_disconnect.3 MPI_Comm_delete_attr.3 MPI_Comm_create_keyval.3 MPI_Comm_create_errhandler.3 MPI_Comm_create.3 MPI_Comm_connect.3 MPI_Comm_compare.3 MPI_Comm_c2f.3 MPI_Comm_accept.3 MPI_Close_port.3 MPI_Cart_sub.3 MPI_Cart_shift.3 MPI_Cart_rank.3 MPI_Cart_map.3 MPI_Cart_get.3 MPI_Cartdim_get.3 MPI_Cart_create.3 MPI_Cart_coords.3 MPI_Cancel.3 MPI_Buffer_detach.3 MPI_Buffer_attach.3 MPI_Bsend_init.3 MPI_Bsend.3 MPI_Bcast.3 MPI_Barrier.3 MPI_Attr_put.3 MPI_Attr_get.3 MPI_Attr_delete.3 MPI_Alltoallw.3 MPI_Alltoallv.3 MPI_Alltoall.3 MPI_Allreduce.3 MPI_Alloc_mem.3 MPI_Allgatherv.3 MPI_Allgather.3 MPI_Address.3 MPI_Accumulate.3 MPI_Abort.3 
generated_man_1 =MPI_Wtime.3 MPI_Wtick.3 MPI_Win_wait.3 MPI_Win_unlock.3 MPI_Win_Test.3 MPI_Win_start.3 MPI_Win_set_name.3 MPI_Win_set_errhandler.3 MPI_Win_set_attr.3 MPI_Win_post.3 MPI_Win_Lock.3 MPI_Win_get_name.3 MPI_Win_get_group.3 MPI_Win_get_errhandler.3 MPI_Win_get_attr.3 MPI_Win_free_keyval.3 MPI_Win_free.3 MPI_Win_fence.3 MPI_Win_f2c.3 MPI_WIN_DUP_FN.3 MPI_Win_delete_attr.3 MPI_Win_create_keyval.3 MPI_Win_create_errhandler.3 MPI_Win_create.3 MPI_Win_complete.3 MPI_Win_call_errhandler.3 MPI_Win_c2f.3 MPI_Waitsome.3 MPI_Waitany.3 MPI_Waitall.3 MPI_Wait.3 MPI_Unpack.3 MPI_Type_vector.3 MPI_Type_ub.3 MPI_Type_struct.3 MPI_Type_size.3 MPI_Type_set_name.3 MPI_Type_set_attr.3 MPI_Type_lb.3 MPI_Type_indexed.3 MPI_Type_hvector.3 MPI_Type_hindexed.3 MPI_Type_get_true_extent.3 MPI_Type_get_name.3 MPI_Type_get_extent.3 MPI_Type_get_envelope.3 MPI_Type_get_contents.3 MPI_Type_get_attr.3 MPI_Type_free_keyval.3 MPI_Type_free.3 MPI_Type_f2c.3 MPI_Type_extent.3 MPI_TYPE_DUP_FN.3 MPI_Type_dup.3 MPI_Type_delete_attr.3 MPI_Type_create_subarray.3 MPI_Type_create_struct.3 MPI_Type_create_keyval.3 MPI_Type_create_hvector.3 MPI_Type_create_hindexed.3 MPI_Type_create_extent.3 MPI_Type_create_darray.3 MPI_Type_contiguous.3 MPI_Type_commit.3 MPI_Type_c2f.3 MPI_Topo_test.3 MPI_Testsome.3 MPI_Test_cancelled.3 MPI_Testany.3 MPI_Testall.3 MPI_Test.3 MPI_Status_f2c.3 MPI_Status_c2f.3 MPI_Startall.3 MPI_Start.3 MPI_Ssend_init.3 MPI_Ssend.3 MPI_Sendrecv_replace.3 MPI_Sendrecv.3 MPI_Send_init.3 MPI_Send.3 MPI_Scatterv.3 MPI_Scatter.3 MPI_Scan.3 MPI_Rsend_init.3 MPI_Rsend.3 MPI_Request_free.3 MPI_Request_f2c.3 MPI_Request_c2f.3 MPI_Reduce_scatter.3 MPI_Reduce.3 MPI_Recv_init.3 MPI_Recv.3 MPI_Query_thread.3 MPI_Put.3 MPI_Publish_name.3 MPI_Probe.3 MPI_Pcontrol.3 MPI_Pack_size.3 MPI_Pack.3 MPI_Op_free.3 MPI_Op_f2c.3 MPI_Open_port.3 MPI_Op_create.3 MPI_Op_c2f.3 MPI_Lookup_name.3 MPI_Keyval_free.3 MPI_Keyval_create.3 MPI_Is_thread_main.3 MPI_Issend.3 MPI_Isend.3 MPI_Irsend.3 MPI_Irecv.3 MPI_Iprobe.3 MPI_Intercomm_merge.3 MPI_Intercomm_create.3 MPI_Init_thread.3 MPI_Initialized.3 MPI_Init.3 MPI_Info_set.3 MPI_Info_get_valuelen.3 MPI_Info_get_nthkey.3 MPI_Info_get_nkeys.3 MPI_Info_get.3 MPI_Info_free.3 MPI_Info_f2c.3 MPI_Info_dup.3 MPI_Info_delete.3 MPI_Info_create.3 MPI_Info_c2f.3 MPI_Ibsend.3 MPI_Group_union.3 MPI_Group_translate_ranks.3 MPI_Group_size.3 MPI_Group_rank.3 MPI_Group_range_incl.3 MPI_Group_range_excl.3 MPI_Group_intersection.3 MPI_Group_incl.3 MPI_Group_free.3 MPI_Group_f2c.3 MPI_Group_excl.3 MPI_Group_difference.3 MPI_Group_compare.3 MPI_Group_c2f.3 MPI_Graph_neighbors_count.3 MPI_Graph_neighbors.3 MPI_Graph_map.3 MPI_Graph_get.3 MPI_Graphdims_get.3 MPI_Graph_create.3 MPI_Get_version.3 MPI_Get_processor_name.3 MPI_Get_elements.3 MPI_Get_count.3 MPI_Get_address.3 MPI_Get.3 MPI_Gatherv.3 MPI_Gather.3 
generated_man_other =XMPI_Coloron.3 XMPI_Coloroff.3 XMPI_Buoy.3 MPIO_Wait.3 MPIO_Test.3 MPIO_Request_f2c.3 MPIO_Request_c2f.3 MPIL_Type_id.3 MPIL_Trace_on.3 MPIL_Trace_off.3 MPIL_Signal.3 MPIL_Request_set_name.3 MPIL_Request_get_name.3 MPIL_Comm_id.3 MPIL_Comm_gps.3 

# Odd.  Automake 1.4 won't allow us to put $(trillium_man) directly in
# man_MANS, but it will let us put it in a macro that goes into
# man_MANS.

if WANT_TRILLIUM_INSTALL
install_trill_man	= $(trillium_man)
else
install_trill_man	=
endif

install_man		= $(generated_man_1) $(generated_man_0) $(generated_man_other) $(install_trill_man)
man_MANS		= $(install_man)

EXTRA_DIST		= $(trillium_man)

# Prevent some duplication

clean-local:
	$(RM) -f *~ FTTIME.3 ltot.3 $(case_sens_man)

msdir			= $(top_srcdir)/man/mans
m3dir			= $(top_srcdir)/man/man3

FTTIME.3: $(m3dir)/ttime.3
	cp -f $(m3dir)/ttime.3 FTTIME.3
LTOT.3: $(m3dir)/ttol.3
	cp -f $(m3dir)/ttol.3 LTOT.3
ltot.3: $(m3dir)/ttol.3
	cp -f $(m3dir)/ttol.3 ltot.3
TTOL.3: $(m3dir)/ttol.3
	cp -f $(m3dir)/ttol.3 TTOL.3
