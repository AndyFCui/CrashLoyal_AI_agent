# Crash Loyal

## Author: Andy(Xiang-yu) Cui
## Northeastern University (Boston)
## Artificial Intellgence Of Khoury College

### Project Overview

* Build collision avoidance for Crash Loyal (Northeastern University-Boston CS5150 Game Artificial Intellgence project), our home-brew clone of the popular mobile game Clash Royale. 

### Demo Show
![CrashLoyal_Colliction](https://github.com/AndyFCui/CrashLoyal_AI_agent/assets/10085168/ce01cd01-24bb-4c9b-baa3-1ecc0bdbca34)


### Game Policy

* Source code for Crash Loyal can be found in the Project2.zip file.  The zip includes a Visual Studio 2019 solution.  This game is still a work in progress, so if you find any bugs, please let me know!  Also, as with project one, I don’t have a working solution for Mac (or Linux) development – Mac users will either need to get things compiling on their own, or find a PC to develop on.  
The current interface allows you to drop units by holding a letter key while you left-click.  The unit you get depends on the letter you hold:

* G – Giant
* S – Swordsman
* A – Archer

Under the covers, but not currently displayed in the UI, is an elixir mechanic.  Like in the game, elixir builds up over time, and each unit costs a different amount of elixir.  Each player starts with 8 elixir, and gains 1 elixir every 3 second, up to a maximum of 10 elixir.  

### Project feature:

* Implement the exist method 

  * checkCollision() : perform the general collission 
  * checkCollision2(): perform the multiple collision
  * processCollision(): perform the steering when characters collision

* Characters’ actions followed rationality

  * Size use center point to check collision

  * Avoid Giant wall issue, when two Giant happen collision, they will bypass each other continue their target (Tower).

    * ![image](https://github.com/AndyFCui/CrashLoyal_AI_agent/assets/10085168/904691a3-1dbe-4eaf-b1ae-bb0e78aea009)

  * Each character (include Giant, Swordsman, Archer) will action to attack their target(Tower), and when they will priority attack their collision object until there are dead.
  
  * All character are use their define route line not over the game border.

### Lack of Project

* No winning mechanism

  * Whatever any parts attack all the tower, their characters will happen screen freezes, since game AI are use the simple logic to create one Giant, and two archers in the back. That is not like real game, it should be stop create characters when bule’s part have no alive tower. On the other hand, when we attack all the red tower become black (dead), same thing the red part should stop create character as well. 

* It still have some improvement parts, but lack of the time deadline. It just implement the grading guidelines.
  * Documentation: I think I am done the most function from the document statement.
  * Game will show a relatively reasonable way of running without breakdown.
  * All game logic compare with official game looks “Right”.



### Citation

* Reynolds, C. (2004, June 6). Reynolds Engineering & Design. *Steering Behaviors For Autonomous Characters.*  Retrieved March 27, 2022, from http://www.red3d.com/cwr/steer/
* Xavier, M. (2021). *C++ Programming: Complete Guide to Learn the Basics of C++ Programming in 7 days*. Webmatt Publishing Ltd. 
