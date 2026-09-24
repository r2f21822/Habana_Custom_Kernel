	.text
	.file	"batch_norm_fwd_f32.c"
	.globl	main                            // -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   // @main
// %bb.0:                               // %entry
{
	mov_irf_dim  0x0 %S5, %I0
	ld_l mmio %S0, 0x418
}
{
	add.i32  b11111 %I2, %I1, %I0
	ld_l mmio %S2, 0x420
}
	ld_l mmio %S3, 0x428
	mul.f32  %S4, %S1, 0x0
{
	mov_irf_dim  0x0 %S6, %I2
	set_indx  %I2, b11111, 0x0
}
{
	shl.i32  %S7, %S5, 0x6
	mov  b11111 %I3, %I2
}
	mov.f32  %V0, %S1
	mov.f32  %V1, %S4
	add.i32  %S1, %S0, -0x1
{
	shl.i32  %S6, %S6, 0x6
	set_indx  %I4, b00001, %S7
}
	add.i32  %S4, %S0, -0x2
{
	cmp_grt.i32  %SP1, %S3, 0x0
	set_indx  %I4, b11110, 0x0
}
	add.i32  %S5, %S0, -0x3
	loop %S7, %S6, 64, <, .LBB0_21
	nop
.LBB0_1:                                // %for.body
                                        // =>This Loop Header: Depth=1
                                        //     Child Loop BB0_3 Depth 2
                                        //       Child Loop BB0_4 Depth 3
                                        //         Child Loop BB0_5 Depth 4
                                        //     Child Loop BB0_9 Depth 2
                                        //       Child Loop BB0_10 Depth 3
                                        //         Child Loop BB0_11 Depth 4
                                        //     Child Loop BB0_15 Depth 2
                                        //       Child Loop BB0_16 Depth 3
                                        //         Child Loop BB0_17 Depth 4
{
	mov.f32  %V4, 0x0
	mov.f32  %V6, 0x0
	set_indx  %I2, b00001, %S32
	set_indx  %I3, b00001, %S32
}
{
	mov.f32  %V3, 0x0
	mov.f32  %V5, 0x0
}
	mov  %V2, %V1
	jmpr .LBB0_14, !%SP1
// %bb.2:                               // %for.body13.preheader
                                        //   in Loop: Header=BB0_1 Depth=1
	nop
{
	mov.f32  %V2, 0x0
	mov.f32  %V3, 0x0
}
{
	mov.f32  %V4, 0x0
	mov.f32  %V5, 0x0
}
	nop
	nop
	loop 0, %S3, 1, <, .LBB0_8
	nop
.LBB0_3:                                // %for.body13
                                        //   Parent Loop BB0_1 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB0_4 Depth 3
                                        //         Child Loop BB0_5 Depth 4
{
	set_indx  %I2, b01000, %S33
	set_indx  %I3, b01000, %S33
}
	loop 0, %S2, 1, <, .LBB0_7
	nop
