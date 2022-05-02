	.import	printi
	.import	printc
	.import	readi
	.import	readc
	.import	exit
	.import	time
	.import	clearAll
	.import	setPixel
	.import	drawLine
	.import	drawCircle
	.import	_indexError

	.code
	.align	4

	.export	main
main:
	sub	$29,$29,12		; allocate frame
	stw	$25,$29,8		; save old frame pointer
	add	$25,$29,12		; setup new frame pointer
	stw	$31,$25,-8		; save return register
	add	$8,$0,4
	stw	$8,$29,0		; store argument #0
	jal	printi
	ldw	$31,$25,-8		; restore return register
	ldw	$25,$29,8		; restore old frame pointer
	add	$29,$29,12		; release frame
	jr	$31			; return
