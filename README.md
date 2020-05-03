openDFS
=======

An OpenSource Distributed File System - 

openDFS is a kind of distributed file system by which we can use a number of different machine's storage space in a single address space. so for say if we have 100 slave machines of 100GB each connected on lan through my DFS, will represent a single address space of 100000GB on the server machine which we can access from any  client over the internet. I this project i used the socket programming on liinux system. I have created a kind of api which resembles same kind of interfaces which we find on normal file systems using linux (Eg. open(), close(), read(), write(), lseek(), chmod(), chown(), rmdir(), mkdir(), creat() and so on in a distributed file system).


If anybody is interested in contributing or using the project. Feel free to use it