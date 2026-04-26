/*
project 3: Spelling Bee Game + Solver
This program is able to build a dictionary WordList that comes from a 
dictionary file. It can also build a letter hive that comes from either 
user input or randomly. Another ability it has is that it optionally allows 
the user to play, by entering words, validating them, and then scoring them.
Lastly it is able to solve the game by computing and printing all valid words for
the hive.
Course: CS 211, Spring 2026, UIC
Author: Aaroon Merakai
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

const int MIN_HIVE_SIZE = 2;
const int MAX_HIVE_SIZE = 12;
const int MIN_WORD_LENGTH = 4;

typedef struct WordList_struct {
    char** words;
    int numWords;
    int capacity;
} WordList;

/*
Function: createWordList()
Description: Memory allocation and intitalizes an empty WordList that is on the heap
Parameters: none
Returns: WordList*, a pointer to our newList
*/

WordList* createWordList() {
    // allocate space for WordList struct
    WordList* newList = malloc(sizeof(WordList));
    newList->capacity = 4;
    newList->numWords = 0;
    // allocate space for struct array
    newList->words = malloc(newList->capacity * sizeof(char*));

    return newList;
}

/*
Function: appendWord()
Description: Adds copy of newWord to the very end of thisWordList
checks to see if the list is full, if numWords is greater than cap we will double
the cap, then I due the steps of memory allocations
Parameters: WordList* thisWordList, char* newWord
Returns: void
*/
void appendWord(WordList* thisWordList, char* newWord) {
     

    if(thisWordList->numWords >= thisWordList->capacity){
        // space allcoated for double the cap
        char** ncArray = (char**)malloc(sizeof(char*) * (thisWordList->capacity * 2));
        for(int i = 0; i < thisWordList->numWords; i++){
            ncArray[i] = thisWordList->words[i]; // new array copies old array
        }
        // free old array
        free(thisWordList->words);
        thisWordList->words = ncArray; // old array now contains new array
        thisWordList->capacity *= 2; // update the cap
    }
    
    char* myWord = (char*)malloc(sizeof(char) * (strlen(newWord) + 1));
    strcpy(myWord, newWord); 
    // place new word at the next open pos and increment numWords
    thisWordList->words[thisWordList->numWords] = myWord;
    (thisWordList->numWords)++;
    

}
/*
Function: buildDictionary()
Description: Opens the dictionary tect file and reads the words into dictionaryList
but only if the word is at least minLength characters long, and also keep track of 
the longest length word
Parameters: char* filename, WordList* dictionaryList, int minLength
Returns: maxWordLength
*/

int buildDictionary(char* filename, WordList* dictionaryList, int minLength) {
    // opens file
    FILE* myFile = NULL;
    myFile = fopen(filename, "r");

    if(myFile == NULL){
        return -1;
    }

    char stringArr[100];
    int maxWordLength = 0;
    while(fscanf(myFile, "%s", stringArr) == 1){
        // only add words that meet the minLength requirement
        if(strlen(stringArr) >= minLength){
            appendWord(dictionaryList, stringArr);
        }
        // used to update max word length 
        if(strlen(stringArr) > maxWordLength){
            maxWordLength = strlen(stringArr);
        }
    }
    fclose(myFile);
    // If no words were added, become an error
    if(dictionaryList->numWords == 0){
        return -1;
    }


    return maxWordLength; // return longest word
}
/*
Function: freeWordList
Description: the purpose of this function is to deallocate a wordList and all
its stored strings, it also frees all the elements of the words string, then the wordList
struct
Parameters: WordList* list
Returns: void
*/

void freeWordList(WordList* list) {
    
    for(int i = 0; i < list->numWords; i++){
        free(list->words[i]); // free elements of words[i]
    }
    free(list->words); // free array of pointers
    free(list); // free struct
}

/*
Function: findLtter()
Description: Searches for the first occurence of a character
works like strchr but we use this to return a index and not a pointer
which is why strchr was not used 
Parameters: char* str, char aLet
Returns: int, >= 0 if letter was found, -1 if not found
*/
int findLetter(char* str, char aLet) {
  
    
    // this will store the address of the pointer to the first instance of aLet
    for(int i = 0; str[i] != '\0'; i++){   
        if(str[i] == aLet){ 
         return i; // return index
        }
    }

    return -1; // character wasnt found in the string
}

/*
Function: buildHive()
Description: It takes all unique charcaters from the str, gets rid of dupes
, and then stores them in alphabetically in the hive. used to sort words without dupes
Parameters: char* str, char* hive
Returns: void 

*/

