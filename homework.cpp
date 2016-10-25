#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

#define ENABLE_DEBUG 0
#define print(str) if(ENABLE_DEBUG) printf str

#define MAX_BOARD_SIZE 26
#define INTEGER_MAX 1000000000
#define INTEGER_MIN -1000000000

int n;
char mode[100];
char player, opposition;
int depth;
int value[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
char state[MAX_BOARD_SIZE + 1][MAX_BOARD_SIZE + 1];

FILE *in, *out;

struct solution
{
    char movetype[10];
    int row;
    int column;
}next_state;
int max_score = INTEGER_MIN;

void print_solution();
int calculate_score();
int calculate_minimax(int depth_t, bool maximizing_player);
void calculate_alphabeta();
int max_value(int depth_t, int alpha, int beta);
int min_value(int depth_t, int alpha, int beta);

long long int total_nodes = 0;

int main(int argc, char const *argv[])
{
    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");
    if(!in || !out)
    {
        printf("File open failed!! \n");
        return 0;
    }
    int i, j;
    char t_play[5];

    fscanf(in, "%d", &n);
    fscanf(in, "%s", mode);
    fscanf(in, "%s", t_play);
    player = t_play[0];
    opposition = (player == 'X' ? 'O' : 'X');
    fscanf(in, "%d", &depth);
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            fscanf(in, "%d", &value[i][j]);
        }
    }
    for(i = 0; i < n; i++)
    {
        fscanf(in, "%s", state[i]);
    }

    print(("%d \n", n));
    print(("%s \n", mode));
    print(("%c \n", player));
    print(("%d \n", depth));
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            print(("%d ", value[i][j]));
        }
        print(("\n"));
    }
    for(i = 0; i < n; i++)
    {
        print(("%s\n", state[i]));
    }
    int count_empty = 0;
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            if(state[i][j] == '.')
                count_empty++;
    depth = min(depth, count_empty);
    if(!strcmp(mode, "MINIMAX"))
    {
        calculate_minimax(depth, true);
    }
    else if(!strcmp(mode, "ALPHABETA"))
    {
        calculate_alphabeta();
    }
    print(("\n\n"));
    print(("Total nodes = %lld \n\n", total_nodes));
    print_solution();
    print(("\n"));
    print(("Score : %d \n", max_score));

    fclose(in);
    fclose(out);
    return 0;
}

void print_solution()
{
    printf("Solution : \n%c%d %s\n", (char)('A' + next_state.column), next_state.row + 1, next_state.movetype);
    fprintf(out, "%c%d %s\n", (char)('A' + next_state.column), next_state.row + 1, next_state.movetype);
    state[next_state.row][next_state.column] = player;
    if(!strcmp(next_state.movetype, "Raid"))
    {
        if(next_state.column > 0 && state[next_state.row][next_state.column - 1] == opposition)
            state[next_state.row][next_state.column - 1] = player;
        if(next_state.column < n - 1 && state[next_state.row][next_state.column + 1] == opposition)
            state[next_state.row][next_state.column + 1] = player;
        if(next_state.row > 0 && state[next_state.row - 1][next_state.column] == opposition)
            state[next_state.row - 1][next_state.column] = player;
        if(next_state.row < n - 1 && state[next_state.row + 1][next_state.column] == opposition)
            state[next_state.row + 1][next_state.column] = player;
    }
    for(int i = 0; i < n; i++)
    {
        printf("%s\n", state[i]);
        fprintf(out, "%s\n", state[i]);
    }
}

int calculate_score(char current_player)
{
    int score = 0;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(state[i][j] == current_player)
            {
                score = score + value[i][j];
            }
            else if(state[i][j] != '.')
            {
                score = score - value[i][j];
            }
        }
    }
    return score;
}

bool can_raid(char current_player, int i, int j)
{
    char current_opposition = (current_player == 'X' ? 'O' : 'X');
    bool found_adjacent_player = false, found_adjacent_opposition = false;
    if(j > 0)
        if(state[i][j - 1] == current_player)
            found_adjacent_player = true;
        else if(state[i][j - 1] == current_opposition)
            found_adjacent_opposition = true;
    if(j < n - 1)
        if(state[i][j + 1] == current_player)
            found_adjacent_player = true;
        else if(state[i][j + 1] == current_opposition)
            found_adjacent_opposition = true;
    if(i > 0)
        if(state[i - 1][j] == current_player)
            found_adjacent_player = true;
        else if(state[i - 1][j] == current_opposition)
            found_adjacent_opposition = true;
    if(i < n - 1)
        if(state[i + 1][j] == current_player)
            found_adjacent_player = true;
        else if(state[i + 1][j] == current_opposition)
            found_adjacent_opposition = true;
    return found_adjacent_player && found_adjacent_opposition;
}

