jhbuild gtk on mac

When I tried to build get with jhbuild on MacOS, I need below additional with official document
https://gitlab.gnome.org/GNOME/gtk-osx/-/wikis/home#quick-start

I can download gtk-osx-setup.sh, no problem.
Next, I am using pyenv in default my shell, so this shell script download another pyenv, which I don't want.
so I override below environment variables

export PYENV_INSTALL_ROOT="$HOME/.pyenv"
export PYENV_ROOT="$HOME/.pyenv"

Next, gtk-osx-setup.sh completed, I need close terminal and open again to use jhbuild required Python.

Next, I troubled, much "jhbuild bootstrap-gtk-osx"
error
```
Loading .env environment variables...
Traceback (most recent call last):
  File "/Users/kawai/Source/jhbuild/jhbuild/config.py", line 204, in load
    execfile(filename, config)
    ~~~~~~~~^^^^^^^^^^^^^^^^^^
  File "/Users/kawai/Source/jhbuild/jhbuild/utils/misc.py", line 34, in execfile
    code = compile(source, filename, "exec")
  File "/Users/kawai/.config/jhbuildrc", line 18
    }
    ^
SyntaxError: unmatched '}'
jhbuild: could not load config file
```
This is due to that this jhbuild tried to download template jhbuildrc from gitlab.gnome.org, but after Russia invasion, gitlab.org needs to enable bot/spam protector otherwise, site keep getting down.
So when I checked above file (jhbuildrc), instead of Python, bot title HTML.

I am getting confused, so I looks at github, then I cloned below repo, (but it might be better official gitlab)
https://github.com/jralls/gtk-osx-build.git to ~/Source/gtk-osx-build

then I copy manual jhbuildrc-example to ./config/jhbuildrc, then next this error
```
Loading .env environment variables...
Cannot access modulesets http://gitlab.gnome.org/GNOME/gtk-osx/raw/master/modulesets-stable/gtk-osx.modules
```
http?? I changed https it it the same, I think again bot protection.
First I set only use_local_modulesets = True, it does not work, so at least gtk-osx.modules itself should accessible.
Then Gemini told me to use github mirror, github.com does not have those bot stuff, and I specified the module folder, so I made a patch "jhbuild.mac.path"
(NOTE: some reason, in my environment jhbuildrc-custom does not work, so I just edtited)

After that jhbuild bootstrap-gtk-osx is success. Jeez...