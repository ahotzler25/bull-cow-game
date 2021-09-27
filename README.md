# 03_BullCowGame

Simple word-guessing game built in Unreal Engine with C++ through GameDev.TV's "Unreal Engine C++ Developer's Course". 

![Title screen shot](/Screenshots/title-screen-screenshot.PNG)

Player has lives according to the length of the hidden word. Guessing correctly will generate a new hidden word. Guessing incorrectly due to incorrect length or repeated letters will prompt player to guess again. Guessing incorrectly but correct length and no repeated letters will decrement a life, but will show one of three things: 1. Any correct letters in the correct position are called "Bulls", and will be displayed in the correct position. 2. Any correct letters in the wrong position are called "Cows", and will be displayed below the bulls. 3. No correct letters guessed (no cows or bulls) and the player will have to try again. 