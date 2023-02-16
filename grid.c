#include "grid.h"

struct _grid_t
{
  size_t size;
  colors_t **cells;
};

struct choice_t
{
  size_t row;
  size_t column;
  colors_t colors;
};

/*function for dynamic grid allocation*/
grid_t *grid_alloc(size_t size)
{
  if (!grid_check_size(size))
  {
    return NULL;
  }

  grid_t *grid;
  grid = calloc(1, sizeof(struct _grid_t));
  if (grid == NULL)
  {
    return NULL;
  }

  grid->cells = calloc(size, sizeof(colors_t *));
  if (grid->cells == NULL)
  {
    return NULL;
  }

  for (size_t i = 0; i < size; i++)
  {
    grid->cells[i] = calloc(size, sizeof(colors_t));

    if (grid->cells[i] == NULL)
    {
      return NULL;
    }
  }

  grid->size = size;
  return grid;
}

/*function used for free the memory*/
void grid_free(grid_t *grid)
{
  if (grid == NULL)
  {
    return;
  }

  for (size_t i = 0; i < grid->size; i++)
  {
    free(grid->cells[i]);
  }
  free(grid->cells);
  free(grid);
}

/*the function print the grid in the output file given (default is stdout)*/
void grid_print(const grid_t *grid, FILE *fd)
{
  bool color_isempty = false;
  size_t k = 0;
  colors_t color = 0;
  if (grid != NULL)
  {
    for (size_t i = 0; i < grid->size; i++)
    {
      for (size_t j = 0; j < grid->size; j++)
      {
        k = 0;
        color = grid->cells[i][j];
        while (k < grid->size)
        {
          if (colors_is_in(color, k))
          {
            color_isempty = false;
            break;
          }

          color_isempty = true;
          k++;
        }

        if (!color_isempty)
        {
          if (!colors_is_singleton(color))
          {
            fprintf(fd, "%c ", EMPTY_CELL);
          }
          else
          {
            fprintf(fd, "%c ", color_table[k]);
          }
        }
        else
        {
          fprintf(fd, "%c ", EMPTY_CELL);
        }
      }
      fprintf(fd, "\n");
    }
  }
}

void grid_print_full(const grid_t *grid, FILE *fd)
{
  size_t k = 0;
  colors_t color = 0;
  if (grid != NULL)
  {
    for (size_t i = 0; i < grid->size; i++)
    {

      for (size_t j = 0; j < grid->size; j++)
      {

        fprintf(fd, "'%s' ", grid_get_cell(grid, i, j));
      }
      fprintf(fd, "\n");
    }
  }
}

/*the function test if the given char are valide*/
bool grid_check_char(const grid_t *grid, const int c)
{
  for (size_t i = 0; i < grid->size; i++)
  {
    if (c == color_table[i] || c == EMPTY_CELL)
    {
      return true;
    }
  }
  return false;
}

/*the function chech if the size of grid are valide*/
bool grid_check_size(size_t size)
{

  bool s = false;
  for (size_t i = 0; i < 8; i++)
  {
    if (size == valid_size[i])
    {
      s = true;
      break;
    }
  }

  return s;
}

/*the function copies the grid to a new memory space*/
grid_t *grid_copy(const grid_t *grid)
{
  if (grid == NULL)
  {
    return NULL;
  }

  grid_t *grid_c;
  grid_c = grid_alloc(grid->size);

  if (grid_c == NULL)
  {
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < grid->size; i++)
  {
    for (size_t j = 0; j < grid->size; j++)
    {
      grid_c->cells[i][j] = grid->cells[i][j];
    }
  }
  return grid_c;
}

/*the function return the size of grid*/
size_t grid_get_size(const grid_t *grid)
{
  if (grid == NULL)
  {
    return 0;
  }
  return grid->size;
}

