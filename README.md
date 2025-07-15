
# 📚 linked_list

A linked list shared library writen in **C** and made in **TDD**
Most operations have a complexity of **O(1)**


## 📦 Building it and checking

```
make lib
make run-tests
```


## 🤔 How to use

Include `include/List.h` in your project and link the library
```
gcc [your sources].c -Llib/ -llist.so -o [your program]
LD_LIBRARY_PATH=lib/ ./[your program]
```


## 😨 Found a bug ?

Create a pull request with a failing test, I'll make it pass
