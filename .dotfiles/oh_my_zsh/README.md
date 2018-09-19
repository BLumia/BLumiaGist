Copy `/path/of/oh-my-zsh/templates/zshrc.zsh-template` to `~/.zshrc` first.

## Theme: Bashy

To make zsh got bashy look-and-feel, apply this bashy theme. It's based on alanpeabody.zsh-theme.

``` plain
ZSH_THEME="bashy" # don't forget to set a correct ZSH_CUSTOM value
```

Also need to tweak some value inside `~/.zshrc`:

``` plain
AUTO_REMOVE_SLASH="false" # can't find in zshrc.zsh-template, just add it.
CASE_SENSITIVE="true" # can find in zshrc.zsh-template
```