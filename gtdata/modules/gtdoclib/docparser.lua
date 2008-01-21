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

module(..., package.seeall)

require 'lpeg'

DocParser = {}

-- Lexical Elements
local Any            = lpeg.P(1)
local Whitespace     = lpeg.S(" \t\n")
local Character      = lpeg.R("AZ", "az")
local CCommentStart  = lpeg.P("/*")
local CCommentEnd    = lpeg.P("*/")
local Space          = Whitespace^1
local ExportCComment = CCommentStart * lpeg.P(" exports the ") *
                       lpeg.Cc("class") * lpeg.C(Character^1) *
                       lpeg.P(" class to Lua:") * (Any - CCommentEnd)^0  *
                       CCommentEnd
local CComment       = CCommentStart * (Any - CCommentEnd)^0 * CCommentEnd
local Code           = (Any - CCommentStart)^1

-- Grammar
local Elem, S = lpeg.V"Elem", lpeg.V"S"
local G = lpeg.P{ S,
 S    = lpeg.Ct(Elem^0);
 Elem = lpeg.Ct(ExportCComment) + CComment + Space + Code;
}

function DocParser:new()
  o = {}
  o.pattern = G
  setmetatable(o, self)
  self.__index = self
  return o
end

function DocParser:parse(filename)
  assert(filename)
  print("parsing " .. filename)
  local file, err = io.open(filename, "r")
  assert(file, err)
  local filecontent = file:read("*a")
  return lpeg.match(self.pattern, filecontent)
end
