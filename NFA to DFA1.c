#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_SYMBOLS 2
typedef struct {
    int states[MAX_STATES];
    int count;
} StateSet;

typedef struct{
    StateSet set;
    char symbol;
}Transition;
typedef struct{
    Transition transitions[MAX_STATES][MAX_SYMBOLS];
    int num_states;
    int num_symbols;
}NFA;
typedef struct{
    StateSet states[MAX_STATES];
    int count;
}DFAStateSet;
typedef struct{
    Transition transitions[MAX_STATES][MAX_SYMBOLS];
    int num_states;
    int num_symbols;
}DFA;
void stateSetInsert(StateSet *set, int state){
    if(!stateSetContains(set state)){
        set->states[set->count]=state;
        set->count++;
    }
}
bool stateSetContains(StateSet set, int next_state){
    int i;
    for ( i = 0; i < set.count; i++) {
        if (set.states[i] == state) {
            return true;
        }
    }
    return false;
}
void checkClosure(NFA nfa,StateSet *states)
{
    StateSet stack=*states;
    int i;
    while(stack.count>0)
    {
        int currentState=stack.states[stack.count-1];
        stack.count--;
        for(i=0;i<nfa.transitions[nfa.num_states][nfa.num_symbols-1].set.count;i++){
            int next_state=nfa.transitions[NFA.num_states][nfa.num_symbols-1].set.states[i];
            if (!stateSetContains(*states, next_state)){
                stateSetInsert(states, next_state);
                stateSetInsert(&stack, next_state);
            }
        }
    }
}
int main()
{
    NFA nfa;
    DFA dfa;
     printf("Enter the number of states in the NFA: ");
     scanf("%d", &nfa.num_states);
     printf("Enter the number of input symbols in the NFA: ");
     scanf("%d",&nfa.num_symbols);
     int i,j,k;
     for ( i = 0; i < nfa.num_states; i++) {
        for ( j = 0; j < nfa.num_symbols; j++) {
            printf("Enter the number of transitions from state %d with symbol %d: ", i, j);
            scanf("%d", &nfa.transitions[i][j].set.count);

            for ( k = 0; k < nfa.transitions[i][j].set.count; k++) {
                scanf("%d", &nfa.transitions[i][j].set.states[k]);
            }
        }
    }

     printf("NFA Transition Table:\n");
     printf("state || symbol 0 || symbol 1\n");
      for ( i = 0; i < nfa.num_states; i++) {
            printf(" %d   ", i);
        for ( j = 0; j < nfa.num_symbols; j++) {
            //printf("Enter the number of transitions from state %d with symbol %d: ", i, j);


            for ( k = 0; k < nfa.transitions[i][j].set.count; k++) {
                printf("%d ", nfa.transitions[i][j].set.states[k]);
            }
            printf("    ");
        }
        printf("\n");
    }
    nfa.num_symbols++;
    printf("Enter the starting state of the NFA: ");
    //int startState;
    //int finalState;
   // scanf("%d",&stateState);
   scanf("%d",&nfa.transitions[nfa.num_states][nfa.num_symbols-1].set.count);
   nfa.transitions[nfa.num_states][nfa.num_symbols-1].set.states[0]=0;
    printf("Enter the number of final states in the NFA: ");
    scanf("%d",&nfa.transitions[nfa.num_states+1][nfa.num_symbols-1].set.count);
    for(k=0;k<nfa.transitions[nfa.num_states+1][nfa.num_symbols-1].set.count;k++)
    {
        scanf("%d",&nfa.transitions[nfa.num_states+1][nfa.num_symbols-1].set.states[k]);
    }
    //scanf("%d",&finalState);
    dfa.num_symbols=nfa.num_symbols-1;
    dfa.num_states=0;
    StateSet initialStateSet;
    initialStateSet.count=1;
    initialStateSet.states[0]=0;
    checkClosure(nfa,&initialStateSet);

    return 0;
}
