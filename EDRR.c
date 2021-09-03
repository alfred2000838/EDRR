#include <stdio.h>
#include <stdlib.h>
 
// C program to sort the array in an
// ascending order using selection sort
// source: https://www.geeksforgeeks.org/c-program-to-sort-an-array-in-ascending-order/
// my add on: while swapping list of process according to arrival time
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
// Function to perform Selection Sort
void selectionSort(int arr[], int ind[], int n)
{
    int i, j, min_idx;
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) { 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j; 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
        swap(&ind[min_idx], &ind[i]); // swap the index (process number)
    }
} 
// Function to print an array 
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
// dynamic array
// source: https://stackoverflow.com/questions/3536153/c-dynamically-growing-array
typedef struct {
  int *array;
  size_t used;
  size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, int element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size += 1;
    a->array = realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

// check if last process is in rq
int inrq(int rq[], int rql, int n){
    for (int i = 0; i < rql; i++){
        if(rq[i] == n-1){
            return 0;
        }
    }    
    return 1;
}

// check if next process <= totaltime
int processArrive(int at[], int totalTime, int rq[], int rql, int n){
    if(inrq(rq, rql, n) == 1){ // if last process not in ready queue
        if (at[rq[rql-1]+1] <= totalTime){ // if next arrive process less than tt
            return 0;
        }
    }
    return 1;
}
// End of helper functions

void main ()
{
    int i, n, t, rql = 0, cs = 0, arr_index, max, remainingProcesses, totalTime = 0;
    float awt = 0.0, att = 0.0;
    Array rq;
    
    // get number of process
    printf("Enter the no of processes: ");
    scanf("%d", &n); // n = number of processes
    int bu[n], at[n], wt[n], tat[n], single_process[n], ct[n], high_bu[n], pl[n];

    // get burst time
    for (i = 0; i < n; i++)
    {
        printf("\nEnter Burst Time for process %d: ", i+1);
        scanf("%d", &bu[i]); //bu[] = burst time
        wt[i] = 0;
        tat[i] = 0;
        single_process[i] = -1; // if they are first process will change to 0
        pl[i] = i; // process list
    }
    // get arrival time
    for (i = 0; i < n; i++)
    {
        printf("\nEnter Arrival Time for process %d: ", i+1);
        scanf("%d", &at[i]); //at[] = arrival time
    }
    
    // sort at
    selectionSort(at, pl, n); // sort according to at
    int *tempArr = malloc(n * sizeof(int));

    // sort burst time using process list
    for (i = 0; i < n; i++){
        tempArr[i] = bu[pl[i]];
    }

    for(i = 0; i < n; i++){
        bu[i] = tempArr[i];
        ct[i] = tempArr[i]; // copy of bu (to be used for printing, as values in bu is changed when assigned to CPU)
    }
    free(tempArr);
    // end of sorting

    printf("\n===========Result===========");
    printf("\nProcess\tArrival Time\tBurst Time\tWT\tTAT");

    // main code
    // IMPORTANT! rq contains respective index in at, bt and pl, first ele in rq is not always 0
    while(bu[n-1] != 0){ // while the last process is not change to 0
        initArray(&rq, 1); // init 1 element
        if(totalTime == 0){ // first arrived process, totaltime == 0
            // check if other process arrive at the same time as the first one
            for (i = 0; i < n; i++){
                if (at[i] == at[0]){ // if same arrival time
                    insertArray(&rq, i); // insert to ready queue 
                }
                else{ // arrive later
                    break; // exit if not equal cuz the rest will have equal or higher
                }
            }
        }
        else{ // not first process
            //initArray(&rq, 1); // new ready queue array
            rq.size = 0; // set size to 0 to check for idling
            for (i = 0; i < n; i++){
                if (at[i] <= totalTime && bu[i] != 0){ // insert index to rq if arrival time less than totaltime and not processed
                    insertArray(&rq, i); // insert to ready queue 
                }
            }
            // for idling CPU (very rare)
            if (rq.size == 0){ // if no process in rdy queue
                for (i = 0; i < n; i++){ // find process that arrive next
                    if (at[i] > totalTime){
                        printf("\nIdle\t%d", at[i] - totalTime);
                        totalTime = at[i]; // set totalTime to the next arrive process time
                        insertArray(&rq, i); // insert to ready queue
                        cs--; // decrease context switch as no process is being switch out
                        break;
                    }
                }                
            }
        }
        // get t, t = time quantum
        rql = rq.size; // number of process in ready queue
        max = bu[rq.array[0]]; // highest burst time
        if (rql > 1){
            for (i = 0; i < rql; i++){ // max = get the highest burst time
                if (max < bu[rq.array[i]]){
                    max = bu[rq.array[i]];
                }
            }
        }
        t = 0.8 * max; // 80% of highest burst time in rdy queue

        // start to assign to CPU
        i = 0; // iteration
        int process_counter = 0; // increases once completed executing
        remainingProcesses = 0; // increases when process > t

        while(process_counter != rql){ // Loop all process in ready queue
            arr_index = rq.array[i]; // set to code easier, rq.array[i]: get process in ready queue at index = i
            if(bu[arr_index] <= t && t != max){ // if process is less than t
                // assign to CPU
                if (single_process[arr_index] == 0){ // if it is first process in rdy queue
                   // tat[arr_index] += bu[arr_index];
                }
                else{ 
                    if(totalTime == 0){ // if first process need to account for the arrival time
                        totalTime += at[arr_index];
                    }
                    wt[arr_index] += totalTime - at[arr_index];
                    // tat[arr_index] += wt[arr_index] + bu[arr_index];    
                }
                totalTime += bu[arr_index];
                bu[arr_index] = 0; // set bu[arr_index] to 0 to indicate completion
                awt += wt[arr_index]; // add to total waiting time
                tat[arr_index] = totalTime - at[arr_index];
                att += tat[arr_index]; // add to total turn around time
                process_counter++; // increment or process_counter indicates completion of process
                cs++; // increment context switch
                printf("\nP%d\t%d\t\t%d\t\t%d\t%d", pl[arr_index]+1, at[arr_index], ct[arr_index], wt[arr_index], tat[arr_index]);

                // check if other process have arrive
                if (processArrive(at, totalTime, rq.array, rql, n) == 0){ // if process arrive
                    break; // break out of the current rdy queue
                }
            }
            // if only 1 in rq
            if (rql == 1 && bu[arr_index] == max){
                single_process[arr_index] = 0; // set to 0 to indicate it is the only process
                bu[arr_index] -= t; // decrease by time quantum
                if (totalTime == 0){ // 1st run time = 0
                    totalTime += at[arr_index]; // Need to add arrival time total time for 1st run time                                      
                }
                else{ // not first process
                    wt[arr_index] += totalTime - at[arr_index];     
                }
                totalTime += t; // increase total time by time quantum
                tat[arr_index] = totalTime - at[arr_index];
                // even if it is last process, it will burst for t*0.8 then burst the remainder, incurring 1 additional cs
                cs++; // Switching out 
                // check if process have arrive
                if (processArrive(at, totalTime, rq.array, rql, n) == 0){ // if process arrive
                    break; // break out of the current rdy queue
                }
                i--; // decrease iteration because require additional iteration as process is not completed
            }
            else if (bu[arr_index] > t){ // process with > 80% max burst time
                // add to back of rdy queue
                insertArray(&rq, arr_index);
                high_bu[remainingProcesses] = arr_index; // store process number
                remainingProcesses++; // increment to check when to set t = max and reset i
            }
            else if(t == max){
                if (single_process[high_bu[i]] == 0){ // if only 1 process in rdy queue and t == max
                    totalTime += bu[high_bu[i]];
                    wt[high_bu[i]] = 0; // first process wt = 0
                }
                else{ // assign process > 80% to CPU
                    wt[high_bu[i]] += totalTime - at[high_bu[i]];
                    totalTime += bu[high_bu[i]];
                }
                tat[high_bu[i]] = totalTime - at[high_bu[i]];
                awt += wt[high_bu[i]]; // add to total waiting time
                att += tat[high_bu[i]]; // add to total turn around time                       
                bu[high_bu[i]] = 0;
                printf("\nP%d\t%d\t\t%d\t\t%d\t%d", pl[high_bu[i]]+1, at[high_bu[i]], ct[high_bu[i]], wt[high_bu[i]], tat[high_bu[i]]);
                process_counter++; // increment process_counter as process is completed
                cs++; // increment context switch as process is completed

                // check if other process have arrive
                if (processArrive(at, totalTime, rq.array, rql, n) == 0){ // if process arrive
                    break; // break out of the current rdy queue
                }
            }
            // if all process < t completed            
            if (process_counter + remainingProcesses == rql && remainingProcesses > 0){ 
                t = max; // set t to max to complete the remaining process in rdy queue
                i = -1; // set i to -1 as i++ is next
            }
            i++; // completed 1 iteration
        }
    }
    printf("\nTotal Context Switch: %d", cs - 1); // minus 1 as the last process completed did not context switch to another
    printf("\nAverage Waiting Time: %f", awt/n);
    printf("\nAverage Turnaround Time: %f", att/n);
}