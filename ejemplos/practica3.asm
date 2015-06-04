.MODEL Tiny

.DATA
   CR equ 13 		 
   LF equ 10

   Salto DB CR,LF,"$"

   fila db 0
   columna db 0

   mensaje1 DB "Universidad de Colima$"
   mensaje2 DB "Facultad de Ingenieria Mecanica y Electrica$"
   mensaje3 DB "Compiladores y Traductores$"
   mensaje4 DB "Dame un Numero (menor de 10)=>$"
   mensaje5 DB "Dame Otro Numero (menor de 10)=>$"

   msg1 DB CR,LF,"La suma de $"
   msg2 DB  " y $"
   msg3 DB " es => $"

   msg4 DB 10,13,"La resta de $"
   msg5 DB 10,13,"La Multiplicacion de $"
   msg6 DB 10,13,"La Divicion de $"
   msg7 DB 10,13," y el Residuo o Resto es => $"

   num1 db 0
   num2 db 0
   num3 db 0
   resto db 0

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
   mov ah,15          ;Obtener en bh pagina de video 
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
   mov ah,15          ;Obtener en bh pagina de video 
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

;Colocamos el cursor en 27,4                                     
   mov fila,4	
   mov columna,27   
   mov ah,15          ;Obtener en bh pagina de video 
   int 10h               
   mov dh,fila
   mov dl,columna
   sub dh,01h
   sub dl,01h
   mov ah,02
   int 10h

;mostrar cadena en pantalla
   mov dx,OFFSET mensaje3
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
;-----------------------------------------------------------------
;mostrar cadena en pantalla    ;pide primer numero
   mov dx,OFFSET mensaje4
   mov ah,9
   int 21h

;recibimos un caracter con eco
     mov ah,1                ;lee un caracter del teclado
     int 21h		     ;lo almacena en AL
     mov num1,al

;salto de linea
     mov dx,OFFSET Salto
     mov ah,9
     int 21h

;mostrar cadena en pantalla
     mov dx,OFFSET mensaje5       ;pide otro numero
     mov ah,9
     int 21h

;recibimos un caracter con eco
     mov ah,1                ;lee un caracter del teclado
     int 21h		     ;lo almacena en AL
     mov num2,al

;salto de linea
     mov dx,OFFSET Salto
     mov ah,9
     int 21h

;salto de linea
     mov dx,OFFSET Salto 
     mov ah,9
     int 21h
;----------------------operacion suma--------------------------
    mov dx,offset msg1           ;La suma de...
    mov ah,09
    int 21h

    mov dl,num1
    mov ah,02
    int 21h

    mov dx,offset msg2          ;y
    mov ah,09
    int 21h

    mov dl,num2
    mov ah,02
    int 21h

    mov dx,offset msg3          ;Es =>
    mov ah,09
    int 21h

    mov ah,num1
    mov bh,num2
    add ah,bh
    mov num3,ah
    sub num3,30h

    mov dl,num3
    mov ah,02
    int 21h

;----------------------operacion Resta--------------------------
    mov dx,offset msg4           ;La Resta de...
    mov ah,09
    int 21h

    mov dl,num1
    mov ah,02
    int 21h

    mov dx,offset msg2          ;y
    mov ah,09
    int 21h

    mov dl,num2
    mov ah,02
    int 21h

    mov dx,offset msg3          ;Es =>
    mov ah,09
    int 21h

    mov ah,num1
    mov bh,num2
    sub ah,bh
    mov num3,ah
    add num3,30h

    mov dl,num3
    mov ah,02
    int 21h

;----------------------operacion Multiplicacion--------------------------
    mov dx,offset msg5           ;La Multiplicacion de...
    mov ah,09
    int 21h

    mov dl,num1
    mov ah,02
    int 21h

    mov dx,offset msg2           ;y
    mov ah,09
    int 21h

    mov dl,num2
    mov ah,02
    int 21h

    mov dx,offset msg3           ;Es =>
    mov ah,09
    int 21h

    mov al,num1
    sub al,30h
    mov bl,num2
    sub bl,30h
    mul bl
    mov num3,al
    add num3,30h

    mov dl,num3
    mov ah,02
    int 21h
;----------------------operacion Divicion--------------------------
    mov dx,offset msg6           ;La Divicion de...
    mov ah,09
    int 21h

    mov dl,num1
    mov ah,02
    int 21h

    mov dx,offset msg2           ;y
    mov ah,09
    int 21h

    mov dl,num2
    mov ah,02
    int 21h

    mov dx,offset msg3           ;Es =>
    mov ah,09
    int 21h
    xor ax,ax        ;limpiamos el registro ax. 
    mov al,num1
    sub al,30h
    mov bl,num2
    sub bl,30h
    div bl
    mov num3,al
    mov resto,ah
    add num3,30h
    add resto,30h
    mov dl,num3
    mov ah,02
    int 21h

    mov dx,offset msg7           ;y el resto es...
    mov ah,09
    int 21h
    mov dl,resto
    mov ah,02
    int 21h

     int 20h
end inicio
