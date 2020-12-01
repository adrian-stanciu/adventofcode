.PHONY: build run clean

FIRST := 2015
LAST := 2020

build:
	for i in $$(seq ${FIRST} ${LAST}); do \
		make -C $$i build; \
	done

run:
	for i in $$(seq ${FIRST} ${LAST}); do \
		make -C $$i run; \
	done

clean:
	for i in $$(seq ${FIRST} ${LAST}); do \
		make -C $$i clean; \
	done