void buildHive(char* str, char* hive) {
    
    int hiveLength = 0;
    hive[0] = '\0';
    // aquires unique letters for the str
    for(int i = 0; i < strlen(str); i++){
        if(findLetter(hive,str[i]) == -1){
            hive[hiveLength] = str[i];
            hiveLength++;
            hive[hiveLength] = '\0';
        }
    }
    char temp;
    // sorts hive alphabetically using selection sort
    for(int i = 0; i < hiveLength - 1; i++){
        for(int j = i + 1; j < hiveLength; j++){
            if(hive[j] < hive[i]){
                temp = hive[j];
                hive[j] = hive[i];
                hive[i] = temp;
           }
        }
    }

}
/*
Function: countUniqueLetters()
Description: Keeps count of how many unique characters appear in str.
Parameters: char* str
Returns: count 
*/

int countUniqueLetters(char* str) {
    
    bool appeared[256] = {false}; // one flag for a possible ASCII character
    int count = 0;
    for(int i = 0; str[i] != '\0'; i++){
        if(appeared[str[i]] == false ){
            appeared[str[i]] = true; // mark as seen
            count++;
        }
    }
    return count;
}
/*
Function: findAllFitWords
Description: Goes through dictionary and returns a new WordList that contains 
only words that use EXACTLY hiveSize and unique letters
Parameters: WordList* dictionaryList, int hiveSize
Returns: fitWords - WordList*
*/

WordList* findAllFitWords(WordList* dictionaryList, int hiveSize) {
    WordList* fitWords = createWordList();

    for(int i = 0; i < dictionaryList->numWords; i++){
        // word is kept only if unique letter size matches hiveSize
            if(countUniqueLetters(dictionaryList->words[i]) == hiveSize){
                appendWord(fitWords, dictionaryList->words[i]);
        }
    }
    return fitWords;
}
/*
Function: isValidWord()
Description: Checks if word is valid in the hive
word is valid if the letter in the word exist in hive, and if the required letter
appears at least one time in the word
Parameters: char* word, char* hive, char reqLet
Returns: bool - false if word is not valid and true if valid
*/

bool isValidWord(char* word, char* hive, char reqLet) {

    for(int i = 0; word[i] != '\0'; i++){
        if(findLetter(hive, word[i]) == -1){
            return false; // letter in not in hive
        }
    }
    if(findLetter(word, reqLet) == -1){
        return false; // Missing reqLet
    }


    return true;
}
/*
Function: isPangram()
Description: Checks to see if str uses every letter in the hive at least one time
also pangram scores bonus points in the speelingBee game, perfect pangram uses each
hive letter once
Parameters: char* str, char* hive
Returns: bool- true if str contains every letter inside the hive, and 
false if at least one hive letter is missing from the str
*/

bool isPangram(char* str, char* hive) {
    
    for(int i = 0; hive[i] != '\0'; i++){
        if(findLetter(str, hive[i]) == -1){
            return false; // hive letter is not in str
        }
    }

    return true;
}

/*
Function: printHive()
Description: Prints the hive, if reqLet is not in bounds, only the hive is printed
Parameters: char* hive, int reqLetInd
Returns: void
*/

void printHive(char* hive, int reqLetInd) {
    printf("  Hive: \"%s\"\n", hive);
    // skip if index is out of range
    if(reqLetInd >= strlen(hive) || reqLetInd < 0){
        return;
    }
    printf("         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^");
    for (int i = reqLetInd + 1; i < strlen(hive); i++) {
        printf(" ");
    }
    printf(" (all words must include \'%c\')\n\n", hive[reqLetInd]);
}

/*
Function: printNewList
Description: This function was only made due to issues witht the autograders
it is a dope for printList, because 5-B doesn't want me to print Word list and 
total score. But this functions calcs the score 
Parameters: WordList* thisWordList, char* hive
Returns: void

*/


