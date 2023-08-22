#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_SYMBOLS 10

typedef struct {
    int states[MAX_STATES];
    int count;
} StateSet;

typedef struct {
    StateSet set;
    char symbol;
} Transition;

typedef struct {
    Transition transitions[MAX_STATES][MAX_SYMBOLS];
    int num_states;
    int num_symbols;
} NFA;

typedef struct {
    StateSet states[MAX_STATES];
    int count;
} DFAStateSet;

typedef struct {
    DFAStateSet transitions[MAX_STATES][MAX_SYMBOLS];
    int num_states;
    int num_symbols;
} DFA;

bool stateSetContains(StateSet set, int state) {
    int i;
    for ( i = 0; i < set.count; i++) {
        if (set.states[i] == state) {
            return true;
        }
    }
    return false;
}

void stateSetInsert(StateSet *set, int state) {
    if (!stateSetContains(*set, state)) {
        set->states[set->count] = state;
        set->count++;
    }
}

void epsilonClosure(NFA nfa, StateSet *states) {
    StateSet stack = *states;
    int i;
    while (stack.count > 0) {
        int current_state = stack.states[stack.count - 1];
        stack.count--;

        for ( i = 0; i < nfa.transitions[current_state][nfa.num_symbols - 1].set.count; i++) {
            int next_state = nfa.transitions[current_state][nfa.num_symbols - 1].set.states[i];
            if (!stateSetContains(*states, next_state)) {
                stateSetInsert(states, next_state);
                stateSetInsert(&stack, next_state);
            }
        }
    }
}

StateSet move(NFA nfa, StateSet states, char symbol) {
    StateSet move_states;
    int i,j;
    for ( i = 0; i < states.count; i++) {
        for ( j = 0; j < nfa.transitions[states.states[i]][symbol - '0'].set.count; j++) {
            stateSetInsert(&move_states, nfa.transitions[states.states[i]][symbol - '0'].set.states[j]);
        }
    }
    return move_states;
}

int findDFAState(DFAStateSet *dfa_states, StateSet states) {
    int i;
    for ( i = 0; i < dfa_states->count; i++) {
        if (memcmp(&dfa_states->states[i], &states, sizeof(StateSet)) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    NFA nfa;
    DFA dfa;

    printf("Enter the number of states in the NFA: ");
    scanf("%d", &nfa.num_states);

    printf("Enter the number of input symbols in the NFA: ");
    scanf("%d", &nfa.num_symbols);
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

    nfa.num_symbols++;  // Adding epsilon as the last symbol

    printf("Enter the starting state of the NFA: ");
    scanf("%d", &nfa.transitions[nfa.num_states][nfa.num_symbols - 1].set.count);
    nfa.transitions[nfa.num_states][nfa.num_symbols - 1].set.states[0] = 0;

    printf("Enter the number of final states in the NFA: ");
    scanf("%d", &nfa.transitions[nfa.num_states + 1][nfa.num_symbols - 1].set.count);
    for ( i = 0; i < nfa.transitions[nfa.num_states + 1][nfa.num_symbols - 1].set.count; i++) {
        scanf("%d", &nfa.transitions[nfa.num_states + 1][nfa.num_symbols - 1].set.states[i]);
    }

    dfa.num_symbols = nfa.num_symbols - 1;
    dfa.num_states = 0;
    StateSet initial_state_set;
    initial_state_set.count = 1;
    initial_state_set.states[0] = 0;
    epsilonClosure(nfa, &initial_state_set);

    DFAStateSet dfa_states[MAX_STATES];
    dfa_states[dfa.num_states] = initial_state_set;
    dfa.num_states++;
    char symbol;
    int current_dfa_state = 0;
    while (current_dfa_state < dfa.num_states) {
        for ( symbol = '0'; symbol < '0' + dfa.num_symbols; symbol++) {
            StateSet move_states = move(nfa, dfa_states[current_dfa_state].states, symbol);
            StateSet epsilon_states = move(nfa, move_states, nfa.num_symbols - 1);
            epsilonClosure(nfa, &epsilon_states);
            stateSetInsert(&move_states, epsilon_states);

            int dfa_state_index = findDFAState(dfa_states, move_states);
            if (dfa_state_index == -1) {
                dfa_states[dfa.num_states] = move_states;
                dfa.transitions[current_dfa_state][symbol - '0'] = dfa_states[dfa.num_states];
                dfa.num_states++;
            } else {
                dfa.transitions[current_dfa_state][symbol - '0'] = dfa_states[dfa_state_index];
            }
        }
        current_dfa_state++;
    }

    printf("\nStates of the DFA:\n");
    for (int i = 0; i < dfa.num_states; i++) {
        printf("DFA State %d: {", i);
        for (int j = 0; j < dfa_states[i].count; j++) {
            printf(" %d", dfa_states[i].states[j]);
        }
        printf(" }\n");
    }

    printf("\nTransition Table of the DFA:\n");
    for ( i = 0; i < dfa.num_states; i++) {
        printf("State %d:\n", i);
        for (char symbol = '0'; symbol < '0' + dfa.num_symbols; symbol++) {
            printf("  %c -> DFA State {", symbol);
            for ( j = 0; j < dfa.transitions[i][symbol - '0'].count; j++) {
                printf(" %d", dfa.transitions[i][symbol - '0'].states[j]);
            }
            printf(" }\n");
        }
    }

    printf("\nStarting state of the DFA: 0\n

