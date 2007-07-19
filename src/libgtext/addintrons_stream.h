/*
  Copyright (c) 2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg
  See LICENSE file or http://genometools.org/license.html for license details.
*/

#ifndef ADDINTRONS_STREAM_H
#define ADDINTRONS_STREAM_H

#include <stdio.h>
#include "libgtext/genome_stream.h"

/* implements the ``genome_stream'' interface */
typedef struct AddIntronsStream AddIntronsStream;

const GenomeStreamClass* addintrons_stream_class(void);
GenomeStream*            addintrons_stream_new(GenomeStream*, Env*);

#endif
