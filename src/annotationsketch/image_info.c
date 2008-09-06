/*
  Copyright (c) 2008 Sascha Steinbiss <ssteinbiss@zbh.uni-hamburg.de>
  Copyright (c) 2008 Center for Bioinformatics, University of Hamburg

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

#include "core/array.h"
#include "core/ensure.h"
#include "core/ma.h"
#include "core/mathsupport.h"
#include "core/range.h"
#include "core/strand.h"
#include "core/unused.h"
#include "extended/feature_type_factory_builtin.h"
#include "annotationsketch/image_info.h"

struct GT_ImageInfo {
  Array* recmaps;
  unsigned int height;
};

GT_ImageInfo* gt_image_info_new()
{
  GT_ImageInfo *ii;
  ii = ma_calloc(1, sizeof (GT_ImageInfo));
  ii->recmaps = array_new(sizeof (GT_RecMap*));
  assert(ii->recmaps);
  return ii;
}

void gt_image_info_delete(GT_ImageInfo *ii)
{
  unsigned long i;
  if (!ii) return;
  for (i=0;i<gt_image_info_num_of_recmaps(ii);i++)
  {
    GT_RecMap *rm = *(GT_RecMap**) array_get(ii->recmaps, i);
    gt_recmap_delete(rm);
  }
  array_delete(ii->recmaps);
  ma_free(ii);
}

void gt_image_info_add_recmap(GT_ImageInfo *ii, GT_RecMap *rm)
{
  assert(ii && rm);
  array_add(ii->recmaps, rm);
}

void gt_image_info_set_height(GT_ImageInfo *ii, unsigned int height)
{
  assert(ii);
  ii->height = height;
}

unsigned int gt_image_info_get_height(GT_ImageInfo *ii)
{
  assert(ii);
  return ii->height;
}

unsigned long gt_image_info_num_of_recmaps(GT_ImageInfo *ii)
{
  assert(ii);
  return array_size(ii->recmaps);
}

GT_RecMap* gt_image_info_get_recmap(GT_ImageInfo *ii, unsigned long n)
{
  assert(ii);
  return *(GT_RecMap**) array_get(ii->recmaps, n);
}

int gt_image_info_unit_test(Error *err)
{
  GT_RecMap* rms[20];
  GenomeNode* gfs[20];
  FeatureTypeFactory *ftf;
  GenomeFeatureType *gft;
  GT_ImageInfo *ii;
  unsigned long i;
  Str *seqid;
  int had_err = 0;
  assert(err);
  error_check(err);

  seqid = str_new_cstr("seqid");
  ii = gt_image_info_new();
  ftf = feature_type_factory_builtin_new();
  gft = feature_type_factory_create_gft(ftf, "gene");

  for (i=0;i<20;i++)
  {
    GT_RecMap* rm;
    unsigned long rbase;
    rbase = rand_max(10);
    Range r = {rbase,rbase+rand_max(20)};
    gfs[i] = (GenomeNode*) genome_feature_new(seqid, gft, r, STRAND_FORWARD);
    rms[i] = gt_recmap_create(rand_max_double(100.0),
                           rand_max_double(100.0),
                           rand_max_double(100.0),
                           rand_max_double(100.0),
                           gfs[i]);
    gt_image_info_add_recmap(ii, rms[i]);
    ensure(had_err, gt_image_info_num_of_recmaps(ii) == i+1);
    ensure(had_err, (rm = gt_image_info_get_recmap(ii, i)) == rms[i]);
    ensure(had_err, rm->gn == rms[i]->gn);
    genome_node_delete((GenomeNode*) gfs[i]);
  }

  gt_image_info_delete(ii);
  feature_type_factory_delete(ftf);
  str_delete(seqid);

  return had_err;
}
