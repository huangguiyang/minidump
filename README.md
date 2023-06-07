# minidump
A dumping tool for Minix 3.1.0 object files.

## Compile & Run

The source code can be compiled both in Minix 3.1.0 and current 64 bit architectures.

```
$ gcc main.c -o minidump 
$ ./minidump -a path-of-Minix-Object-File
```

## Example output

example output for command `minidump -a kprintf.o`

```
Header:
Magic: 0x0201
Stamp: 0x00
Flags: 0x04
# Sections: 4
# Relocations: 25
# Names: 13
# Emits: 838
# Chars: 79

Section:
	  Name	    Base	    Size	File Off	File Len	Align
[   0]	 .text	       0	   0x2fa	    0x64	   0x2fa	2
[   1]	  .rom	       0	    0x28	   0x35e	    0x28	2
[   2]	 .data	       0	    0x24	   0x386	    0x24	2
[   3]	  .bss	       0	       0	   0x3aa	       0	2

Relocation:
	Ref Type	Rel Len	Rel Etc	Ref Sect	Sym Index	Sym Addr
[   0]	     0x2	      2	       	       3	        9	       0
[   1]	     0x2	      2	       	       3	        9	     0x6
[   2]	     0x2	      2	       	       3	        9	     0xa
[   3]	     0x2	      2	       	       3	        9	     0xe
[   4]	     0x2	      2	       	       3	        9	    0x12
[   5]	     0x2	      2	       	       3	        9	    0x16
[   6]	     0x2	      2	       	       3	        9	    0x1a
[   7]	     0x2	      2	       	       3	        9	    0x1e
[   8]	     0x2	      2	       	       3	        9	    0x22
[   9]	     0x2	      2	       	       3	        9	    0x26
[  10]	     0xa	      2	     PC	       2	        7	    0x27
[  11]	     0x2	      2	       	       2	       11	   0x121
[  12]	     0x2	      2	       	       2	       11	   0x1ae
[  13]	     0xa	      2	     PC	       2	        6	   0x1f7
[  14]	     0xa	      2	     PC	       2	        4	   0x20e
[  15]	     0xa	      2	     PC	       2	        7	   0x288
[  16]	     0xa	      2	     PC	       2	        7	   0x297
[  17]	     0xa	      2	     PC	       2	        7	   0x2b0
[  18]	     0xa	      2	     PC	       2	        7	   0x2be
[  19]	     0xa	      2	     PC	       2	        7	   0x2d5
[  20]	     0xa	      2	     PC	       2	        7	   0x2e0
[  21]	     0x2	      2	       	       2	       10	   0x2e9
[  22]	     0xa	      2	     PC	       2	        5	   0x2ec
[  23]	     0xa	      2	     PC	       2	        7	   0x2f2
[  24]	     0xa	      2	     PC	       2	        8	   0x2f8

Symbol:
	File Off	  Type	 Etc	Ext	Desc	Value	Name
[   0]	   0x50e	  USR0	    	  1	   0	    0	_printf
[   1]	   0x516	  USR2	    	  0	   0	    0	__II0
[   2]	   0x51c	  USR2	    	  0	   0	 0x12	__II1
[   3]	   0x522	  USR1	    	  0	   0	    0	__II2
[   4]	   0x528	   UND	    	  1	   0	    0	.dvu4
[   5]	   0x52e	   UND	    	  1	   0	    0	.csb2
[   6]	   0x534	   UND	    	  1	   0	    0	.rmu4
[   7]	   0x53a	   UND	    	  1	   0	    0	_putk
[   8]	   0x540	   UND	    	  1	   0	    0	.dsret
[   9]	   0x547	  USR0	 SCT	  0	   0	    0	.text
[  10]	   0x54d	  USR1	 SCT	  0	   0	    0	.rom
[  11]	   0x552	  USR2	 SCT	  0	   0	    0	.data
[  12]	   0x558	  USR3	 SCT	  0	   0	    0	.bss
```

## License

minidump is free software. See the file LICENSE for copying
permission.

## Author

guiyang.huangATgmail.com