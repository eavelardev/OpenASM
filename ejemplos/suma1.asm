.model tiny
.data
	texto1 db "La suma de $"
	texto2 db " y $"
	texto3 db 13,10,"es => $"

	a db 5
	b db 3
	c db 0
.code
	org 100h

	inicio:

		mov dx,offset texto1
		mov ah,9
		int 21h

		mov dl,a
		add dl,30h
		mov ah,02
		int 21h	

		mov dx,offset texto2
		mov ah,9
		int 21h

		mov dl,b
		add dl,30h
		mov ah,02
		int 21h	

		mov dx,offset texto3
		mov ah,9
		int 21h

		mov ah,a
		mov bh,b
		add ah,bh
		add ah,30h
		mov c,ah
		mov dl,c
		mov ah,02
		int 21h
		int 20h
END inicio