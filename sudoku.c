#include "sudoku.h"
#include "colors.h"
#include "grid.h"

#include <err.h>
#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

#define MAX_GRID_SIZE 64

static const int SIZE_OF_GRIDS[8] = {1, 4, 9, 16, 25, 36, 49, 64};
/*declaration of structure and enumurate */

typedef enum which_mode
{
  gen,
  sol,
  not_set
} w_mode_t;

typedef enum mode
{
  mode_first,
  mode_all
} mode_t;

/*declaration of global variables*/
static bool verbose = false;
/*the help function :o*/
static void print_help(FILE *stream, int exit_code)
{
  if (!exit_code)
  {
    fprintf(stream, "------------------------\n*\t SUDOKU "
                    "\t*\n------------------------\n"
                    "Solve or generate Sudoku grids of various sizes (1.4.9.16.25.36.49.64)\n");
  }

  fprintf(stream, "\nUsage : sudoku [-a|-o FILE|-v|-V|-h] ... FILE\n"
                  " \tsudoku -g[SIZE] [-u|-o FILE|-v|-V|-h]\n");
  if (!exit_code)
  {
    {

      fprintf(stream, "\n-a,--all \t \t search for all possible solutions\n"
                      "-g[N],--generate[=N]\t generate a grid of size N x N (default : 9)\n"
                      "-o FILE, --output FILE\t write solution to FILE\n"
                      "-u,--unique \t \t generate a grid with unique solution\n"
                      "-v,--verbose \t \t verbose output\n"
                      "-V,--version \t \t display version and exit\n"
                      "-h,--help    \t \t display help\n");
    }

    exit(exit_code);
  }
}

/*the function checks if the value is valid (I will improve it)*/

static bool check_if_valide(char *size_grid)
{
  int i;
  bool k;

  if (strlen(size_grid) <= 2)
  {

    if (strlen(size_grid) == 2)
    {
      i = (((int)size_grid[0] - '0') * 10) + ((int)size_grid[1] - '0');
    }
    else
    {
      i = (int)size_grid[0] - '0';
    }

    if (i > 0 && i <= 64)
    {

      for (size_t j = 0; j < 8; j++)
      {
        if (i == SIZE_OF_GRIDS[j])
        {

          k = true;
          break;
        }
        else
        {
          k = false;
        }
      }
    }
    else
    {
      k = false;
    }
  }
  else
  {
    k = false;
  }

  return k;
}

/*set the output stream*/
static FILE *set_defaut_stream(char *file)
{

  if (file == NULL)
  {
    warnx("warning : No output file given the flow will be "
          "redirected to the standard output\n");
    return stdout;
  }
  if (fopen(file, "a+") == NULL)
  {
    warnx("warning : Unable to open the file in writing mode the "
          "flow will be redirected to the standard output\n");
    return stdout;
  }
  else
  {
    return fopen(file, "a+");
  }
}

/*the function check if the size given are valid*/
static bool check_quick(size_t size)
{
  bool k = false;
  for (size_t j = 0; j < 8; j++)
  {
    if (size == (size_t)SIZE_OF_GRIDS[j])
    {
      k = true;
      break;
    }
    else
    {
      k = false;
    }
  }

  return k;
}

