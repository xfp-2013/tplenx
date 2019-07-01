# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)

# If this is an xterm set the title to user@host:dir
PS1='\h:\w\$ '

# colored GCC warnings and errors
export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
alias ls='ls -h --group-directories-first --color=auto'
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

umask 022
