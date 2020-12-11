# Readme
## Author: Cristian C. Castillo <br />
🗄️ HTTP Server <br />
--------
## Requirements 🖥️ 
The following installations are required to execute code:
- Ubuntu 18.04 environment 

- Virtual Machine 

- Clang

- Optional: Hit the like if you think its useful!

--------
## Disclaimer
--------
- Do not use this program for commerical usage. I am not responsible for any liabilities that incur should you set this out in the public, and please use at one's own risk.

- Use this program only as a reference to learn about HTTP Server Socket Programming. Should you decide to use this program in the Academia, credit the author and source. Also note do not copy/plagiarise this work for personal academic advancement, you risk dire consequences such as being given a failing grade, and possibly expelled.

## Application GIF Overview

<img src="http://g.recordit.co/ZjvlZChYrV.gif" width=650><br>

## Directory Contents 📂
- httpserver.cpp: main driver program.

- httpserver_definitions.cpp: function definitions/operations

- httpserver_headers.h: Declared void functions that work with httpserver definitions to enable driver program.

- DesignDocument.pdf: Contains Design plan, pseudo code (each part), Test Case,fucntions, assignment solution, assumption, goals, and data structures.

- Makefile: Runs and compiles the necessary flags for the program and initiates a ready state for the program to be executed.

- README.me: What you are currently reading; Contains concise specs about program information and setup.

----------
## Notes 📝
- This program was developed on a virtual machine environment with Ubuntu and utilizes the Httpserver Client-Model, via system calls to the Kernel's Operating System.

- Program is tested vigorously Valgrind for memory leaks.

- Testing can be done by typing in terminal: valgrind ./httpserver (optional --leak-check=full). Resulting in total heap usage: 52 allocs, 52 frees, 86,742 bytes allocated. Server is not in run mode.

- Testing can be done by typing in terminal: valgrind ./httpserver localhost 8080 --leak-check=full. Reseulting in total heap usage: 3 allocs, 3 frees, 74,280 bytes allocated. Server is not in run mode.

- Testing can be done ./httpserver localhost 8080 with no flags, while the server is running with the following command: valgrind ./httpserver localhost 8080. Since the program is concurrently in run mode, 64 bytes will be in use in 1 block, a total heap usage of 59 allocs, 58 frees, and 94,882 bytes allocated. The amount reachable is 64 bytes in 1 blocks, but this due to getaddrinfo(), a linked list, that has yet to be freed because the HTTP Server is active while conducting this test. Should the HTTP Server not be in run mode, you will have all no leaks as expected.

- Binary files were tested and compared with the diff -s foo1 foo2. Both are identical when generated and compared to original file. You may also compare the original file interchangeably with foo1 and foo2.

- Files persist with this program should the client disconnect from the server. To test this feature, start the server, and go into a remote directory or desktop. In the terminal type in the following command: curl -T foo localhost:8080/1234567890 -v. This command will put the file, along with its contents, in the httpserver directory from your remote location. You may than retrieve this file from your location by typing in the following command in the terminal: curl localhost:8080/1234567890 > new_file01. To compare if your contents indeed where retrieve successfully, you may apply the following command: diff foo new_file01 -sq. For further reassurance, you may locate file 1234567890 within the HTTP Server directory, and compare all three contents interchangeably to ensure contents were recorded accurately.

- The HTTP Server program is to remain connected during all request, exluding a Put Request with no content-length (resulting in a closed connection that is conducted manually with no client response from the server). The following test checks for prolong connection persistence, in the terminal type the following: curl http://localhost:8080/123456789[a-b], a and b as any integer that pertain to files. This command will generate a Get Request, returning the content sequentially in the order that it was called, with the content, status msg e.g 200, 404, etc..., on each iteration, all while maintaing a connected status.

- The HTTP Server program was tested with the bible's length of 4.5 million bytes on a (Put Request) curl -T bible localhost:8080/abc123456[0-9] -v for a consecutive ten iterations,all while succesfully keeping the connection intact to the server. This command may also be implented as curl -T bible localhost:8080/123456789[0-9], the iteration numbers within the brackets may be of the clients choosing in regards to range.

- The HTTP Server program was tested with the bible's length of 4.5 million bytes on a (Get Request) curl 'localhost:8080/abc123456[0-9]' -v | grep "Connection" for a consecutive ten iterations, all while succesfully keeping the connection intact to the server. This command may also be implemented as curl localhost:8080/123456789[0-9] -v from within the brackets may be of the clients choosing in regards to range.

- The HTTP Server program when prompted with multiple Get Request/Put Request will remain open during the entire process, patiently awaiting for the next curl upon completing its objective, and will respond accordingly with the appropriate response should a file be present, internal error, forbidden, created, etc... all while keeping the connection alive.

- The HTTP Server program will acknowledged a file of content-length 0 and null upon a Put Request, and will retrieve them succesfully upon a Get Request.
---------
## Limitations/Issues ❗
- HTTP Server program runs properly on Unix files and environment.

- HTTP Server program can handle volume of extremely large file files, but upon a PUT Request must be manually closed!

- Allowed terminal inputs are in the following formats: ./httpserver ip address port number, ./httpserver ip address, ./httpserver localhost, ./httpserver localhost port number. The port number can be customized, but numbers ranging from 0-1023 are reserved for privileged services and designated as well known ports.

- Note: Port number 80, is the HTTP reserved port, to run the following port input the following in the terminal: sudo ./httpserver ip address or localhost. You can either indicate 80 after the ip address/localhost, but the program has set this port to default should a port number not be provided.