/*HOLA!! the file parser (I will document it in more detail later )*/
static grid_t *file_parser(FILE *file)
{
  grid_t *grid;
  int c;
  size_t j = 0, t = 0, k = 0, count_mis_row = 0, count_mis_col = 0;
  size_t count_col = 0, row_num = 0;
  do
  {
    c = fgetc(file);
    if (j == 0 && c == '\n')
    {
      continue;
    }

    if (c == EOF || c == '\n')
    {
      break;
    }

    if (c == ' ')
    {
      continue;
    }
    else if (c == '#' && j == 0)
    {
      do
      {
        c = fgetc(file);
      } while (c != '\n' && c != EOF);
    }
    else
    {
      j++;
    }

  } while (c != EOF);

  if (!check_quick(j))
  {
    errx(EXIT_FAILURE, "error : the size %ld is invalid", j);
  }

  rewind(file);

  grid = grid_alloc(j);

  if (grid == NULL)
  {
    errx(EXIT_FAILURE, "error : Cannot allocate memory ");
  }

  do
  {
    c = fgetc(file);
    if (c == '#')
    {
      break;
    }

    while (c != '\n' && c != EOF)
    {
      if (c == ' ')
      {
        c = fgetc(file);
        continue;
      }
      else
      {
        count_col++;
        c = fgetc(file);
      }
    }
    if (count_col > grid_get_size(grid) && row_num < grid_get_size(grid))
    {
      errx(EXIT_FAILURE, "ERROR : Extra charcter in row %ld", row_num + 1);
    }
    else
    {
      row_num++;
      count_col = 0;
    }

  } while (c != EOF);

  rewind(file);

  do
  {
    c = fgetc(file);
    while (t < grid_get_size(grid))
    {
      while (k < grid_get_size(grid))
      {
        if (c == EOF)
        {
          break;
        }
        if (c == '\n' && count_mis_col >= grid_get_size(grid))
        {
          errx(EXIT_FAILURE, "ERROR : missing charcter in row %ld", t + 1);
        }

        if (c == ' ' || c == '\n')
        {
          c = fgetc(file);
          count_mis_col++;
          count_mis_row++;
          continue;
        }

        if (c == '#' && k == 0)
        {
          do
          {
            c = fgetc(file);

          } while (c != '\n' && c != EOF);
          t--;
          k = 0;
          break;
        }

        if (grid_check_char(grid, c) && c != EOF)
        {
          grid_set_cell(grid, t, k, c);
          c = fgetc(file);
          k++;
        }
        else
        {
          errx(EXIT_FAILURE, "ERROR : in Row %ld Column %ld wrong charcter %c", t + 1, k + 1, c);
        }
      }

      k = 0;
      count_mis_col = 0;
      t++;
    }
  } while (c != EOF && t < grid_get_size(grid));
  c = fgetc(file);
  if (c == '#')
  {
    c = EOF;
  }
  if (c != EOF && c != ' ' && c != '\n')
  {
    errx(EXIT_FAILURE, "ERROR : Grid have an extra line(s)");
  }
  return grid;
}

static grid_t *grid_solver_heuristics(grid_t *grid)
{
  size_t n;
  do
  {
    n = grid_heuristics(grid);
    if (n == 1 || n == 3)
    {
      return grid;
    }
  } while (n != 2);

  return NULL;
}

static grid_t *grid_solver(grid_t *grid, mode_t *mode)
{

  grid_t *grid_c, *grid_s;
  grid_c = grid_copy(grid);
  choice_t *choice;
  choice_t *choice_c = NULL;
  size_t n;

  if (grid_is_solved(grid) && grid_is_consistent(grid))
  {
    return grid;
  }

  do
  {
    n = grid_heuristics(grid);
    printf("n = %ld \n",n);
    if (n == 1)
    {
      return grid;
    }
    if (n == 3)
    {
      choice = grid_choice(grid);
      choice_c = choice;
      grid_choice_apply(grid, choice);
    }
  } while (n != 2);
  grid_choice_discard(grid_c, choice);
  grid_free(grid);
  grid_solver(grid_c, mode_first);
}