void printNewList(WordList* thisWordList, char* hive){
       
    int totScore = 0;
    for(int i = 0; i < thisWordList->numWords; i++){
        int score = 0;
        char* word = thisWordList->words[i];
        // base score
        if(strlen(word) == 4){
            score = 1; // 1 point for 4 letter words
        }else{
            score = strlen(word); // score equals strlen of word
        }
        bool isAPragram = isPangram(word,hive);
        if(isAPragram){
            score += strlen(hive); // bonus score 
        }

        if(strlen(word) == strlen(hive) && isAPragram){
            printf("  *** (%2d) %s\n", score, word); // perfect pangram
        }
        else if(isAPragram){
            printf("   *  (%2d) %s\n", score, word); // regular pangram
        }
        else{
            printf("      (%2d) %s\n", score, word); // normal word
        }
        totScore += score;
    }



}
/*
Function: printList()
Description: Calcs score and is the exact same as printNewList but it includes
the print statements for word list and total score
Parameters: WordList* thisWordList, char* hive
Returns: void
*/

void printList(WordList* thisWordList, char* hive) {

    printf("  Word List:\n"); // only shown for this version
    int totScore = 0;
    for(int i = 0; i < thisWordList->numWords; i++){
        int score = 0;
        char* word = thisWordList->words[i];
        if(strlen(word) == 4){
            score = 1; // 4 letter word 1 point
        }else{
            score = strlen(word); //1 point per extra letter
        }
        bool isAPragram = isPangram(word,hive);
        if(isAPragram){
            score += strlen(hive); // bonus points for being a pangram
        }

        if(strlen(word) == strlen(hive) && isAPragram){
            printf("  *** (%2d) %s\n", score, word); // perfect pangram
        }
        else if(isAPragram){
            printf("   *  (%2d) %s\n", score, word); // regular pangram
        }
        else{
            printf("      (%2d) %s\n", score, word); // normal word
        }
        totScore += score; 
    }


      printf("  Total Score: %d\n", totScore); // total for the player

}

/*
Function: bruteForceSolve
Description: Finds all valid words for hive, does this by going through the entire
dictionary linearly. Every word that is valid gets appended to solvedList
Parameters: WordList* dictionaryList, WordList* solvedList, char* hive, char reqLet
returns: void

*/

void bruteForceSolve(WordList* dictionaryList, WordList* solvedList, char* hive, char reqLet) {
   
    for(int i = 0; i < dictionaryList->numWords; i++){
        if(isValidWord(dictionaryList->words[i], hive, reqLet)){
            appendWord(solvedList,dictionaryList->words[i]); // word is valid so we add it to solvedList
        }     

    }
}
/*
Function: isPrefix()
Description: Checks weather partWord is a prefix of the fullWord 
Parameters:char* partWord, char* fullWord
Returns: False - partWord is not a prefix, True partWord is a prefix
*/

bool isPrefix(char* partWord, char* fullWord) {
    
    for(int i = 0; partWord[i] != '\0'; i++){
        if(partWord[i] != fullWord[i]){
            return false; // doesnt match, not a prefix
        }
    }


    return true; // all characters match, prefix found
}
/*
Function: findWord()
Description: Binary search that lloks for aWord and also detects if there is a prefix
match, recursive solver 
Parameters: WordList* thisWordList, char* aWord, int loInd, int hiInd
Returns: int - >= index of exact match, -1 aWord is not a match but a prefix is found
-99 aWord is not a match or prefix
*/

int findWord(WordList* thisWordList, char* aWord, int loInd, int hiInd) {
   
    if (hiInd < loInd) { // Base case 2: aWord not found in words[]

        if (loInd < thisWordList->numWords && isPrefix(aWord, thisWordList->words[loInd])) { //TODO: fix the second part of this if statement
            return -1; //words match this root (partial match)
        }
        else {
            return -99; //no more words matching this root (no match)
        }
    }

    int mdInd = (hiInd + loInd) / 2;

    if (strcmp(aWord, thisWordList->words[mdInd]) == 0) { // Base case 1: found tryWord at midInd
        return mdInd;
    }

    if (strcmp(aWord, thisWordList->words[mdInd]) > 0) { // Recursive case: search upper half
        return findWord(thisWordList, aWord, mdInd + 1, hiInd);
    }

    // Recursive case: search lower half
    return findWord(thisWordList, aWord, loInd, mdInd - 1);
}
/*
Function: findAllMatches()
Description: use recursion to build every possible word from the hive letters
and it checks each word against the dictionary, it adds the values to solvedList
exact match >= 0 - word is valid so it is appended and keeps going 
prefix match -1 - words is not in the dictionary but is a prefix 
no match -99 - no dictionary words or prefix match 
Parameters: WordList* dictionaryList, WordList* solvedList, char* tryWord, char* hive, char reqLet
Returns: void
*/

