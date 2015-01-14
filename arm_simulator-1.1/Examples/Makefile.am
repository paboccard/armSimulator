AM_CFLAGS=-g -mbig-endian
AM_CCASFLAGS=-mbig-endian
AM_LDFLAGS=-nostdlib -T $(srcdir)/linker_script -n --entry main
AM_LDFLAGS+=-Wl,-EB

if HAVE_ARM_COMPILER
<<<<<<< HEAD
noinst_PROGRAMS=example1 example2 example3 example4 example5 insertion_sort
=======
noinst_PROGRAMS=example1 example2 example3 example4 example5 example6 example7 insertion_sort
>>>>>>> 827ba9424eaafe30b03f509ae7d9617e8703ef7a

all-am: $(PROGS)
endif

example1_SOURCES=example1.s
example2_SOURCES=example2.s
example3_SOURCES=example3.s
example4_SOURCES=example4.s
<<<<<<< HEAD
exemple5_SOURCES=example5.s
=======
example5_SOURCES=example5.s
example6_SOURCES=example6.s
example7_SOURCES=example7.s
>>>>>>> 827ba9424eaafe30b03f509ae7d9617e8703ef7a
insertion_sort_SOURCES=insertion_sort.c

EXTRA_DIST=linker_script

clean-local:
	$(RM) $(PROGS)
