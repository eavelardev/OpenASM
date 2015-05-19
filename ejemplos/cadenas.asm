.model tiny
.data
	CR equ 13
	LF equ 10

	texto1 db "Practica de archivos .COM $"
	texto2 db CR,LF,"Son parecidos a los archivos .EXE$"
	texto3 db CR,LF,"la diferencia es que Datos y Codigo$"
	texto4 db CR,LF,"se encuentran en el mismo segmento$"

.code
	org 100h

	inicio:
		mov dx,offset texto1
		mov ah,9
		int 21h

		mov dx,offset texto2
		mov ah,9
		int 21h

		mov dx,offset texto3
		mov ah,9
		int 21h

		mov dx,offset texto4
		mov ah,9
		int 21h

		int 20h

END inicio