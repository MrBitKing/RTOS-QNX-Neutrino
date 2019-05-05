CST8244 – Real-Time Programming
Page 1 of 4
Laboratory 4: Namespaces

This laboratory will result in your ability to build a simple program that passes data structures in messages between a client and server program.
Objective:
Write a client that sends a message containing two integers and a character representing the operation to a server. The server computes the numeric value for the result of the numbers and the given operation, and then returns a message containing a structure as the result. The client will pass the two numbers as integer members of an “input parameters” struct and the operation as a character value in the struct. The server will reply with a different struct containing a double, a flag representing whether or not an error occurred (or would have occurred if the operation had been attempted), and a character string of up to 128 bytes containing the error description. (You can use your imagination for the string content but try to cover all types of errors.)
The operation should be one of: ‘+’, ‘-‘, ‘x’, and ‘/’. (Note: that is an ‘x’ not a ‘*’). The input integer values could be any valid integer. The server must check for division by 0. The client should check for valid inputs.
You should start the server in the background (using &) and have it print out its process_id. Then multiple client applications could be run in the foreground that use a command-line argument on the client application to pass the PID of the server which they want to use to perform the operation.
The client program will block until receiving the result message from the server program and then the client program will output the result (or error condition) to the console. You can find an example below.
Example:
#./lab4_server &
The server is running as PID 77234
#./lab4_client 77234 6 + 9
The server has calculated the result of 6 + 9 as 15.
CST8244 – Real-Time Programming

Page 2 of 4
Steps:
1. Create a new Momentics workspace named: cst8244_lab04
2. Create two new projects named lab4_server and lab4_client. Create both projects as QNX Projects > QNX Executable.
3. Download the include file “calc_message.h” (src: Brightspace) to define the typedefs to be used for sending requests and receiving responses. Only one copy of the header file should appear in your Momentics.IDE workspace. Put the header file in the lab4_server project space. The client (lab4_client) is to reference the header file.
4. Create the lab4_server and lab4_client programs.
5. Test your programs.

Deliverables
Prepare a zip-file archive that contains the following items:
1. Export your projects as a zip-archive file.
Momentics IDE provides a wizard to export your project:
File > Export… > General > Archive File > Next > Select All > Click ‘Browse…” > Save As: cst8244_lab04_yourAlgonquinCollegeUsername.zip > Save > Finish
2. Take a (single) screen-shot of your Neutrino terminal window that captures these scenarios:
i. A normal scenario for: +. For example, client sends: 6 + 9
ii. A normal scenario for: -
iii. A normal scenario for: x
Note: avoid testing with ‘*’ for multiplication, as the Neutrino command interpreter will perform file globbling (i.e. the interpreter sees ‘*’ as wildcard).
iv. A normal scenario for: /
v. An abnormal scenario for server: division by 0 (zero)
vi. An abnormal scenario for client: invalid input (not see in reference screenshot; left for you to demo)
CST8244 – Real-Time Programming
Page 3 of 4
Compare yours to mine:
CST8244 – Real-Time Programming
Page 4 of 4
3. A “README.txt” file reporting the status of your lab. Follow this template:
Title {give your work a title}
Status
{Tell me that status of your project. Does your program meet all of the requirements of the specification? Does your program run, more importantly, does your program behave as expected? Does your program terminate unexpectantly due to a run-time error? Any missing requirements? A small paragraph is sufficient.}
Known Issues
{Tell me of any known issues that you’ve encountered.}
Expected Grade
{Tell me your expected grade.}
Upload your zip-file to Brightspace before the due date.
How to Run the Client and Server
Please refer to the Resources module on Brightspace for suggestions on how to run your client and server programs on Neutrino. My personal fav… drag ‘n drop from Momentics IDE to Neutrino 
