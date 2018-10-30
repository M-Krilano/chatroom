Instructions
  
1) Make sure "chat_client.c" and "chat_server.py" are in the same directory

//chat_server.py
2) Type: "chmod +x chat_server.py"
3) Type: ./chat_server.py <port number>
4) You will be prompted to enter a SERVER name, enter a username and hit ENTER
5) Wait for CLIENT to connect and send a message

//chat_client.c
***In a different termianl window***
6) Type: "gcc -Wall chat_client.c -o client"
7) Type: "client <host address> <port number>"
8) You will be prompted to enter a CLIENT name, enter a username and hit enter
9) Type in a message to send to the SERVER and hit ENTER

//chat_server.py
10) You should receieve a message from the CLIENT, type in a message to send the CLIENT and press ENTER.
