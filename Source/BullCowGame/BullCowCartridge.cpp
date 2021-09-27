// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    Isograms = GetValidWords(Words);
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
    {
         return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word);
    });

    SetupGame();
    PrintLine(TEXT("Number of valid words is %i"), Isograms.Num());
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver) {
        ClearScreen();
        SetupGame();
    } else {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame() {
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    PrintLine(TEXT("Welcome to Bull Cow!"));
    PrintLine(TEXT("Press tab, type your guess for \n%i-letter word, then press enter."), HiddenWord.Len());
    PrintLine(TEXT("Word: %s"), *HiddenWord); // Debug Line
    PrintLine(TEXT("Lives: %i"), Lives);
}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    if (Guess == HiddenWord) {
        ClearScreen();
        PrintLine(TEXT("Congrats! That's correct!"));
        EndGame();
        return;
    } 
    
    if (Guess.Len() != HiddenWord.Len()) {
        PrintLine(TEXT("Your guess should be %i-letters long.\nPlease try again."), HiddenWord.Len());
        return;
    }
    
    // Check if IsIsogram
    if(!IsIsogram(Guess)) {
        PrintLine(TEXT("There are no repeating letters in the hidden word.\nPlease try again."));
        return;
    }


    --Lives;
    if (Lives > 0) {
        if (Lives > 1) {
            PrintLine(TEXT("You lost a life.\nYou have %i lives remaining."), Lives);
        } else {
            PrintLine(TEXT("You lost a life.\nYou have %i life remaining."), Lives);
        }

        PrintLine(TEXT("Please guess again."));
        // return;
    } else {
        // This is loss screen; separate function may be good, but not necessary at this point
        ClearScreen();
        PrintLine(TEXT("You have zero lives left."));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
    }

    FBullCowCount BullCowInfo = GetBullCows(Guess);
    PrintLine(TEXT("Bulls: %i -- Cows: %i."), BullCowInfo.BullsCount, BullCowInfo.CowsCount);
    if (BullCowInfo.Bulls.Len() > 0) {
        PrintLine(TEXT("Bulls found: %s"), *BullCowInfo.Bulls);
    }
    // if (BullCowInfo.Cows.Len() > 0) {
    //     PrintLine(TEXT("Cows found: %s"), *BullCowInfo.Cows);
    // }

}

bool UBullCowCartridge::IsIsogram(const FString& Word) {
    for (int32 i = 0; i < Word.Len(); i++) {
        for (int32 j = i + 1; j < Word.Len(); j++) {
            if (Word[i] == Word[j]) {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const {
    TArray<FString> ValidWords;

    for (FString Word : WordList) {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}


// Addt feature: for each bull, put the correctly-guessed letter in its place on screen so player can see it
// this makes it slightly easier for player to figure out the word
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const {
    FBullCowCount Count;

    for (int32 i = 0; i < Guess.Len(); i++) {
        if (Guess[i] == HiddenWord[i]) {
            Count.BullsCount++;
            Count.Bulls += Guess[i];
            continue;
        } else {
            Count.Bulls += "_";
        }

        for (int32 j = 0; j < HiddenWord.Len(); j++) {
            if (Guess[i] == HiddenWord[j]) {
                Count.CowsCount++;
                // Count.Cows += Guess[j];
                break;
            }
        }
    }

    return Count;
}