<h2>Compiling The Code</h2>
The makefile in this folder will compile the client and server code and create the respective executables for you. In order to run the makefile, simply type <b>"make"</b> in the linux command line while inside of the project directory. This will compile all .c files associated with the TCP server and client into object files, and create executables from those. 
<br><br>
The makefile simply uses gcc to compile the .c files into .o files, and uses the .o files to create the server and client executables.

<h2>Running The Code</h2>
Once you have run <b>"make"</b> in the linux command line, there should be two executables, one called "server" and another called "client". To run these, type <b>"./server"</b> or <b>"./client"</b> into the command line.

<h3>Usage</h3>
./server \<Server Port\>
<br>
./client \[-p\] \<Server IP\> \[\<Echo Port\>\]
<br>
For client, the -p flag will print the RTT (Round Trip Time) in milliseconds.

<h3>Cleanup</h3>
While you *can* just retype <b>"make"</b>, you can tidy up the project directory by removing all .o, .h.gch, and executable files by typing <b>"make clean"</b>.