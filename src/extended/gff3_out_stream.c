/*
  Copyright (c) 2006-2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2006-2007 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "extended/gff3_out_stream.h"
#include "extended/gff3_visitor.h"
#include "extended/node_stream_rep.h"

struct GFF3OutStream {
  const GtNodeStream parent_instance;
  GtNodeStream *in_stream;
  GtNodeVisitor *gff3_visitor;
};

#define gff3_out_stream_cast(GS)\
        gt_node_stream_cast(gff3_out_stream_class(), GS);

static int gff3_out_stream_next_tree(GtNodeStream *gs, GtGenomeNode **gn,
                                     GtError *err)
{
  GFF3OutStream *gff3_out_stream;
  int had_err;
  gt_error_check(err);
  gff3_out_stream = gff3_out_stream_cast(gs);
  had_err = gt_node_stream_next(gff3_out_stream->in_stream, gn, err);
  if (!had_err && *gn)
    had_err = gt_genome_node_accept(*gn, gff3_out_stream->gff3_visitor, err);
  return had_err;
}

static void gff3_out_stream_free(GtNodeStream *gs)
{
  GFF3OutStream *gff3_out_stream = gff3_out_stream_cast(gs);
  gt_node_stream_delete(gff3_out_stream->in_stream);
  gt_node_visitor_delete(gff3_out_stream->gff3_visitor);
}

const GtNodeStreamClass* gff3_out_stream_class(void)
{
  static const GtNodeStreamClass gsc = { sizeof (GFF3OutStream),
                                         gff3_out_stream_next_tree,
                                         gff3_out_stream_free };
  return &gsc;
}

GtNodeStream* gff3_out_stream_new(GtNodeStream *in_stream, GtGenFile *outfp)
{
  GtNodeStream *gs = gt_node_stream_create(gff3_out_stream_class(),
                                          gt_node_stream_is_sorted(in_stream));
  GFF3OutStream *gff3_out_stream = gff3_out_stream_cast(gs);
  gff3_out_stream->in_stream = gt_node_stream_ref(in_stream);
  gff3_out_stream->gff3_visitor = gff3_visitor_new(outfp);
  return gs;
}

void gff3_out_stream_set_fasta_width(GtNodeStream *gs,
                                     unsigned long fasta_width)
{
  GFF3OutStream *gff3_out_stream = gff3_out_stream_cast(gs);
  assert(gff3_out_stream);
  gff3_visitor_set_fasta_width(gff3_out_stream->gff3_visitor, fasta_width);
}
