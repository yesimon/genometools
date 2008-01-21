--[[
  Copyright (c) 2008 Gordon Gremme <gremme@zbh.uni-hamburg.de>
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
]]

require 'gtdoclib'

function usage()
  io.stderr:write(string.format("Usage: %s gt_home\n", arg[0]))
  io.stderr:write("Generate documentation for the GenomeTools home directory " ..
                  "gt_home.\n")
  os.exit(1)
end

if #arg == 1 then
  gt_home = arg[1]
else
  usage()
end

local export = { "src/libgtlua" }

local doc_parser      = DocParser:new()
local doc_base        = DocBase:new()
local doc_visitor_txt = DocVisitorTxt:new()

for _, v in ipairs(export) do
  if is_dir(v) then
    for f in lfs.dir(v) do
      local filename = v .. "/" .. f
      if is_header(filename) then
        local ast = doc_parser:parse(filename)
        doc_base:process_ast(ast)
      end
    end
  else
    assert(false, "not implemented") -- XXX
  end
end

doc_base:accept(doc_visitor_txt)
