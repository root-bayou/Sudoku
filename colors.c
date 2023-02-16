#include "colors.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*preface :
(a set = a set of color) i.e the presence of a
*character in a set is coded by 1,
*its absence 0, the whole is coded in an integer of 64 bits */
/*the function initializes to 1 all the possibilities
for a cell according to the size of the grid  */

colors_t colors_full(const size_t size)
{
  colors_t color;
  color = 0xFFFFFFFFFFFFFFFF;
  color >>= 63 - (size - 1);
  return color;
}

/*it simply returns 0*/
colors_t colors_empty(void)
{
  return 0;
}

/*the function sets to 1 the given index
 and set to 0 all the others*/
colors_t colors_set(const size_t colors_id)
{
  colors_t color = 1;
  if (colors_id > 63)
  {
    return colors_empty();
  }
  return (color <<= colors_id);
}

/*the function adds a 1 in a given index*/
colors_t colors_add(const colors_t colors, const size_t colors_id)
{
  if (colors_id > 63)
  {
    return colors_empty();
  }
  colors_t color;
  color = colors_set(colors_id);
  return (color |= colors);
}

/*the function remove a 1 in a given index*/
colors_t colors_discard(const colors_t colors, const size_t colors_id)
{
  if (colors_id > 63)
  {
    return colors_empty();
  }
  colors_t color;
  color = colors_set(colors_id);
  color ^= colors;
  return (color &= colors);
  ;
}

/*the function searches if a character is in the set */
bool colors_is_in(const colors_t colors, const size_t colors_id)
{
  colors_t color;
  color = colors_set(colors_id);
  if (colors_id > 63)
  {
    return false;
  }

  return ((color & colors) == color);
}

/*the function puts in negation a given set*/
colors_t colors_negate(const colors_t colors)
{
  colors_t color;
  return (color = ~colors);
}

/*the function returns 'AND' between two sets*/
colors_t colors_and(const colors_t colors1, const colors_t colors2)
{
  colors_t color;
  return (color = (colors1 & colors2));
}

/*the function returns 'OR' between two sets*/
colors_t colors_or(const colors_t colors1, const colors_t colors2)
{
  colors_t color;
  return (color = (colors1 | colors2));
}

/*the function returns 'XOR' between two sets*/
colors_t colors_xor(const colors_t colors1, const colors_t colors2)
{
  colors_t color;
  return (color = (colors1 ^ colors2));
}

/*the function subtracts from set 1 the set 2*/
colors_t colors_subtract(const colors_t colors1, const colors_t colors2)
{

  colors_t color = 0;
  color = colors1 ^ colors2;
  return (color &= colors1);
}

/*the function checks if two sets are equal*/
bool colors_is_equal(const colors_t colors1, const colors_t colors2)
{
  return ((colors1 ^ colors2) == 0);
}

/*the function checks if the set 1 is included in the set 2*/
bool colors_is_subset(const colors_t colors1, const colors_t colors2)
{
  return (colors_and(colors1, colors2) == colors1);
}

/*the function checks if the set
(seen as an integer coded on 64 bits)* is a power of 2*/
bool colors_is_singleton(const colors_t colors)
{
  colors_t color = colors;
  return ((color != 0) && ((color & (color - 1)) == 0));
}

/*the function counts the number of times 1 appears in the set*/
size_t colors_count(const colors_t colors)
{

  size_t count = 0;
  colors_t color_id, color;

  for (size_t i = 0; i < MAX_COLORS; i++)
  {
    color_id = colors_set(i);
    color = colors_subtract(colors, color_id);
    if (color != colors)
    {
      count++;
    }
  }

  return count;
}

/*the function returns the least(right) significant bits*/
colors_t colors_rightmost(const colors_t colors)
{
  colors_t color;
  return (color = colors & (-colors));
}

/*the function returns the most(left) significant bits */
colors_t colors_leftmost(const colors_t colors)
{
  colors_t color = colors;
  size_t most_sigbit = 0;
  if (color == 0)
  {
    return colors_empty();
  }
  color >>= 1;
  while (color != 0)
  {
    color >>= 1;
    most_sigbit++;
  }

  color = colors_set(0);
  color <<= most_sigbit;

  return color;
}

