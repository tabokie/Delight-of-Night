=====
brief
=====

O(e) version of image segmentation.

Standalone version with head file "img_seg.h" and C file "img_seg.c"

Alone with manual test program "img_seg_test.c"

=================
build instruction
=================

Please link img_seg.c with img_seg_test.c using C compiler to rebuild executable file for manual testing.

`gcc img_seg.c img_seg_test.c -o new_build`

=NOTICE=: gcc compiler preferred.
	  Originally built with gcc 4.9.2.

=NOTICE=:If your system doesn't support timing routine in "img_seg_test.c" 
	and comes to compiling error due to this,
	Please refer to system doc and modify the timing MACRO in "img_seg_test.c"