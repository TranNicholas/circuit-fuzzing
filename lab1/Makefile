.PHONY: all
all: fuzz

build_Vtop_fuzz: ../lock.sv fuzz.cpp
	verilator $^ --Mdir $@ --cc --exe --O3 --no-timing --prefix Vtop

Vtop_afl: build_Vtop_fuzz
	$(MAKE) -C $< -f Vtop.mk \
		CC=afl-clang-lto++ \
		CXX=afl-clang-lto++ \
		Vtop__ALL.a fuzz.o

	$(MAKE) -C $< -f Vtop.mk \
		LINK=afl-clang-lto++ \
		Vtop

	cp -a $</Vtop $@

.PHONY: fuzz
fuzz: Vtop_afl
	afl-fuzz -i ../seeds -o findings -- ./$<

.PHONY: clean
clean:
	$(RM) -r build_Vtop_fuzz Vtop_afl
