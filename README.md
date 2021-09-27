# 03_BullCowGame

Simple word-guessing game built in Unreal Engine with C++ through GameDev.TV's "Unreal Engine C++ Developer's Course". I added additional features, such as a level counter (will introduce level progression at a later time) and a "Hangman"-style display of bulls to make the UX more enjoyable and the game overall slightly easier. 

![Title screen shot](/Screenshots/title-screen-screenshot.PNG)

Player has lives according to the length of the hidden word. Guessing correctly will generate a new hidden word. Guessing incorrectly due to incorrect length or repeated letters will prompt player to guess again. Guessing incorrectly but correct length and no repeated letters will decrement a life, but will show one of three things: 
1. Any correct letters in the correct position are called "Bulls", and will be displayed in the correct position.
![Bulls and Cows Displayed](/Screenshots/bulls-cows-displayed.PNG)
2. Any correct letters in the wrong position are called "Cows", and will be displayed below the bulls. 
3. No correct letters guessed (no cows or bulls) and the player will have to try again.
![No Bulls or Cows](/Screenshots/no-bulls-cows.PNG)

If the player runs out of lives, the UI will display the hidden word and prompt the player to press enter to play again.
![Game Over Screen](/Screenshots/game-over-screen.PNG)