// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
// #include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    Isograms = GetValidWords(Words);
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
    {
         return Word.Len() >= 2 && Word.Len() <= 15 && IsIsogram(Word);
    });
    CurrentLevel = 1;
    
    PrintLine(TEXT("Welcome to Bull Cow!"));
    PrintLine(TEXT("Press tab, type your guess and hit enter."));
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver) {
        CurrentLevel = 1;
        ClearScreen();
        SetupGame();
    } else {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame() {
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;
    // PrintLine(TEXT("Word: %s"), *HiddenWord); // Debug Line
    PrintLine(TEXT("Level: %i"), CurrentLevel);
    PrintLine(TEXT("The word's length is: %i"), HiddenWord.Len());
    PrintLine(TEXT("Lives: %i"), Lives);
}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
    CurrentLevel = 1;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    
    // Create Instructions screen/function --help
    if (Guess == HiddenWord) {
        ClearScreen();
        ++CurrentLevel;
        NextLevel();
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
        ClearScreen();
        PrintLine(TEXT("Guess: %s"), *Guess);
        if (Lives > 1) {
            PrintLine(TEXT("You lost a life.\nYou have %i lives remaining."), Lives);
        } else {
            PrintLine(TEXT("You lost a life.\nYou have %i life remaining."), Lives);
        }

        PrintLine(TEXT("Please guess again.\n"));
    } else {
        // This is loss screen; separate function may be good, but not necessary at this point
        ClearScreen();
        PrintLine(TEXT("You have zero lives left."));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount BullCowInfo = GetBullCows(Guess);
    PrintLine(TEXT("Bulls: %i | Cows: %i."), BullCowInfo.BullsCount, BullCowInfo.CowsCount);
    if (BullCowInfo.Bulls.Len() > 0) {
        PrintLine(TEXT("Bulls found: %s"), *BullCowInfo.Bulls);
    }
    if (BullCowInfo.Cows.Len() > 0) {
        PrintLine(TEXT("Cows found: %s"), *BullCowInfo.Cows);
    }

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


// this function does actual fuck all
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const {
    TArray<FString> ValidWords;

    for (FString Word : WordList) {
        if (Word.Len() <= 4 && IsIsogram(Word) && CurrentLevel <= 5) {
            ValidWords.Emplace(Word);
            break;
        }

        if (Word.Len() >= 4 && Word.Len() <= 6 && IsIsogram(Word) && CurrentLevel > 5 && CurrentLevel <= 10) {
            ValidWords.Emplace(Word);
            break;
        }

        if (Word.Len() > 6 && IsIsogram(Word) && CurrentLevel > 10) {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}


// Addt feature: for each bull, put the correctly-guessed letter in its place on screen so player can see it
// this makes it slightly easier for player to figure out the word
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const {
    FBullCowCount Count;
    FString Bulls = "";
    FString Cows = "";

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
                Count.Cows += Guess[i];
            }
        }
    }

    return Count;
}

void UBullCowCartridge::NextLevel() {
    ClearScreen();

    PrintLine(TEXT("Congrats! That's correct!\n"));
    SetupGame();
}