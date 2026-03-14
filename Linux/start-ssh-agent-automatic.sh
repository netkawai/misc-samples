# only work on Ubuntu
#
systemctl --user start ssh-agent

if [ $(ps ax | grep [s]sh-agent | wc -l) -gt 0 ] ; then
    export SSH_AUTH_SOCK=${XDG_RUNTIME_DIR}/openssh_agent
    ssh-add
else
    echo "No ssh-agent !!!"
fi
