section .text

extern isr_handler
extern irq_handler
global idt_int_table

%macro isr_no_err_stub 1
global int_stub_%1
int_stub_%1:
    push 0
    push dword %1
    pushad
    call isr_handler
    popad
    add esp, 8
    iret
%endmacro

%macro isr_err_stub 1
global int_stub_%1
int_stub_%1:
    push dword %1
    pushad
    call isr_handler
    popad
    add esp, 8
    iret
%endmacro

%macro irq_stub 1
global int_stub_%1
int_stub_%1:
    push 0
    push dword %1
    pushad
    call irq_handler
    popad
    add esp, 8
    iret
%endmacro

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

%assign i 32
%rep 224
    irq_stub i
    %assign i i+1
%endrep

section .data

idt_int_table:
    %assign i 0
    %rep 256
        dd int_stub_ %+ i
        %assign i i+1
    %endrep