#!/usr/bin/python
from socket import *
import sys

#Author: Matthew Krilanovich
#Program Name: chat_server.py
#Description: One part of a paired chat system.
# - Accepts a port number, creates a socket, binds the socket with port number,listens for new requests, and initiates a chat system. The client host initiates the chat. 
#!/usr/bin/python

def receiveMsg(connectionSocket):
    while True:
        #Receive Message
        message = ""
        message = connectionSocket.recv(512)
        #decode
        return message.decode()

def sendMsg(connectionSocket,s_message):
    #print("test: entered sendMsg func\n")
    counter = 0
    #send one char of message, stopping at buffer length
    while (counter < len(s_message)):
        # encode 
        sent = connectionSocket.send(s_message[counter:].encode())
        if sent == 0:
            connectionSocket.close()
        counter += sent
    #print("test: exiting sendMsg func\n")

#accept port number
portNumber = int(sys.argv[1])

#create socket
serverSocket = socket(AF_INET, SOCK_STREAM)

#bind socket
serverSocket.bind(("",portNumber))

#listen for new requests
serverSocket.listen(1)
#print("1) The server on port {} is now listening for a connection\n".format(portNumber))

#get user handle
serverHandle = ""
while(len(serverHandle) > 10 or len(serverHandle) < 1):
    serverHandle = raw_input("Enter server handle thats 10 characters or less: ")

print("Waiting for client to initiate chat.\n")

#accept connection at socket
while 1:
    connectionSocket,addr = serverSocket.accept()
    print "Started connection at address: {}\n".format(addr)
    #print ("3) Started connection at address: %s\n", addr)
         
    #tcp connection
    clientHandle = receiveMsg(connectionSocket)
    #print"4) {} has connected to the server\n".format(clientHandle)

    #used for responses
    client_title = clientHandle + "> "

    connectionSocket.send(serverHandle.encode())
    server_title = serverHandle + "> "
    #print("Test: server_title =  {}\n".format(server_title))    
    #begin chat
    while 1:
        #reveieve msg
        r_message = receiveMsg(connectionSocket)
        if r_message:
            r_message = r_message.rstrip()
            print(client_title + r_message)

        #send msg
        #print("Test: before send input\n")
        s_message = raw_input(server_title)
        #print("Test: input = {}\n".format(s_message))
        if s_message == '\quit':
            print ("Quitting Chat session\n")
            break
        #print("Test: before send func\n")
        sendMsg(connectionSocket, s_message)
      
    #close connection
    connectionSocket.close()
    print("Connection now closed\n")
    sys.exit(0)
