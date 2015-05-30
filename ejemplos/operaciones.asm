.model tiny 			; directiva de modelo de programación

.data
  CR equ 13 		 
  LF equ 10
 
  num1 db 3
  num2 db 2
  num3 db 0

  V1 DB 5
  V2 DB 3
  V3 DB 0

  text_sum DB CR,LF,"La Suma es =>$"		
  text_mult DB CR,LF,"La Multiplicacion es =>$"
  text_rest DB CR,LF,"La Resta es =>$"

  texto1 db "Practica de archivos .COM $"
  texto2 DB CR,LF,"Son parecidos a los archivos .EXE$" 
  texto3 DB CR,LF,"la diferencia es que Datos y Codigo$"
  texto4 DB CR,LF,"se encuentran en el mismo segmento$"
 
  msg1 db CR,LF,"La suma de $"
  msg2 db  " y $"
  msg3 db 10,13,"es => $"
  msg4 db 10,13,"La resta de $"

.code
  org 100h 			; direccion de inicio de archivos .COM

  inicio: 			; inicio de programa
    mov dx,OFFSET texto1 	; imprime mensaje en pantalla
    mov ah,9 			
    int 21h
 			
    mov dx,OFFSET texto2 	; imprime mensaje en pantalla
    mov ah,9 			
    int 21h 

    mov dx,OFFSET texto3 	; imprime mensaje en pantalla
    mov ah,9 			
    int 21h 

    mov dx,OFFSET texto4 	; imprime mensaje en pantalla
    mov ah,9 			
    int 21h
 
    mov dx,OFFSET text_sum 	; imprime mensaje en pantalla
    mov ah,9 			
    int 21h
    
    mov ah,5
    mov bh,3
    add ah,bh
    add ah,30h

    mov dl,ah               ;imprimer el caracter
    mov ah,02
    int 21h

    mov dx,OFFSET text_mult 	; imprime mensaje en pantalla
    mov ah,9 			
    int 21h
    
    mov al,num1
    mov bl,num2
    mul bl                    ;mult al=al*bl
    mov num3,al
    add num3,30h

    mov dl,num3               ;imprimer el caracter
    mov ah,02
    int 21h

    mov dx,offset msg1
    mov ah,09
    int 21h

    mov dl,V1
    add dl,30h
    mov ah,02
    int 21h

    mov dx,offset msg2
    mov ah,09
    int 21h

    mov dl,V2
    add dl,30h
    mov ah,02
    int 21h

    mov ah,v1
    mov bh,v2
    add ah,bh
    mov V3,ah
    add V3,30h

    mov dx,offset msg3
    mov ah,09
    int 21h

    mov dl,V3
    mov ah,02
    int 21h

    mov dx,offset msg4
    mov ah,09
    int 21h

    mov dl,V1
    add dl,30h
    mov ah, 02
    int 21h

    mov dx,offset msg2
    mov ah,09
    int 21h

    mov dl,V2
    add dl,30h
    mov ah,02
    int 21h

    mov dx,offset msg3
    mov ah,09
    int 21h

    mov ah,V1
    mov ch,v2
    sub ah,ch
    add ah,30h
    mov dl,ah
    mov ah,02
    int 21h
  
    int 20h

END inicio

