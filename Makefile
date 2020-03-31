# On Bridges we will check versus your performance versus Intel MKL library's BLAS. 

CC = icc
OPT = -Ofast
CFLAGS = -Wall -std=gnu99 $(OPT) -march=core-avx2 -restrict -ipo -unroll[=15] -fma -fp-model fast -qopt-malloc-options=3# -funroll-all-loops -unroll-aggressive 
#MKLROOT = /opt/intel/composer_xe_2013.1.110loops7/mkl
#LDLIBS = -lrt -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a $(MKLROOT)/lib/intel64/libmkl_sequential.a $(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm
LDLIBS = -lrt  -I$(MKLROOT)/include -Wl,-L$(MKLROOT)/lib/intel64/ -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm -ldl

targets = dgemm-project1 benchmark-naive benchmark-blocked benchmark-blas 
objects = benchmark.o dgemm-naive.o dgemm-blocked.o dgemm-blas.o dgemm-project1.o

.PHONY : default
default : all

.PHONY : all
all : clean $(targets)

benchmark-naive : benchmark.o dgemm-naive.o 
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blocked : benchmark.o dgemm-blocked.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blas : benchmark.o dgemm-blas.o
	$(CC) -o $@ $^ $(LDLIBS)
dgemm-project1 : benchmark.o dgemm-project1.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

.PHONY : clean
clean:
	rm -f $(targets) $(objects) *.stdout
