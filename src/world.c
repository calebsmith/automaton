#include "world.h"

/*
 * Parses command line arguments and returns them in a Config_t.
 *
 * Parameters: int argc, char* argv[]
 * Returns: Config_t
 * Side-Effects: Exits with status 1 if filename is not provided
 */
Config_t get_config(int argc, char* argv[])
{
    Config_t config;
    int i;
    char *arg;
    bool filename_arg = false;

    config.fullscreen = false;
    config.help = false;
    config.graphical = 0;
    config.toroidal = 0;
    config.sleep_time = DEFAULT_SLEEP_TIME;
    for(i = 1; i < argc; i++) {
        arg = argv[i];
        if (strncmp(arg, "-t", 2) == 0 ||
            strncmp(arg, "--toroidal", 10) == 0) {
            // handle toroidal flag
            config.toroidal = 1;
        } else if (strncmp(arg, "-g", 2) == 0 ||
            strncmp(arg, "--graphical", 11) == 0) {
            // handle graphical flag
            config.graphical = 1;
        } else if (strncmp(arg, "-f", 2) == 0 ||
            strncmp(arg, "--fullscreen", 12) == 0) {
            // handle fullscreen flag
            config.fullscreen = true;
        } else if (strncmp(arg, "-h", 2) == 0 ||
            strncmp(arg, "--help", 6) == 0) {
            // handle help flag
            config.help = true;
        } else if (strcmp(arg, "-s") == 0) {
            if (i + 1 < argc) {
                // handle sleep time argument
                config.sleep_time  = atoi(argv[i + 1]);
                i++;
            } else {
                printf("-s option takes an argument\n");
                exit(1);
            }
        } else if (strncmp(arg, "--sleep=", 8) == 0) {
            arg = strtok(arg, "="); arg = strtok(NULL, "=");
            config.sleep_time = atoi(arg);
        } else {
            // handle filename argument
            strncpy(config.filename, arg, MAX_FILENAME);
            filename_arg = true;
        }
    }
    // error handling
    if (!filename_arg && config.help == false) {
        printf("Must provide a filename to a data file\n");
        exit(EXIT_STATUS_TOO_FEW_ARGUMENTS);
    }
    return config;
}


int world_init(World_t* world, Config_t config)
{
    FILE *infile;
    world->board = malloc(sizeof(Board_t));
    world->next_board = malloc(sizeof(Board_t));
    world->rule = malloc(sizeof(Rule_t));

    // Load board file
    if ((infile = fopen(config.filename, "r")) == NULL) {
        printf("Could not open file %s\n", config.filename);
        world_destroy(world);
        return EXIT_STATUS_NO_FILE;
    } else {
        board_init(world->board, infile, config.toroidal);
        fclose(infile);
    }
    world->next_board->cells = (unsigned char*) calloc(
        world->board->width * world->board->height, sizeof(unsigned char)
    );
    board_copy(world->next_board, world->board);
    // Load rule file
    if ((infile = fopen(world->board->rule_filename, "r")) == NULL) {
        printf("Could not open rule file\n");
        world_destroy(world);
        return 1;
    } else {
        if (rule_init(world->rule, infile)) {
            printf("Failed to load rule file\n");
            world_destroy(world);
            return 1;
        }
        fclose(infile);
    }
    return 0;
}

void world_destroy(World_t* world)
{
    if (world->board != NULL) {
        board_destroy(world->board);
    }
    if (world->next_board != NULL) {
        board_destroy(world->next_board);
    }
    if (world->rule != NULL) {
        rule_destroy(world->rule);
    }
    free(world->board);
    free(world->next_board);
    free(world->rule);
}
