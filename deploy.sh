#!/bin/bash
  
scp ./src/cat/s21_cat root@192.168.31.187:/usr/local/bin 
scp ./src/grep/s21_grep root@192.168.31.187:/usr/local/bin  

if [ $? -eq 0 ]; then
    echo "Deploy files passed!"
else
    echo "Deploy files failed!"
    exit 1  
fi
    