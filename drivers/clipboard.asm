************************************************************
*
* Clipboard device driver
*
* open - does nothing (defers until first read/write)
* read - Gets the scrap, reads from that
* write - calls ZeroScrap, then PutScrap to append after each write.
*
************************************************************

	case	on
	mcopy	clipboard.mac
	copy	../gno/inc/tty.inc
	copy	13:AInclude:E16.GSOS
	copy	13:AInclude:E16.Memory
	copy	13:AInclude:E16.Scrap

Header	START

	ds	t_open
* For speed/simplicity we use a non-standard, non-documented line
* discipline for the .null device
	dc	i4'Open'
	dc	i4'Close'
	dc	i4'IOCTL'
	dc	i4'Read'
	dc	i4'Write'
	dc	i4'NoFunc'
	dc	i4'NoFunc'
	dc	i4'NoFunc'
	dc	i4'NoFunc'
	dc	i4'NoFunc'
	dc	i4'NoFunc'
	dc	i4'NoFunc'
	dc	i4'NoFunc'
	ds	t_signalIO-editInd
	dc	i4'NoFunc'		; t_signalIO
	dc	i2'$FFFF'		; t_select_proc
	dc	i4'Select'		; t_select
	dc	i4'0'			; t_selwakeup jmp set during install



NoFunc	anop
	rtl
	END

data DATA

State ds 2 ; 0 = ready, 2 = reading, 4 = writing
Handle ds 4
Offset ds 4

	END

;
; init for reading/writing are basically the same - the difference is 
; in retrieving the scrap.
;

OpenForReading START

; get the current text scrap

	using data

	stz Offset
	stz Offset+2
	lda #0
	pha
	pha
	pha
	pha
	pha
	_MMStartUp
	pea attrNoSpec
	pea 0
	pea 0
	_NewHandle
	bcs nherr
	pl4 Handle

	~GetScrap Handle,#textScrap
	clc
	lda #0
	rts

nherr	anop
	pla
	pla
	lda #outOfMem
	sec
	rts


	END

Open	START

	subroutine (2:devNum)

	return #0
	END

Close	START

	using data

	subroutine (2:devNum),0



	ldx State
	stz State

	jmp (table,x)

table anop
	dc i2'closed'
	dc i2'read'
	dc i2'write'


read	anop
	lda Handle
	ora Handle+2
	beq closed

	~DisposeHandle Handle
	stz Handle
	stz Handle+2
	stz Offset
	stz Offset+2

write	anop
closed	anop
	return
	END

Write	START

	using data

count	equ	0
retval	equ	2
	subroutine (2:reqCount,4:dataPtr,2:devNum),4


	stz count
	stz retval

	ldx State
	jmp (table,x)

table	anop
	dc i2'closed,read,write'

read	anop
	lda #drvrPriorOpen ; ???
	brl exit

closed	anop
	_ZeroScrap

	lda #4
	sta State

write	anop
	lda reqCount
	beq exit

	pea 0
	pei reqCount
	pea textScrap
	pei dataPtr+2
	pei dataPtr
	_PutScrap		; this *appends*
	sta retval
	bcs exit

	lda reqCount
	sta count

exit	return 4:count
	END


Read	START

	using data

count	equ 0
retval	equ 2
ptr     equ 4
h	equ 8
size	equ 12

	subroutine (2:reqCount,4:dataPtr,2:devNum),16


	stz count
	stz retval

	ldx State
	jmp (table,x)
table	anop
	dc i2'closed,read,write'

write	anop
!error!
	lda #drvrPriorOpen
	brl exit

closed	anop

	
	jsr OpenForReading
	sta retval
	jcs exit
	lda #2
	sta State

read	anop

; handle:
; 0: pointer (long)
; 4: attr
; 6: mem ID
; 8: size (long)


	lda Handle
	sta h
	lda Handle+2
	sta h+2


	ldy #4
	lda [h],y
	ora #attrLocked
	sta [h],y

! get the handle size...
	ldy #8
	lda [h],y
	sta size
	ldy #10
	lda [h],y
	sta size+2

	clc
	lda [h]
	adc Offset
	sta ptr
	ldy #2
	lda [h],y
	adc Offset+2
	sta ptr+2


	sec
	lda size
	sbc Offset
	sta size
	lda size+2
	sbc Offset+2
	sta size+2
	bmi eof
	ora size
	bne ok

eof	lda #eofEncountered
	sta retval
	brl unlock

ok	anop
	lda size+2
	bne copy

! check for a partial copy

	lda size
	cmp reqCount
	bcs copy

	sta reqCount

! copy in 2 stages - words, then the final last byte.
copy	anop

	ldy #0
	lda reqCount
	lsr a
	tax
	beq loop1


loop2	anop
	lda [ptr],y
	sta [dataPtr],y
	iny
	iny
	dex
	bne loop2

loop1	anop
	bcc copydone
	short m
	lda [ptr],y
	sta [dataPtr],y
	long m

copydone anop


	lda Offset
	clc
	adc reqCount
	sta Offset
	lda Offset+2
	adc #0
	sta Offset+2

	lda reqCount
	sta count

unlock	anop
	ldy #4
	lda [h],y
	and #-attrLocked-1
	sta [h],y

exit	anop
	return 4:count
	END

IOCTL	START

	subroutine (4:tioc,4:dataPtr,2:devNum)

	return #0
	END

Select	START
	subroutine (2:ttyn,2:which,2:pid)
! ??? if open for reading and more data?
! if open for writing, always?
	return	#1
	END
