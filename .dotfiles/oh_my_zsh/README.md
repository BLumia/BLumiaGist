Copy `/path/of/oh-my-zsh/templates/zshrc.zsh-template` to `~/.zshrc` first.

## Completion

Execuse `compinstall` to install completion support if haven't installed yet. `compinstall` will append `zstyle :compinstall filename '/home/blblb/.zshrc'` (Not sure what does this one do, seems useless), `autoload -U compinit` and `compinit` to `~/.zshrc` by default.

### `killall` Completion

``` plain
zstyle ':completion:*:processes' command 'NOCOLORS=1 ps -U $(whoami)|sed "/ps/d"'
zstyle ':completion:*:processes' insert-ids menu yes select
zstyle ':completion:*:processes-names' command 'NOCOLORS=1 ps xho command|sed "s/://g"'
zstyle ':completion:*:processes' sort false
```

Source: [Killall completion](https://github.com/zsh-users/zsh-completions/issues/93#issuecomment-8311702).

## Theme: Bashy

To make zsh got bashy look-and-feel, apply this bashy theme. It's based on alanpeabody.zsh-theme.

``` plain
ZSH_THEME="bashy" # don't forget to set a correct ZSH_CUSTOM value
```

Also need to tweak some value inside `~/.zshrc`:

``` plain
AUTO_REMOVE_SLASH="false" # can't find in zshrc.zsh-template, just add it.
CASE_SENSITIVE="true" # can find in zshrc.zsh-template
setopt no_hist_verify # should append to the bottom of the zshrc file.
```

`no_hist_verify` will disable `HIST_VERIFY` so you can use something like `sudo !!` without press `Enter` again to confrim execuse the command. This setting should append to the bottom of the zshrc file, not sure why. For more about zsh options, see `man zshoptions`.
