#!/bin/bash
gcc main.c copy.c daemon.c currentTime.c daemon_at_work.c sigusr1.c -o program
echo "Kompilacja zakończona powodzeniem"
