clear
make re
valgrind --track-origins=yes --leak-check=full ./codexion 20 5000 2 1 100 2 10 fifo
