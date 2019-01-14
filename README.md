# bfcompiler
Brainfuckのコンパイラです。久しぶりに雑なコードだが書いてみました。テストでHello Worldを出力するコード(hello.bf)も入ってるので試してみてください。

# 使い方
1. compiler.cをコンパイルします。
```bash
cc -o bfc compiler.c
```

2. brainfuckで作成されたコードをコンパイルします。
```bash
./bfc source_code.bf
```

3. 2を実行するとsource_code.bf.sが生成されます。アセンブラでオブジェクトファイルを生成します。
ここでは64bit Linux環境を基準とします。
```bash
nasm -f -o source_code.bf.o elf64 source_code.bf.s
```

4. 3で生成されたオブジェクトファイルを用い、実行ファイルを生成します。
```bash
ld -m elf_x86_64 -o a.out source_code.bf.o
```
