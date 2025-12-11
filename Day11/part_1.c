#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

int number_of_lines = 588;

typedef struct {
    char *name;
    char **destination;
    int destination_count;
    int dist_to_exit;
    int number_of_exits;
} Node;

void print_nodes(Node *nodes, int count) {
    for (int i = 0; i < count; i++) {
        printf("Node %s: ", nodes[i].name);
        for (int j = 0; j < nodes[i].destination_count; j++) {
            printf("%s ", nodes[i].destination[j]);
        }
        printf("     \tNumber of exits: %d", nodes[i].number_of_exits);
        printf("\n");
    }
}

void order_nodes(Node *nodes, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(nodes[j].name, nodes[j + 1].name) > 0) {
                Node temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
        }
    }
}

int search_node(Node *nodes, int count, char *name, int start_index, int end_index) {
    if (start_index > end_index) {
        return -1;
    }
    int mid_index = start_index + (end_index - start_index) / 2;
    int cmp = strcmp(nodes[mid_index].name, name);
    if (cmp == 0) {
        return mid_index; 
    } else if (cmp > 0) {
        return search_node(nodes, count, name, start_index, mid_index - 1);
    } else {
        return search_node(nodes, count, name, mid_index + 1, end_index);
    }
}

int is_visited(char *name, char **visited, int visited_count) {
    for (int i = 0; i < visited_count; i++) {
        if (strcmp(name, visited[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}

void print_visited(char **visited, int visited_count) {
    printf("Visited nodes: ");
    for (int i = 0; i < visited_count; i++) {
        printf("%s ", visited[i]);
    }
    printf("\n");
}

int find_exit_distances(Node *nodes, int count, int current_index, char **visited, int visited_count, int *number_of_exits) {
    if (nodes[current_index].dist_to_exit != -1) {
        (*number_of_exits)+=nodes[current_index].number_of_exits;
        print_visited(visited, visited_count);
        return nodes[current_index].dist_to_exit + 1;
    }
    else if (nodes[current_index].destination_count == 0) {
        return -1; 
    }
    for (int i = 0; i < nodes[current_index].destination_count; i++) {
        if (is_visited(nodes[current_index].destination[i], visited, visited_count)) {
            continue; 
        }
        int dest_index = search_node(nodes, count, nodes[current_index].destination[i], 0, count - 1);

        char **new_visited = malloc((visited_count + 1) * sizeof(char *));
        for (int j = 0; j < visited_count; j++) {
            new_visited[j] = visited[j];
        }
        new_visited[visited_count] = nodes[current_index].destination[i];

        if (dest_index != -1) {
            int distance = find_exit_distances(nodes, count, dest_index, new_visited, visited_count + 1, number_of_exits);
            nodes[current_index].dist_to_exit = distance;
            if (distance != -1) {
                nodes[current_index].number_of_exits += nodes[dest_index].number_of_exits;
            }
        }
    }
    if (nodes[current_index].dist_to_exit == -1) {
        return -1; 
    }
    return nodes[current_index].dist_to_exit + 1;
}

int day11(Node *nodes, int count) {
    int start_index = search_node(nodes, count, "you", 0, count - 1);
    char **visited = malloc(sizeof(char *));
    int visited_count = 1;
    visited[0] = malloc(4 * sizeof(char));
    visited[0] = "you";
    int number_of_exits = 0;
    find_exit_distances(nodes, count, start_index, visited, visited_count, &number_of_exits);
    printf("Start index: %d\n", start_index);
    return number_of_exits;
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening input.txt");
        return 1;
    }

    char line[1024];
    int count = 0;
    int case_num = 0;

    Node *nodes = malloc(number_of_lines * sizeof(Node));

    while (fgets(line, 1024, file)) {
        char *ptr = line;
        Node current_node;
        current_node.name = malloc(4 * sizeof(char));
        current_node.name[0] = line[0];
        current_node.name[1] = line[1];
        current_node.name[2] = line[2];
        current_node.name[3] = '\0';
        current_node.destination_count = 0;
        current_node.dist_to_exit = -1;
        current_node.number_of_exits = 0;

        current_node.destination = malloc(sizeof(char *));

        ptr += 4; 
        while (*ptr != '\n' && *ptr != '\0') {
            ptr++;

            current_node.destination = realloc(current_node.destination, (current_node.destination_count + 1) * sizeof(char *));
            current_node.destination[current_node.destination_count] = malloc(4 * sizeof(char));
            current_node.destination[current_node.destination_count][0] = *ptr;
            current_node.destination[current_node.destination_count][1] = *(ptr+1);
            current_node.destination[current_node.destination_count][2] = *(ptr+2);
            current_node.destination[current_node.destination_count][3] = '\0';

            current_node.destination_count++;

            if (strcmp(current_node.destination[current_node.destination_count - 1], "out") == 0) {
                current_node.dist_to_exit = 1;
                current_node.number_of_exits = 1;
                break; //exit found no need for the rest of the line
            }
            
            ptr += 3; 
        }
        nodes[count] = current_node;
        count++;
    }

    order_nodes(nodes, count);
    //print_nodes(nodes, count);
    int number_of_exits = day11(nodes, count);
    print_nodes(nodes, count);
    printf("Number of exits: %d\n", number_of_exits);

    fclose(file);
    return 0;
}

