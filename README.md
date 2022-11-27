Name: Roberto Villarreal Andrade
Student ID: 1627153
Course Name: CMPUT 379

Assignment 2

Included Files:
  *server_main.cpp
  *server.cpp
  *server.h
  *client_main.cpp
  *client.cpp
  *client.h
  *trans.cpp
  *trans.h
  *server.man
  *client.man
  *README
  *Makefile

Makefile Targets:
  -make (all): builds the project into two exacutables called server and client
  -make clean: Removes all object files and exacutables

Running Instructions:
  as specified in assignment

Notes:
  -time is calculated based on the start of the server and when the last transaction was completed.

Acknowledgements:
  https://www.ibm.com/docs/en/i/7.1?topic=designs-example-nonblocking-io-select
  https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select
  https://stackoverflow.com/questions/28098563/errno-after-accept-in-linux-socket-programming