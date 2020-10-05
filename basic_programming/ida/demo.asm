; ---------------------------------------------------------------------------

mach_header_64  struc ; (sizeof=0x20, align=0x4, copyof_1)
magic           dd ?
cputype         dd ?
cpusubtype      dd ?
filetype        dd ?
ncmds           dd ?
sizeofcmds      dd ?
flags           dd ?
reserved        dd ?
mach_header_64  ends

; ---------------------------------------------------------------------------

segment_command_64 struc ; (sizeof=0x48, align=0x8, mappedto_6)
                                        ; XREF: HEADER:0000000100000020/r
                                        ; HEADER:0000000100000068/r ...
cmd             dd ?
cmdsize         dd ?
segname         db 16 dup(?)            ; string(C)
vmaddr          dq ?
vmsize          dq ?
fileoff         dq ?
filesize        dq ?
maxprot         dd ?
initprot        dd ?
nsects          dd ?
flags           dd ?
segment_command_64 ends

; ---------------------------------------------------------------------------

section_64      struc ; (sizeof=0x50, align=0x8, mappedto_9)
                                        ; XREF: HEADER:00000001000000B0/r
                                        ; HEADER:0000000100000100/r ...
sectname        db 16 dup(?)            ; string(C)
segname         db 16 dup(?)            ; string(C)
addr            dq ?
size            dq ?
offset          dd ?
align           dd ?
reloff          dd ?
nreloc          dd ?
flags           dd ?
reserved1       dd ?
reserved2       dd ?
reserved3       dd ?
section_64      ends

; ---------------------------------------------------------------------------

dyld_info_command struc ; (sizeof=0x30, align=0x4, copyof_10)
                                        ; XREF: HEADER:0000000100000370/r
cmd             dd ?
cmdsize         dd ?
rebase_off      dd ?
rebase_size     dd ?
bind_off        dd ?
bind_size       dd ?
weak_bind_off   dd ?
weak_bind_size  dd ?
lazy_bind_off   dd ?
lazy_bind_size  dd ?
export_off      dd ?
export_size     dd ?
dyld_info_command ends

; ---------------------------------------------------------------------------

symtab_command  struc ; (sizeof=0x18, align=0x4, copyof_11)
                                        ; XREF: HEADER:00000001000003A0/r
cmd             dd ?
cmdsize         dd ?
symoff          dd ?
nsyms           dd ?
stroff          dd ?
strsize         dd ?
symtab_command  ends

; ---------------------------------------------------------------------------

dysymtab_command struc ; (sizeof=0x50, align=0x4, copyof_12)
                                        ; XREF: HEADER:00000001000003B8/r
cmd             dd ?
cmdsize         dd ?
ilocalsym       dd ?
nlocalsym       dd ?
iextdefsym      dd ?
nextdefsym      dd ?
iundefsym       dd ?
nundefsym       dd ?
tocoff          dd ?
ntoc            dd ?
modtaboff       dd ?
nmodtab         dd ?
extrefsymoff    dd ?
nextrefsyms     dd ?
indirectsymoff  dd ?
nindirectsyms   dd ?
extreloff       dd ?
nextrel         dd ?
locreloff       dd ?
nlocrel         dd ?
dysymtab_command ends

; ---------------------------------------------------------------------------

dylinker_command struc ; (sizeof=0xC, align=0x4, copyof_13)
                                        ; XREF: HEADER:0000000100000408/r
cmd             dd ?
cmdsize         dd ?
name            lc_str ?
dylinker_command ends

; ---------------------------------------------------------------------------

lc_str          union ; (sizeof=0x4, align=0x4, copyof_14)
                                        ; XREF: dylinker_command/r
                                        ; dylib/r
offset          dd ?
lc_str          ends

; ---------------------------------------------------------------------------

uuid_command    struc ; (sizeof=0x18, align=0x4, copyof_15)
                                        ; XREF: HEADER:0000000100000428/r
cmd             dd ?
cmdsize         dd ?
uuid            db 16 dup(?)
uuid_command    ends

; ---------------------------------------------------------------------------

