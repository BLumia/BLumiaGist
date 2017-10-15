#!/bin/sh

git log --reverse --format='%aN <%aE>' | perl -we '

BEGIN {
  %seen = (), @authors = ();
}

while (<>) {
  next if $seen{$_};
  next if /forfun/;
  next if /Gary Wang <wzc782970009\@/;
  $seen{$_} = 1;
  if (/wzc782970009/) {
    push @authors, "BLumia <blumia\@qq.com>\n"
  } else {
    push @authors, $_;
  }
}

END {
  print "# Authors ordered by first contribution.\n";
  print "\n", @authors, "\n";
  print "# BLumiaOJ (c) 2015 ~ 2017\n";
}

' > AUTHORS
