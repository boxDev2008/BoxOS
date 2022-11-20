section .text
    global IDT_Load

IDT_Load:
    mov eax, [esp + 4]
    lidt [eax]
    ret