version_min_command struc ; (sizeof=0x10, align=0x4, copyof_17)
                                        ; XREF: HEADER:0000000100000440/r
cmd             dd ?
cmdsize         dd ?
version         dd ?
reserved        dd ?
version_min_command ends

; ---------------------------------------------------------------------------

linkedit_data_command struc ; (sizeof=0x10, align=0x4, copyof_19)
                                        ; XREF: HEADER:0000000100000450/r
                                        ; HEADER:0000000100000460/r ...
cmd             dd ?
cmdsize         dd ?
dataoff         dd ?
datasize        dd ?
linkedit_data_command ends

; ---------------------------------------------------------------------------

dylib_command   struc ; (sizeof=0x18, align=0x4, copyof_21)
                                        ; XREF: HEADER:0000000100000478/r
cmd             dd ?
cmdsize         dd ?
dylib           dylib ?
dylib_command   ends

; ---------------------------------------------------------------------------

dylib           struc ; (sizeof=0x10, align=0x4, copyof_22)
                                        ; XREF: dylib_command/r
name            lc_str ?
timestamp       dd ?
current_version dd ?
compatibility_version dd ?
dylib           ends

; ---------------------------------------------------------------------------

source_version_command struc ; (sizeof=0x10, align=0x8, copyof_18)
                                        ; XREF: HEADER:00000001000004C0/r
cmd             dd ?
cmdsize         dd ?
version         dq ?
source_version_command ends


;
; +-------------------------------------------------------------------------+
; |   This file has been generated by The Interactive Disassembler (IDA)    |
; |           Copyright (c) 2018 Hex-Rays, <support@hex-rays.com>           |
; |                            Freeware version                             |
; +-------------------------------------------------------------------------+
;
; Input SHA256 : D31B60E0852C85340FD08456F4C3885B97CA215A41041BE5BE5FB915E45D77DB
; Input MD5    : 625E556F04887209A48ABBC27A0CDAED
; Input CRC32  : 16589C74


                .686p
                .mmx
                .model flat
.intel_syntax noprefix

; ===========================================================================

; [00000F10 BYTES: COLLAPSED SEGMENT HEADER. PRESS CTRL-NUMPAD+ TO EXPAND]
; ===========================================================================

; Segment type: Pure code
__text          segment para public 'CODE' use64
                assume cs:__text
                ;org 100000F10h
                assume es:nothing, ss:nothing, ds:nothing, fs:nothing, gs:nothing

; =============== S U B R O U T I N E =======================================

; Attributes: bp-based frame

                public start
start           proc near

var_14          = dword ptr -14h
var_10          = dword ptr -10h
var_C           = dword ptr -0Ch
var_8           = dword ptr -8
var_4           = dword ptr -4

                push    rbp
                mov     rbp, rsp
                sub     rsp, 20h
                lea     rdi, aD         ; "%d"
                lea     rsi, [rbp+var_8]
                mov     [rbp+var_4], 0
                mov     al, 0
                call    _scanf
                cmp     [rbp+var_8], 0
                mov     [rbp+var_C], eax
                jle     loc_100000F54
                lea     rdi, aA0        ; "b > 0\n"
                mov     al, 0
                call    _printf
                mov     [rbp+var_10], eax
                jmp     loc_100000F65
; ---------------------------------------------------------------------------

loc_100000F54:                          ; CODE XREF: start+28↑j
                lea     rdi, aN0        ; "n < 0\n"
                mov     al, 0
                call    _printf
                mov     [rbp+var_14], eax

loc_100000F65:                          ; CODE XREF: start+3F↑j
                mov     eax, [rbp+var_4]
                add     rsp, 20h
                pop     rbp
                retn
start           endp

__text          ends

; ===========================================================================

; Segment type: Pure code
__stubs         segment word public 'CODE' use64
                assume cs:__stubs
                ;org 100000F6Eh
                assume es:nothing, ss:nothing, ds:nothing, fs:nothing, gs:nothing
