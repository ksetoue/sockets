# Simple Mail Protocol using Sockets in C

Project developed for Distributed Systems class. 

## Contents

* Client application that connects with the server. It sends the mail content to the server application, which verify the sender and receiver mail contact, sending the message. 
The Client application also waits the server response. If the connection is acepted, then it sends a file with the mail text. After the text is successfully transfered, the application shows a success message. Otherwise, it shows an error message. 

* Server application that verifies if the contact info is valid, and returns a messege with the request response. After that, the server awaits for the mail content from the client, and, after receiving it, the connection is closed.

The email text content is retrived from file named `send.txt`. The server output is located at `sample_file.txt`.

## Usage

Compile: 
``` sh 
    gcc client.c -o client
    gcc server.c -o server
```

Run: 

``` sh
    ./server
    ./client    # in another bash
```





