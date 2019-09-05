#include "constants.h"
#include "character.h"
#include "bomb.h"

t_character* create_character(int x, int y)
{
	t_character *character = malloc(sizeof(*character));

	if (!character)
		error("Character create error");

    character->x = x;
    character->y = y;
    character->current_direction = DOWN;
    character->life = 1;
	  character->screen_position.x = character->x * BLOCK_SIZE;
	  character->screen_position.y = character->y * BLOCK_SIZE - BLOCK_SIZE;
    character->surface = malloc(sizeof(SDL_Surface*) * 4);
    character->surface[DOWN] = IMG_Load("assets/sprites/bman-front.png");
    character->surface[LEFT] = IMG_Load("assets/sprites/bman-left.png");
    character->surface[UP] = IMG_Load("assets/sprites/bman-back.png");
    character->surface[RIGHT] = IMG_Load("assets/sprites/bman-right.png");

	return character;
}

void add_character_to_list(t_character **character_list, int x, int y)
{
    t_character_node *new_node = malloc(sizeof(*new_node));
    t_character *new_character = create_character(x, y);

	new_node->character = new_character;
	new_node->next = *character_list;
	*character_list = new_node;
}

bool is_character_in_flames(t_character *character, int **map)
{
    if (map[character->y][character->x] == FLAMES)
        return true;
        
    return false;
}

void remove_character_by_index(t_character_node **head, int n) {
    int i = 0;
    t_character_node *current = (*head);
    t_character_node *node_to_remove = NULL;

    if (n == 0) {
        free(*head);
        *head = NULL;
        return NULL;
    }

    for (i = 0; i < n - 1; i++)
        current = current->next;

    node_to_remove = current->next;
    current->next = node_to_remove->next;
    free(node_to_remove);
}

void draw_characters_on_screen(SDL_Surface *screen, t_character_node **character_list, int **map)
{
    int i = 0;
    t_character_node *current = *character_list;
    Mix_Chunk *game_over = NULL;
    game_over = Mix_LoadWAV("assets/son/game_over.wav");
    while (current != NULL)
    {
        if (is_character_in_flames(current->character, map))
            current->character->life = 0;
        
        if (current->character->life > 0)
        {
            SDL_BlitSurface(
                current->character->surface[current->character->current_direction],
                NULL,
                screen,
                &current->character->screen_position
            );

        } else {
            Mix_PlayChannel(-1, game_over, 0);
            remove_character_by_index(character_list, i);
        }
        current = current->next;
        i++;
    }
}

void moveCharacter(int **map, t_character *character, t_bomb_node *bomb_list, int direction)
{
    switch(direction)
    {
        case UP:
            character->current_direction = UP;
            if ((character->y - 1 < 0) || (map[character->y - 1][character->x] != EMPTY))
                break;
            else if (is_bomb_at_position(bomb_list, character->x, character->y - 1))
                break;
            character->y--;
            character->screen_position.y = character->y * BLOCK_SIZE - BLOCK_SIZE;
            break;

        case DOWN:
            character->current_direction = DOWN;
            if ((character->y + 1 >= NB_BLOCK_HEIGHT) || (map[character->y + 1][character->x] != EMPTY))
                break;
            else if (is_bomb_at_position(bomb_list, character->x, character->y + 1))
                break;
            character->y++;
            character->screen_position.y = character->y * BLOCK_SIZE - BLOCK_SIZE;
            break;

        case LEFT:
            character->current_direction = LEFT;
            if ((character->x - 1 < 0) || (map[character->y][character->x - 1] != EMPTY))
                break;
            else if (is_bomb_at_position(bomb_list, character->x - 1, character->y))
                break;
            character->x--;
            character->screen_position.x = character->x * BLOCK_SIZE;
            break;

        case RIGHT:
            character->current_direction = RIGHT;
            if ((character->x + 1 >= NB_BLOCK_WIDTH) || (map[character->y][character->x + 1] != EMPTY))
                break;
            else if (is_bomb_at_position(bomb_list, character->x + 1, character->y))
                break;
            character->x++;
            character->screen_position.x = character->x * BLOCK_SIZE;
            break;
    }
}