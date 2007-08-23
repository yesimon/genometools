/*
  Copyright (c) 2007 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg
  See LICENSE file or http://genometools.org/license.html for license details.
*/

#include "lauxlib.h"
#include "gtlua.h"
#include "libgtcore/fileutils.h"
#include "libgtext/genome_stream.h"
#include "libgtext/genome_stream_lua.h"
#include "libgtext/gff3_in_stream.h"

#define GENOME_STREAM_METATABLE  "GenomeTools.genome_stream"
#define check_genome_stream(L) \
        (GenomeStream**) luaL_checkudata(L, 1, GENOME_STREAM_METATABLE);

static int gff3_in_stream_lua_new_sorted(lua_State *L)
{
  GenomeStream **gs;
  const char *filename;
  Env *env;
  assert(L);
  /* get/check parameters */
  filename = luaL_checkstring(L, 1);
  luaL_argcheck(L, file_exists(filename), 1, "file does not exist");
  /* construct object */
  env = get_env_from_registry(L);
  gs = lua_newuserdata(L, sizeof (GenomeStream**));
  *gs = gff3_in_stream_new_sorted(filename, false, env);
  assert(*gs);
  luaL_getmetatable(L, GENOME_STREAM_METATABLE);
  lua_setmetatable(L, -2);
  return 1;
}

static int genome_stream_lua_next_tree(lua_State *L)
{
#if 0
  GenomeStream **gs = check_genome_stream(L);
  lua_pushstring(L, genome_stream_get_filename(*gs));
#endif
  return 1;
}

static int genome_stream_lua_delete(lua_State *L)
{
  GenomeStream **gs = check_genome_stream(L);
  Env *env;
  env = get_env_from_registry(L);
  genome_stream_delete(*gs, env);
  return 0;
}

static const struct luaL_Reg genome_stream_lib_f [] = {
  { "gff3_in_stream_new_sorted", gff3_in_stream_lua_new_sorted },
  { NULL, NULL }
};

static const struct luaL_Reg genome_stream_lib_m [] = {
  { "next_tree", genome_stream_lua_next_tree },
  { NULL, NULL }
};

int luaopen_genome_stream(lua_State *L)
{
  assert(L);
  luaL_newmetatable(L, GENOME_STREAM_METATABLE);
  /* metatable.__index = metatable */
  lua_pushvalue(L, -1); /* duplicate the metatable */
  lua_setfield(L, -2, "__index");
  /* set its _gc field */
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, genome_stream_lua_delete);
  lua_settable(L, -3);
  /* register functions */
  luaL_register(L, NULL, genome_stream_lib_m);
  luaL_register(L, "gt", genome_stream_lib_f);
  return 1;
}
