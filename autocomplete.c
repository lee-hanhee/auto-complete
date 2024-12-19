#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include "autocomplete.h"

// HELPER FUNCTIONS -------------------------------------------------------------------------------------------------------------------------------------------------------------
int binary_search_deluxe_left(term *arr, int sz, char *target)
{
    // Purpose: Take in a sorted list of integers and a target, and returns both the first index where target appears.
    
    if(strlen(target) == 0){
        return -1;
    }

    int left = 0;
    int right = sz - 1;
    while (left <= right){
        int mid = (left + right) / 2; // casts into a integer

        // Making term an char * instead of an array. 
        char *p_term = arr[mid].term;
        int cmp_str = strncmp(p_term, target, strlen(target)); // Comparing the target string with all the terms in the list.

        // Case 1 cmp_str < 0: arr->term[mid] is lexicographically less than the target. 
        if(cmp_str < 0){
            left = mid + 1;
        }
        
        // Case 2 & 3 cmp_str = 0: two strings are equals | cmp_str > 0: arr->term[mid] is lexicographically greater than the target. 
        else{
            right = mid - 1;
        }
    }
    char *p_term = arr[left].term;
    if (left < sz && strncmp(p_term, target, strlen(target)) == 0){ // DOUBLE CHECK THE SECOND CONDITION
        return left;
    }
    return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int binary_search_deluxe_right(term *arr, int sz, char *target) // THIS IS WRONG, ITS RETURNING TOQ, not TOR
{
    // Purpose: Take in a sorted list of integers and a target, and returns both the last index where target appears.

    if(strlen(target) == 0){
        return -1;
    }

    int left = 0;
    int right = sz - 1;

    while (left <= right){
        int mid = (left + right) / 2; // casts into a integer

        // Making term an char * instead of an array. 
        char *p_term = arr[mid].term;
        int cmp_str = strncmp(p_term, target, strlen(target)); // Comparing the target string with all the terms in the list.

        // Case 1 cmp_str < 0: arr->term[mid] is lexicographically less than the second. 
        if (cmp_str < 0){
            left = mid + 1;
        }

        // Case 2 cmp_str = 0: two strings are equals
        else if (cmp_str == 0){
            left = mid + 1;
        }

        // Case 3 cmp_str > 0: arr->term[mid] is lexicographically greater than the second. 
        else if (cmp_str > 0){
            right = mid - 1;
        }
    }

    char *p_term = arr[right].term;
    if (right < sz && strncmp(p_term, target, strlen(target)) == 0){ // DOUBLE CHECK THE SECOND CONDITION
        return right;
    }
    return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int comparator_1(const void *p_ele_1, const void *p_ele_2)
{
    // Return value of the function will effect the sorting order. 
        // < 0 if p_ele_1 should go before p_ele_2. 
        // 0 if p_ele_1 is equivalent to p_ele_2.
        // > 0 if p_ele_1 should go after p_ele_2.
    // Note: The above is referring to the values pointed to by x and y. 

    // 1. Cast the pointer to the type we are working with and derference the pointer. 
    const char *ele_1 = ((term *)p_ele_1)->term; 
    const char *ele_2 = ((term *)p_ele_2)->term;

    // 2. Return < 0, 0, or >0
    return strcmp(ele_1, ele_2);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int comparator_2(const void *p_ele_1, const void *p_ele_2)
{
    const double ele_1 = ((term *)p_ele_1)->weight;
    const double ele_2 = ((term *)p_ele_2)->weight;

    if (ele_1 < ele_2){
        return 1; 
    }
    else if (ele_1 > ele_2){
        return -1;
    }
    return 0;
}
// read_in_terms -----------------------------------------------------------------------------------------------------------------------------------------------------------------

void read_in_terms(term **terms, int *pnterms, char *filename)
{
    /* Parameters
    terms: Takes in a pointer to a pointer to term, which will hold all the terms from cities.txt.
    pnterms: A pointer to an int that stores the number of terms in pnterms from cities.txt.
    filename: The name of a file that is formatted like cities.txt.
    */ 
    
    // 0. Read the file 
    FILE *p_f = fopen(filename, "r"); // Reading and opening the file and putting it in a pointer f_p. 

    // 0.5 Check if p_f is NULL (ie. bad filename)
    if(p_f == NULL){

        exit(1);
    }

    // 1. Storing the number of terms into *pnterms (ie. this is the first line of the cities.txt)
    char line[200]; // Allocating the max length of each line based on the structure.
    *pnterms = atoi(fgets(line, sizeof(line), p_f));

    // 2. Allocates memory for all the terms in the file in *terms
    *terms = (term *)malloc((*pnterms) * sizeof(term));

    // 2.5 Check if *terms is valid
    if(*terms == NULL){
        fclose(p_f);
        exit(1);
    }
    
    // 3. Place terms in the block pointed to by *terms using a for-loop again. 
    for(int i = 0; i < *pnterms; i++){
        fgets(line, sizeof(line), p_f); // Reading the file and putting the term into line. 

        int j = 0;
        // Part 1: Index j is going up until the number (ie. weight)
        while(line[j] == ' '){
            j++;
        }

        // Part 2: Index j is going up until after the number is done. 
            // Note: An alternative choice would be to use atof by getting the index, but this is fine.
        double res = 0.0;
        while(line[j] != ' ' && isdigit(line[j]) > 0){
            res = (res * 10) + (line[j] - '0');
            j++;
        }
        (*terms)[i].weight = res;

        // Part 3: Index j is going up until the term (ie. character string)
        while(line[j] == '\t'){
            j++;
        }

        // Part 4: Index j is going up until \n. 
        int k = 0;
        while(line[j] != '\n'){ // Check until \n since each line has a \n.
            (*terms)[i].term[k] = line[j]; // or (*terms)[i].term[j]
            j++;
            k++;
        }
        (*terms)[i].term[k] = '\0';
        // All non-latin characters in the city.txt file were automatically converted to latin characters, which is fine. 
    }

    // 4. Sort the terms using qsort in non-descending (ie. ascending) lexicographic order. 
        // Note: Don't sort the weights yet. 
    qsort(*terms, *pnterms, sizeof(term), comparator_1);
    fclose(p_f);
}

// lowest_match -----------------------------------------------------------------------------------------------------------------------------------------------------------------

int lowest_match(term *terms, int nterms, char *substr)
{
    // Returns the index in terms of the first term in lexicographic ordering that matches the string substr.
    // Change the binary search to include strings. 
    // The function will be in O(log(nterms)) because binary search is O(log(n)) by definition. 
    int lowest_index = binary_search_deluxe_left(terms, nterms, substr);
    return lowest_index;
}

// highest_match -----------------------------------------------------------------------------------------------------------------------------------------------------------------

int highest_match(term *terms, int nterms, char *substr)
{
    // Returns the index in terms of the last term in lexicographic order that matches the string substr.
    // Change the binary search to include strings. 
    // The function will be in O(log(nterms)) because binary search is O(log(n)) by definition. 
    int highest_index = binary_search_deluxe_right(terms, nterms, substr);
    return highest_index;
}

// autocomplete -----------------------------------------------------------------------------------------------------------------------------------------------------------------

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    // Use all the functions to be able to do this. 
    // basically know how the helper functions work to be able to use this, so basically placing the variables in the correct places. 
    // Assume read_in_terms has been used already to sort lexicographically in increasing order).

    // 1. Find the lowest match
    int lowest_index = lowest_match(terms, nterms, substr);

    // 2. Find the highest match 
    int highest_index = highest_match(terms, nterms, substr);

    // 2.5 Check if *nanswer is vaild.
    if(highest_index == -1 && lowest_index == -1){
        exit(1); 
    }

    // 3. Calculate *n_answer
    *n_answer = highest_index - lowest_index + 1; 

    // 3.75 Allocate memory for *answer. 
    (*answer) = (term *)malloc((*n_answer) * sizeof(term));

    // 3.90 Check if *answer is valid 
    if(*answer == NULL){
        exit(1);
    }

    // 4. Store the terms between lowest_match and highest_match into answers 
    for(int i = 0; i < *n_answer; i++){
        (*answer)[i].weight = terms[lowest_index+i].weight;

        int j = 0;
        while(terms[lowest_index+i].term[j] != '\0'){ 
            (*answer)[i].term[j] = terms[lowest_index+i].term[j]; 
            j++;
        }
        (*answer)[i].term[j] = '\0';
    }

    // 5. Sort by weight of BOTH THE TERMS AND THE WEIGHTING
        // Create a qsort compare function that helps you sort the weight in decreasing order. 
    qsort(*answer, *n_answer, sizeof(term), comparator_2);

}