size_t colors_position(const colors_t colors)
{
  size_t pos_ofbit = 0, k = 0;
  while ((colors ^ colors_set(k)) != colors_empty())
  {
    pos_ofbit++;
    k++;
  }

  return pos_ofbit;
}

/*the function returns a random set based on a given set*/
colors_t colors_random(const colors_t colors)
{
  colors_t color, rand_colors = colors;
  size_t pos_ofbit = 0;
  size_t i = 0, r = 0;
  // we look for the position of the most significant bit
  pos_ofbit = colors_position(colors);
  if (colors == 0)
  {
    return colors_empty();
  }
  for (size_t i = 0; i < pos_ofbit; i++)
  {
    r = rand() % pos_ofbit;
    color = colors_set(r);
    rand_colors ^= color;
  }

  return colors_leftmost(rand_colors);
}

bool subgrid_consistency(colors_t subgrid[], const size_t size)
{
  colors_t colors_c = 0, colors_f;
  colors_f = colors_full(size);

  for (size_t i = 0; i < size; i++)
  {
    colors_c = colors_or(colors_c, subgrid[i]);
    if (subgrid[i] == colors_empty())
    {
      return false;
    }
    if (colors_is_singleton(subgrid[i]))
    {
      for (size_t j = 0; j < size; j++)
      {
        if (j == i)
        {
          continue;
        }
        else if (colors_is_equal(subgrid[i], subgrid[j]))
        {
          return false;
        }
        else
        {
          continue;
        }
      }
    }
  }
  if (colors_is_equal(colors_c, colors_f))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool subgrid_heuristics(colors_t *subgrid[], size_t size)
{
  bool change = false;
  colors_t colors = 0, colors_id;
  size_t occur[size], count = 0, count_n = 0;

  for (size_t i = 0; i < size; i++)
  {
    occur[i] = 0;
  }

  for (size_t k = 0; k < size; k++)
  {
    for (size_t j = 0; j < size; j++)
    {
      if (colors_is_in((colors_t)subgrid[k], j))
      {
        occur[j] = occur[j] + 1;
      }
    }
  }

  for (size_t i = 0; i < size; i++)
  {
    count_n = 0;
    if (colors_is_singleton((colors_t)subgrid[i]))
    {
      colors = (colors_t)subgrid[i];

      for (size_t j = 0; j < size; j++)
      {
        if (i == j)
        {
          continue;
        }
        if (colors_is_in((colors_t)subgrid[j], colors_position(colors)))
        {
          subgrid[j] = colors_xor((colors_t)subgrid[j], (colors_t)subgrid[i]);
          printf("OK1 \n");
          change = true;
        }
      }
    }
    else
    {/*
      count = colors_count((colors_t)subgrid[i]);
      for (size_t k = 0; k < size; k++)
      {
        if (colors_is_equal((colors_t)subgrid[i], (colors_t)subgrid[k]))
        {
          colors = (colors_t)subgrid[k];
          count_n++;
        }
      }
      if (count == count_n)
      {
        for (size_t k = 0; k < size; k++)
        {
          if ((k == i) || colors_is_singleton((colors_t)subgrid[k]))
          {
            continue;
          }
          if (colors_is_equal((colors_t)subgrid[k], colors) || !colors_is_subset(colors, (colors_t)subgrid[k]))
          {
            continue;
          }
          subgrid[k] = colors_xor((colors_t)subgrid[k], colors);
          change = true;
          printf("OK2 \n");
        }
      }*/
    }
    count = 0;
    for (size_t k = 0; k < size; k++)
     {
       if (occur[k] == 1)
       {
         count++;
         colors_id = k;
       }
     }

     if (count == 1)
     {
       colors = colors_set(colors_id);
       for (size_t k = 0; k < size; k++)
       {
         if (colors_is_in((colors_t)subgrid[k], colors_id))
         {
           subgrid[k] = colors;
           printf("OK3 \n");
           change = true;
         }
       }
     }
  }
  return change;
}
