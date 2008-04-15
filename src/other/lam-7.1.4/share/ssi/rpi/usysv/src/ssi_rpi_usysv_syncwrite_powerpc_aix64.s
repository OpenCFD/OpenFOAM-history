	.file	"/tmp/lam_atomic_84054.c"
	.toc
	.csect .text[PR]
	.align 2
	.globl lam_ssi_rpi_usysv_syncwrite
	.globl .lam_ssi_rpi_usysv_syncwrite
	.csect lam_ssi_rpi_usysv_syncwrite[DS]
lam_ssi_rpi_usysv_syncwrite:
	.llong .lam_ssi_rpi_usysv_syncwrite, TOC[tc0], 0
	.csect .text[PR]
.lam_ssi_rpi_usysv_syncwrite:
	.extern __mulh
	.extern __mull
	.extern __divss
	.extern __divus
	.extern __quoss
	.extern __quous
	sync

	blr
LT..lam_ssi_rpi_usysv_syncwrite:
	.llong 0
	.byte 0,0,32,96,128,1,0,1
	.llong LT..lam_ssi_rpi_usysv_syncwrite-.lam_ssi_rpi_usysv_syncwrite
	.short 27
	.byte "lam_ssi_rpi_usysv_syncwrite"
	.byte 31
	.align 2
_section_.text:
	.csect .data[RW],3
	.llong _section_.text
