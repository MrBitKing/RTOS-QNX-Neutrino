CST8244 – Real-Time Programming

Page 1 of 5
Assignment 1

This assignment consists of designing and building a set of programs to simulate a building entry controller system and provide a testing harness. The system is similar to that discussed during the lectures – a two door system with card scanners on each door, a scale that weighs the person after they have entered the space between the two doors, and a human guard that uses a switch to open each of the doors.
Details:

1. People enter the entry control space from the left to enter the building and enter the control space from the right to exit the building.
2. Each person requesting to enter or exit the building scans their unique personal id card containing an integer person_id number. The scanner sends a message to the controller with the input information (which door opening is being requested, and which person_id is being used.; (e.g., “left 12345”)
3. Only 1 person at a time should be able to be inside the lock.
4. Assume that the door is self-closing (but not self-locking), and that an event (see below under Step 4.a.) will be sent to the controller when the status of the door changes.

Steps:

1. (20%) Design a STATE MACHINE for the system. Provide the following:
a. A list of inputs, including persistent saved data (e.g., the weight of the person on entry)
b. A list of outputs, including persistent saved data.
c. A list of conditions / events that cause transitions between states.
d. A list of states
e. A diagram showing the state machine.

2. (20%) Write a program assign1_display that displays the status of the system – which door is open / closed, if there is a user waiting to enter from the left or right, etc. The program should run in the background and print out status information to the console each time a status update is sent to it using a message from the assign1_controller program. The assign1_display program can print out its process id when it first starts up (as in Lab4).

3. (40%) Write a program assign1_controller that runs in the background, operates the state machine for the controller, and directly maintains the persistent data and status for the controller. It should have separate functions for the state handler for each state. Each state handler should perform the actions required for that state, send a message to the assign1_display program to update the display (as required), and then check the exit conditions for that state. When an exit condition is met, the state handler should return the function pointer for the next state handler. This does not have to be done using a lookup table but could be implemented that way. The assign1_controller program should print out its process id when it first starts up.

CST8244 – Real-Time Programming
Page 2 of 5

4. (20%) Write a program assign1_inputs that prompts the user for inputs to the controller. This program is simulating all of the input events from the devices; e.g., card readers, door latches, scale.
Prompt for the input:

a. The first prompt should be:
Enter the event type (ls= left scan, rs= right scan, ws= weight scale, lo =left open, ro=right open, lc = left closed, rc = right closed , gru = guard right unlock, grl = guard right lock, gll=guard left lock, glu = guard left unlock)
b. If the event is the lo, ro, lc, rc, glu, gll, gru, or grl, no further prompt is required.

c. If the event is ls or rs, prompt for the person_id.
Enter the person_id

d. If the event is ws, prompt for the weight.
Enter the weight
Once the input is finished, send a message to the assign1_controller program to provide the input “event”, and loop back to prompt again.

Note: You can store input scenarios in text files and then run them using the command:
# ./assign1_inputs 77235 < myinputsfile &
With an input file “myinputsfile” containing:
ls
12345
glu
lo
ws
70
lc
gll
gru
ro
rc
grl
exit

CST8244 – Real-Time Programming
Page 3 of 5
Example Startup:

#./assign1_display &
The display is running as process_id 77234.
[status update : initial startup]

#./assign1_controller 77234 &
The controller is running as process_id 77235.

#./assign1_inputs 77235
Enter the device (ls= left scan, rs= right scan, ws= weight scale, lo =left open, ro=right open, lc = left closed, rc = right closed , gru = guard right unlock, grl = guard right lock, gll=guard left lock, glu = guard left unlock)
…..
Steps:

1. Get Lab 4 working! (Use it as a model).

2. Design the state machine.

3. Create a new Momentics workspace named: cst8244_assign1

4. Create new projects named assign1_display, assign1_controller, and assign1_inputs. Create these projects as QNX Projects > QNX Executable.

5. Create an include file “mystruct.h” to define the typedefs for structs to be used for sending requests and receiving responses, and for storing the status information.

6. Create the assign1_controller, assign1_display, and assign1_inputs programs.

7. Test your programs.

CST8244 – Real-Time Programming
Page 4 of 5

Deliverables
Please note: the deliverables have changed for this assignment. Please read and note the differences.
Upload the following deliverables to Brightspace:

1. Export your projects as a zip-archive file.
Momentics IDE provides a wizard to export your project:
File > Export… > General > Archive File > Next > Select All > Click ‘Browse…” > Save As: cst8244_assign1_yourAlgonquinCollegeUsername.zip > Save > Finish
2. The diagram showing your state machine.
3. Upload a zip-file of screenshots showing the run-time behaviour of your assignment. Capture these 2 scenarios:
a. The scenario described in Step 4 (see above).
b. This scenario, called “enteringLeftData”:
ls
12345
glu
lo
ws
123
lc
gll
gru
ro
rc
grl
exit
Reference Screenshots: your screenshots for the first scenario should look very, very similar to mine. See “Memo: Reference Screenshots” in the “Assignment 1” folder on Brightspace.
4. A “README.txt” file reporting the status of your assignment. Follow this template:
Title {give your work a title}
Author
{Please sign your work. For example: @author Gerald.Hurdle@AlgonquinCollege.com
Include your partner’s name (if you have one)}
{IF you worked with a partner, provide a brief description of what you worked on, and contributed to the assignment.}
Status

CST8244 – Real-Time Programming
Page 5 of 5

{Tell me that status of your project. Does your program meet all of the requirements of the specification? Does your program run, more importantly, does your program behave as expected? Does your program terminate unexpectantly due to a run-time error? Any missing requirements? A small paragraph is sufficient.}
Known Issues
{Tell me of any known issues that you’ve encountered.}
Expected Grade
{Tell me your expected grade.}
