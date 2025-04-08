section .data
  result db 0
  
section .text
  global _start
  
_start:
  mov eax, 5
  mov ebx, 7
  
  add eax,ebx
  add eax, '0'
  
  mov [result],eax
  
  mov eax, 4
  mov ebx, 1
  mov ecx, result
  mov edx, 1
  int 0x80
  
  mov eax, 1
  mov ebx, 0
  int 0x80