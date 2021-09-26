// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    // GetValidWords(Words);
    SetupGame();
    PrintLine(TEXT("Number of valid words is %i"), GetValidWords(Words).Num());
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver) {
        ClearScreen();
        SetupGame();
    } else {
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
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess) {
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
        return;
    }

    // This is loss screen; separate function may be good, but not necessary at this point
    ClearScreen();
    PrintLine(TEXT("You have zero lives left."));
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    EndGame();
    return;
}

bool UBullCowCartridge::IsIsogram(FString Word) const {
    for (int32 i = 0; i < Word.Len(); i++) {
        for (int32 j = i + 1; j < Word.Len(); j++) {
            if (Word[i] == Word[j]) {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> WordList) const {
    TArray<FString> ValidWords;

    for (FString Word : WordList) {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) {
            ValidWords.Emplace(Word);
        }
    }

    // for (int32 i = 0; i < WordList.Num(); i++) {
    //     if (WordList[i].Len() >= 4 && WordList[i].Len() <= 8) {
    //         if (IsIsogram(WordList[i])) {
    //             ValidWords.Emplace(WordList[i]);
    //         }
    //     }
    // }

    return ValidWords;
}