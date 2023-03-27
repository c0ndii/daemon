#!/bin/bash
gcc main.c daemon.c copy.c currentTime.c daemon_at_work.c sigusr1.c copyRecursive.c -o program
cowsay "Kompilacja zakończona powodzeniem"
