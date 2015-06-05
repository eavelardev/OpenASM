.MODEL Tiny

.DATA
   CR equ 13 		 
   LF equ 10
   Salto DB CR,LF,"$"
   fila db 0
   columna db 0
   mensaje1 DB "Universidad de Colima$"
   mensaje2 DB "Facultad de Ingenieria Mecanica y Electrica$"
   mensaje3 DB "Dame un Numero (menor de 10)=>$"
   mensaje4 DB "Dame Otro Numero (menor de 10)=>$"
   num1 db 0
   num2 db 0
   num3 db 0

.CODE
  org 100h 	
  inicio:

;limpiar pantalla
   mov ax,0600h
   mov bh,07h	;atributo
   mov cx,0000h
   mov dx,184fh
   int 10h

;Colocamos el cursor en 29,2
   mov fila,2
   mov columna,29      
   mov ah,15
   int 10h                    
   mov dh,fila
   mov dl,columna
   sub dh,01h
   sub dl,01h
   mov ah,02
   int 10h

;mostrar cadena en pantalla
   mov dx,OFFSET mensaje1
   mov ah,9  
   int 21h

;Colocamos el cursor en 18,3                                     
   mov fila,3	
   mov columna,18   
   mov ah,15
   int 10h                    
   mov dh,fila
   mov dl,columna
   sub dh,01h
   sub dl,01h
   mov ah,02
   int 10h

;mostrar cadena en pantalla
   mov dx,OFFSET mensaje2
   mov ah,9
   int 21h

;salto de linea
   mov dx,OFFSET Salto
   mov ah,9                ;mostrar cadena en pantalla
   int 21h

;salto de linea
   mov dx,OFFSET Salto 
   mov ah,9                ;mostrar cadena en pantalla
   int 21h

;mostrar cadena en pantalla
   mov dx,OFFSET mensaje3
   mov ah,9
   int 21h

;recibimos un caracter con eco
     mov ah,1                ;lee un caracter del teclado
     int 21h		     ;lo almacena en AL
     mov num1,al

;salto de linea
     mov dx,OFFSET Salto
     mov ah,9                ;mostrar cadena en pantalla
     int 21h

;mostrar cadena en pantalla
     mov dx,OFFSET mensaje4       ;pide otro numero
     mov ah,9
     int 21h

;recibimos un caracter con eco
     mov ah,1                ;lee un caracter del teclado
     int 21h		     ;lo almacena en AL
     mov num2,al

     int 20h
end inicio
