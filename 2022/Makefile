.PHONY: build run clean

N := 5

build:
	for i in $$(seq -f "%02g" 1 ${N}); do \
		make -C $$i build; \
	done

run:
	for i in $$(seq -f "%02g" 1 ${N}); do \
		make -C $$i run; \
	done

clean:
	for i in $$(seq -f "%02g" 1 ${N}); do \
		make -C $$i clean; \
	done

