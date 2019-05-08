Git package (at least in Debian) provides a `__git_ps1` bash function which can be used to display current git branch, try <code>cat `locate git-prompt`</code> to know more.

To get a right aligned prompt, we can use `printf`.

``` bash
rightprompt()
{
    printf "%*s" $COLUMNS "$(__git_ps1 " (%s)")"
}
```

Current `PS1` I use (actually is just a Debian default `PS1` with right prompt):

``` bash
if [ "$color_prompt" = yes ]; then
    PS1='\[$(tput sc; rightprompt; tput rc)\]${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[
00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
```
