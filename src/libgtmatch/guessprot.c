/*
  Copyright (c) 2007 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg
  See LICENSE file or http://genometools.org/license.html for license details.
*/

#include <ctype.h>
#include <stdbool.h>
#include "libgtcore/env.h"
#include "libgtcore/str.h"
#include "types.h"
#include "inputsymbol.h"
#include "genstream.h"
#include "fbs-def.h"

#include "genericstream.pr"

#include "fbsadv.pr"

#include "readnextUchar.gen"

int guessifproteinsequencestream(const StrArray *filenametab,Env *env)
{
  Seqpos countnonbases = 0,
         currentposition;
  Uchar currentchar;
  Fastabufferstate fbs;
  int retval;

  initfastabufferstate(&fbs,
                       filenametab,
                       NULL,
                       NULL,
                       env);
  for (currentposition = 0; currentposition < (Seqpos) 1000; currentposition++)
  {
    retval = readnextUchar(&currentchar,&fbs,env);
    if (retval < 0)
    {
      return -1;
    }
    if (retval == 0)
    {
      break;
    }
    switch (currentchar)
    {
      case 'L':
      case 'I':
      case 'F':
      case 'E':
      case 'Q':
      case 'P':
      case 'X':
      case 'Z': countnonbases++;
                break;
      default:  break;
    }
  }
  if (countnonbases >= currentposition/10)
  {
    return 1;
  }
  return 0;
}

bool guessifproteinsequencestream2(const char *inputfile)
{
  Fgetcreturntype currentchar;
  Seqpos countnonbases = 0,
         countcharacters = 0;
  bool indesc = false;
  Genericstream inputstream;

  opengenericstream(&inputstream,inputfile);
  for (;;)
  {
    if (inputstream.isgzippedstream)
    {
      currentchar = gzgetc(inputstream.stream.gzippedstream);
    } else
    {
      currentchar = fgetc(inputstream.stream.fopenstream);
    }
    if (indesc)
    {
      if (currentchar == NEWLINESYMBOL)
      {
        indesc = false;
      }
    } else
    {
      if (currentchar == FASTASEPARATOR)
      {
        indesc = true;
      } else
      {
        if (!isspace((Ctypeargumenttype) currentchar))
        {
          countcharacters++;
          switch (currentchar)
          {
            case 'L':
            case 'I':
            case 'F':
            case 'E':
            case 'Q':
            case 'P':
            case 'X':
            case 'Z':
              countnonbases++;
              break;
            default:
              break;
          }
        }
      }
    }
    if (countcharacters >= (Seqpos) 1000)
    {
      break;
    }
  }
  closegenericstream(&inputstream,inputfile);
  if (countnonbases >= countcharacters/10)
  {
    return true;
  }
  return false;
}