/*get a string from the set of colors */
char *grid_get_cell(const grid_t *grid, const size_t row, const size_t column)
{
  if ((grid == NULL) || (row > grid->size) || (column > grid->size))
  {
    return NULL;
  }

  char *color_ch;
  color_ch = calloc(MAX_COLORS, sizeof(char));
  colors_t color = grid->cells[row][column];
  size_t pos = 0;
  for (size_t i = 0; i < grid->size; i++)
  {
    if (colors_is_in(color, i))
    {
      color_ch[pos] = color_table[i];
      pos++;
    }
  }

  return color_ch;
}

/*set a char (color) in the given cell*/
void grid_set_cell(grid_t *grid, const size_t row,
                   const size_t column, const char color)
{

  size_t pos_char = 0;
  if (grid != NULL && row <= grid->size && column <= grid->size)
  {
    for (size_t i = 0; i < MAX_COLORS; i++)
    {
      if ((color == color_table[i]))
      {
        pos_char = i;
      }
    }
    grid->cells[row][column] = colors_add(grid->cells[row][column], pos_char);

    if (color == EMPTY_CELL)
    {
      grid->cells[row][column] = colors_discard(grid->cells[row][column],
                                                pos_char);
    }
  }
}

bool grid_is_solved(grid_t *grid)
{
  for (size_t i = 0; i < grid->size; i++)
  {
    for (size_t j = 0; j < grid->size; j++)
    {
      if (!colors_is_singleton(grid->cells[i][j]))
      {
        return false;
      }
    }
  }
  return true;
}

void set_full(grid_t *grid)
{
  for (size_t i = 0; i < grid->size; i++)
  {
    for (size_t j = 0; j < grid->size; j++)
    {
      if (grid->cells[i][j] == 0)
      {
        grid->cells[i][j] = colors_full(grid->size);
      }
    }
  }
}
void get_sub_row(const grid_t *grid, colors_t *sub_row[], const size_t pos_row)
{
  for (size_t j = 0; j < grid->size; j++)
  {
    sub_row[j] = (colors_t)grid->cells[pos_row][j];
  }
}

void get_sub_column(const grid_t *grid, colors_t *sub_row[],
                    const size_t pos_col)
{
  for (size_t j = 0; j < grid->size; j++)
  {
    sub_row[j] = (colors_t)grid->cells[j][pos_col];
  }
}

bool grid_is_consistent(grid_t *grid)
{
  size_t i = 0, j = 0, t = 0, pos_i, pos_j, pos = 0;
  colors_t subgride[grid->size];
  set_full(grid);
  for (size_t i = 0; i < grid->size; i++)
  {
    get_sub_row(grid, &subgride, i);
    if (!subgrid_consistency(subgride, grid->size))
    {
      return false;
    }
  }

  for (size_t i = 0; i < grid->size; i++)
  {
    get_sub_column(grid, &subgride, i);
    if (!subgrid_consistency(subgride, grid->size))
    {
      return false;
    }
  }

  while (i < grid->size)
  {
    pos_i = i;
    while (j < grid->size)
    {
      pos_j = j;
      for (size_t k = 0; k < sqrt(grid->size); k++)
      {
        for (size_t l = 0; l < sqrt(grid->size); l++)
        {
          subgride[t] = grid->cells[i][j];
          j++;
          t++;
        }
        i++;
        j = pos_j;
      }
      if (!subgrid_consistency(subgride, grid->size))
      {
        return false;
      }
      j = pos_j + sqrt(grid->size);
      i = pos_i;
      pos++;
      t = 0;
    }
    i = pos_i + sqrt(grid->size);
    j = 0;
  }

  return true;
}

