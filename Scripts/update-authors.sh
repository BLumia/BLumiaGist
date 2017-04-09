#!/bin/sh

# AUTHORS generate script
# Modified form GitHub:nodejs/node, tools/update-authors.sh
# Ref: https://github.com/nodejs/node/blob/master/tools/update-authors.sh

git log --reverse --format='%aN <%aE>' | perl -we '

BEGIN {
  %seen = (), @authors = ();
}

while (<>) {
  next if $seen{$_};
  next if /forfun/;
  next if /Gary Wang <wzc782970009\@/;
  $seen{$_} = push @authors, $_;
}

END {
  print "# Authors ordered by first contribution.\n";
  print "\n", @authors, "\n";
  print "# BLumiaOJ (c) 2015 ~ 2017\n";
}

' > AUTHORS