.LBB0_4:                                // %for.body19
                                        //   Parent Loop BB0_1 Depth=1
                                        //     Parent Loop BB0_3 Depth=2
                                        // =>    This Loop Header: Depth=3
                                        //         Child Loop BB0_5 Depth 4
	loop 0, %S0, 4, <, .LBB0_6
{
	set_indx  %I2, b00010, 0x0
	set_indx  %I2, b00100, %S34
}
.LBB0_5:                                // %for.body27
                                        //   Parent Loop BB0_1 Depth=1
                                        //     Parent Loop BB0_3 Depth=2
                                        //       Parent Loop BB0_4 Depth=3
                                        // =>      This Inner Loop Header: Depth=4
{
	ld_tnsr  %V6, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
{
	ld_tnsr  %V7, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
{
	ld_tnsr  %V8, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
{
	ld_tnsr  %V9, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
	add.f32  %V5, %V5, %V6
	add.f32  %V4, %V4, %V7
	add.f32  %V3, %V3, %V8
	add.f32  %V2, %V2, %V9
.LBB0_6:                                // %for.cond.cleanup26
                                        //   in Loop: Header=BB0_4 Depth=3
	nop
	nop
	nop
{
	set_indx  %I3, b00010, 0x0
	set_indx  %I3, b00100, %S34
}
.LBB0_7:                                // %for.cond.cleanup18
                                        //   in Loop: Header=BB0_3 Depth=2
	nop
	nop
	nop
	nop
.LBB0_8:                                // %for.cond.cleanup12
                                        //   in Loop: Header=BB0_1 Depth=1
{
	add.f32  %V7, %V5, %V4
	mov.f32  %V5, 0x0
}
{
	add.f32  %V2, %V3, %V2
	mov.f32  %V6, 0x0
}
{
	mov.f32  %V3, 0x0
	mov.f32  %V4, 0x0
}
	nop
	nop
	add.f32  %V2, %V7, %V2
	nop
	nop
	nop
	mul.f32   %V2, %V0, %V2
	loop 0, %S3, 1, <, .LBB0_14
	nop
.LBB0_9:                                // %for.body59
                                        //   Parent Loop BB0_1 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB0_10 Depth 3
                                        //         Child Loop BB0_11 Depth 4
{
	set_indx  %I2, b01000, %S33
	set_indx  %I3, b01000, %S33
}
	loop 0, %S2, 1, <, .LBB0_13
	nop
.LBB0_10:                               // %for.body66
                                        //   Parent Loop BB0_1 Depth=1
                                        //     Parent Loop BB0_9 Depth=2
                                        // =>    This Loop Header: Depth=3
                                        //         Child Loop BB0_11 Depth 4
	loop 0, %S0, 4, <, .LBB0_12
{
	set_indx  %I2, b00010, 0x0
	set_indx  %I2, b00100, %S34
}
.LBB0_11:                               // %for.body75
                                        //   Parent Loop BB0_1 Depth=1
                                        //     Parent Loop BB0_9 Depth=2
                                        //       Parent Loop BB0_10 Depth=3
                                        // =>      This Inner Loop Header: Depth=4
{
	ld_tnsr  %V7, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
{
	cmp_less.i32  %SP2, %S35, %S1
	ld_tnsr  %V8, 0x0, %I2
}
	add.i32  b00010 %I2, 0x1, %I2
{
	cmp_less.i32  %SP3, %S35, %S4
	ld_tnsr  %V9, 0x0, %I2
}
{
	add.i32  b00010 %I2, 0x1, %I2
	sub.f32  %V7, %V7, %V2
}
{
	cmp_less.i32  %SP4, %S35, %S5
	ld_tnsr  %V10, 0x0, %I2
	sub.f32  %V8, %V8, %V2
}
	add.i32  b00010 %I2, 0x1, %I2
	sub.f32  %V9, %V9, %V2
	mac.f32   %V4, %V7, %V7
	sub.f32  %V7, %V10, %V2
	mac.f32   %V3, %V8, %V8, %SP2
	mac.f32   %V6, %V9, %V9, %SP3
	nop
	mac.f32   %V5, %V7, %V7, %SP4
.LBB0_12:                               // %for.cond.cleanup74
                                        //   in Loop: Header=BB0_10 Depth=3
	nop
	nop
	nop
{
	set_indx  %I3, b00010, 0x0
	set_indx  %I3, b00100, %S34
}
.LBB0_13:                               // %for.cond.cleanup65
                                        //   in Loop: Header=BB0_9 Depth=2
	nop
	nop
	nop
	nop
.LBB0_14:                               // %for.cond.cleanup58
                                        //   in Loop: Header=BB0_1 Depth=1
	nop
{
	add.f32  %V3, %V4, %V3
	mov.i32  %V4, 0x0
}
{
	add.f32  %V5, %V6, %V5
	mov.f32  %V6, 0x7f800000
}
	mov.f32  %V7, 0x0
	mov.f32  %V8, 0x7fffffff
	mov.f32  %V9, -0x800000
{
	ld_tnsr  %V10, 0x2, %I4
	add.f32  %V5, %V3, %V5
}
	ld_tnsr  %V3, 0x1, %I4
	nop
	nop
	mul.f32   %V5, %V0, %V5
	nop
	nop
	nop
	nop
	nop
	add.f32  %V11, %V5, 0x3727c5ac
	nop
	nop
	nop
	extract_exp.f32  %V12, %V11
	nop
	nop
	nop
	and.i32  %V5, %V12, 0x1
	nop
	nop
	nop
	form_fp_num.f32 exp_add_bias exp_is_num %V13, %V5, %V11, %V11
	sel_less.i32    %V14, %V12, 0x0, %V5, %V4
	nop
	nop
	get_lut_entry_and_interval_start.f32 sqrt_rsqrt %D4, %V13, 0x10
	sub.i32  %V12, %V12, %V14
	nop
	nop
{
	sub.f32  %V13, %V13, %V5
	lookup_1c   BV32 %V14, %V4, 0x1
}
	nop
	lookup_2c   BV32 %D4, %V4, 0x1
	shl.i32  %V12, %V12, 0x16
	nop
	nop
	nop
	mac.f32   %V4, %V5, %V13
	and.i32  %V5, %V12, -0x800000
	nop
	nop
	nop
	nop
	mac.f32   %V14, %V4, %V13
	nop
	nop
	nop
	nop
	nop
	sub.i32  %V4, %V14, %V5
	nop
	nop
	nop
	sel_eq.f32      %V4, %V11, 0x0, %V6, %V4
	nop
	nop
	nop
	sel_eq.u32      %V4, %V11, 0x7f800000, %V7, %V4
	nop
	nop
	nop
	sel_grt.u32     %V4, %V11, 0x7f800000, %V8, %V4
	nop
	nop
	nop
	sel_eq.f32      %V4, %V11, -0x80000000, %V9, %V4
	st_tnsr  0x5, %I4, %V4
	st_tnsr  0x4, %I4, %V2
	nop
	mul.f32   %V4, %V10, %V4
	nop
	nop
	nop
	nop
	nop
	mac.f32  neg %V3, %V4, %V2
	loop 0, %S3, 1, <, .LBB0_20
	nop
.LBB0_15:                               // %for.body120
                                        //   Parent Loop BB0_1 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB0_16 Depth 3
                                        //         Child Loop BB0_17 Depth 4
{
	set_indx  %I2, b01000, %S33
	set_indx  %I3, b01000, %S33
}
	loop 0, %S2, 1, <, .LBB0_19
	nop
.LBB0_16:                               // %for.body127
                                        //   Parent Loop BB0_1 Depth=1
                                        //     Parent Loop BB0_15 Depth=2
                                        // =>    This Loop Header: Depth=3
                                        //         Child Loop BB0_17 Depth 4
{
	set_indx  %I2, b00010, 0x0
	set_indx  %I3, b00010, 0x0
	set_indx  %I2, b00100, %S34
}
	loop 0, %S0, 4, <, .LBB0_18
	set_indx  %I3, b00100, %S34
.LBB0_17:                               // %for.body136
                                        //   Parent Loop BB0_1 Depth=1
                                        //     Parent Loop BB0_15 Depth=2
                                        //       Parent Loop BB0_16 Depth=3
                                        // =>      This Inner Loop Header: Depth=4
	nop
	nop
	nop
	nop
{
	ld_tnsr  %V2, 0x0, %I2
	mov  %V5, %V3
	add.i32  b00010 %I2, 0x1, %I2
}
{
	mov  %V7, %V3
	ld_tnsr  %V6, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
{
	mov  %V9, %V3
	ld_tnsr  %V8, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
{
	mov  %V11, %V3
	ld_tnsr  %V10, 0x0, %I2
	add.i32  b00010 %I2, 0x1, %I2
}
	mac.f32   %V5, %V2, %V4
{
	st_tnsr  0x3, %I3, %V5
	mac.f32   %V7, %V6, %V4
	add.i32  b00010 %I3, 0x1, %I3
}
{
	st_tnsr  0x3, %I3, %V7
	mac.f32   %V9, %V8, %V4
	add.i32  b00010 %I3, 0x1, %I3
}
{
	st_tnsr  0x3, %I3, %V9
	mac.f32   %V11, %V10, %V4
	add.i32  b00010 %I3, 0x1, %I3
}
{
	st_tnsr  0x3, %I3, %V11
	add.i32  b00010 %I3, 0x1, %I3
}
.LBB0_18:                               // %for.cond.cleanup135
                                        //   in Loop: Header=BB0_16 Depth=3
	nop
	nop
	nop
	nop
.LBB0_19:                               // %for.cond.cleanup126
                                        //   in Loop: Header=BB0_15 Depth=2
	nop
	nop
	nop
	nop
.LBB0_20:                               // %for.cond.cleanup119
                                        //   in Loop: Header=BB0_1 Depth=1
	nop
	nop
	nop
	add.i32  b00001 %I4, 0x40, %I4
.LBB0_21:                               // %for.cond.cleanup
	nop
	nop
{
	halt
	halt
}
	nop
	nop
	nop
$func_end0:
	.size	main, ($func_end0)-main
                                        // -- End function
	.type	tpc_compiler,@object            // @tpc_compiler
	.section	.tpc_compiler,"a",@progbits
	.p2align	4
tpc_compiler:
	.ascii	"\"-cc1\" \"-triple\" \"tpc\" \"-S\" \"-disable-free\" \"-clear-ast-before-backend\" \"-disable-llvm-verifier\" \"-main-file-name\" \"batch_norm_fwd_f32.c\" \"-mrelocation-model\" \"static\" \"-mframe-pointer=all\" \"-fmath-errno\" \"-ffp-contract=on\" \"-fno-rounding-math\" \"-mconstructor-aliases\" \"-Wfloat-conversion\" \"-Wmissing-braces\" \"-Wuninitialized\" \"-Werror=implicit-function-declaration\" \"-fembed-bitcode=bitcode\" \"-bfloat16\" \"-slm\" \"1\" \"-mllvm\" \"-simplifycfg-sink-common=false\" \"-mllvm\" \"-enable-load-pre=false\" \"-max-tensors\" \"16\" \"-tpc-special\" \"-mlink-builtin-bitcode\" \"/usr/lib/clang/14.0.5/lib/gaudi.bc\" \"-mllvm\" \"-loop-unswitch-threshold=0\" \"-mllvm\" \"-dontAnalysis=1\" \"-target-cpu\" \"gaudi\" \"-instr-compress\" \"-mllvm\" \"-treat-scalable-fixed-error-as-warning\" \"-debugger-tuning=gdb\" \"-fcoverage-compilation-dir=/home/rafa/Habana_Custom_Kernel\" \"-resource-dir\" \"/usr/lib/clang/14.0.5\" \"-std=rc99\" \"-O2\" \"-fdebug-compilation-dir=/home/rafa/Habana_Custom_Kernel\" \"-ferror-limit\" \"19\" \"-fgnuc-version=4.2.1\" \"-fcolor-diagnostics\" \"-faddrsig\" \"-o\" \"batch_norm_fwd_f32.s\" \"-x\" \"c\" \"kernels/gaudi/batch_norm_fwd_f32.c\""
	.size	tpc_compiler, 1095

	.type	tpc_metadata,@object            // @tpc_metadata
	.section	.tpc_metadata,"aw",@progbits
	.globl	tpc_metadata
	.p2align	4
tpc_metadata:
	.asciz	"\024\000\000\000\001\000\000\001\002\000\000\000\003\017\001\000\000\000m\275HJ\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\001\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
	.size	tpc_metadata, 262

	.section	".linker-options","e",@llvm_linker_options
	.ident	"clang version 14.0.5 (ssh://gerrit.habana-labs.com:29418/tpc_llvm10 f302857618a91f72dea8024241d82a232f9ea1fb)"
	.ident	"clang version 14.0.5 (ssh://gerrit.habana-labs.com:29418/tpc_llvm10 f302857618a91f72dea8024241d82a232f9ea1fb)"
	.ident	"clang version 14.0.5 (ssh://gerrit.habana-labs.com:29418/tpc_llvm10 f302857618a91f72dea8024241d82a232f9ea1fb)"
	.ident	"clang version 14.0.5 (ssh://gerrit.habana-labs.com:29418/tpc_llvm10 f302857618a91f72dea8024241d82a232f9ea1fb)"
	.ident	"clang version 14.0.5 (ssh://gerrit.habana-labs.com:29418/tpc_llvm10 f302857618a91f72dea8024241d82a232f9ea1fb)"
	.ident	"clang version 14.0.5 (ssh://gerrit.habana-labs.com:29418/tpc_llvm10 f302857618a91f72dea8024241d82a232f9ea1fb)"
	.ident	"clang version 14.0.5 (ssh://gerrit.habana-labs.com:29418/tpc_llvm10 f302857618a91f72dea8024241d82a232f9ea1fb)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym tpc_compiler
