.model tiny
.data
	texto db "La suma es => $"
	a db 5
	b db 3
.code
	org 100h

	inicio:
		mov dx,offset texto
		mov ah,9
		int 21h

		mov ah,a
		mov bh,b
		add ah,bh
		add ah,30h
		mov dl,ah
		mov ah,02
		int 21h
		int 20h
END inicio