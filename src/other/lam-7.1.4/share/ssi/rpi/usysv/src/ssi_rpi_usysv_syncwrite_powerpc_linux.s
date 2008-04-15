	.section	".text"
	.align 2
	.globl lam_ssi_rpi_usysv_syncwrite

lam_ssi_rpi_usysv_syncwrite:
	stwu 1,-32(1)
	stw 31,28(1)
	mr 31,1
#APP
	sync

#NO_APP
	lwz 11,0(1)
	lwz 31,-4(11)
	mr 1,11
	blr
