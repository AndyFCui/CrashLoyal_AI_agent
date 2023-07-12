# Document statement about project 2 CS5150 Game AI - Crash Loyal Spring 2022

## Author: Andy(Xiang-yu) Cui
## Northeastern University (Boston)
## Artificial Intellgence Of Khoury College



### Project feature:

* Implement the exist method 

  * checkCollision() : perform the general collission 
  * checkCollision2(): perform the multiple collision
  * processCollision(): perform the steering when characters collision

* Characters’ actions followed rationality

  * Size use center point to check collision

  * Avoid Giant wall issue, when two Giant happen collision, they will bypass each other continue their target (Tower).

    * ![image-20220326235458907](C:\Users\Andy Cui\AppData\Roaming\Typora\typora-user-images\image-20220326235458907.png)

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
