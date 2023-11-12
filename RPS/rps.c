// rps.c

#include <stdio.h>
#include <string.h>

const char* determine_winner(const char* player_choice, const char* opponent_choice) {
    if (strcmp(player_choice, opponent_choice) == 0) {
        return "It's a tie!";
    }

    if (
        (strcmp(player_choice, "rock") == 0 && strcmp(opponent_choice, "scissors") == 0)
        || (strcmp(player_choice, "paper") == 0 && strcmp(opponent_choice, "rock") == 0)
        || (strcmp(player_choice, "scissors") == 0 && strcmp(opponent_choice, "paper") == 0)
    ) {
        return "You win!";
    }

    return "You lose!";
}
