#!/usr/bin/env ruby
# To use type parse.rb in the same folder as gl-abstraction.h
# and pipe the output to a file
#
# Assemble the gl function call
#
# turns this: GLBoolean BlendEquation(GLEnum mode);
#
# into an inline function like this:
#
# GLBoolean BlendEquation(GLEnum mode)
# {
#    return glBlendEquation(mode);
# }

f = File.new("x11-gles.h")
#scan each line in the file
f.each do |x|

  # x is original line, y is the new gl function call
  y = String.new(x)

  y.lstrip!		     # strip leading white spaces
  returns = y.index("void"); # see if the function returns void
  y.gsub!'void',' '          # delete the return types ...
  y.gsub!(/GL[a-z]*/, '')    # remove GL types such as GLenum
  y.gsub!('const','')        # remove const, *
  y.gsub!('char','')         # remove char
  y.gsub!('*','')            # remove pointer *
  y.gsub!(/\s+/,"")          # remove all spaces
  y.insert(0,'  gl')         # add gl to function name

  if (returns != 0)          # insert a return if the function returns
   y.lstrip!
   y.insert(0,'  return ')
  end

  # print the function out
  y << "\n"
  x.lstrip!
  x.gsub!(';','')
  print x
  print "{\n"
  print y
  print "}\n\n"
end



