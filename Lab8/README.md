ECE382_Lab8 - Robot Maze
========================

####Required Func Video
https://www.youtube.com/watch?v=OdXxg5vZ69Y&list=UUlRCgLUgi5g9Fu_dqypjz5A&index=4

####B Func Video
https://www.youtube.com/watch?v=tzsUJcCBht8&list=UUlRCgLUgi5g9Fu_dqypjz5A&index=3

####A Func Video
https://www.youtube.com/watch?v=-oUeEMapHPk&index=2&list=UUlRCgLUgi5g9Fu_dqypjz5A

###Background Info:
During this lab, we will combine the previous laboratory assignments and program our robot to autonomously navigate through a maze.  On the last day of the lab, each section will hold a competition to see who can solve the maze the fastest.  The fastest time in ECE 382 will have their name engraved on a plaque in the CSD lab.  Believe it or not, the main goal of this lab is for you to have some fun with computer engineering!

![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab8/master/maze_diagram.png "Maze")

####Required Functionality:
Get through door 1 of the maze

####B Functionality:
Get through door 2 of the maze

####A Functionality:
Get through door 3 of the maze without hitting the wall

##Preliminary Design
####Prelab
  I'm going to have an interrupt that goes through each IR sensor and saves the reading.  Constantly cycling through the sensors so the robot always knows where it's at in the maze.  When I have a wall approaching in front, a wall constantly there on the right, but suddenly nothing on the left, I know the first left turn of the maze is about to happen.  Once the robot is x inches away from the wall in the front and there's a hole on the left (as a sanity check) for the first left turn, it can turn and be on its merry way.  The first functionality is just a left turn.  The next functionality is left then right.  And the next is a left turn, 2 right turns, and a left turn.  Cycling through all the sensors will keep track of where the robot is at in the sequence of turns.  Like a state machine!!  All the EI will be had.

#Code
##Given Code
Really our given code was our lab6 and lab7 files.  The better our previous labs were, the easier time we had getting to work on lab8.  Unfortunately, I just tried to get previous labs done and nothing more, so I struggled... A lot.  I tried to breakdown the problem as much as possible so my lab8 code is filled with subroutines.  My main pretty much initializes everything, then calls the functionality routines.  We (other cadets) found out that once you had required functionality, b and a functionality follows pretty quick.

##Required Functionality
For required functionality, I wanted the robot to move forward until it saw a wall in front of it.  So I needed to constantly check the sensors for a reading greater than that cutoff value that meant a wall was there.  Once it was a couple of inches from a wall, the robot would turn left, and go full speed ahead through the door!!

##B and A Functionality
You can see that I had 4 versions of my lab8 code before getting required functionality.  Then my next version got me b anda functionality.  Once you had the initial led of the maze down, you really just had to tweak how the robot turned and boom, A functionality.

##Debugging
A problem I had was one motor running faster than the other.  I ended up tweaking with two variables until the robot was going (for the most part) straight.  In the end, POS favored going to the left a little bit, but it wasn't bad enough to mess with my final results.

    TA1CCR0 = 0x0045;						// set signal period

    TA1CCR1 = 0x001F; // lower to veer more left

    TA1CCR2 = 0x0021;

So the above three lines controlled the speed of the robot and the speed of each individual motor.  I tweaked with it quite a bit over a couple of days so I left a note for me to help code it.

##Testing Methodology / Results
The rest of my classmates weren't joking that b and a functionality followed close behind required functionality.  Once I had requried, I just had to mess with the cutoff values and how much the robot turned.  Within just a few hours, I had a functionality.  POS was a fast little sucker.  If I had gotten the robot to work during the in class competition, I might've given Sabin a run for his money!

##Observations and Conclusions
Get help sooner.  Dr. Coulston came over, took maybe 3 minutes looking over my code, and gave me a list of problems that fixed everything. 

##Documentation
Besides instructors, no outside help was received.
