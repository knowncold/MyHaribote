# Day 6

## 分割编译

### 不用头文件

分别编译，宏定义两边都要写，如果用汇编中定义的函数，用到的C就要写，用到函数就要先定义，小文件中再去实现。

```makefile
%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst
```

普通规则优先级高于一般性规则。

### 头文件


## 中断处理
