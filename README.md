# osx-paste-term

So copy-pasting things from the web into Terminal is not safe, it never was, this is not a [new thing](http://thejh.net/misc/website-terminal-copy-paste).

Here is a proof of concept, very dirty, unsecure implementation of clipboard _filter_ that replaces any non printable characters with underscore (`_`) character.

This is OSX only.
