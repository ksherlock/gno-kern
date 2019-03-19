CC = occ
AS = occ

CPPFLAGS = -DKERNEL
CFLAGS = -w-1
ASFLAGS = 
LDFLAGS = -M

KERN_OBJS = main.a patch.a kern.a sys.a signal.a ctool.a sem.a \
	queue.a data.a diag.a resource.a tty.a select.a gsos.a \
	p16.a pipe.a shellcall.a fastfile.a texttool.a driver.a \
	util.a var.a ep.a err.a regexp.a stat.a ports.a \
	sleep.a pty.a net.a \
	inout.a console.a box.a conpatch.a

CHTYP = iix chtyp
MAKELIB = iix makelib

VPATH = gno:drivers

TARGETS = kern null console modem printer libsim sys/sim

.PHONY: all
all: $(TARGETS)

kern: $(addprefix o/,$(KERN_OBJS)) | o
	occ $(LDFLAGS) -o $@ $^
	$(CHTYP) -t s16 $@


null: o/null.a
	occ $(LDFLAGS) -o $@ $^
	$(CHTYP) -t 187 -a 32257 $@

console: o/console.a o/inout.a o/box.a o/conpatch.a
	occ $(LDFLAGS) -o $@ $^
	$(CHTYP) -t 187 -a 32257 $@


modem: o/port.a o/msccf.a libsim
	occ $(LDFLAGS) -o $@ o/port.a o/msccf.a -L. -lsim
	$(CHTYP) -t 187 -a 32257 $@

printer: o/port.a o/psccf.a libsim
	occ $(LDFLAGS) -o $@ o/port.a o/psccf.a -L. -lsim
	$(CHTYP) -t 187 -a 32257 $@

sys/sim: o/sim/sim.a | sys
	occ $(LDFLAGS) -o $@ $^
	$(CHTYP) -t 182 $@

libsim: o/sim/simlib.a
	$(RM) -f $@
	$(MAKELIB) $@ $(addprefix +,$^)


.PHONY: clean clobber

clean:
	$(RM) -rf -- o

clobber:
	$(RM) -rf -- o $(TARGETS)

.PHONY: fixtypes

fixtypes:
	$(CHTYP) -l asm gno/inc/*.inc
	$(CHTYP) -l asm drivers/equates
	$(CHTYP) -l asm sim/simequates.equ

o :
	mkdir $@

o/sim :
	mkdir -p $@

sys :
	mkdir $@

# main doesn't need the -r/noroot flag.
o/main.a: main.c| o
	cd $(dir $<) && $(CC) -c $(CPPFLAGS) $(CFLAGS) -o ../$@ $(notdir $<)

o/%.a : %.c | o
	cd $(dir $<) && $(CC) -c $(CPPFLAGS) $(CFLAGS) -r -o ../$@ $(notdir $<)

o/sim/%.a : sim/%.asm | o o/sim
	cd $(dir $<) && $(CC) -c $(ASFLAGS) -o ../$@ $(notdir $<)
	$(RM) -f o/sim/$*.root

o/%.a : %.asm | o o/sim
	cd $(dir $<) && $(CC) -c $(ASFLAGS) -o ../$@ $(notdir $<) 


# gno (doesn't include include/)
o/data.a: conf.h kernel.h proc.h q.h sem.h
o/diag.a: proc.h sys.h
o/ep.a: proc.h sys.h
o/fastfile.a: proc.h sys.h
o/main.a: proc.h gno.h sys.h sem.h tty.h
o/net.a: gno.h proc.h sys.h kernel.h net.h
o/patch.a: proc.h gno.h sys.h sem.h conf.h
o/ports.a: conf.h kernel.h proc.h gno.h
o/queue.a: q.h conf.h proc.h kernel.h sys.h
o/sem.a: sys.h sem.h proc.h q.h conf.h kernel.h gno.h
o/signal.a: conf.h kernel.h proc.h sys.h gno.h
o/sleep.a: proc.h sys.h kernel.h
o/stat.a: proc.h sys.h kernel.h
o/sys.a: conf.h proc.h kernel.h kvm.h gno.h sys.h
o/driver.a: m/driver.mac global.equates inc/tty.inc
o/err.a: m/err.mac
o/gsos.a: m/gsos.mac global.equates inc/gsos.inc inc/tty.inc
o/p16.a: m/p16.mac global.equates inc/gsos.inc inc/tty.inc
o/pipe.a: m/pipe.mac inc/kern.inc inc/gsos.inc
o/pty.a: inc/tty.inc
o/regexp.a: m/regexp.mac
o/resource.a: inc/gsos.inc
o/select.a: m/select.mac inc/tty.inc inc/gsos.inc inc/kern.inc global.equates
o/shellcall.a: m/shellcall.mac global.equates
o/texttool.a: m/texttool.mac global.equates
o/tty.a: m/tty.mac inc/tty.inc
o/util.a: m/util.mac
o/var.a: m/var.mac

# drivers
o/box.a: box.mac console.equates
o/conpatch.a: conpatch.mac
o/console.a: port.mac console.equates kern.equates inc/tty.inc
o/inout.a: inout.mac console.equates kern.equates inc/tty.inc
o/msccf.a: sccf.mac equates md.equates sccf.asm inc/tty.inc
o/null.a: port.mac inc/tty.inc
o/port.a: port.mac equates inc/tty.inc
o/psccf.a: sccf.mac equates pr.equates sccf.asm inc/tty.inc

# sim
o/sim/sim.a: sim/sim.mac sim/simequates.equ
o/sim/simlib.a: sim/simlib.mac sim/simequates.equ