size_t grid_heuristics(grid_t *grid)
{
  colors_t subgride[grid->size];
  size_t i = 0, j = 0, t = 0, pi, pj;
  bool change = false;

  for (size_t i = 0; i < grid->size; i++)
  {
    get_sub_row(grid, &subgride, i);
    if (subgrid_heuristics(&subgride, grid->size))
    {
      change = true;
      for (size_t j = 0; j < grid->size; j++)
      {
        grid->cells[i][j] = subgride[j];
      }
    }
  }
  for (size_t i = 0; i < grid->size; i++)
  {
    get_sub_column(grid, &subgride, i);
    if (subgrid_heuristics(&subgride, grid->size))
    {
      change = true;
      for (size_t j = 0; j < grid->size; j++)
      {
        grid->cells[j][i] = subgride[j];
      }
    }
  }
  while (i < grid->size)
  {
    pi = i;
    while (j < grid->size)
    {
      pj = j;
      for (size_t k = 0; k < sqrt(grid->size); k++)
      {
        for (size_t l = 0; l < sqrt(grid->size); l++)
        {

          subgride[t] = grid->cells[i][j];
          j++;
          t++;
        }
        i++;
        j = pj;
      }
      if (subgrid_heuristics(&subgride, grid->size))
      {
        change = true;
        i = pi;
        j = pj;
        t = 0;
        for (size_t k = 0; k < sqrt(grid->size); k++)
        {
          for (size_t l = 0; l < sqrt(grid->size); l++)
          {
            grid->cells[i][j] = subgride[t];
            j++;
            t++;
          }
          i++;
          j = pj;
        }
      }
      j = pj + sqrt(grid->size);
      i = pi;
      t = 0;
    }
    i = pi + sqrt(grid->size);
    j = 0;
  }

  grid_print(grid,stdout);
  if (change == false && grid_is_consistent(grid))
  {
    return 3;
  }

  if (!grid_is_solved(grid) && grid_is_consistent(grid))
  {
    return 0;
  }

  if (grid_is_solved(grid) && grid_is_consistent(grid))
  {
    return 1;
  }

  if (!grid_is_consistent(grid))
  {
    return 2;
  }
}

void grid_choice_free(choice_t *choice)
{
  if (choice == NULL)
  {
    return;
  }
  free(choice->colors);
  free(choice);
}

bool grid_choice_is_empty(const choice_t *choice)
{

  if (choice->colors == 0)
  {
    return true;
  }

  return false;
}

void grid_choice_apply(grid_t *grid, const choice_t *choice)
{
  grid->cells[choice->row][choice->column] = choice->colors;
}

void grid_choice_blank(grid_t *grid, const choice_t *choice)
{
  grid->cells[choice->row][choice->column] = colors_full(grid->size);
}
void grid_choice_discard(grid_t *grid, const choice_t *choice)
{
  printf("(%ld , %ld) = '%s' --> ",
         choice->row + 1, choice->column + 1, grid_get_cell(grid, choice->row, choice->column));

  grid->cells[choice->row][choice->column] =
      colors_subtract(grid->cells[choice->row][choice->column], choice->colors);

  printf("'%s' subtract '%c' \n", grid_get_cell(grid, choice->row, choice->column),
         color_table[colors_position(choice->colors)]);
}

choice_t *grid_choice_alloc()
{
  choice_t *choice;
  choice = calloc(1, sizeof(struct choice_t));
  choice->colors = calloc(1, sizeof(colors_t));

  return choice;
}
choice_t *grid_choice(grid_t *grid)
{
  choice_t *choice;
  choice = grid_choice_alloc();

  choice->colors = colors_full(grid->size);
  for (size_t i = 0; i < grid->size; i++)
  {
    for (size_t j = 0; j < grid->size; j++)
    {
      if (colors_is_singleton(grid->cells[i][j]))
      {
        continue;
      }
      if (colors_count(grid->cells[i][j]) <= (colors_count(choice->colors)))
      {
        choice->row = i;
        choice->column = j;
        choice->colors = grid->cells[i][j];
      }
    }
  }

  fprintf(stdout, "Next choice at grid[%ld][%ld] = '%s' and choice is '%c' \n",

          choice->row + 1, choice->column + 1, grid_get_cell(grid, choice->row, choice->column),
          color_table[colors_position(colors_leftmost(choice->colors))]);

  choice->colors = colors_leftmost(choice->colors);

  return choice;
}

void grid_simple_copy(grid_t *grid, grid_t *grid_c)
{
  for (size_t i = 0; i < grid->size; i++)
  {
    for (size_t j = 0; j < grid->size; j++)
    {
      grid->cells[i][j] = grid_c->cells[i][j];
    }
  }
}