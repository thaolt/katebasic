#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"
#include "Printer.h"
#include "Absyn.h"
#include "codegen.h"

int main(int argc, char ** argv)
{
  FILE *input;
  Program parse_tree;
  if (argc > 1) 
  {
    input = fopen(argv[1], "r");
    if (!input)
    {
      fprintf(stderr, "Error opening input file.\n");
      exit(1);
    }
  }
  else input = stdin;
  /* The default entry point is used. For other options see Parser.h */
  parse_tree = pProgram(input);
  if (parse_tree)
  {
    /*printf("\nParse Succesful!\n");*/
    printf("\n[Abstract Syntax]\n");
    printf("%s\n\n", showProgram(parse_tree));
    /*printf("[Linearized Tree]\n");
    printf("%s\n\n", printProgram(parse_tree)); */
    initCodeGen();
    printf("; Chip8 Assembly\n");
    visitProgram(parse_tree);
    printf("; End Chip8 Assembly\n");
    return 0;
  }
  return 1;
}

