// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame(); // Setting Up Game
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    // if bGameOver is true ClearScreen() and SetupGame()
    // else check PlayerGuess
    if (bGameOver) {
        ClearScreen();
        SetupGame();
    } else { // checking PlayerGuess
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame() {
    HiddenWord = TEXT("cake");
    Lives = HiddenWord.Len();
    bGameOver = false;
    PrintLine(TEXT("Welcome to Bull Cow!"));
    PrintLine(TEXT("Press tab, type your guess for \n%i-letter word, then press enter."), HiddenWord.Len());
    PrintLine(TEXT("Word: %s"), *HiddenWord); // Debug Line
    PrintLine(TEXT("Lives: %i"), Lives);
}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess) {
    if (Guess == HiddenWord) {
        PrintLine(TEXT("Congrats! That's correct!"));
        EndGame();
    } else {
        PrintLine(TEXT("You've lost a life."));
        PrintLine(TEXT("Lives: %i"), --Lives);

        if (Lives > 0) {
            if (Guess.Len() != HiddenWord.Len()) {
                PrintLine(TEXT("Try again. Your guess should be %i-letters long."), HiddenWord.Len());
            }
        } else {
            PrintLine(TEXT("You have zero lives left."));
            EndGame();
        }
    }
}