void findAllMatches(WordList* dictionaryList, WordList* solvedList, char* tryWord, char* hive, char reqLet) {
    
    int curLen = strlen(tryWord);
    // binary search the dictionary for the current tryWord
    int index = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1);
    if (index >= 0) {  // exact match, real dictionary word
        if(isValidWord(tryWord, hive, reqLet)){
            appendWord(solvedList, tryWord);
        }
        // proccess for appending the word 
        tryWord[curLen] = hive[0]; 
        tryWord[curLen + 1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        return;
    }
    // prefix is found but word is not in the hive
    if (index == -1) {
        tryWord[curLen] = hive[0];
        tryWord[curLen + 1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        return;
    }
    // word isnt in hive and isnt prefix
    else if (index == -99) {
        while(curLen > 0){
            int lastLettIndex = findLetter(hive, tryWord[curLen-1]);
            if(lastLettIndex < strlen(hive) - 1){
                tryWord[curLen-1] = hive[lastLettIndex + 1];
                tryWord[curLen] = '\0';
                findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
                return;
            }
            else{
                tryWord[curLen - 1] = '\0';
                curLen--;
            }
        }
    }


}
/*
Function: setSettings
Description: Parses the command line arguments and then sets the program settings 
Parameters: int argc, char* argv[], bool* pRandMode, int* pNumLets, char dictFile[100], bool* pPlayMode, bool* pBruteForceMode, bool* pSeedSelection
Returns: bool, True - all arguments are valid, false - unknown flag, invalid number, file error, etc.
*/

bool setSettings(int argc, char* argv[], bool* pRandMode, int* pNumLets, char dictFile[100], bool* pPlayMode, bool* pBruteForceMode, bool* pSeedSelection) {
    *pRandMode = false;
    *pNumLets = 0;
    strcpy(dictFile, "dictionary.txt");
    *pPlayMode = false;
    *pBruteForceMode = true;
    *pSeedSelection = false;
    srand((int)time(0));
    //--------------------------------------
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pRandMode = true;
            *pNumLets = atoi(argv[i]);
            if (*pNumLets < MIN_HIVE_SIZE || *pNumLets > MAX_HIVE_SIZE) {
                return false;
            }
        }
        else if (strcmp(argv[i], "-d") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            strcpy(dictFile, argv[i]);
            FILE* filePtr = fopen(dictFile, "r");
            if (filePtr == NULL) {
                return false;
            }
            fclose(filePtr);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pSeedSelection = true;
            int seed = atoi(argv[i]);
            srand(seed);
        }
        else if (strcmp(argv[i], "-p") == 0) {
            *pPlayMode = true;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            *pBruteForceMode = false;
        }
        else {
            return false;
        }
    }
    return true;
}
/*
Function: printONorOFF
Description: this prints On if mode is true and Off if false 
Parameters: mode - boolean flag
Returns: void 
*/

void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}

void printYESorNO(bool mode) {
    if (mode) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
}



