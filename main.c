#include <stdio.h>
#include <stdlib.h>
#define Max_Resources 25
#define Max_Processes 25

int totalR, totalP; //total resources and processes the user chooses during claimGraph
int resource[Max_Resources];
int available[Max_Resources];
int maxClaim[Max_Processes][Max_Resources];
int allocated[Max_Processes][Max_Resources];
int need[Max_Processes][Max_Resources];
void printData(int choice) {
    //todo fix spacing
    if (choice) {
        printf("\nResources: \n");
        for (int i = 0; i < totalR; i++) {
            printf("%6s%d", "r", i);
        }
        printf("\n");

        for (int i = 0; i < totalR; i++) {
            if (!i) {
                printf("%8d", resource[i]);
            } else { printf("%6d", resource[i]); }
        }
        printf("\n   ");
    }
    //available
    printf("\n\nAvailable:\n   ");
    for (int i = 0; i < totalR; i++) {
        printf("%6s%d", "r", i);
    }
    printf("\n  ");
    for (int i = 0; i < totalR; i++) {
        printf("%7d", available[i]);
    }

    //Max Claim
    if (choice) {
        printf("\n\nMax Claim:\n    ");
        for (int i = 0; i < totalR; i++) {
            printf("%6s%d", "r", i);
        }
        for (int i = 0; i < totalP; i++) {
            printf("\np%d ", i);
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
    printf("Enter number of units for resources (r0 to r%d): \n", totalR - 1);
    for(int i = 0; i<totalR; i++){
        scanf("%d", &resource[i]);
        available[i]=resource[i];
    }
    printf("Enter number of processes: \n");
    scanf("%d", &totalP);
    for(int i = 0; i<totalP; i++){
        printf("Enter maximum number of units process p%d will "
               "claim from each resource (r0 to r%d): \n", i,totalR-1);
        for(int j = 0; j<totalR; j++){
            scanf("%d", &maxClaim[i][j]);
            need[i][j] = maxClaim[i][j];
        }
    }
    for(int i = 0; i<totalP; i++){
        printf("Enter number of units of each resource (r0 to r%d) "
               "currently allocated to process p%d: \n", totalR-1,i);
        for(int j = 0; j<totalR; j++){
            //todo create check for allocating more than available
            scanf("%d", &allocated[i][j]);
            need[i][j] -= allocated[i][j];
            available[j] -= allocated[i][j];
        }
    }
    printData(1);
}
void requestResource(){
    //todo check for invalid entries
    char processID[10];
    int processNum;
    printf("Enter requesting process: \n");
    scanf("%9s", &processID);
    if(sscanf(processID,"%*[^0-9]%d", &processNum) == 1){
        printf("Process number is: %d\n", processNum);
    }

    //requested resources
    char resourceID[10];
    int resourceNum;
    printf("Enter requested resource: \n");
    scanf("%9s", &resourceID);
    sscanf(resourceID, "%*[^0-9]%d", &resourceNum);

    //units
    int units;
    printf("Enter number of units process p2 is requesting from resource r0: \n");
    scanf("%d",&units);
    // take away units from available and need. add the same units back to allocated
    if (available[resourceNum]-units >0 || need[processNum][resourceNum]-units >0){
        available[resourceNum] -= units;
        need[processNum][resourceNum]-=units;
        allocated[processNum][resourceNum]+=units;
    }


    //print graph
    printData(0);
}
void releaseResource(){}
void determineSafeSequence(){}
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
