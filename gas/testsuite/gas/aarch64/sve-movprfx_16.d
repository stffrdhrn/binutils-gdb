#source: sve-movprfx_16.s
#as: -march=armv8-a+sve -I$srcdir/$subdir
#objdump: -Dr -M notes

.* file format .*

Disassembly of section .*:

0+ <.*>:
[^:]+:	2598e3e0 	ptrue	p0.s
[^:]+:	04912461 	movprfx	z1.s, p1/m, z3.s
[^:]+:	6589a441 	fcvt	z1.s, p1/m, z2.h
[^:]+:	d65f03c0 	ret