; [00000006 BYTES: COLLAPSED FUNCTION _printf. PRESS CTRL-NUMPAD+ TO EXPAND]
; [00000006 BYTES: COLLAPSED FUNCTION _scanf. PRESS CTRL-NUMPAD+ TO EXPAND]
; ---------------------------------------------------------------------------
; ===========================================================================

; Segment type: Pure code
__stub_helper   segment dword public 'CODE' use64
                assume cs:__stub_helper
                ;org 100000F7Ch
                assume es:nothing, ss:nothing, ds:nothing, fs:nothing, gs:nothing

loc_100000F7C:                          ; CODE XREF: __stub_helper:0000000100000F91↓j
                                        ; __stub_helper:0000000100000F9B↓j
                lea     r11, off_100001008
                push    r11
                jmp     cs:dyld_stub_binder_ptr
; ---------------------------------------------------------------------------
                align 4
                push    0
                jmp     loc_100000F7C
; ---------------------------------------------------------------------------
                push    0Eh
                jmp     loc_100000F7C
__stub_helper   ends

; ===========================================================================

; Segment type: Pure data
__cstring       segment byte public 'DATA' use64
                assume cs:__cstring
                ;org 100000FA0h
; char aD[]
aD              db '%d',0               ; DATA XREF: start+8↑o
; char aA0[]
aA0             db 'b > 0',0Ah,0        ; DATA XREF: start+2E↑o
; char aN0[]
aN0             db 'n < 0',0Ah,0        ; DATA XREF: start:loc_100000F54↑o
__cstring       ends

; ===========================================================================

; Segment type: Regular
__unwind_info   segment dword public '' use64
                assume cs:__unwind_info
                ;org 100000FB4h
                assume es:nothing, ss:nothing, ds:nothing, fs:nothing, gs:nothing
                db    1
                db    0
                db    0
                db    0
                db  1Ch
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db  1Ch
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db  1Ch
                db    0
                db    0
                db    0
                db    2
                db    0
                db    0
                db    0
                db  10h
                db  0Fh
                db    0
                db    0
                db  34h ; 4
                db    0
                db    0
                db    0
                db  34h ; 4
                db    0
                db    0
                db    0
                db  6Fh ; o
                db  0Fh
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db  34h ; 4
                db    0
                db    0
                db    0
                db    3
                db    0
                db    0
                db    0
                db  0Ch
                db    0
                db    1
                db    0
                db  10h
                db    0
                db    1
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db    0
                db    1
__unwind_info   ends

; ===========================================================================

; Segment type: Pure data
; Segment alignment 'qword' can not be represented in assembly
__nl_symbol_ptr segment para public 'DATA' use64
                assume cs:__nl_symbol_ptr
                ;org 100001000h
dyld_stub_binder_ptr dq offset dyld_stub_binder
                                        ; DATA XREF: __stub_helper:0000000100000F85↑r
off_100001008   dq 0                    ; DATA XREF: __stub_helper:loc_100000F7C↑o
__nl_symbol_ptr ends

; ===========================================================================

; Segment type: Pure data
; Segment alignment 'qword' can not be represented in assembly
__la_symbol_ptr segment para public 'DATA' use64
                assume cs:__la_symbol_ptr
                ;org 100001010h
; int (*printf_ptr)(const char *, ...)
_printf_ptr     dq offset __imp__printf ; DATA XREF: _printf↑r
; int (*scanf_ptr)(const char *, ...)
_scanf_ptr      dq offset __imp__scanf  ; DATA XREF: _scanf↑r
__la_symbol_ptr ends

;
; Imports from /usr/lib/libSystem.B.dylib
;
; ===========================================================================

; Segment type: Externs
; UNDEF
                extrn dyld_stub_binder:qword
                                        ; DATA XREF: __nl_symbol_ptr:dyld_stub_binder_ptr↑o
; int _printf(const char *, ...)
                extrn __imp__printf:qword
                                        ; DATA XREF: __la_symbol_ptr:_printf_ptr↑o
; int _scanf(const char *, ...)
                extrn __imp__scanf:qword
                                        ; DATA XREF: __la_symbol_ptr:_scanf_ptr↑o


                end start
