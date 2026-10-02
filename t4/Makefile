# Para revisar las opciones de compilacion y ejecucion,
# ingrese en el terminal el comando: make
#
#

PROB=definir

SRCS= $(PROB).c pss.c
HDRS= pss.h

SHELL=bash -o pipefail

INCLUDE=
CFLAGS=-Wall -Werror -pedantic -std=c18 -Wno-unused-function $(INCLUDE)
LDLIBS=-lm
ARCH=$(shell arch)

MAK=make --no-print-directory

readme:
	@less README.txt

revisar:
	$(MAK) PROB=revisar revisar.bin
	@echo
	./revisar.bin dicc.ht
	@echo
	@echo "Estudie este programa ejecutandolo bajo ddd con este comando:"
	@echo "make revisar.ddd"

revisar.ddd:
	$(MAK) PROB=revisar revisar.bin-g
	ddd revisar.bin-g &
	@echo "Coloque un breakpoint con: b main"
	@echo "Ejecute el programa con: r dicc.ht"
	@echo "Ejecute paso a paso con el boton next"

$(PROB).bin $(PROB).bin-g $(PROB).bin-san: $(SRCS) $(HDRS)

run-san: $(PROB).bin-san
	@if grep -P '\	' $(PROB).c ; then echo "Su archivo $(PROB).c contiene tabs" ; echo "Reemplacelos por espacios en blanco con el comando expand!" ; echo "No olvide revisar su indentacion"; false ; else true; fi
	bash test-$(PROB).sh $(PROB).bin-san

run-g: $(PROB).bin-g
	bash test-$(PROB).sh $(PROB).bin-g

prof.ref: $(PROB).bin
	cp $(PROB).bin prof.ref-$(ARCH)
	strip prof.ref-$(ARCH)

run: $(PROB).bin
	$(MAK) PROB=bench bench.bin
	@echo "*** Tests unitarios ******************************************"
	bash test-$(PROB).sh $(PROB).bin
	@echo "*** Benchmark ************************************************"
	./bench.bin ./prof.ref-$(ARCH) 1000000 300

ddd: $(PROB).ddd

ddd-san: $(PROB).ddd-san

zip:
	@if grep -P '\	' $(PROB).c ; then echo "Su archivo $(PROB).c contiene tabs.  Reemplacelos por espacios en blanco!" ; false ; else true; fi
	@rm -f resultados.txt $(PROB).zip
	@echo "Sistema operativo utilizado" > resultados.txt
	@lsb_release -d >> resultados.txt
	@echo "Versión del núcleo de linux:" >> resultados.txt
	@uname -a >> resultados.txt
	@cat resultados.txt
	@echo ==== run-san ==== | tee -a resultados.txt
	@$(MAK) -B run-san | tee -a resultados.txt
	@echo ==== run-g ==== | tee -a resultados.txt
	@$(MAK) -B run-g | tee -a resultados.txt
	@echo ==== run ==== | tee -a resultados.txt
	@$(MAK) -B run | tee -a resultados.txt
	@echo ==== zip ====
	zip -r $(PROB).zip resultados.txt $(PROB).c
	@echo "Entregue por u-cursos el archivo $(PROB).zip"
	@echo "Descargue de u-cursos lo que entrego, descomprimalo y revise"
	@echo "que esta su ultima version de $(PROB).c"

%.bin: %.c
	gcc -O -DOPT=1 $(CFLAGS) $(SRCS) $(LDLIBS) -o $@

%.bin-g: %.c
	gcc -g $(CFLAGS) $(SRCS) $(LDLIBS) -o $@

%.bin-san: %.c
	gcc -g -DSAN=1 -fsanitize=address -fsanitize=undefined $(CFLAGS) $(SRCS) $(LDLIBS) -o $@

%.ddd: %.bin-g
	ddd $(*F).bin-g &

%.ddd-san: %.bin-san
	ddd $(*F).bin-san &

clean:
	rm -rf *.o *.log *.bin* core $(PROB).zip resultados.txt tmp-test
