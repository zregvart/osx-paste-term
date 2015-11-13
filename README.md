# osx-paste-term

So copy-pasting things from the web into Terminal is not safe, it never was, this is not a [new thing](http://thejh.net/misc/website-terminal-copy-paste).

*Note:* If you're using Terminal.app you can uncheck `Paste newlines as carriage returns` under Advanced in the Preferences.

Here is a proof of concept, very dirty, unsecure implementation of clipboard _filter_ that replaces any non printable characters with underscore (`_`) character.

This is OSX only.

After you compile with `make`, run with `sudo ./alterkeys`.

Yes alterkeys is sample from [Mac OS X Internals](http://osxbook.com/book/bonus/chapter2/alterkeys/).