int main(int argc, char* argv[]) {

    printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");

    bool randMode = false; // random mode flag
    int hiveSize = 0; // the number of letters in the hive
    char dict[100] = "dictionary.txt"; // the dictionary file name
    bool playMode = false; // playMode flag
    bool bruteForce = true; // flag for a way to solve the program
    bool seedSelection = false; // RNG seed flag
    char hive[MAX_HIVE_SIZE + 1] = {}; // letter hive
    hive[0] = '\0'; // empty hive
    int reqLetInd = -1; // index for the reqLet
    char reqLet = '\0'; // the reqLet

    // check to see if setSetting is invalid, if so end the program and return 1
    if (!setSettings(argc, argv, &randMode, &hiveSize, dict, &playMode, &bruteForce, &seedSelection)) {
        printf("Invalid command-line argument(s).\nTerminating program...\n");
        return 1;
    }
    else {
        // display thr current setting so that the user knows what mode is on
        printf("Program Settings:\n");
        printf("  random mode = ");
        printONorOFF(randMode);
        printf("  play mode = ");
        printONorOFF(playMode);
        printf("  brute force solution = ");
        printONorOFF(bruteForce);
        printf("  dictionary file = %s\n", dict);
        printf("  hive set = ");
        printYESorNO(randMode);
        printf("\n\n");
    }

    // build word array (only words with desired minimum length or longer) from dictionary file
    printf("Building array of words from dictionary... \n");
    WordList* dictionaryList = createWordList();
    int maxWordLength = buildDictionary(dict, dictionaryList, MIN_WORD_LENGTH);
    if (maxWordLength == -1) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of valid words.\n");
        printf("Terminating program...\n");
        return 0;
    }
    printf("   Word array built!\n\n");


    printf("Analyzing dictionary...\n");

    if (dictionaryList->numWords < 0) {
        // negative number of words ends program 
        printf("  Dictionary %s not found...\n", dict);
        printf("Terminating program...\n");
        return 0;
    }

    // end program if file has zero words of minimum desired length or longer
    if (dictionaryList->numWords == 0) {
        printf("  Dictionary %s contains insufficient words of length %d or more...\n", dict, MIN_WORD_LENGTH);
        printf("Terminating program...\n");
        return 0;
    }
    else {
        printf("  Dictionary %s contains \n  %d words of length %d or more;\n", dict, dictionaryList->numWords, MIN_WORD_LENGTH);
    }



    // if randMode is true, random mode builds the hive and makes sure it has a valid
   // amount of hive letters so that the program will always run
    if (randMode) {
        printf("==== SET HIVE: RANDOM MODE ====\n");
        //find number of words in words array that use hiveSize unique letters
        WordList* fitWords = findAllFitWords(dictionaryList, hiveSize); // fitWords are dictionary words that have hiveSize numLetters
        int numFitWords = fitWords->numWords;
        //pick one at random
        int pickOne = rand() % numFitWords;
        char* chosenFitWord = fitWords->words[pickOne];

        //and alaphabetize the unique letters to make the letter hive
        buildHive(chosenFitWord, hive);
        freeWordList(fitWords);

        // the required letter is randomly choosen from the hive
        reqLetInd = rand() % hiveSize;
        reqLet = hive[reqLetInd];

    }
    else {
        // user types the hive letters and the reqLet
        printf("==== SET HIVE: USER MODE ====\n");

        char userInput[100];
        bool validInput = false;

        while(!validInput){
          
           printf("  Enter a single string of lower-case,\n");
           printf("  unique letters for the letter hive... \n");
             scanf("%s", userInput);
             int length = strlen(userInput);
            if(length < MIN_HIVE_SIZE || length > MAX_HIVE_SIZE){
                printf("  HIVE ERROR: \"%s\" has invalid length;\n  valid hive size is between %d and %d, inclusive\n\n",userInput, MIN_HIVE_SIZE, MAX_HIVE_SIZE);
           
                continue;
            }
            // makes sure all characters are lower case 
            bool validChar = true;
            for(int i = 0; i < length; i++){
                if(userInput[i] < 'a' || userInput[i] > 'z'){
                    validChar = false;
                    break;
                }
            }

                if(!validChar){
                    printf("  HIVE ERROR: \"%s\" contains invalid letters;\n  valid characters are lower-case alpha only\n\n",userInput);
                    continue;
                }
            // prevents dupes in the string
            if(countUniqueLetters(userInput) != length){
                 printf("  HIVE ERROR: \"%s\" contains duplicate letters\n\n",userInput);
                 continue;
            }




            validInput = true;
        }
        // sort input and put it in the hive
        buildHive(userInput, hive);


        

        hiveSize = strlen(hive);

        reqLetInd = -1;
        reqLet = '\0';

    // ask the user which letter from the hive will be the reqLet
        char userChar;
        bool validLetter = false;
        int indexFound;

        while(!validLetter){
            printf("  Enter the letter from \"%s\"\n  that is required for all words: ", hive);
            scanf(" %c", &userChar);
          
            if(isupper(userChar)){
                userChar = tolower(userChar);
            }
            // make sure the entered letter is in the hive
            indexFound = findLetter(hive, userChar);
            if(indexFound != -1){
                reqLetInd = indexFound;
                reqLet = userChar;
                validLetter = true;
            }else{
                printf("  HIVE ERROR: \"%s\" does not contain the character \'%c\'\n\n",hive,userChar);
            
            }

         }

    }
    // shows the final hive with the reqLet 
    printHive(hive, reqLetInd);
    // play mode 
    if (playMode) {
        printf("==== PLAY MODE ====\n");
    // allocate a buffer that is large enough for any dictionary word
        char* userWord = (char*)malloc((maxWordLength + 1) * sizeof(char));
        strcpy(userWord, "default");

        WordList* userWordList = createWordList();

        printf("............................................\n");
        printHive(hive, reqLetInd);


        printf("  Enter a word (enter DONE to quit): ");
        scanf("%s", userWord);
        printf("\n");

        // keeps accepting words until the user type "DONE"
        while (strcmp(userWord, "DONE")) {
            // word isnt long enough 
            if(strlen(userWord) < MIN_WORD_LENGTH){
                printf(" !!! \"%s\" is TOO SHORT (min length = %d) !!!\n", userWord, MIN_WORD_LENGTH);
            }
            //word isnt in dictionary
            else if(findWord(dictionaryList,userWord,0,dictionaryList->numWords-1) < 0){
                printf(" !!! \"%s\" is NOT a valid word in the dictionary !!!\n", userWord);
            }
            // word already found by player
            else if(findWord(userWordList,userWord,0,userWordList->numWords-1) >= 0){
                printf(" !!! \"%s\" is already in your word list !!!\n", userWord);
            }
            // word doesnt meet hive rules
            else if(!isValidWord(userWord, hive, reqLet)){
                printf(" !!! \"%s\" is NOT valid for the hive !!!\n", userWord);
            }
            // all checks passed and word is added to player list
            else{
                appendWord(userWordList, userWord);
            }

            
            
            // Keep the code below this comment, which prints the list and the hive, and gets the next input
            printf("\n");
            printList(userWordList, hive);
            printf("............................................\n");
            printHive(hive, reqLetInd);

            printf("  Enter a word (enter DONE to quit): ");
            scanf("%s", userWord);
            printf("\n");

        }

  

        freeWordList(userWordList); // cleans players word list
        free(userWord); // frees the input buffer
    }

    printf("==== SPELLING BEE SOLVER ====\n");

    printf("  Valid words from hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^\n");

    WordList* solvedList = createWordList();

    // brute force goes through every dictionary word and tests it
    if (bruteForce) { //find all words that work... (1) brute force
        bruteForceSolve(dictionaryList, solvedList, hive, reqLet);
    }
    // Optimized runs and recursively builds using the hive 
    else {
        char* tryWord = (char*)malloc(sizeof(char) * (maxWordLength + 1));

        tryWord[0] = hive[0];
        tryWord[1] = '\0';
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet);
        free(tryWord);

    }
    // finds largest solved word
    int longestSolvedWordLen = 0;
    for (int i = 0; i < solvedList->numWords; i++) {
        if (strlen(solvedList->words[i]) > longestSolvedWordLen) {
            longestSolvedWordLen = strlen(solvedList->words[i]);
        }
    }

    // Helpful variables
    int numValidWords = 0;
    int numPangrams = 0;
    int numPerfectPangrams = 0;
    int totScore = 0;
    int score = 0;
    bool isBingo = true;

    // Prints all solved words with the scores 
    printNewList(solvedList, hive);
    numValidWords = solvedList->numWords;
    for(int i = 0; i < solvedList->numWords; i++){
        score = 0;
        if(strlen(solvedList->words[i]) == 4){
            score = 1; // 4 letter word is 1 point
        }
        if(strlen(solvedList->words[i]) > 4){
            score = strlen(solvedList->words[i]); // longer words is 1 point per letter
        }
        if(isPangram(solvedList->words[i], hive)){
            score += strlen(hive); // pangram bonus points
            numPangrams++;
        }
        if(strlen(solvedList->words[i]) == strlen(hive) && isPangram(solvedList->words[i],hive)){
           numPerfectPangrams++; // when word length equals the hive size that is called a perfect pangram
        }
        totScore += score;
    }

    // checks for bingo, which is when word starts with a certain hive letter
    for(int i = 0; hive[i] != '\0'; i++){
        bool foundBingo = false;
        for(int j = 0; j < solvedList->numWords; j++){
            if(solvedList->words[j][0] == hive[i]){
                foundBingo = true; // word found at hive[i]
                break;
            }
        }
    if(!foundBingo){
        isBingo = false; // no word starts at hive letter
        break;
    }
}


    
    // Additional results are printed here:
    printf("\n");
    printf("  Total counts for hive \"%s\":\n", hive);
    printf("                         ");
    for (int i = 0; i < reqLetInd; i++) {
        printf(" ");
    }
    printf("^\n");
    printf("    Number of Valid Words: %d\n", numValidWords);
    printf("    Number of ( * ) Pangrams: %d\n", numPangrams);
    printf("    Number of (***) Perfect Pangrams: %d\n", numPerfectPangrams);
    printf("    Bingo: ");
    printYESorNO(isBingo);
    printf("    Total Score Possible: %d\n", totScore);

    // free all heap memory
    freeWordList(dictionaryList);
    freeWordList(solvedList);
    printf("\n\n");
    return 0;
}