- HTTP Server program only takes the following ASCII characters:A-Z, a-z,0-9. Should the client make a bad request, the client shall received a 400 hundred message response, and will be prompted via the terminal for the next request. However, the server will not crash, present the error on its side, and continue to run, waiting for the next request.

- HTTP Server program only takes the ASCII length of exactly ten characters as a valid request. Should the client make a bad request, the client shall received a 400 bad request message response, and will be prompted via the terminal for the next request.  However, the server will not crash, present the error on its side, and continue to run, waiting for the next request.

- Should the client make a request to fetch a file from the HTTP Server program, and the file does not exist, the client will be presented with a response message of 404 file not found.  However, the server will not crash, present the error on its side, and continue to run, waiting for the next request.

- Should the client make a request to fetch a file from the HTTP Server program, and server malfunctions, the client will be presented with a response of message 500 internal server error. Indicating a malfunction within the server program.  However, the server will not crash, present the error on its side, and continue to run, waiting for the next request.

- Should the client make a request to put a file to the HTTP Server program, and the file fails along the way, the client will be presented with a response message of 404 file not found. However, the server will not crash, present the error on its side, and continue to run, waiting for the next request.

- The HTTP Server Program does not close the connection during any request, only on the condition that the request itself has no content length on a put, will the the connection close; manually exited by the user (the client) who serves a file to the server. 

---------
## Instructions 👾
- To utilize the HTTP Server program you require two open terminals.

- On one terminal run the server (see Limitations and Issues for formatting inputs) and hit enter. The terminal will began processing client request. The additional terminal is where the client, you, or anyone can began making multiple Get/Put request as need.

- Every successful request will result in a response with the clients terminal.However, the server will not crash should an error present itself during such a request. The server will present the error on its side, and continue to run, awaiting the clients next request.

- The client can send a Get Request (see Notes on format) to retrieve file contents, which will generate on the client side. Upon a successful fetched file, the client will be notified of a 200 OK message, indicating that the file was retrieved succesfully from the server.

- The client can send a Put Request (see Notes on format) to store persistent files in the HTTP Server program. Upon a successful put, the client will be notified via the server of a 201 Created message, indicating that their file has been successfully uploaded to the HTTP Server program.

- Each PUT/GET request will terminate on its on, while the HTTP Server runs gracefully. Hence, there is no need for the client to terminate anything on their end. However, the client must terminate the server in the other terminal with the command: ctrl+c to terminate the server program.

- Should the server encounter a Put Request from the client with an unknown content-length file, the server shall remain open until the client manually closes the connection, ending all activity.

-------
## Sources 📄
[<img align="left" alt="C++" width="80" height="80" src="https://cdn.freebiesupply.com/logos/large/2x/c-logo-png-transparent.png" />][cPlusPlus]

[<img align="left" alt="ShellWave" width="80" height="80" src="https://upload.wikimedia.org/wikipedia/commons/9/9f/Youtube%28amin%29.png" />][youtubeShell]

[<img align="left" alt="Beej's Guide" width="80" height="80" src="https://images-na.ssl-images-amazon.com/images/I/41VzZdCKtFL._SX360_BO1,204,203,200_.jpg" />][beejs]

[<img align="left" alt="Geeks Picture" width="80" height="80" src="https://media-exp1.licdn.com/dms/image/C4E0BAQEF0x_q7m5JjQ/company-logo_200_200/0?e=2159024400&v=beta&t=tEbDQX9zSpsKFIZHJzuUCLCTEIXdSAbvaRirs5PKA1M" />][geeks]

[<img align="left" alt="MDN Web" width="80" height="80" src="https://developer.mozilla.org/static/img/opengraph-logo.72382e605ce3.png" />][MDN]

[<img align="left" alt="Linux" width="80" height="80" src="https://media.glassdoor.com/sqll/771794/netcat-biz-squarelogo-1462451320719.png" />][linuxTech]

[<img align="left" alt="Berkley Sockets" width="80" height="80" src="https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcSm5a8lKt9XFTLvZwJKxFxuo52oTa2Rvlj0fQ&usqp=CAU" />][berkleysocks]

[<img align="left" alt="Linux Man" width="80" height="80" src="https://man7.org/tlpi/cover/TLPI-front-cover-small-256.png" />][man7]


[<img align="left" alt="Tutor point" width="80" height="80" src="https://i.pinimg.com/280x280_RS/da/08/dc/da08dcf59c717413b711e2bfbaf2ab45.jpg" />][tutorpoint]

[<img align="left" alt="Linux" width="80" height="80" src="https://image.flaticon.com/icons/png/512/518/518713.png" />][linux]
<br /><br /><br />

[cPlusPlus]: http://www.cplusplus.com/doc/tutorial/
[geeks]: https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/#:~:text=File%20descriptor%20is%20integer%20that,pointers%20to%20file%20table%20entries.&nbsp;
[youtubeShell]: https://www.youtube.com/watch?v=LERSkW_pLBs 
[linux]: https://man7.org/linux/man-pages/man2/syscalls.2.html
[berkleysocks]: https://en.wikipedia.org/wiki/Berkeley_sockets
[beejs]: http://beej.us/guide/bgnet/html/
[linuxTech]: https://linuxtechlab.com/nc-command-ncat-for-beginners/
[MDN]: https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
[tutorpoint]: https://www.tutorialspoint.com/http/http_requests.htm
[man7]: https://man7.org/linux/man-pages/man2/accept.2.html

