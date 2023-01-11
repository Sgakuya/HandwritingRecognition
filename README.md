# HANDWRITING RECOGNITION - GERMAN ALPHABET
## Author:
Smith Gakuya

Design and Code overview
--
The StrokeParser is a notepad that is aimed to recognize characters present in the German and
English alphabets.
It works by following mouse/stylus movements and takes note of changes in direction and mouse velocity.
Points where the mouse slows down significantly are recorded as vertices.
Currently it works by dividing each drawn object into smaller segments or dots. The order in which the
segments are drawn matters. After a new letter is drawn, which is done when there is a significant enough
difference between the right most X value of the drawspace and the left most X value of the new object, the 
previous letter's sub-segments are deciphered which determines which method that letter will be taken into.
There are currently 16 sub categories for the 60 letters which allows for a maximum of 7 checks before getting
to the most probable letter. 
KEY:
	C - Curve
	S - Straight line
	D - Dot
The categories are :
    C - C, O, U, c, e, l, o
    CC - e*, m , w, y
    CCC - S, s
    CD - j
    CDD - Ö, ö , UE
    CS - Q, a, d, t, u, f
    CSDD - ae, ue
    CSS - G, q
    SC - D, J, P, b, h, n, p, r
    SCC - B, ß
    SCS - R
    SD - i
    SS - L, T, V, X, Y, v, x
    SSS - A, F, H, I, K, N, V, Y, Z, k, z
    SSSDD - AE
    SSSS - E, M, W
The letter is then printed in the console. If the letter doesn't match the checking criteria, it prints an empty string.
In debug mode instead of the letter, you print the details of each segment to the console e.g. SCC.

State of project
--
__Current Features__

Allows recognition of certain letters of the English and German alphabet.
Allows clearing of the screen and console simultaneously.

__Bugs__

Depending on the speed at which you write, some letters might be falsely deciphered: when written slowly the current speed 
	check deciphers all coded letters correctly. A more sensitive mouse speed checker resulted in more unnecessary vertices.
Some letters are inconsistent due to the curves in the segments, thus they aren't currently deciphered by the program.
	These letters are : ß, Q, t, f, Ö, ö, e, g, w, y, m, w, O, l, o.
	This happens since I have not figured out a way to properly combine curved segments.
Currently doesn't separate words.

__To-Do List__

Need to figure out a way to properly combined curved segments.   
Need to generalize code to cover a broader spectrum of handwritings i.e. order of segments shouldn't matter.   
Need to separate words
Need to cross check correctness of outputted words with a dictionary
Need to resize the writing panel to allow writing more words without clearing
Implement some AI to account for differences in handwriting and ways of writing

Basic user guide
--
For debug mode:     
  1) _Go to mainwindow.cpp and change debugMode variable to true._   
  2) _Run program._      
  3) _Use top panel with red and blue writing lines to write characters._      
  4) _Scroll left to have more space to write(also only allows writing from left to right)._      
  5) _Information on each vertex will be printed to the console with the following format:_       
> x-coord y-coord timestamp delta-x * delta-y * delta-time * displacement one side / displacement other side

For regular mode:    
  1) _Go to mainwindow.cpp and change debugMode variable to false(!true)._  
  2) _Run program._    
  3) _Use top panel with red and blue writing lines to write characters._     
  4) _Scroll left to have more space to write(also only allows writing from left to right)._     
  5) _Each read letter will be output to console._ 