int calculate_minimax(int depth_t, bool maximizing_player)
{
    if(depth_t == 0)
    {
        total_nodes++;
        int current_val = calculate_score(player);
        //print(("current_val = %d \n", current_val));
        return current_val;
    }
    if(maximizing_player)
    {
        int best_value = INTEGER_MIN, ret;
        /* Stake */
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(state[i][j] == '.')
                {
                    state[i][j] = player;
                    ret = calculate_minimax(depth_t - 1, false);
                    if(depth_t == depth && best_value < ret)
                    {
                        best_value = ret;
                        strcpy(next_state.movetype, "Stake");
                        next_state.row = i;
                        next_state.column = j;
                        max_score = max(best_value, max_score);
                    }
                    best_value = max(ret, best_value);
                    state[i][j] = '.';
                }
            }
        }
        /* Raid */
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(state[i][j] == '.' && can_raid(player, i, j))
                {
                    state[i][j] = player;
                    char board_left = '\0', board_right = '\0', board_up = '\0', board_down = '\0';
                    if(j > 0)
                    {
                        board_left = state[i][j - 1];
                        if(state[i][j - 1] == opposition)
                        {
                            state[i][j - 1] = player;
                        }
                    }
                    if(j < n - 1)
                    {
                        board_right = state[i][j + 1];
                        if(state[i][j + 1] == opposition)
                        {
                            state[i][j + 1] = player;
                        }
                    }
                    if(i > 0)
                    {
                        board_up = state[i - 1][j];
                        if(state[i - 1][j] == opposition)
                        {
                            state[i - 1][j] = player;
                        }
                    }
                    if(i < n - 1)
                    {
                        board_down = state[i + 1][j];
                        if(state[i + 1][j] == opposition)
                        {
                            state[i + 1][j] = player;
                        }
                    }
                    ret = calculate_minimax(depth_t - 1, false);
                    if(depth_t == depth && best_value < ret)
                    {
                        best_value = ret;
                        strcpy(next_state.movetype, "Raid");
                        next_state.row = i;
                        next_state.column = j;
                        max_score = max(best_value, max_score);
                    }
                    best_value = max(ret, best_value);
                    if(board_left != '\0')
                    {
                        state[i][j - 1] = board_left;
                    }
                    if(board_right != '\0')
                    {
                        state[i][j + 1] = board_right;
                    }
                    if(board_up != '\0')
                    {
                        state[i - 1][j] = board_up;
                    }
                    if(board_down != '\0')
                    {
                        state[i + 1][j] = board_down;
                    }
                    state[i][j] = '.';
                }
            }
        }
        return best_value;
    }
    else
    {
        int best_value = INTEGER_MAX, ret;
        /* Stake */
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(state[i][j] == '.')
                {
                    state[i][j] = opposition;
                    ret = calculate_minimax(depth_t - 1, true);
                    best_value = min(ret, best_value);
                    state[i][j] = '.';
                }
            }
        }
        /* Raid */
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(state[i][j] == '.'  && can_raid(opposition, i, j))
                {
                    state[i][j] = opposition;
                    char board_left = '\0', board_right = '\0', board_up = '\0', board_down = '\0';
                    if(j > 0)
                    {
                        board_left = state[i][j - 1];
                        if(state[i][j - 1] == player)
                        {
                            state[i][j - 1] = opposition;
                        }
                    }
                    if(j < n - 1)
                    {
                        board_right = state[i][j + 1];
                        if(state[i][j + 1] == player)
                        {
                            state[i][j + 1] = opposition;
                        }
                    }
                    if(i > 0)
                    {
                        board_up = state[i - 1][j];
                        if(state[i - 1][j] == player)
                        {
                            state[i - 1][j] = opposition;
                        }
                    }
                    if(i < n - 1)
                    {
                        board_down = state[i + 1][j];
                        if(state[i + 1][j] == player)
                        {
                            state[i + 1][j] = opposition;
                        }
                    }
                    ret = calculate_minimax(depth_t - 1, true);
                    best_value = min(ret, best_value);
                    if(board_left != '\0')
                    {
                        state[i][j - 1] = board_left;
                    }
                    if(board_right != '\0')
                    {
                        state[i][j + 1] = board_right;
                    }
                    if(board_up != '\0')
                    {
                        state[i - 1][j] = board_up;
                    }
                    if(board_down != '\0')
                    {
                        state[i + 1][j] = board_down;
                    }
                    state[i][j] = '.';
                }
            }
        }
        return best_value;
    }
}

void calculate_alphabeta()
{
    max_value(depth, INTEGER_MIN, INTEGER_MAX);
}

