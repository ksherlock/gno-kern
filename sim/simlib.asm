	setcom 60
	mcopy	simlib.mac
	copy	simequates.equ

; ends up in .root
dummysim1 start
	end

* High-level access library for SIM tool

InstallIntVect	START
	using	SIMLibData

result	equ	0

	subroutine (4:address,2:port),2

	lda	port
	sta   SIMport
	lda	address
	sta	SIMaddress
	lda	address+2
	sta	SIMaddress+2
	lda	#SIMInstallHand
	jsr	DoSIMCall
	lda	|Gerror
	sta	result

	return 2:result
	END

RemoveIntVect	START
	using	SIMLibData

result	equ	0

	subroutine (4:address,2:port),2

	lda	port
	sta   SIMport
	lda	address
	sta	SIMaddress
	lda	address+2
	sta	SIMaddress+2
	lda	#SIMRemoveHand
	jsr	DoSIMCall
	lda	|Gerror
	sta	result

	return 2:result
	END

SIMVersion	START
	using	SIMLibData

result	equ	0
	subroutine (4:versionPtr),2

	lda	#SIMGetVersion
	jsr	DoSIMCall
	lda	|Gerror
	sta	result
       	bne	goodbye	if an error, don't copy version

	lda	Gversion
	sta	[versionPtr]

goodbye	return 2:result
	END

DoSIMCall	START
	using	SIMLibData

	pha		push request code
	pea	1	how to flag
	ph4	#SIMName
	ph4	#SIMInData
	ph4	#pBlock
	_SendRequest

	bcc	noToolErr
	cmp	#$0120	; nobody accepted the request
	beq	noAccept
	rts

noAccept	lda	#SIMNotFound
	sta	|Gerror
noToolErr	rts
	END

SIMLibData	DATA
SIMName	str	'SerialIntrMgr~Entry~'
pBlock	dc	i2'0'	; count field
Gerror	dc	i2'0'	; error code
Gversion	dc	i2'0'	; version number
SIMInData	ENTRY
SIMport	dc	i2'0'
SIMaddress	dc	i4'0'
	END