int main(int argc, char *argv[])
{
  /*declaration of local variable for main function*/
  grid_t *grid_test;
  bool unique_mode = false;
  w_mode_t w_mode = not_set;
  int check_conflict = 0;
  w_mode = sol;
  FILE *default_stream = stdout;
  FILE *default_input_stream = NULL;
  char *output_file_name = NULL;
  char *input_file_name = NULL;
  char *size_of_grid = NULL;
  size_t n = 0;

  srand(time(NULL));

  /*if no argument was given, exit the program*/
  if (argc == 1)
  {
    printf("error : no input file given\n");
    print_help(stderr, EXIT_FAILURE);
  }

  const struct option long_opts[] = {
      {"help", no_argument, NULL, 'h'},
      {"output", required_argument, NULL, 'o'},
      {"verbose", no_argument, NULL, 'v'},
      {"version", no_argument, NULL, 'V'},
      {"unique", no_argument, NULL, 'u'},
      {"generate", optional_argument, NULL, 'g'},
      {"all", no_argument, NULL, 'a'},
      {NULL, 0, NULL, 0}};

  int optc;

  /*we get the options*/
  while ((optc = getopt_long(argc, argv, "o:g::auvVh", long_opts, NULL)) != -1)
  {
    switch (optc)
    {
    case 'V':
      printf("%d.%d.%d \n", VERSION, SUBVERSION, REVISION);
      printf("Solve or generate Sudoku grids of various sizes (1.4.9.16.25.36.49.64)\n");
      exit(EXIT_SUCCESS);
      break;

    case 'h':
      print_help(stdout, EXIT_SUCCESS);
      break;

    case 'v':
      verbose = true;
      break;

    case 'o':
      if (optarg == NULL)
      {
        print_help(stderr, EXIT_FAILURE);
      }
      else
      {
        output_file_name = optarg;
      }
      break;

    case 'g':
      if (optarg == NULL)
      {
        size_of_grid = "9";
      }
      else
      {
        size_of_grid = optarg;
      }

      w_mode = gen;
      check_conflict++;

      break;
    case 'a':
      w_mode = sol;
      check_conflict++;
      break;

    case 'u':
      unique_mode = true;
      break;

    default:

      print_help(stderr, EXIT_FAILURE);
      break;
    }
  }

  if (check_conflict == 2)
  {
    printf("Mode generator conflict with solver mode\n");
    print_help(stderr, EXIT_FAILURE);
  }

  if (w_mode == sol && unique_mode == true)
  {
    warnx("warning : option unique conflict with solver mode, disabling it \n");
    unique_mode = false;
  }

  default_stream = set_defaut_stream(output_file_name);

  /*the body of the solver mode*/
  if (w_mode == sol)
  {
    do
    {
      input_file_name = argv[optind];
      if (input_file_name == NULL)
      {
        errx(EXIT_FAILURE, "error : No input file geven !");
      }
      if (fopen(input_file_name, "r") == NULL)
      {
        errx(EXIT_FAILURE, "error : Unable to open the file");
      }

      default_input_stream = fopen(input_file_name, "r");
      grid_test = file_parser(default_input_stream);
      grid_print(grid_test, default_stream);

      if (grid_is_consistent(grid_test))
      {

        grid_test = grid_solver_heuristics(grid_test);

        if (grid_test == NULL)
        {
          fprintf(default_stream, "\nThe grid is inconsistent! \n");
          optind++;
          continue;
        }

        fprintf(default_stream, "\nThe grid is consistent! \n");

        if (!grid_is_solved(grid_test))
        {
          grid_test = grid_solver(grid_test, mode_first);
        }
      }
      else
      {
        fprintf(stderr, "The grid is inconsistent! \n");
      }

      if (grid_is_solved(grid_test))
      {
        printf("\n");
        grid_print(grid_test, default_stream);
        fprintf(default_stream, "\nThe grid is solved! \n");
        fclose(default_input_stream);
        grid_free(grid_test);
      }
      optind++;
    } while (optind < argc);
  }
  /*the body of the generator mode*/
  if (w_mode == gen)
  {
    if (check_if_valide(size_of_grid))
    {
      default_stream = set_defaut_stream(output_file_name);
    }
    else if (!check_if_valide(size_of_grid))
    {
      errx(EXIT_FAILURE, "error : the size is invalid");
    }
  }

  fclose(default_stream);
  return (EXIT_SUCCESS);
}