int max_value(int depth_t, int alpha, int beta)
{
    if(depth_t == 0)
    {
        total_nodes++;
        int current_val = calculate_score(player);
        return current_val;
    }
    int best_value = INTEGER_MIN, ret;
    /* Stake */
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(state[i][j] == '.')
            {
                state[i][j] = player;
                ret = min_value(depth_t - 1, alpha, beta);
                if(depth_t == depth && best_value < ret)
                {
                    best_value = ret;
                    strcpy(next_state.movetype, "Stake");
                    next_state.row = i;
                    next_state.column = j;
                }
                best_value = max(ret, best_value);
                state[i][j] = '.';
                if(best_value >= beta)
                {
                    return best_value;
                }
                alpha = max(best_value, alpha);
            }
        }
    }
    /* Raid */
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(state[i][j] == '.' && can_raid(player, i, j))
            {
                state[i][j] = player;
                char board_left = '\0', board_right = '\0', board_up = '\0', board_down = '\0';
                if(j > 0)
                {
                    board_left = state[i][j - 1];
                    if(state[i][j - 1] == opposition)
                    {
                        state[i][j - 1] = player;
                    }
                }
                if(j < n - 1)
                {
                    board_right = state[i][j + 1];
                    if(state[i][j + 1] == opposition)
                    {
                        state[i][j + 1] = player;
                    }
                }
                if(i > 0)
                {
                    board_up = state[i - 1][j];
                    if(state[i - 1][j] == opposition)
                    {
                        state[i - 1][j] = player;
                    }
                }
                if(i < n - 1)
                {
                    board_down = state[i + 1][j];
                    if(state[i + 1][j] == opposition)
                    {
                        state[i + 1][j] = player;
                    }
                }
                ret = min_value(depth_t - 1, alpha, beta);
                if(depth_t == depth && best_value < ret)
                {
                    best_value = ret;
                    strcpy(next_state.movetype, "Raid");
                    next_state.row = i;
                    next_state.column = j;
                }
                best_value = max(ret, best_value);
                if(board_left != '\0')
                {
                    state[i][j - 1] = board_left;
                }
                if(board_right != '\0')
                {
                    state[i][j + 1] = board_right;
                }
                if(board_up != '\0')
                {
                    state[i - 1][j] = board_up;
                }
                if(board_down != '\0')
                {
                    state[i + 1][j] = board_down;
                }
                state[i][j] = '.';
                if(best_value >= beta)
                {
                    return best_value;
                }
                alpha = max(best_value, alpha);
            }
        }
    }
    return best_value;
}

int min_value(int depth_t, int alpha, int beta)
{
    if(depth_t == 0)
    {
        total_nodes++;
        int current_val = calculate_score(player);
        return current_val;
    }
    int best_value = INTEGER_MAX, ret;
    /* Stake */
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(state[i][j] == '.')
            {
                state[i][j] = opposition;
                ret = max_value(depth_t - 1, alpha, beta);
                best_value = min(ret, best_value);
                state[i][j] = '.';
                if(best_value <= alpha)
                {
                    return best_value;
                }
                beta = min(best_value, beta);
            }
        }
    }
    /* Raid */
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(state[i][j] == '.'  && can_raid(opposition, i, j))
            {
                state[i][j] = opposition;
                char board_left = '\0', board_right = '\0', board_up = '\0', board_down = '\0';
                if(j > 0)
                {
                    board_left = state[i][j - 1];
                    if(state[i][j - 1] == player)
                    {
                        state[i][j - 1] = opposition;
                    }
                }
                if(j < n - 1)
                {
                    board_right = state[i][j + 1];
                    if(state[i][j + 1] == player)
                    {
                        state[i][j + 1] = opposition;
                    }
                }
                if(i > 0)
                {
                    board_up = state[i - 1][j];
                    if(state[i - 1][j] == player)
                    {
                        state[i - 1][j] = opposition;
                    }
                }
                if(i < n - 1)
                {
                    board_down = state[i + 1][j];
                    if(state[i + 1][j] == player)
                    {
                        state[i + 1][j] = opposition;
                    }
                }
                ret = max_value(depth_t - 1, alpha, beta);
                best_value = min(ret, best_value);
                if(board_left != '\0')
                {
                    state[i][j - 1] = board_left;
                }
                if(board_right != '\0')
                {
                    state[i][j + 1] = board_right;
                }
                if(board_up != '\0')
                {
                    state[i - 1][j] = board_up;
                }
                if(board_down != '\0')
                {
                    state[i + 1][j] = board_down;
                }
                state[i][j] = '.';
                if(best_value <= alpha)
                {
                    return best_value;
                }
                beta = min(best_value, beta);
            }
        }
    }
    return best_value;
}
