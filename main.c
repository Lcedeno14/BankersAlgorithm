#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define Max_Resources 25
#define Max_Processes 25

int totalR, totalP; //total resources and processes the user chooses during claimGraph
int resource[Max_Resources];
int available[Max_Resources];
int maxClaim[Max_Processes][Max_Resources];
int allocated[Max_Processes][Max_Resources];
int need[Max_Processes][Max_Resources];
void printData(int choice) {
    printf("\n");
    if (choice) {
        printf("\nResources: \n   ");
        for (int i = 0; i < totalR; i++) {
            printf("%6s%d", "r", i);
        }
        printf("\n  ");

        for (int i = 0; i < totalR; i++) {
            if (!i) {
                printf("%8d", resource[i]);
            } else { printf("%6d", resource[i]); }
        }
    }
    //available
    printf("\nAvailable: \n   ");
    for (int i = 0; i < totalR; i++) {
        printf("%6s%d", "r", i);
    }
    printf("\n  ");
    for (int i = 0; i < totalR; i++) {
        printf("%7d", available[i]);
    }

    //Max Claim
    if (choice) {
        printf("\n\nMax Claim:\n   ");
        for (int i = 0; i < totalR; i++) {
            printf("%6s%d", "r", i);
        }
        for (int i = 0; i < totalP; i++) {
            printf("\np%d", i);
            for (int j = 0; j < totalR; j++) {
                printf("%7d", maxClaim[i][j]);
            }
        }
    }
    // Allocated resources
    printf("\n\nAllocated:\n    ");
    for (int i = 0; i < totalR; i++) {
        printf("%5s%d", "r", i);
    }
    for (int i = 0; i < totalP; i++) {
        printf("\np%d ", i);
        for (int j = 0; j < totalR; j++) {
            printf("%6d", allocated[i][j]);
        }
    }

    // Need matrix
    printf("\n\nNeed:\n    ");
    for (int i = 0; i < totalR; i++) {
        printf("%5s%d", "r", i);
    }
    for (int i = 0; i < totalP; i++) {
        printf("\np%d ", i);
        for (int j = 0; j < totalR; j++) {
            printf("%6d", need[i][j]);
        }
    }
    printf("\n\n\n");
}
void enterClaimGraph(){
    printf("Enter number of resources: \n");
    scanf("%d", &totalR);
    printf("Enter number of units for resources (r0 to r%d): ", totalR - 1);
    for(int i = 0; i<totalR; i++){
        scanf("%d", &resource[i]);
        available[i]=resource[i];
    }
    printf("\nEnter number of processes: ");
    scanf("%d", &totalP);
    for(int i = 0; i<totalP; i++){
        printf("\nEnter maximum number of units process p%d will "
               "claim from each resource (r0 to r%d): ", i,totalR-1);
        for(int j = 0; j<totalR; j++){
            scanf("%d", &maxClaim[i][j]);
            need[i][j] = maxClaim[i][j];
        }
    }
    for(int i = 0; i<totalP; i++){
        printf("\nEnter number of units of each resource (r0 to r%d) "
               "currently allocated to process p%d: ", totalR-1,i);
        for(int j = 0; j<totalR; j++){
            //todo create check for allocating more than available
            scanf("%d", &allocated[i][j]);
            need[i][j] -= allocated[i][j];
            available[j] -= allocated[i][j];
        }
    }
    printf("\n");
    printData(1);
}
void requestResource(){
    //todo check for invalid entries
    char processID[10];
    int processNum;
    printf("Enter requesting process: \n");
    scanf("%9s", processID);
    sscanf(processID,"%*[^0-9]%d", &processNum);


    //requested resources
    char resourceID[10];
    int resourceNum;
    printf("Enter requested resource: \n");
    scanf("%9s", resourceID);
    sscanf(resourceID, "%*[^0-9]%d", &resourceNum);

    //units
    int units;
    printf("Enter number of units process p%d is requesting from resource r0: \n",processNum);
    scanf("%d",&units);
    // take away units from available and need. add the same units back to allocated
    if (available[resourceNum]-units >= 0 && need[processNum][resourceNum]-units >= 0){
        available[resourceNum] -= units;
        need[processNum][resourceNum]-=units;
        allocated[processNum][resourceNum]+=units;
    }


    //print graph
    printData(0);
}
void releaseResource(){
    //todo check for invalid entries
    char processID[10];
    int processNum;
    printf("Enter releasing processor: \n");
    scanf("%9s", processID);
    sscanf(processID,"%*[^0-9]%d", &processNum);
    //requested resources
    char resourceID[10];
    int resourceNum;
    printf("Enter released resource: \n");
    scanf("%9s", resourceID);
    sscanf(resourceID, "%*[^0-9]%d", &resourceNum);

    //units
    int units;
    printf("Enter number of units process p%d is requesting from resource r0: \n",processNum);
    scanf("%d",&units);
    // take away units from available and need. add the same units back to allocated
    //todo what if allocated increases maxClaim
    if (allocated[processNum][resourceNum] - units>= 0){
        available[resourceNum] += units;
        need[processNum][resourceNum]+=units;
        allocated[processNum][resourceNum]-= units;
    }
    printData(0);
}
void determineSafeSequence(){
    if(totalP == 0) {return;}
    bool safe[Max_Processes] = {false}; // Can be sequenced
    int safeSequence[Max_Processes]; // Store safe sequences
    int count = 0;
    int avail[Max_Resources];
    for(int i =0; i <totalR;i++){
        avail[i] = available[i];
    }

    while(count < totalP) {
        bool found = false;
        for(int p = 0; p < totalP; p++) {
            if (!safe[p]) {
                bool possible = true;
                printf("Comparing: < ");
                for(int r = 0; r < totalR; r++) {
                    printf("%d ",need[p][r]);
                    if(need[p][r] > avail[r]) {
                        possible = false;
                    }
                }
                printf("> <= < ");

                for(int r = 0; r< totalR; r++){
                    printf("%d ",avail[r]);
                }
                printf("> : Process p%d ",p);
                possible ? printf("can ") : printf("cannot ");
                printf("be sequenced\n");
                if(possible) {
                    for(int r = 0; r < totalR; r++)
                        avail[r] += allocated[p][r];

                    safeSequence[count++] = p;
                    safe[p] = true; // set as safe
                    found = true;
                }
            }
        }
    }
    printf("Safe sequence of processes:");
    for(int i = 0; i < totalP; i++)
        printf(" p%d ", safeSequence[i]);
    printf("\n");
}

void quit(){
    exit(0);
}

int main() {
    int input;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while(1) {
        printf("Banker's Algorithm\n"
               "------------------\n"
               "1) Enter claim graph\n"
               "2) Request resource\n"
               "3) Release resource\n"
               "4) Determine safe sequence\n"
               "5) Quit program\n"
               "\nEnter Selection:");
        scanf("%d", &input);
        printf("\n");
        switch (input) {
            case 1:
                enterClaimGraph();
                break;
            case 2:
                requestResource();
                break;
            case 3:
                releaseResource();
                break;
            case 4:
                determineSafeSequence();
                break;
            case 5:
                quit();
                break;
            default:
                printf("Invalid input, please try again. \n");
        }
    }
#pragma clang diagnostic pop

}
