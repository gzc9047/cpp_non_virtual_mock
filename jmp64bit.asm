bits 64

;; movd [rsp+4], 0x12345678
push dword 0x90abcdef
mov dword [rsp+4], 0x12345678
ret

push qword 0x654321

